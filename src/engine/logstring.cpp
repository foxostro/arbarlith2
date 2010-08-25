/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2003,2007,2009,2010 Game Creation Society
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

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <ctime>
#include <string>

using std::string;



// Returns a formatted timestamp for the current time.
static string Timestamp(void)
{
	time_t curTime = std::time(NULL);
	char timestamp[32];
	std::strftime(timestamp,
	              sizeof(timestamp),
				  "%Y%m%dT%H%M%S",
				  localtime(&curTime));
	return string(timestamp);
}



namespace Engine {

void Log(const string & msg, const string & fun, const string & fil, int ln)
{
	static FILE * log = NULL;

	string ts = Timestamp();
	
	if(!log) {
		/* XXX: Not thread-safe! Since we aren't using threads during
		 * initialization, that's probably OK for now.
		 */
		string f = string("arbarlith2_") + ts + string(".log");
		log = fopen(f.c_str(), "w");
	}

	fprintf(log, "%s\t%s:%d\t%s\t%s\n", ts.c_str(), fil.c_str(), ln,
	        fun.c_str(), msg.c_str());
}

} // namespace Engine
