/*
Modified by Andrew Fox in 2003-2007
E-Mail: mailto:andrewfox@cmu.edu

Original Source:
McCuskey, Mason. "Game Programming Tricks of the Trade".
	"Trick 15: Serialization Using XML Property Bags".
	Premier Press. 2002.
*/

#include "stdafx.h"
#include "file.h"
#include "profile.h"
#include "PropertyBag.h"

// stdafx.cpp
namespace Engine
{
int stoi(const _tstring &s);
float stof(const _tstring &s);
}

namespace Engine {

PropertyBagItem::~PropertyBagItem(void)
{}

PropertyBagItem::PropertyBagItem(void)
{}

bool PropertyBagItem::operator!=( const PropertyBagItem &r )
{
	return(!((*this) == r));
}

_tstring PropertyBagString::save(int) const
{
	return(itemData);
}

_tstring PropertyBagString::makeStringSafe(const _tstring &str)
{
	/*
	replace all &'s with &amp's
	replace all <'s with &lt's
	replace all >'s with &gt's
	*/

	return replace (
				replace
				(
					replace(str, _T("&"), _T("&amp;")),
					_T("<"),
					_T("&lt;")
				),
				_T(">"),
				_T("&gt;")
			);
}

_tstring PropertyBagString::restoreFromSafeString(const _tstring &str)
{
	/*
	replace all &amp's with &'s
	replace all &lt's with <'s
	replace all &gt's with >'s
	*/

	return replace (
				replace
				(
					replace(str, _T("&amp;"), _T("&")),
					_T("&lt;"),
					_T("<")
				),
				_T("&gt;"),
				_T(">")
			);
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

PropertyBagString::~PropertyBagString(void)
{}

PropertyBag::PropertyBag(void)
{
	clear();
}

PropertyBag::PropertyBag(const PropertyBag &r)
: PropertyBagItem(r)
{
	copy(r);
}

PropertyBag::PropertyBag(const _tstring &s)
{
	clear();
	loadMergeFromString(s, true);
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
	add(key, _tstring((data) ? _T("true") : _T("false")));
}

void PropertyBag::add(const _tstring& key, float data)
{
	add(key, ftoa(data, 4));
}

void PropertyBag::add(const _tstring& key, const XmlDataType *pData)
{
	ASSERT(pData!=0, _T("Cannot add a NULL object to the Prop Bag"));

	data.insert(  make_pair(key, new PropertyBagString(key, pData->ToString(), false))  );
}

void PropertyBag::add(const _tstring& key, const PropertyBag &contents)
{
	if(!contents.data.empty())
	{
		data.insert(make_pair(key, new PropertyBag(contents)));
	}
}

void PropertyBag::remove(const _tstring &key)
{
	while(data.count(key)!=0)
	{
		PropertyMap::iterator i = data.lower_bound(key);
		delete(i->second);
		data.erase(i);
	}

	ASSERT(getNumInstances(key)==0, _T("Failed to remove items sharing key: ") + key);
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

void PropertyBag::saveToFile(const _tstring &fileName, int indentLevel) const
{
	File file;
	file.write(save(indentLevel));
	file.saveFile(fileName, false);
}

_tstring PropertyBag::save(int indentlevel) const
{
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

void PropertyBag::loadFromFile(const _tstring &filename, bool merge)
{
	if(!File::isFileOnDisk(filename))
	{
		FAIL(_T("File not found: ") + filename);
		return;
	}

	File file(filename, false);

	if(!file.loaded())
	{
		FAIL(_T("File failed to load: ") + filename);
		return;
	}

	_tstring fileContents;

	fileContents.resize(file.getSize()); // Resize to fit the data

	// Copy file data
	{
		char *buffer = new char[file.getSize()];
		file.read(buffer, file.getSize());
		fileContents = toTString(buffer);
		delete [] buffer;
	}

	// Load / Merge the data
	if(!merge)
	{
		data.clear();
	}

	if(!loadMergeFromString(fileContents, true))
	{
		FAIL(_T("Failed to merge file contents on load: ") + filename);
	}
}

bool PropertyBag::loadMergeFromString(const _tstring &data, bool allowInheritance)
{
	enum eElanPropBagReadState
	{
		SearchingForOpenTag,
		ReadingOpenTag,
		ReadingTagContents
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
				// Add the character to the contents of the tag
				tagvalue += b;

				// If we are possibly reading the closing tag now
				if(isPossibleClosingTag)
				{
					// Build the _tstring for what may be the closing tag
					possibleClosingTag += b;

					// Otherwise, search for the real closing tag
					if(possibleClosingTag == closetag) // if(tagvalue.find(closetag) != _tstring::npos)
					{
						// Remove that closing tag from the tag contents
						tagvalue = replace(tagvalue, closetag, _T(""));

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

	// Possibly inherit properties from another file
	if(allowInheritance)
	{
		const _tstring inheritTag = _T("@inherit");
		_tstring parentFileName = _T("nill");
		if(get(inheritTag, parentFileName))
		{
			PropertyBag data, prototype;
			prototype.loadFromFile(parentFileName); // may recurse

			// Merge in the data specific to this creature (or add it to begin with)
			data = prototype;
			data.merge(*this, true);

			// Remove the <@inherit> ... </@inherit> element
			data.remove(inheritTag);

			// 'data' contains the entire merged structure
			(*this) = data;
		}
	}

	return(true);
}

void PropertyBag::insertTag(const _tstring &tagName, const _tstring &tagValue)
{
	PropertyBagItem *item = 0;

	// a < and > mean it's a bag within a bag
	if(tagValue.find(_T("<")) != _tstring::npos && tagValue.find(_T(">")) != _tstring::npos)
	{
		item = new PropertyBag(tagValue);
	}
	else
	{
		item = new PropertyBagString(tagName, tagValue, false);
	}

	data.insert(make_pair(tagName, item));
}

bool PropertyBag::get(const _tstring& key, _tstring &dest, size_t instance) const
{
	if(data.find(key) == data.end())
		return(false);

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

	dest = (str == _T("true"));

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
	if(data.find(key)==data.end())
		return false;

	PropertyMap::const_iterator iter;

	// go to the desired instance
	ASSERT(instance<getNumInstances(key), _T("parameter \'instance\' is incorrect: ") + itoa((int)instance));
	iter = data.lower_bound(key);
	for(size_t q=0; q < instance; q++) iter++;

	ASSERT(dynamic_cast<PropertyBag*>(iter->second)!=0, _T("iter->second cannot be cast to a PropertyBag object"));
	dest = dynamic_cast<PropertyBag&>(*iter->second);

	return true;
}

_tstring PropertyBag::getString(const string &key, size_t instance) const
{
    ASSERT(getNumInstances(key) > instance,
           _T("Not enough instances of key: ") + key +
           _T(" (wanted instance #") + itoa((int)instance) + 
           _T(" of only ") + itoa((int)getNumInstances(key)) + _T(")"));

	_tstring x = _T("nill");
	if(!get(key, x, instance))
	{
        FAIL(_T("Failed to get string from PropertyBag: ") + key +
             _T(" (wanted instance #") + itoa((int)instance) + 
             _T(" of ") + itoa((int)getNumInstances(key)) + _T(")"));
	}
	return x;
}

int PropertyBag::getInt(const _tstring &key, size_t instance) const
{
	int x = 0xdeadbeaf;
	if(!get(key, x, instance))
	{
		FAIL( _T("Failed to get int from PropertyBag: ") + key + _T(" (instance #)") + itoa((int)instance) );
	}
	return x;
}

size_t PropertyBag::getSizeT(const _tstring &key, size_t instance) const
{
	size_t x = 0xdeadbeaf;
	if(!get(key, x, instance))
	{
		FAIL( _T("Failed to get size_t from PropertyBag: ") + key + _T(" (instance #)") + itoa((int)instance) );
	}
	return x;
}

double PropertyBag::getDouble(const _tstring &key, size_t instance) const
{
	double x = 0.0;
	if(!get(key, x, instance))
	{
		FAIL( _T("Failed to get double from PropertyBag: ") + key + _T(" (instance #)") + itoa((int)instance) );
	}
	return x;
}

float PropertyBag::getFloat(const _tstring &key, size_t instance) const
{
	float x = 0.0f;
	if(!get(key, x, instance))
	{
		FAIL( _T("Failed to get float from PropertyBag: ") + key + _T(" (instance #)") + itoa((int)instance) );
	}
	return x;
}

bool PropertyBag::getBool(const _tstring &key, size_t instance) const
{
	bool x = false;
	if(!get(key, x, instance))
	{
		FAIL( _T("Failed to get bool from PropertyBag: ") + key + _T(" (instance #)") + itoa((int)instance) );
	}
	return x;
}

PropertyBag PropertyBag::getBag(const _tstring &key, size_t instance) const
{
	PropertyBag x;
	if(!get(key, x, instance))
	{
		FAIL( _T("Failed to get PropertyBag from PropertyBag: ") + key + _T(" (instance #)") + itoa((int)instance) );
	}
	return x;
}

size_t PropertyBag::getNumInstances(const _tstring &key) const
{
	return data.count(key);
}

void PropertyBag::clear(void)
{
	for(PropertyMap::iterator propIter = data.begin(); propIter != data.end(); ++propIter)
	{
		delete(propIter->second);
	}
	data.clear();
}

void PropertyBag::copy(const PropertyBag &copyMe)
{
	clear();

#if 0
	loadMergeFromString(copyMe.save(), false);
#else
	for(PropertyMap::const_iterator iter = copyMe.data.begin();
	    iter != copyMe.data.end();
	    ++iter)
	{
		// alias the iterator's pair
		const _tstring &tagName = iter->first;
		const PropertyBagItem *tagItem = iter->second;

		const PropertyBagString *pStr = dynamic_cast<const PropertyBagString*>(tagItem);

		if(pStr) // if the item is a PropertyBagString
		{
			add(tagName, pStr->getData(), false);
		}
		else
		{
			const PropertyBag *pBag = dynamic_cast<const PropertyBag*>(tagItem);

			if(pBag) // if the item is a PropertyBag
			{
				add(tagName, *pBag);
			}
		}
	} // loop
#endif

	ASSERT(copyMe == (*this), _T("Failed to copy bag"));
}

void PropertyBag::merge(const PropertyBag &newstuff, bool overwrite)
{
	for(PropertyMap::const_iterator newiter = newstuff.data.begin();
	    newiter != newstuff.data.end();
	    ++newiter)
	{
		// alias the iterator's pair
		const _tstring &tagName = newiter->first;
		const PropertyBagItem *tagItem = newiter->second;

		const bool tagAlreadyExists = data.find(tagName)!=data.end();

		const PropertyBagString *pStr = dynamic_cast<const PropertyBagString*>(tagItem);
		const PropertyBag *pBag = dynamic_cast<const PropertyBag*>(tagItem);

		if(pStr) // if the item is a PropertyBagString
		{
			if(!tagAlreadyExists)
			{
				add(tagName, pStr->getData(), false);
			}
			else if(overwrite && tagAlreadyExists)
			{
				remove(tagName);
				add(tagName, pStr->getData(), false);
			}
		}

		if(pBag) // if the item is a PropertyBag
		{
			// if it doesn't exist, just add the bag (easy!)
			if(!tagAlreadyExists)
			{
				add(tagName, *pBag);
			}
			else
			{
				PropertyBagItem *originalItem = data.find(tagName)->second;
				ASSERT(originalItem != 0, _T("originalItem was null!"));

				/*
				The tag we are trying to add is a PropertyBag,
				but it is possible that the existing tag is a
				PropertyBagString.  If this is the case, we
				should *always* overwrite the existing tag.
				*/

				if(dynamic_cast<const PropertyBagString*>(originalItem) != 0)
				{
					remove(tagName);
					add(tagName, *pBag);
				}
				else
				{
					(dynamic_cast<PropertyBag&>(*originalItem)).merge(*pBag, overwrite);
				}
			}
		}
	} // loop
}

bool PropertyBag::operator==(const PropertyBag &r) const
{
	if(r.data.size() != data.size())
		return false;

	PropertyMap::const_iterator riter = r.data.begin();
	PropertyMap::const_iterator liter = data.begin();

	for(; riter != r.data.end() && liter != data.end(); ++riter, ++liter)
	{
		if((liter->first) != (riter->first))
			return(false);

		if(*(liter->second) != *(riter->second))
			return(false);
	}

	return true;
}

bool PropertyBag::operator==( const PropertyBagItem &r ) const
{
	return(  (*this) == dynamic_cast<const PropertyBag &>(r)  );
}

PropertyBag & PropertyBag::operator=( const PropertyBag &r )
{
	copy(r);
	return(*this);
}

} // namespace Engine
