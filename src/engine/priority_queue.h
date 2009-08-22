/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright © 2006-2007 Game Creation Society
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

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <vector>
#include <queue>


namespace Engine { 


/** Allows us to use a priority queue on an Object without storing the priority in the Effect */
struct prioritize
{
	int priority;
	void *object;
};

/** Allows us to use a priority queue on an Object without storing the priority in the Effect */
struct prioritize_f
{
	float priority;
	void *object;
};

/** Creates an Functor-Operator to be able to support comparison of Effect priorities */
struct less_prioritize
{
	bool operator()(const prioritize& left, const prioritize& right) const
	{
		return (left.priority < right.priority);
	}
};


/** Creates an Functor-Operator to be able to support comparison of Effect priorities */
struct greater_prioritize
{
	bool operator()(const prioritize& left, const prioritize& right) const
	{
		return (left.priority > right.priority);
	}
};

/** Creates an Functor-Operator to be able to support comparison of Effect priorities */
struct less_prioritize_f
{
	bool operator()(const prioritize_f& left, const prioritize_f& right) const
	{
		return (left.priority < right.priority);
	}
};

/** Creates an Functor-Operator to be able to support comparison of Effect priorities */
struct greater_prioritize_f
{
	bool operator()(const prioritize_f& left, const prioritize_f& right) const
	{
		return (left.priority > right.priority);
	}
};

/** An alias for the priority_queue we are sepecializing */
typedef std::priority_queue<prioritize, vector<prioritize>, less_prioritize> PriorityQueue_Less_I;

/** An alias for the priority_queue we are sepecializing */
typedef std::priority_queue<prioritize_f, vector<prioritize_f>, less_prioritize_f> PriorityQueue_Less_F;

/** An alias for the priority_queue we are sepecializing */
typedef std::priority_queue<prioritize_f, vector<prioritize_f>, greater_prioritize_f> PriorityQueue_Greater_F;


}; // namespace


#endif
