/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2003-2007,2009,2010 Game Creation Society
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Game Creation Society nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE Game Creation Society ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE Game Creation Society BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "stdafx.h" // Master Header
#include "CallbackTask.h"
#include "gl.h"
#include "widgetmanager.h"
#include "ListPaneWidget.h"
#include "ListElementTweaker.h"
#include "world.h"
#include "player.h"
#include "StateMachineFactory.h"
#include "StateMachine.h"
#include "creature.h"

namespace Engine {


GEN_ACTOR_RTTI_CPP(Creature, "class Engine::Creature")



Creature::Creature(OBJECT_ID ID)
: Actor(ID)
{
	clear();
}

Creature::~Creature()
{
	destroy();
}

void Creature::resurrect(void)
{
	state = NORMAL;

	healthPoints = maxHealthPoints;

	ChangeAnimation(getIdleAnim());

	// Start a resurrection particle effect
	getZone().SpawnPfx("data/particle/summon.xml", getPos());
}

void Creature::OnPlace(void)
{
	getZone().SpawnPfx("data/particle/summon.xml", getPos());
}

bool Creature::isInProximity(OBJECT_ID actor) const
{
	return Actor::isInProximity(actor);
}

bool Creature::isInProximity(OBJECT_ID actor, float triggerRadius) const
{
	const ActorSet &s = getZone().getObjects();

	// Ignore invalid handles and zombies
	if(zombie || !s.isMember(actor))
		return false;

	// Ignore dead monsters
	const Creature *creature = dynamic_cast<const Creature*>(s.getPtr(actor));
	if(creature!=0 && !creature->isAlive())
		return false;

	return Actor::isInProximity(actor, triggerRadius);
}

bool Creature::canMove(void) const
{
	return(state == NORMAL || state == GHOST);
}

void Creature::heal(int healValue)
{
	healthPoints += healValue;

	// cap health at 100%
	if(healthPoints > maxHealthPoints) healthPoints = maxHealthPoints;

	// Heal FX
	getZone().SpawnPfx("data/particle/heal.xml", getPos());
	g_SoundSystem.play("data/sound/magical-healing.wav");
}

void Creature::damage(int amount, OBJECT_ID attackerID)
{
	if(!isAlive()) return; // cannot harm a dead creature

	if(amount<0) return;

	int finalDamage = (int)floorf(amount * armorMultiplier);

	// damage to self is reduced
	if(attackerID == m_ID)
	{
		finalDamage /= 2;
	}

	healthPoints -= finalDamage;

	if(getZone().getObjects().isMember(attackerID))
	{
		OnAttacked(dynamic_cast<Creature&>(getZone().getObjects().get(attackerID)), finalDamage);
	}

	if(healthPoints<0)
	{
		kill();
	}
	else
	{
		ChangeAnimation("flinch");
		g_SoundSystem.play(getHurtSfx());
	}
}

void Creature::freeze(void)
{
	if(isAlive())
	{
		state = FROZEN;
		stateTimer = maxStunTime * 3;
	}
}

void Creature::OnAttacked(Creature &attacker, int finalDamage)
{
	if(!isAlive()) return; // this message is useless if the recipient is dead

	// signal the AI fsm
	Message_s message;
	message.m_Recipient = m_ID;
	message.m_Sender = attacker.m_ID;
	message.m_Type = MSG_ATTACKED;
	getZone().router.Send(message);

	// Stun and knockback if damaged enough
	float p = (float)finalDamage/maxHealthPoints;
	if(p >= damagePercentToStun)
	{
		state = STUNNED;
		stateTimer = maxStunTime;
		applyKnockBack(getPos() - attacker.getPos());
	}
}

void Creature::clear(void)
{
	Actor::clear();

	// Reset data for this Creature
	state = NORMAL;
	m_strName			= "invalid name";
	timeUntilOrderCancelled		= 0.0f;
	m_pFSM				= 0;
	haveMessage			= false;

	healthPoints			= 1;
	maxHealthPoints			= 1;
	attackDamage			= 0;
	weaponMultiplier		= 1.f;
	armorMultiplier			= 1.f;
	attackCoolDown			= 700.f;
	attackCoolDownMultiplier	= 1.f;
	attackChargeTime		= 3000.f;
	timeSinceLastAttack		= 0.f;
	attackCoolDownTimer		= 0.f;
	spellCoolDownMultiplier		= 1.f;
	spellTimer			= 0.f;
	fsmUpdateTimer			= 0.f;

	maxStunTime = 1200.f;
	damagePercentToStun = 0.07f;
	stateTimer = 0.0f;
	starAngle = 0.0f;
	starSpeed = 80 / 1000.0f;

	maxKnockBackTime= 10.0;
	knockBackTimer = 0;
	knockBackSpeed = 12.0;
	currentKnockBackVelocity.zero();

	CancelOrders();
	memset(&lastMessage, 0, sizeof(lastMessage));
}

void Creature::destroy(void)
{
	clear();
}

void Creature::applyKnockBack(const vec3 &direction)
{
	knockBackTimer = maxKnockBackTime;
	velocity = currentKnockBackVelocity = direction.getNormal() * knockBackSpeed;

	OnKnockBack();
}

void Creature::OnKnockBack(void)
{
	// do nothing
}

void Creature::update(float deltaTime)
{
	timeSinceLastAttack += deltaTime;
	timeUntilOrderCancelled -= deltaTime;

	if(timeUntilOrderCancelled<=0)
	{
		timeUntilOrderCancelled = 0;
		gotoNextOrder();
	}

	switch(state)
	{
	case GHOST:
		stateTimer -= deltaTime;

		if(stateTimer<=0)
		{
			stateTimer=0;
			resurrect();
		}

		// fall through to NORMAL case

	case NORMAL:
		if(attackCoolDownTimer>0) attackCoolDownTimer -= deltaTime;
		if(spellTimer>0) spellTimer -= deltaTime * spellCoolDownMultiplier;

		// Process High-Level AI
		fsmUpdateTimer -= deltaTime;
		if(m_pFSM!=0 && fsmUpdateTimer<=0)
		{
			fsmUpdateTimer = FRAND_RANGE(50.0f, 200.0f);

			if(haveMessage==true)
			{
				m_pFSM->Update(&lastMessage);
				haveMessage = false;
			}
			else
			{
				m_pFSM->Update(0);
			}
		}

		// Process the queued commands sent by the FSM
		ProcessCommand();

		ChangeAnimation(getWalkAnim(velocity.getMagnitude()));

		break;

	case STUNNED:
		stateTimer -= deltaTime;
		knockBackTimer -= deltaTime;

		if(knockBackTimer<0)
		{
			knockBackTimer = 0;
		}
		else
		{
			// as long as the creature is still knocking back, keep the velocity constant
			velocity = currentKnockBackVelocity;
		}

		if(stateTimer<=0)
		{
			state = NORMAL;
			stateTimer = 0;
		}

		starAngle = starAngle > 359 ? 0 : starAngle + deltaTime*starSpeed;
		ChangeAnimation("stun");
		break;

	case FROZEN:
		ChangeAnimation("stun");

		stateTimer -= deltaTime;

		if(stateTimer<=0)
		{
			state = NORMAL;
			stateTimer=0;
		}
		break;

	case DYING:
		stateTimer -= deltaTime;

		if(stateTimer <= 0.0f)
		{
			state = DEAD;
			stateTimer = 1000.0f; // min time before we are allowed to delete the dead Creature
		}

		break;

	case DEAD:
		ChangeAnimation("dead");

		stateTimer -= deltaTime;

		// Is this dead actor currently off-screen?
		if(stateTimer <= 0.0f)
		{
			zombie |= !g_Camera.getFrustum().SphereInFrustum2(getPos(), getSphereRadius());
			stateTimer = 0.0f;
		}

		break;
	};

	// AnimationSequence and housekeeping
	Actor::update(deltaTime);
}

bool Creature::QueueCommand(const Command &wp)
{
	if(ordersRemaining.empty())
	{
		// Update the timer if this is the only order in the queue
		timeUntilOrderCancelled = wp.getTimeOut();
	}

	ordersRemaining.push_back(wp);

	return true;
}

bool Creature::InjectCommand(const Command &wp)
{
	ordersRemaining.push_front(wp);
	timeUntilOrderCancelled = wp.getTimeOut();

	return true;
}

void Creature::createToolBar(ListPaneWidget *pane)
{
	Actor::createToolBar(pane);
	pane->addElement(new ListElementTweaker<float>("Top Speed", &topSpeed));
}

bool Creature::saveTidy(PropertyBag &xml, PropertyBag &dataFile) const
{
	saveTag(xml, dataFile, "healthPoints",             healthPoints);
	saveTag(xml, dataFile, "maxHealthPoints",          maxHealthPoints);
	saveTag(xml, dataFile, "attackDamage",             attackDamage);
	saveTag(xml, dataFile, "weaponMultiplier",         weaponMultiplier);
	saveTag(xml, dataFile, "armorMultiplier",          armorMultiplier);
	saveTag(xml, dataFile, "attackCoolDown",           attackCoolDown);
	saveTag(xml, dataFile, "attackCoolDownMultiplier", attackCoolDownMultiplier);
	saveTag(xml, dataFile, "spellCoolDownMultiplier",  spellCoolDownMultiplier);
	saveTag(xml, dataFile, "attackChargeTime",         attackChargeTime);
	saveTag(xml, dataFile, "damagePercentToStun",      damagePercentToStun);

	saveList(xml, "dyingSounds",	dyingSounds);
	saveList(xml, "painSounds",	painSounds);
	saveList(xml, "attackSounds",	attackSounds);
	saveList(xml, "attnSounds",	attnSounds);

	return Actor::saveTidy(xml, dataFile);
}

void Creature::load(const PropertyBag &Bag)
{
	Actor::load(Bag);

	Bag.get_optional("healthPoints", healthPoints);
	Bag.get_optional("maxHealthPoints", maxHealthPoints);
	Bag.get_optional("attackDamage", attackDamage);
	Bag.get_optional("weaponMultiplier", weaponMultiplier);
	Bag.get_optional("armorMultiplier", armorMultiplier);
	Bag.get_optional("attackCoolDown", attackCoolDown);
	Bag.get_optional("attackCoolDownMultiplier", attackCoolDownMultiplier);
	Bag.get_optional("spellCoolDownMultiplier", spellCoolDownMultiplier);
	Bag.get_optional("attackChargeTime", attackChargeTime);
	Bag.get_optional("damagePercentToStun", damagePercentToStun);
	Bag.get_optional("maxKnockBackTime", maxKnockBackTime);
	Bag.get_optional("knockBackSpeed", knockBackSpeed);

	// Load sound FX lists
	loadList(Bag, "dyingSounds",  dyingSounds);
	loadList(Bag, "painSounds",   painSounds);
	loadList(Bag, "attackSounds", attackSounds);
	loadList(Bag, "attnSounds",   attnSounds);

	// Get the name of the High-Level FSM to use
	{
		string strFSM = "none";
		delete(m_pFSM);

		Bag.get_optional("fsm", strFSM);
		setFSM(strFSM);
	}

	starMaterial.loadTexture("data/particle/star.png", 0);
}

void Creature::setFSM(const string &strFSM)
{
	if(strFSM == "none") {
		SetStateMachine(0);
	} else {
		StateMachine *stateMachine = getStateMachineFactory().createPtr(strFSM);

		ASSERT(stateMachine != 0, "Failed to create state machine: " + strFSM);

		if(stateMachine == 0) {
			SetStateMachine(0);
		} else {
			stateMachine->setOwner(this);
			SetStateMachine(stateMachine);
		}
	}
}

void Creature::walkTowards(const vec3 &target, float speed)
{
	ASSERT(speed >= -1.0f, "Speed is too negative: " + ftoa(speed));
	ASSERT(speed <= +1.0f, "Speed is too positive: " + ftoa(speed));

	if(canMove())
	{
		velocity.zero(); // reset

		if(target.x > position.x) {
			velocity.x = 1;
		} else if(target.x < position.x) {
			velocity.x = -1;
		}

		if(target.z > position.z) {
			velocity.z = 1;
		} else if(target.z < position.z) {
			velocity.z = -1;
		}

		velocity = velocity.getNormal() * (getTopSpeed()*speed);

		lookAt(target);
	}
}

void Creature::ProcessCommand(void)
{
	if(HasOrders())
	{
		// This is the current order
		const Command &wp = getCurrentOrder();

		// Retrieve the target, if possible
		target = getZone().getObjects().getPtr(wp.getTarget());

		// Handle the command
		switch(wp.getCommand())
		{
		case Command::FREEZE:
			ASSERT(target != 0, "target was null");
			lookAt(target->getPos());
			break;

		case Command::MOVE_POS:
			Process_MoveToPos(static_cast<const CommandMoveToLocation&>(wp));
			break;

		case Command::MOVE_TARGET:
			Process_MoveToTarget(static_cast<const CommandMoveToTarget&>(wp));
			break;

		case Command::ATTACK:
			Process_AttackTarget(static_cast<const CommandAttack&>(wp));
			break;

		case Command::FLEE:
			Process_FleeTarget(static_cast<const CommandFlee&>(wp));
			break;

		case Command::USE:
			Process_UseTarget(static_cast<const CommandUse&>(wp));
			break;
		};
	}
}

void Creature::Process_MoveToTarget(const CommandMoveToTarget &wp)
{
	if(target==0 || wasCollision(wp.getTarget())) // No valid target or we're colliding with the target
		gotoNextOrder();
	else
		walkTowards(target->getPos(), wp.getDesiredSpeed());
}

void Creature::Process_MoveToPos(const CommandMoveToLocation &wp)
{
	vec3 toTarget = getPos() - wp.getLocation();
	float distance = toTarget.getMagnitude() - getCylinderRadius();

	if(distance < wp.getDesiredProximity()) // Has the goal been achieved yet?
		gotoNextOrder();
	else
		walkTowards(wp.getLocation(), wp.getDesiredSpeed());
}

void Creature::startAttackAction(OBJECT_ID id)
{
	Creature &o = dynamic_cast<Creature&>(getZone().getObjects().get(id));

	if(canMove() && o.isAlive() && attackCoolDownTimer <= 0)
	{
		// calculate the damage modifer from attack charge-up
		float chargeMultiplier = min(timeSinceLastAttack/attackChargeTime, 1.f)*0.90f + 0.10f;

		// calculate the damage inflicted
		int damageValue = (int)floorf(attackDamage * chargeMultiplier * weaponMultiplier);

		// calculate the length of the attack animation
		size_t animHandle = getAnimationHandle(getAttackAnim());
		float delay = getAnimationLength(animHandle);
		ChangeAnimation(animHandle);

		// calculate the time till we can attack again
		attackCoolDownTimer = max(attackCoolDown * attackCoolDownMultiplier, delay); // reset

		// inflict damage once the animation is underway
		boost::function<void (void)> fn = boost::bind(&Creature::dealAttackDamage, this, damageValue, o.m_ID);
		Task *task = makeCallbackTask(delay*0.33f, fn);
		g_Application.addTask(task);

		// Misc
		timeSinceLastAttack = 0;
		g_SoundSystem.play(getAttackSfx());
		lookAt(o.getPos());
	}
}

void Creature::dealAttackDamage(int amount, OBJECT_ID attackedCreature)
{
	if(getZone().getObjects().isMember(attackedCreature))
	{
		Creature &o = dynamic_cast<Creature&>(getZone().getObjects().get(attackedCreature));

		float distance = vec3(getPos()-o.getPos()).getMagnitude();
		float maxAttackDistance = (getCylinderRadius() + o.getCylinderRadius()) * 1.1f;

		if(!zombie && !o.zombie && isAlive() && o.isAlive() && canMove() && distance<maxAttackDistance)
		{
			o.damage(amount, m_ID);
		}
	}
}

void Creature::Process_AttackTarget(const CommandAttack &wp)
{
	if(!canMove() || target==0)
	{
		gotoNextOrder();
		return;
	}

	float speed = wp.getDesiredSpeed()*getTopSpeed();
	float attackChargeThreshold = 1.0f;
	float attackCharge = timeSinceLastAttack/attackChargeTime;
	float chargeTimeRemaining = max(0.0f, attackChargeTime - timeSinceLastAttack) / 1000.0f; // seconds
	float distance = getDistance(this, *target);
	float vulnerableRadius = (getCylinderRadius() + target->getCylinderRadius()) * 2.0f;
	float distancetoLeaveVulnerableRange = vulnerableRadius - distance;
	float timeToLeaveVulnerableRange = speed*distancetoLeaveVulnerableRange; // seconds

	if(isInProximity(wp.getTarget()))
	{
		if(attackCharge >= attackChargeThreshold)
		{
			startAttackAction(wp.getTarget());
		}
		else
		{
			// can this creature leave the vulnerable zone before attack finishes charging?
			if(timeToLeaveVulnerableRange<chargeTimeRemaining && attackCoolDownTimer<=0)
			{
				// back away from the target

				const vec3 z = vec3(getPos() - target->getPos()).getNormal();
				const vec3 y = vec3(0, 1, 0);
				const vec3 x = y.cross(z);
				setOrientation(  mat4(vec3(0,0,0), x, y, z)  );

				velocity = z * speed;
			}
			else
			{
				// hold position
				lookAt(target->getPos());
			}
		}
	}
	else
	{
		InjectCommand(CommandMoveToTarget(wp.getTarget()));
	}
}

void Creature::Process_FleeTarget(const CommandFlee &wp)
{
	if(!canMove() || target==0 || getDistance(this, *target) > wp.getDesiredProximity())
		gotoNextOrder();
	else
	{
		const vec3 z = -vec3(getPos() - target->getPos()).getNormal();
		const vec3 y = vec3(0, 1, 0);
		const vec3 x = y.cross(z);
		setOrientation(  mat4(vec3(0,0,0), x, y, z)  );

		const float speed = wp.getDesiredSpeed()*getTopSpeed();
		velocity = -z * speed;
	}
}

void Creature::Process_UseTarget(const CommandUse &wp)
{
	// The distance to the target
	vec3 toTarget = getPos() - wp.getLocation();
	float distance = toTarget.getMagnitude() - getCylinderRadius();

	// The attack distance
	float attackDistance = 1.0f;

	// are we within range to attack?
	if(distance < attackDistance)
	{
		// TODO: Have the Creature use the object
		g_SoundSystem.play("data/sound/activate.wav");
	}
	else // if not, then let's move to the target
	{
		InjectCommand(CommandMoveToTarget(wp.getTarget(), 1.0f, attackDistance));
	}
}

void Creature::CancelOrders(void)
{
	while(!ordersRemaining.empty())
	{
		ordersRemaining.pop_front();
	}
}

void Creature::OnDeath(void)
{
	solid = false;
}

void Creature::kill(void)
{
	// Play a sound effect for the creature dying
	g_SoundSystem.play(getDyingSfx());

	// calculate the length of the dying animation
	size_t animHandle = getAnimationHandle(getDyingAnim());
	float delay = getAnimationLength(animHandle);
	ChangeAnimation(animHandle);

	state = DYING;
	stateTimer = delay;
	healthPoints = 0;

	OnDeath();
}

void Creature::OnMessage(Message_s Msg)
{
	haveMessage = true;
	lastMessage = Msg;

	Actor::OnMessage(Msg);
}

const string Creature::getPainAnim(void) const
{
	return "flinch";
}

const string Creature::getDyingAnim(void) const
{
	return "dying";
}

const string Creature::getAttackAnim(void) const
{
	return "attack";
}

const string Creature::getIdleAnim(void) const
{
	return "idle";
}

const string Creature::getRunFwdAnim(void) const
{
	return "run";
}

const string Creature::getRunRevAnim(void) const
{
	return "runback";
}

const string Creature::getRunLeftAnim(void) const
{
	return "runleft";
}

const string Creature::getRunRightAnim(void) const
{
	return "runright";
}

const string Creature::getRunLeftFwdAnim(void) const
{
	return "runleftforward";
}

const string Creature::getRunRightFwdAnim(void) const
{
	return "runrightforward";
}

const string Creature::getRunLeftRevAnim(void) const
{
	return "runleftback";
}

const string Creature::getRunRightRevAnim(void) const
{
	return "runrightback";
}

const string Creature::getWalkAnim(float speed) const
{
	if(speed < minWalkingVelocity)
	{
		return getIdleAnim();
	}
	else
	{
		return "run";
	}
}

const string Creature::getDyingSfx(void) const
{
	if(dyingSounds.empty())
		return  "data/sound/default.wav"; // default sound
	else if(dyingSounds.size()==1)
		return dyingSounds[0];
	else
		return dyingSounds[IRAND_RANGE(0, dyingSounds.size()-1)];
}

const string Creature::getHurtSfx(void) const
{
	if(painSounds.empty())
		return  "data/sound/default.wav"; // default sound
	else if(painSounds.size()==1)
		return painSounds[0];
	else
		return painSounds[IRAND_RANGE(0, painSounds.size()-1)];
}

const string Creature::getAttackSfx(void) const
{
	if(attackSounds.empty())
		return  "data/sound/default.wav"; // default sound
	else if(attackSounds.size()==1)
		return attackSounds[0];
	else
		return attackSounds[IRAND_RANGE(0, attackSounds.size()-1)];
}

const string Creature::getAttnSfx(void) const
{
	if(attnSounds.empty())
		return  "data/sound/default.wav"; // default sound
	else if(attnSounds.size()==1)
		return attnSounds[0];
	else
		return attnSounds[IRAND_RANGE(0, attnSounds.size()-1)];
}

void Creature::gotoNextOrder(void)
{
	if(!ordersRemaining.empty())
	{
		ordersRemaining.pop_front();

		// If there are orders left after cancelling the current order
		if(!ordersRemaining.empty())
		{
			Command &c = ordersRemaining.front();
			timeUntilOrderCancelled = c.getTimeOut();
		}
	}
}

void Creature::drawTransparentObject(void) const
{
	if(state != GHOST || m_pModel==0)
	{
		return;
	}

	CHECK_GL_ERROR();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 0.3f);

		glPushMatrix();
		glMultMatrixf(toWorldSpace());
		m_pModel->draw();
		glPopMatrix();

	glPopAttrib();

	CHECK_GL_ERROR();
}

void Creature::drawObject(void) const
{
	if(state == GHOST || m_pModel==0)
	{
		return;
	}

	CHECK_GL_ERROR();

	if(state==FROZEN)
	{
		glColor4f(98/255.f, 176/255.f, 1.0f, 1.0f); // coliander
		Actor::drawObject();
		glColor4fv(white);
	}
	else
	{
		Actor::drawObject();
	}

	if(isAlive())
	{
		if(g_Application.displayDebugData)
		{
			drawHealthBar();
			drawChargeBar();
		}

		if(state==STUNNED && stateTimer>500.0f)
		{
			drawStarsAboveHead(m_desiredHeight + 0.02f);
		}
	}

	CHECK_GL_ERROR();
}

void Creature::drawObjectDebugData(void) const
{
	const vec3 base = getPos() + vec3(0, getHeight(), 0);

	string stateString = "invalid state";
	{
		string timeString = "(" + ftoa(stateTimer/1000, 1) + ") ";

		switch(state)
		{
		case NORMAL:	stateString = timeString + "NORMAL";	break;
		case STUNNED:	stateString = timeString + "STUNNED";	break;
		case FROZEN:	stateString = timeString + "FROZEN";	break;
		case DYING:	stateString = timeString + "DYING";		break;
		case DEAD:	stateString = timeString + "DEAD";		break;
		case GHOST:	stateString = timeString + "GHOST";		break;
		};
	}

	string commandString = "invalid command";
	if(HasOrders())
	{
		// This is the current order
		const Command &wp = getCurrentOrder();

		string timeString = "(" + ftoa(timeUntilOrderCancelled/1000, 1) + " of " + ftoa(wp.getTimeOut()/1000, 1) + " ";

		// Handle the command
		switch(wp.getCommand())
		{
		case Command::FREEZE:		commandString = timeString + "FREEZE";		break;
		case Command::MOVE_POS:		commandString = timeString + "MOVE TO POS";		break;
		case Command::MOVE_TARGET:	commandString = timeString + "MOVE TO TARGET";	break;
		case Command::ATTACK:		commandString = timeString + "ATTACK";		break;
		case Command::USE:		commandString = timeString + "USE";			break;
		case Command::FLEE:		commandString = timeString + "FLEE";		break;
		};
	}
	else
	{
		commandString = "No Orders";
	}

	glPushMatrix();
		glTranslatef(base.x, base.y + 0.25f, base.z);
		g_Application.fontSmall.write(commandString, white, FONT_SIZE_NORMAL, true);

		glTranslatef(0.0f, 0.2f, 0.0f);
		g_Application.fontSmall.write(stateString, white, FONT_SIZE_NORMAL, true);

		glTranslatef(0.0f, 0.2f, 0.0f);
		g_Application.fontSmall.write(getName(), white, FONT_SIZE_NORMAL, true);

		glTranslatef(0.0f, 0.2f, 0.0f);
		g_Application.fontSmall.write(getTypeString(), white, FONT_SIZE_NORMAL, true);
	glPopMatrix();
}

void Creature::drawHealthBar(void) const
{
	const float p = (float)healthPoints / maxHealthPoints;

	drawFloatingBar(m_desiredHeight, p, green, red);
}

void Creature::drawChargeBar(void) const
{
	const float p = min(timeSinceLastAttack/attackChargeTime, 1.f);

	drawFloatingBar(m_desiredHeight + 0.05f, p, white, white*0.3f);
}

void Creature::drawStarsAboveHead(float y) const
{
	const int numStars = 7;
	const vec3 center = getPos() + vec3(0, y, 0);

	const float WIDTH = 0.10f;
	const float HEIGHT = 0.10f;

	float starsRadius = getCylinderRadius() > 1.0f ? 1.0f : getCylinderRadius();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

		glColor4fv(white);

		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_CULL_FACE);

		// Transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_BLEND);

		glEnable(GL_TEXTURE_2D);
		starMaterial.bind();

		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);

		for(int i=0; i<numStars; ++i)
		{
			glPushMatrix();
			glRotatef(starAngle + i*360.0f/numStars, 0, 1, 0);
			glTranslatef(starsRadius, 0, 0);

				const mat4 &m = g_Camera.getOrientation();
				const vec3 &right = m.getAxisX().getNormal();
				const vec3 &up = m.getAxisY().getNormal();

				const vec3 a = -right * (WIDTH * 0.5f);
				const vec3 b =  right * (WIDTH * 0.5f);
				const vec3 c =  right * (WIDTH * 0.5f) + up * HEIGHT;
				const vec3 d = -right * (WIDTH * 0.5f) + up * HEIGHT;

				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);	glVertex3f(a.x, a.y, a.z);
					glTexCoord2f(1.0f, 0.0f);	glVertex3f(b.x, b.y, b.z);
					glTexCoord2f(1.0f, 1.0f);	glVertex3f(c.x, c.y, c.z);
					glTexCoord2f(0.0f, 1.0f);	glVertex3f(d.x, d.y, d.z);
				glEnd();

			glPopMatrix();
		}

		glPopMatrix();
	glPopAttrib();
}

void Creature::drawFloatingBar(float y, float p, COLOR CA, COLOR CB) const
{
	const vec3 center = getPos() + vec3(0, y, 0);

	const float WIDTH = 1.0f;
	const float HEIGHT = 0.05f;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);

		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);

			const mat4 &m = g_Camera.getOrientation();
			const vec3 &right = m.getAxisX().getNormal();
			const vec3 &up = m.getAxisY().getNormal();

			const vec3 a = -right * (WIDTH*p * 0.5f);
			const vec3 b =  right * (WIDTH*p * 0.5f);
			const vec3 c =  right * (WIDTH*p * 0.5f) + up * HEIGHT;
			const vec3 d = -right * (WIDTH*p * 0.5f) + up * HEIGHT;

			const COLOR color = CA*p + CB*(1-p);
			glColor4fv(color);

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);	glVertex3f(a.x, a.y, a.z);
				glTexCoord2f(1.0f, 0.0f);	glVertex3f(b.x, b.y, b.z);
				glTexCoord2f(1.0f, 1.0f);	glVertex3f(c.x, c.y, c.z);
				glTexCoord2f(0.0f, 1.0f);	glVertex3f(d.x, d.y, d.z);
			glEnd();

		glPopMatrix();
	glPopAttrib();
}

} // namespace Engine
