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

PropertyBag::PropertyBag(void)
{
	clear();
}

PropertyBag::PropertyBag( const PropertyBag &r ) : PropertyBagItem(r)
{
	copy(r);
}

PropertyBag::PropertyBag( const _tstring &s )
{
	clear();
	LoadFromString(s);
}

PropertyBag::~PropertyBag(void)
{}

void PropertyBag::add(const _tstring& key, const _tstring &contents, bool convert)
{
	data.insert(make_pair(key, new PropertyBagString(key, contents, convert)));
}

void PropertyBag::add(const _tstring& key, int data)
{
	add(key, itoa(data));
}

void PropertyBag::add(const _tstring& key, size_t data)
{
	add(key, itoa((int)data));
}

void PropertyBag::add(const _tstring& key, double data)
{
	add(key, ftoa((float)data, 4));
}

void PropertyBag::add(const _tstring& key, bool data)
{
	if(data)
		add(key, _tstring(_T("true")));
	else
		add(key, _tstring(_T("false")));
}

void PropertyBag::add(const _tstring& key, float data)
{
	add(key, ftoa(data, 4));
}

void PropertyBag::add(const _tstring& key, const XmlDataType *pData)
{
	ASSERT(pData!=NULL, _T("CPropBag::Add  ->  Cannot add a NULL object to the Prop Bag"));

	PropertyBagString *newprop = new PropertyBagString(pData->ToString(), false);

	newprop->setName(key);
	data.insert(make_pair(key, newprop));
}

void PropertyBag::add(const _tstring& key, const PropertyBag &contents)
{
	if(!contents.getData().empty())
	{
		data.insert(make_pair(key, new PropertyBag(contents)));
	}
}

void PropertyBag::remove(const _tstring &key)
{
	if(data.empty()) return;

	PropertyMap::iterator i;
	PropertyMap::iterator stop = data.upper_bound(key);
	stop++;

	i = data.lower_bound(key);
	
	if(i == data.end()) return; // nothing to remove

	do
	{
		delete((*i).second);
		data.erase(i);
	} while (data.size() && i++ != stop);
}

void PropertyBag::remove(const _tstring &key, int instance)
{
	if(data.empty()) return;

	PropertyMap::iterator i;
	PropertyMap::iterator stop = data.upper_bound(key);
	stop++;

	i = data.lower_bound(key);
	
	if(i == data.end()) return; // nothing to remove
	
	for(int q=0; q < instance; q++, i++);
	
	if(i != data.end())
	{
		do
		{
			delete((*i).second);
			data.erase(i);
		} while (data.size() && i++ != stop);
	}
}

_tstring PropertyBagString::save(int) const
{
	return(itemData);
}

void PropertyBag::saveToFile(const _tstring &fileName, int indentLevel) const
{
	File file;
	file.write(save(indentLevel));
	file.saveFile(fileName, false);
}

_tstring PropertyBag::save(int indentlevel) const
{
	// This function was commented by Andrew Fox in an effort to understand it
	// Modified a bit by Andrew Fox as well

	_tstring out;
	_tstring indent(indentlevel, '\t');

	// Cycle through all the tags in this bag
	for (PropertyMap::const_iterator i = data.begin(); i != data.end(); i++)
	{
		PropertyBagItem *data = (*i).second; // Tag Value can be retrieved from this object
		_tstring key = (*i).first;       // Tag Name
		_tstring line;                   // TagValue is put in here
		_tstring withname;               // <TagName>TagValue</TagName> is put in here

		// Dynamic cast will return NULL if data is not a CPropBag object
		bool IsBag = dynamic_cast<PropertyBag*>(data) != NULL;

		// Get the tag value as a _tstring.  CPropItem::Save() does this for us.
		// Note that if the data is a CPropString, then line is simple the _tstring's value
		// But, if the data is a CPropBag, then this function recurses to make line = bag
		// (If the conversion exists, have the data be indented like a property bag should be)
		line = data->save( IsBag ? indentlevel+1 : indentlevel);

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

bool PropertyBag::LoadMerge(const _tstring &filename)
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

bool PropertyBag::Load(const _tstring &filename)
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

bool PropertyBag::LoadMergeFromString(const _tstring &data)
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
						insertTag(tagname, tagvalue);

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
	}

	return(true);
}

bool PropertyBag::LoadFromString(const _tstring &contents)
{
	data.clear();

	return LoadMergeFromString(contents);
}

void PropertyBag::insertTag(const _tstring &tagname, const _tstring &tagvalue)
{
	// a < and > mean it's a bag within a bag
	if(tagvalue.find(_T("<")) != _tstring::npos && tagvalue.find(_T(">")) != _tstring::npos)
	{
		// it's a bag.
		PropertyBag *newbag = new PropertyBag;

		ASSERT(newbag!=NULL, _T("CPropBag::PutTagIntoBag  ->  newbag was NULL"));

		newbag->LoadFromString(tagvalue);
		data.insert(make_pair(tagname, newbag));
	}
	else
	{
		// it's a primitive data type.
		PropertyBagString *newstr = new PropertyBagString(tagvalue, false);

		ASSERT(newstr!=NULL, _T("CPropBag::PutTagIntoBag  ->  newstr was NULL"));

		newstr->setName(tagname);
		data.insert(make_pair(tagname, newstr));
	}
}

bool PropertyBag::get(const _tstring& key, _tstring &dest, size_t instance) const
{
	if (data.find(key) == data.end()) return(false);
	PropertyMap::const_iterator iter = data.lower_bound(key);
	for(size_t q=0; q < instance; q++) iter++;
	dest = (iter->second)->save();

	return(true);
}

bool PropertyBag::get(const _tstring& key, int &dest, size_t instance) const
{
	_tstring str;
	if (!get(key, str, instance)) return(false);
	dest = stoi(str);
	return(true);
}

bool PropertyBag::get(const _tstring& key, size_t &dest, size_t instance) const
{
	_tstring str;
	if (!get(key, str, instance)) return(false);
	dest = static_cast<size_t>(stoi(str));
	return(true);
}

bool PropertyBag::get(const _tstring& key, double &dest, size_t instance) const
{
	_tstring str;
	if (!get(key, str, instance)) return(false);
	dest = stof(str);
	return(true);
}

bool PropertyBag::get(const _tstring& key, float &dest, size_t instance) const
{
	_tstring str;
	if (!get(key, str, instance)) return(false);
	dest = stof(str);
	return(true);
}

bool PropertyBag::get(const _tstring& key, bool &dest, size_t instance) const
{
	_tstring str;
	if (!get(key, str, instance)) return(false);

	str = toLowerCase(str);

	if(str == _T("true"))
		dest = true;
	else
		dest = false;
	return(true);
}

bool PropertyBag::get(const _tstring& key, XmlDataType *dest, size_t instance) const
{
	ASSERT(dest!=0, _T("Cannot save to a NULL object"));

	_tstring str;
	if (!get(key, str, instance)) return(false);
	dest->FromString(str.c_str());
	return(true);
}

bool PropertyBag::get(const _tstring& key, PropertyBag &dest, size_t instance) const
{
	if(getNumInstances(key) <= 0)
	{
		return false;
	}

	if (data.find(key) == data.end())
	{
		// key doesn't exist
		return(false);
	}

	// find the first instance
	PropertyMap::const_iterator iter = data.lower_bound(key);

	// go to the desired instance
	for(size_t q=0; q < instance; q++)
		iter++;

	// Get the data
	PropertyBag *bag = dynamic_cast<PropertyBag*>(iter->second);

	// Error check
	if(NULL == bag)
	{
		FAIL(_T("Wasn't a bag"));
		return(false);
	}

	// set the data and return with success
	dest = *bag;
	return(true);
}

size_t PropertyBag::getNumInstances(const _tstring &key) const
{
	return data.count(key);
}

const _tstring& PropertyBagString::makeStringSafe(const _tstring &str)
{
	_tstring ampEntity = _T("&amp;");
	_tstring ltEntity = _T("&lt;");
	_tstring gtEntity = _T("&gt;");

	_tstring ampStr = _T("&");
	_tstring ltStr = _T("<");
	_tstring gtStr = _T(">");

	// replace all &'s with &amp;
	str = Replace(str, ampStr, ampEntity);

	// replace all <'s with &lt;
	str = Replace(str, ltStr, ltEntity);

	// replace all >'s with &gt's
	str = Replace(str, gtStr, gtEntity);
}

const _tstring& PropertyBagString::restoreFromSafeString(const _tstring &str)
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

bool PropertyBagString::operator==( const PropertyBagItem &r ) const
{
	return(itemData == static_cast<const PropertyBagString &>(r).itemData);
}

const _tstring& PropertyBagString::getName( void ) const
{
	return itemName;
}

const _tstring& PropertyBagString::setName( const _tstring &name )
{
	itemName = name;
	return itemName;
}

const _tstring& PropertyBagString::setData( const _tstring &data, bool convert )
{
	if(convert)
	{
		itemData = makeStringSafe(data);
		itemHasBeenConverted = true;
	}
	else
	{
		itemData = data;
		itemHasBeenConverted = false;
	}

	return itemData;
}

_tstring PropertyBagString::getData( void ) const
{
	if(itemHasBeenConverted)
	{
		return restoreFromSafeString(itemData);
	}
	else
	{
		return itemData;
	}
}

PropertyBagString::PropertyBagString( const _tstring &data, bool convert )
{
	setData(data, convert);
}

PropertyBagString::PropertyBagString( const _tstring &name, const _tstring &data, bool convert )
{
	setData(data, convert);
	setName(name);
}

PropertyBagString::~PropertyBagString( void )
{

}

void PropertyBag::clear()
{
	for (PropertyMap::iterator propIter = data.begin(); propIter != data.end(); ++propIter)
	{
		delete(propIter->second);
	}
	data.clear();
}

void PropertyBag::copy(const PropertyBag &r)
{
	clear();
	_tstring rsave = r.save();
	LoadFromString(rsave);
}

void PropertyBag::merge(const PropertyBag &newstuff, bool overwrite)
{
  for (PropertyMap::const_iterator newiter = newstuff.data.begin(); newiter != newstuff.data.end(); ++newiter) {
    PropertyBagString *pStr = dynamic_cast<PropertyBagString*>(newiter->second);
    PropertyBag *pBag = dynamic_cast<PropertyBag *>(newiter->second);

    if (pStr) {
		  // if it doesn't already exist here, or if overwrite is set,
		  if (data.find(newiter->first) != data.end() || overwrite) {
		    // add it to this bag
		    remove(newiter->first);
		    add(newiter->first, pStr->getData(), false);
		  }
	  }
    if (pBag) {
		  // if it doesn't exist, just add the bag (easy!)
		  PropertyMap::iterator origbagiter = data.find(newiter->first);
		  if (origbagiter == data.end()) {
			  add(newiter->first, *pBag);
		  }
		  else {
			  // it exists, so we need to recurse into the subbag
			  PropertyBag *origbag = (PropertyBag *)(origbagiter->second);
			  if (origbag) {
				  origbag->merge(*pBag, overwrite);
        } else {
          // it's a _tstring, and we have a bag...
          // if we should overwrite, do so.
          if (overwrite) {
            remove(newiter->first);
            add(newiter->first, *pBag);
          }
        }
		  }
	  }
  }
}

bool PropertyBag::operator==(const PropertyBag &r) const
{
  if (r.data.size() != data.size()) return(false); // that was easy

  PropertyMap::const_iterator riter = r.data.begin();
  PropertyMap::const_iterator liter = data.begin();

  for (; riter != r.data.end() && liter != data.end(); ++riter, ++liter) {
	if ((liter->first) != (riter->first))   return(false);
	if (*(liter->second) != *(riter->second)) return(false);
  }
  return(true);
}

bool PropertyBag::operator==( const PropertyBagItem &r ) onst
{
	try {
		const PropertyBag &rBag = (const PropertyBag &)(r);
		return((*this) == rBag);
	} catch(...) { return(false); }
}

PropertyBag & PropertyBag::operator=( const PropertyBag &r )
{
	copy(r); return(*this);
}



PropertyBagItem::~PropertyBagItem( void )
{

}

PropertyBagItem::PropertyBagItem( void )
{

}

bool PropertyBagItem::operator!=( const PropertyBagItem &r )
{
	return(!((*this) == r));
}

} // namespace Engine
