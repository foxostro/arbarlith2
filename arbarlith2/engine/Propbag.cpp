/*
Modified by Andrew Fox in 2003-2007
Original Source: McCuskey, Mason. "Game Programming Tricks of the Trade". "Trick 15: Serialization Using XML Property Bags". Premier Press. 2002.
E-Mail: mailto:andrewfox@cmu.edu
*/

#include "stdafx.h"
#include "profile.h"
#include "PropBag.h"


namespace Engine {

// stdafx.cpp
int stoi(const _tstring &s);
float stof(const _tstring &s);



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropBag::CPropBag()
{
  Init();
}

void CPropBag::Init()
{
}

CPropBag::~CPropBag()
{
  Clear();
}


void CPropBag::Add(const _tstring& key, _tstring data, bool convert)
{
  CPropString *newprop = new CPropString(data, convert);

  newprop->SetName(key);
  m_Data.insert(make_pair(key, newprop));
}

void CPropBag::Add(const _tstring& key, int data)
{
	Add(key, itoa(data));
}

void CPropBag::Add(const _tstring& key, size_t data)
{
	Add(key, itoa((int)data));
}

void CPropBag::Add(const _tstring& key, double data)
{
	Add(key, ftoa((float)data, 4));
}

void CPropBag::Add(const _tstring& key, bool data)
{
	if(data)
		Add(key, _tstring(_T("true")));
	else
		Add(key, _tstring(_T("false")));
}

void CPropBag::Add(const _tstring& key, float data)
{
	Add(key, ftoa(data, 4));
}

void CPropBag::Add(const _tstring& key, const XmlDataType *pData)
{
	ASSERT(pData!=NULL, _T("CPropBag::Add  ->  Cannot add a NULL object to the Prop Bag"));

	CPropString *newprop = new CPropString(pData->ToString(), false);

	newprop->SetName(key);
	m_Data.insert(make_pair(key, newprop));
}

void CPropBag::Add(const _tstring& key, CPropBag data)
{
	// Only add the tag if it contains data
	if(data.GetData().empty() == false) // Conditional added by Andrew Fox, 2004
	{
		CPropBag *newbag = new CPropBag(data);

		if(newbag)
		{
			m_Data.insert(make_pair(key, newbag));
		}
	}
}

void CPropBag::Remove(const _tstring &key, int instance)
{
  if (!m_Data.size()) return; // bag is empty, no need to continue.
  PropertyMap::iterator i;
  PropertyMap::iterator stop = m_Data.upper_bound(key);
  stop++;

  i = m_Data.lower_bound(key);
  if (i == m_Data.end()) return; // nothing to remove
  if (instance != -1) for(int q=0; q < instance; q++, i++);
  if (i != m_Data.end()) {

    if (instance == -1) { // kill all instances
      do {
        delete((*i).second);
        m_Data.erase(i);
      } while (m_Data.size() && i++ != stop);
    }
    else {
      delete((*i).second);
      m_Data.erase(i);
    }
  }
}

_tstring CPropString::Save(int) const
{
	return(m_Data);
}

void CPropBag::saveToFile(const _tstring &fileName, int indentLevel) const
{
	File file;
	file.write(Save(indentLevel));
	file.saveFile(fileName, false);
}

_tstring CPropBag::Save(int indentlevel) const
{
	// This function was commented by Andrew Fox in an effort to understand it
	// Modified a bit by Andrew Fox as well

	_tstring out;
	_tstring indent(indentlevel, '\t');

	// Cycle through all the tags in this bag
	for (PropertyMap::const_iterator i = m_Data.begin(); i != m_Data.end(); i++)
	{
		CPropItem *data = (*i).second; // Tag Value can be retrieved from this object
		_tstring key = (*i).first;       // Tag Name
		_tstring line;                   // TagValue is put in here
		_tstring withname;               // <TagName>TagValue</TagName> is put in here

		// Dynamic cast will return NULL if data is not a CPropBag object
		bool IsBag = dynamic_cast<CPropBag*>(data) != NULL;

		// Get the tag value as a _tstring.  CPropItem::Save() does this for us.
		// Note that if the data is a CPropString, then line is simple the _tstring's value
		// But, if the data is a CPropBag, then this function recurses to make line = bag
		// (If the conversion exists, have the data be indented like a property bag should be)
		line = data->Save( IsBag ? indentlevel+1 : indentlevel);

		if(IsBag)
		{
			// Put a new line after the opening tag, then indent and put the data
			withname  = indent;
			withname += _T("<");
			withname += key;
			withname += _T(">\n");
			withname += line;
			withname += indent;
			withname += _T("</");
			withname += key;
			withname += _T(">\n");
		}
		else
		{
			// Everything on one line
			withname  = indent;
			withname += _T("<");
			withname += key;
			withname += _T(">");
			withname += line;
			withname += _T("</");
			withname += key;
			withname += _T(">\n");
		}

		// Add this tag to the total _tstring dump of the property bag
		out += withname;
	}

	return(out);
}

bool CPropBag::LoadMerge(const _tstring &filename)
{
	_tstring strData;

	// Load in the file
	File file(filename, false);

	if(file.loaded())
	{
		try
		{
			const size_t size = file.getSize();

			// Resize the _tstring so that we can fit the data
			strData.resize(size);

			// Allocate a buffer
			char *buffer = new char[size];

			// Error checking
            if(buffer==0)
			{
				ERR(_tstring(_T("Failed to allocate memory buffer: ")) + filename);
				return false;
			}

			// Copy file data
			file.read(buffer, size);

			// Copy the buffer into the _tstring
			strData = toTString(buffer);

			// Release the buffer
			delete [] buffer;
			buffer=0;
		}
		catch(...)
		{
			ERR(_tstring(_T("Exception while extracting file data from ")) + filename);
			return false;
		}

		// Load and merge the tags
		return(LoadMergeFromString(strData));
	}
	else
	{
		ERR(_tstring(_T("File failed to load: ")) + filename);
		return false;
	}
}

bool CPropBag::Load(const _tstring &filename)
{
	// Load in the file
	File file(filename, false);

	if(file.loaded())
	{
		size_t size = file.getSize();
		char *buffer = new char[size];

		file.read(buffer, size);

		_tstring strData = toTString(buffer);

		delete [] buffer;
		buffer=0;

		// Return the Prop Bag object
		return(LoadFromString(strData));
	}
	else
	{
		return false;
	}
}

bool CPropBag::LoadMergeFromString(const _tstring &data)
{
	enum eElanPropBagReadState
	{
		SearchingForOpenTag = 0,
		ReadingOpenTag = 1,
		ReadingTagContents = 2
	} curstate = SearchingForOpenTag;

	_tstring tagname;
	_tstring tagvalue;
	_tstring closetag;

	unsigned char previous=0;
	_tstring possibleClosingTag;
	bool isPossibleClosingTag=false;

	for(_tstring::const_iterator iter = data.begin(); iter != data.end(); ++iter)
	{
		const unsigned char b = (unsigned char)(*iter);

		switch(curstate)
		{
			case SearchingForOpenTag:
			{
				if (b == _T('<'))
				{
					// we've found our open tag!
					curstate = ReadingOpenTag;
				}
			}
			break;

			case ReadingOpenTag:
			{
				if (b == _T('>'))
				{
					// end of tag
					curstate = ReadingTagContents;
					closetag = _tstring(_T("</")) + tagname + _tstring(_T(">"));
				}
				else
				{
					// add the character to the name of the tag
					tagname += b;
				}
			}
			break;

			case ReadingTagContents:
			{
				// Add the characer to the contents of the tag
				tagvalue += b;

				// If we are possibly reading the closing tag now
				if(isPossibleClosingTag)
				{
					// Build the _tstring for what may be the closing tag
					possibleClosingTag += b;

					// Otherwise, search for the real closing tag
					if(possibleClosingTag == closetag)
					//if(tagvalue.find(closetag) != _tstring::npos)
					{
						// Remove that closing tag from the tag contents
						_tstring replace(_T(""));
						Replace(tagvalue, closetag, replace);

						// Put the completed tag into the bag here
						PutTagIntoBag(tagname, tagvalue);

						// Reset the state
						curstate = SearchingForOpenTag;
						tagname = _T("");
						tagvalue = _T("");
					}

					// Has it become impossible that this is the closing tag?
					if(b == _T('>'))
					{
						isPossibleClosingTag = false;
					}
				}

				// Have we begun to encounter what may be the closing tag?
				if(previous == _T('<') && b == _T('/'))
				{
					isPossibleClosingTag = true;
					possibleClosingTag = _T("</");
				}
			}
			break;
		}; // end switch

		previous = b;
	} // loop

	return(true);
}

bool CPropBag::LoadFromString(const _tstring &data)
{
	m_Data.clear();
	return LoadMergeFromString(data);
}

bool CPropBag::PutTagIntoBag(const _tstring &tagname, const _tstring &tagvalue)
{
	// a < and > mean it's a bag within a bag
	if(tagvalue.find(_T("<")) != _tstring::npos && tagvalue.find(_T(">")) != _tstring::npos)
	{
		// it's a bag.
		CPropBag *newbag = new CPropBag;

		ASSERT(newbag!=NULL, _T("CPropBag::PutTagIntoBag  ->  newbag was NULL"));

		newbag->LoadFromString(tagvalue);
		m_Data.insert(make_pair(tagname, newbag));
	}
	else
	{
		// it's a primitive data type.
		CPropString *newstr = new CPropString(tagvalue, false);

		ASSERT(newstr!=NULL, _T("CPropBag::PutTagIntoBag  ->  newstr was NULL"));

		newstr->SetName(tagname);
		m_Data.insert(make_pair(tagname, newstr));
	}

	return(true);
}

bool CPropBag::Get(const _tstring& key, _tstring &dest, int instance)
{
  if (m_Data.find(key) == m_Data.end()) return(false);
  PropertyMap::iterator iter = m_Data.lower_bound(key);
  for (int q=0; q < instance; q++) iter++;
  dest = iter->second->Save();

  return(true);
}

bool CPropBag::Get(const _tstring& key, int &dest, int instance)
{
  _tstring str;
  if (!Get(key, str, instance)) return(false);
  dest = stoi(str);
  return(true);
}

bool CPropBag::Get(const _tstring& key, size_t &dest, int instance)
{
  _tstring str;
  if (!Get(key, str, instance)) return(false);
  dest = static_cast<size_t>(stoi(str));
  return(true);
}

bool CPropBag::Get(const _tstring& key, double &dest, int instance)
{
  _tstring str;
  if (!Get(key, str, instance)) return(false);
  dest = stof(str);
  return(true);
}

bool CPropBag::Get(const _tstring& key, float &dest, int instance)
{
  _tstring str;
  if (!Get(key, str, instance)) return(false);
  dest = stof(str);
  return(true);
}

bool CPropBag::Get(const _tstring& key, bool &dest, int instance)
{
	_tstring str;
	if (!Get(key, str, instance)) return(false);

	str = toLowerCase(str);

	if(str == _T("true"))
		dest = true;
	else
		dest = false;
	return(true);
}

bool CPropBag::Get(const _tstring& key, XmlDataType *dest, int instance)
{
	ASSERT(dest!=NULL, _T("CPropBag::Get  ->  Cannot save to a NULL object"));

	_tstring str;
	if (!Get(key, str, instance)) return(false);
	dest->FromString(str.c_str());
	return(true);
}

bool CPropBag::Get(const _tstring& key, CPropBag &dest, int instance)
{
	if(GetNumInstances(key) <= 0)
	{
		return false;
	}

	if (m_Data.find(key) == m_Data.end())
	{
		// key doesn't exist
		return(false);
	}

	// find the first instance
	PropertyMap::iterator iter = m_Data.lower_bound(key);

	// go to the desired instance
	for (int q=0; q < instance; q++)
	{
		iter++;
	}

	// Get the data
	CPropBag *bag = dynamic_cast<CPropBag*>(iter->second);

	// Error check
	if (NULL == bag)
	{
		TRACE(_T("Wasn't a bag"));
		return(false);
	}

	// set the data and return with success
	dest = *bag;
	return(true);
}

int CPropBag::GetNumInstances(_tstring key)
{
	return (int)(m_Data.count(key));
}

void CPropString::MakeStringSafeForPropBag(_tstring &str)
{
	_tstring ampEntity = _T("&amp;");
	_tstring ltEntity = _T("&lt;");
	_tstring gtEntity = _T("&gt;");

	_tstring ampStr = _T("&");
	_tstring ltStr = _T("<");
	_tstring gtStr = _T(">");

	// replace all &'s with &amp;
	Replace(str, ampStr, ampEntity);

	// replace all <'s with &lt;
	Replace(str, ltStr, ltEntity);

	// replace all >'s with &gt's
	Replace(str, gtStr, gtEntity);
}
void CPropString::RestoreOrigFromSafeString(_tstring &str)
{
	_tstring ampEntity = _T("&amp;");
	_tstring ltEntity = _T("&lt;");
	_tstring gtEntity = _T("&gt;");

	_tstring ampStr = _T("&");
	_tstring ltStr = _T("<");
	_tstring gtStr = _T(">");

	// replace all &amp; with &
	Replace(str, ampEntity, ampStr);

	// replace all &lt; with <
	Replace(str, ltEntity, ltStr);

	// replace all &gt; with >
	Replace(str, gtEntity, gtStr);
}

void CPropBag::Clear()
{
	for (PropertyMap::iterator propIter = m_Data.begin(); propIter != m_Data.end(); ++propIter)
	{
		delete(propIter->second);
	}
	m_Data.clear();
}

void CPropBag::Copy(const CPropBag &r)
{
	Clear();
	_tstring rsave = r.Save();
	LoadFromString(rsave);
}

void CPropBag::Merge(const CPropBag &newstuff, bool overwrite)
{
  for (PropertyMap::const_iterator newiter = newstuff.m_Data.begin(); newiter != newstuff.m_Data.end(); ++newiter) {
    CPropString *pStr = dynamic_cast<CPropString*>(newiter->second);
    CPropBag *pBag = dynamic_cast<CPropBag *>(newiter->second);

    if (pStr) {
		  // if it doesn't already exist here, or if overwrite is set,
		  if (m_Data.find(newiter->first) != m_Data.end() || overwrite) {
		    // add it to this bag
		    Remove(newiter->first);
		    Add(newiter->first, pStr->GetData(), false);
		  }
	  }
    if (pBag) {
		  // if it doesn't exist, just add the bag (easy!)
		  PropertyMap::iterator origbagiter = m_Data.find(newiter->first);
		  if (origbagiter == m_Data.end()) {
			  Add(newiter->first, *pBag);
		  }
		  else {
			  // it exists, so we need to recurse into the subbag
			  CPropBag *origbag = (CPropBag *)(origbagiter->second);
			  if (origbag) {
				  origbag->Merge(*pBag, overwrite);
        } else {
          // it's a _tstring, and we have a bag...
          // if we should overwrite, do so.
          if (overwrite) {
            Remove(newiter->first);
            Add(newiter->first, *pBag);
          }
        }
		  }
	  }
  }
}

bool CPropBag::operator==(const CPropBag &r)
{
  if (r.m_Data.size() != m_Data.size()) return(false); // that was easy

  PropertyMap::const_iterator riter = r.m_Data.begin();
  PropertyMap::const_iterator liter = m_Data.begin();

  for (; riter != r.m_Data.end() && liter != m_Data.end(); ++riter, ++liter) {
	if ((liter->first) != (riter->first))   return(false);
	if (*(liter->second) != *(riter->second)) return(false);
  }
  return(true);
}


}; // namespace
