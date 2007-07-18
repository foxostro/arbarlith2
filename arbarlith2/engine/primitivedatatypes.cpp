/*
Modified by Andrew Fox in 2003-2007
Original Source: McCuskey, Mason. "Game Programming Tricks of the Trade". "Trick 15: Serialization Using XML Property Bags". Premier Press. 2002.
E-Mail: mailto:andrewfox@cmu.edu
*/

#include "stdafx.h"
#include "PrimitiveDataTypes.h"

namespace Engine {

// simple _tstring tokenization function.
// you probably have a better one.
void Tokenize(const _tstring& str, vector<_tstring>& tokens, const _tstring& delimiters)
{
    // Skip delimiters at beginning.
    _tstring::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    _tstring::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (_tstring::npos != pos || _tstring::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

} // namespace Engine
