/*
Copyright © 2006-2007 Andrew Fox
E-Mail: mailto:foxostro@gmail.com
*/

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "world.h"
#include "Trigger.h"
#include "TriggerPrompt.h"

namespace Engine {

/** Abstract class for a Trigger that is activated through player input */
class Switch : public Trigger
{
public:
	GEN_RTTI(Switch, "class Engine::Switch")

public:
	/**
	Constructs the Switch
	@param ID The desired unique ID of the object
	*/
	Switch(OBJECT_ID ID);

	/** Destroy and clear out the object */
	virtual void destroy(void);

	/** Clear out everything to defaults */
	virtual void clear(void);

	/**
	Updates the object without displaying it
	@param deltaTime milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

	/**
	Gets the action label string
	@return The action label
	*/
	const string& getActionLabel(void) const
	{
		return actionLabel;
	}

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

	/**
	Activate the switch
	@param a The actor that uses the switch
	*/
	void activate(Actor *a);

protected:
	/**
	Determine whether the proper conditions have been attained for trigger activation.
	By default the trigger condition is the mere proximity of the PLAYER
	@return true if so, false otherwise
	*/
	virtual bool pollConditions(void) const;

	/** Called in the event of the Trigger activating */
	virtual void onTrigger(void);

	/**
	Called when the switch is triggered and actually used
	@param a The actor that uses the switch
	*/
	virtual void onUse(Actor *a);

	/**
	Saves the object state to an XML data source, but only if it differs from the default value
	@param xml The XML data source returned
	@param dataFile The data file containing the default values
	@return true if successful, false otherwise
	*/
	virtual bool saveTidy(PropertyBag &xml, PropertyBag &dataFile) const;

	/** Text that describes the Player's action on the switch "Use", "Activate", "Ring the bell" */
	string actionLabel;

	/** Milliseconds until the option to use the switch expires */
	float fadeTimer;

	/** The time to give till the option fades */
	float time;

	/** Handle to the message we set up on the prompt */
	TriggerPrompt::HANDLE promptHandle;
};

}; // namespace

#endif
