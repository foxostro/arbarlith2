/*
Modified by Andrew Fox in 2003-2007,2009,2010
E-Mail: mailto:foxostro@gmail.com

Original Source:
McCuskey, Mason. "Game Programming Tricks of the Trade".
	"Trick 15: Serialization Using XML Property Bags".
	Premier Press. 2002.
*/

#include <boost/lexical_cast.hpp>

#include "stdafx.h"
#include "file.h"
#include "profile.h"
#include "PropertyBagImpl.h"

namespace Engine {

PropertyBagItem::~PropertyBagItem(void) {}

PropertyBagItem::PropertyBagItem(void) {}

bool PropertyBagItem::operator!=( const PropertyBagItem &r )
{
	return(!((*this) == r));
}

string PropertyBagString::save(int) const
{
	return(itemData);
}

string PropertyBagString::makeStringSafe(const string &str)
{
	/*
	replace all &'s with &amp's
	replace all <'s with &lt's
	replace all >'s with &gt's
	*/

	return replace(replace(replace(str, "&", "&amp;"),
										"<", "&lt;"),
										">", "&gt;");
}

string PropertyBagString::restoreFromSafeString(const string &str)
{
	/*
	replace all &amp's with &'s
	replace all &lt's with <'s
	replace all &gt's with >'s
	*/

	return replace (replace(replace(str, "&amp;", "&"),
					                     "&lt;",  "<"),
					                     "&gt;",  ">");
}

bool PropertyBagString::operator==( const PropertyBagItem &r ) const
{
	return(itemData == static_cast<const PropertyBagString &>(r).itemData);
}

const string& PropertyBagString::getName( void ) const
{
	return itemName;
}

const string& PropertyBagString::setName( const string &name )
{
	itemName = name;
	return itemName;
}

const string& PropertyBagString::setData( const string &data, bool convert )
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

string PropertyBagString::getData( void ) const
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

PropertyBagString::PropertyBagString(const string &data, bool convert)
{
	setData(data, convert);
}

PropertyBagString::PropertyBagString(const string &name,
                                     const string &data,
                                     bool convert )
{
	setData(data, convert);
	setName(name);
}

PropertyBagString::~PropertyBagString(void)
{}

PropertyBagImpl::PropertyBagImpl(void)
{
	clear();
}

PropertyBagImpl::PropertyBagImpl(const PropertyBagImpl &r)
: PropertyBagItem(r)
{
	copy(r);
}

PropertyBagImpl::PropertyBagImpl(const string &s)
{
	clear();
	loadMergeFromString(s, true);
}

PropertyBagImpl::~PropertyBagImpl(void)
{}

void PropertyBagImpl::add(const string& key, const char* contents, bool convert)
{
	add(key, string(contents), convert);
}

void PropertyBagImpl::add(const string& key, const string &contents, bool convert)
{
	data.insert(make_pair(key, new PropertyBagString(key, contents, convert)));
}

void PropertyBagImpl::add(const string& key, int data)
{
	add(key, itoa(data));
}

void PropertyBagImpl::add(const string& key, size_t data)
{
	add(key, itoa((int)data));
}

void PropertyBagImpl::add(const string& key, double data)
{
	add(key, ftoa((float)data, 4));
}

void PropertyBagImpl::add(const string& key, bool data)
{
	if(data)
	{
		add(key, "true");
	}
	else
	{
		add(key, "false");
	}
}

void PropertyBagImpl::add(const string& key, float data)
{
	add(key, ftoa(data, 4));
}

void PropertyBagImpl::add(const string& key, const PropertyBagImpl &contents)
{
	if(!contents.data.empty())
	{
		data.insert(make_pair(key, new PropertyBagImpl(contents)));
	}
}

void PropertyBagImpl::remove(const string &key)
{
	while(data.count(key)!=0)
	{
		PropertyMap::iterator i = data.lower_bound(key);
		delete(i->second);
		data.erase(i);
	}

	ASSERT(count(key)==0,
	       "Failed to remove items sharing key: " + key);
}

void PropertyBagImpl::remove(const string &key, int instance)
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

void PropertyBagImpl::saveToFile(const string &fileName, int indentLevel) const
{
	File file;
	file.write(save(indentLevel));
	file.saveFile(fileName, false);
}

string PropertyBagImpl::save(int indentlevel) const
{
	string out;
	string indent(indentlevel, '\t');

	// Cycle through all the tags in this bag
	for (PropertyMap::const_iterator i = data.begin(); i != data.end(); i++)
	{
		PropertyBagItem *data = (*i).second; // Tag Value can be retrieved
		string key = (*i).first; // Tag Name
		string line; // TagValue is put in here
		string withname; // <TagName>TagValue</TagName> in here

		// Dynamic cast will return NULL if data is not a CPropBag object
		bool IsBag = dynamic_cast<PropertyBagImpl*>(data) != NULL;

		/*
		Get the tag value as a string.  CPropItem::Save() does this for us.
		Note that if the data is a CPropString, then line is simple the
		string's value. However, if the data is a CPropBag, then this function
		recurses to make line = bag (If the conversion exists, have the data
		be indented like a property bag should be)
		*/
		line = data->save(IsBag ? indentlevel+1 : indentlevel);

		if(IsBag)
		{
			// Put a new line after the opening tag, then indent and put the data
			withname  = indent;
			withname += "<";
			withname += key;
			withname += ">\n";
			withname += line;
			withname += indent;
			withname += "</";
			withname += key;
			withname += ">\n";
		}
		else
		{
			// Everything on one line
			withname  = indent;
			withname += "<";
			withname += key;
			withname += ">";
			withname += line;
			withname += "</";
			withname += key;
			withname += ">\n";
		}

		// Add this tag to the total string dump of the property bag
		out += withname;
	}

	return(out);
}

void PropertyBagImpl::loadFromFile(const string &filename, bool merge)
{
	if(!File::isFileOnDisk(filename))
	{
		FAIL("File not found: " + filename);
		return;
	}

	File file(filename, false);

	if(!file.loaded())
	{
		FAIL("File failed to load: " + filename);
		return;
	}

	string fileContents;

	fileContents.resize(file.getSize()); // Resize to fit the data

	// Copy file data
	{
		char *buffer = new char[file.getSize()];
		file.read(buffer, file.getSize());
		fileContents = buffer;
		delete [] buffer;
	}

	// Load / Merge the data
	if(!merge)
	{
		data.clear();
	}

	if(!loadMergeFromString(fileContents, true))
	{
		FAIL("Failed to merge file contents on load: " + filename);
	}
}

bool PropertyBagImpl::loadMergeFromString(const string &data, bool allowInheritance)
{
	enum eElanPropBagReadState
	{
		SearchingForOpenTag,
		ReadingOpenTag,
		ReadingTagContents
	} curstate = SearchingForOpenTag;

	string tagname;
	string tagvalue;
	string closetag;

	unsigned char previous=0;
	string possibleClosingTag;
	bool isPossibleClosingTag=false;

	for(string::const_iterator iter = data.begin(); iter != data.end(); ++iter)
	{
		const unsigned char b = (unsigned char)(*iter);

		switch(curstate)
		{
			case SearchingForOpenTag:
			{
				if (b == '<')
				{
					// we've found our open tag!
					curstate = ReadingOpenTag;
				}
			}
			break;

			case ReadingOpenTag:
			{
				if (b == '>')
				{
					// end of tag
					curstate = ReadingTagContents;
					closetag = "</" + tagname + ">";
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
					// Build the string for what may be the closing tag
					possibleClosingTag += b;

					// Otherwise, search for the real closing tag
					if(possibleClosingTag == closetag)
					{
						// Remove that closing tag from the tag contents
						tagvalue = replace(tagvalue, closetag, "");

						// Put the completed tag into the bag here
						insertTag(tagname, tagvalue);

						// Reset the state
						curstate = SearchingForOpenTag;
						tagname = "";
						tagvalue = "";
					}

					// Has it become impossible that this is the closing tag?
					if(b == '>')
					{
						isPossibleClosingTag = false;
					}
				}

				// Have we begun to encounter what may be the closing tag?
				if(previous == '<' && b == '/')
				{
					isPossibleClosingTag = true;
					possibleClosingTag = "</";
				}
			}
			break;
		}; // end switch

		previous = b;
	}

	// Possibly inherit properties from another file
	if(allowInheritance)
	{
		const string inheritTag = "@inherit";
		string parentFileName = "nill";
		if(get(inheritTag, parentFileName))
		{
			PropertyBagImpl data, prototype;
			prototype.loadFromFile(parentFileName); // may recurse

			// Merge or add data specific to this creature
			data = prototype;
			data.merge(*this, true);

			// Remove the <@inherit> ... </@inherit> element
			data.remove(inheritTag);

			/*
			And replace it with another special tag.
			The aim is to prevent re-inheritance of the same base,
			and still allow clients to see lineage.
			*/
			if(data.count("@parentFileName") < 1) // only the topmost ancestor
			{
				data.add("@parentFileName", parentFileName);
			}

			// 'data' contains the entire merged structure
			(*this) = data;
		}
	}

	return(true);
}

void PropertyBagImpl::insertTag(const string &tagName, const string &tagValue)
{
	PropertyBagItem *item = 0;

	// a < and > mean it's a bag within a bag
	if(tagValue.find("<") != string::npos && tagValue.find(">") != string::npos)
	{
		item = new PropertyBagImpl(tagValue);
	}
	else
	{
		item = new PropertyBagString(tagName, tagValue, false);
	}

	data.insert(make_pair(tagName, item));
}

bool PropertyBagImpl::get(const string& key, string &dest, size_t instance) const
{
	if(data.find(key) == data.end())
		return(false);

	PropertyMap::const_iterator iter = data.lower_bound(key);

	for(size_t q=0; q < instance; q++) iter++;

	dest = (iter->second)->save();

	return(true);
}

bool PropertyBagImpl::get(const string& key, int &dest, size_t instance) const
{
	string str;
	if (!get(key, str, instance)) return(false);
	dest = stoi(str);
	return(true);
}

bool PropertyBagImpl::get(const string& key, unsigned int &dest, size_t instance) const
{
	string str;
	if (!get(key, str, instance)) return(false);
	dest = boost::lexical_cast<unsigned int>(str);
	return(true);
}

bool PropertyBagImpl::get(const string& key, size_t &dest, size_t instance) const
{
	string str;
	if (!get(key, str, instance)) return(false);
	dest = boost::lexical_cast<size_t>(str);
	return(true);
}

bool PropertyBagImpl::get(const string& key, double &dest, size_t instance) const
{
	string str;
	if (!get(key, str, instance)) return(false);
	dest = stof(str);
	return(true);
}

bool PropertyBagImpl::get(const string& key, float &dest, size_t instance) const
{
	string str;
	if (!get(key, str, instance)) return(false);
	dest = stof(str);
	return(true);
}

bool PropertyBagImpl::get(const string& key, bool &dest, size_t instance) const
{
	string str;

	if (!get(key, str, instance)) return(false);

	str = toLowerCase(str);

	dest = (str == "true");

	return(true);
}

bool PropertyBagImpl::get(const string& key,
                          PropertyBagImpl &dest,
                          size_t instance) const
{
	if(data.find(key)==data.end())
		return false;

	PropertyMap::const_iterator iter;

	// check that the desired instance exists
	ASSERT(instance < count(key),
		   "parameter \'instance\' is incorrect: " + itoa((int)instance));

	// go to the desired instance
	iter = data.lower_bound(key);
	for(size_t q=0; q < instance; q++) iter++;

	ASSERT(dynamic_cast<PropertyBagImpl*>(iter->second)!=0,
	       "iter->second cannot be cast to a PropertyBagImpl object: " \
	       "key = \"" + key + "\"");

	// I would rather have invalid behavior than a crash
	if(dynamic_cast<PropertyBagImpl*>(iter->second)!=0)
	{
		dest = dynamic_cast<PropertyBagImpl&>(*iter->second);
		return true;
	}
	else
	{
		return false;
	}
}

size_t PropertyBagImpl::count(const string &key) const
{
	return data.count(key);
}

void PropertyBagImpl::clear(void)
{
	for(PropertyMap::iterator propIter = data.begin();
		propIter != data.end(); ++propIter)
	{
		delete(propIter->second);
	}
	data.clear();
}

void PropertyBagImpl::copy(const PropertyBagImpl &copyMe)
{
	clear();

	for(PropertyMap::const_iterator iter = copyMe.data.begin();
	    iter != copyMe.data.end();
	    ++iter)
	{
		// alias the iterator's pair
		const string &tagName = iter->first;
		const PropertyBagItem *tagItem = iter->second;

		const PropertyBagString *pStr =
				dynamic_cast<const PropertyBagString*>(tagItem);

		if(pStr) // if the item is a PropertyBagString
		{
			add(tagName, pStr->getData(), false);
		}
		else
		{
			const PropertyBagImpl *pBag = dynamic_cast<const PropertyBagImpl*>(tagItem);

			if(pBag) // if the item is a PropertyBagImpl
			{
				add(tagName, *pBag);
			}
		}
	} // loop

	ASSERT(copyMe == (*this), "Failed to copy bag");
}

void PropertyBagImpl::merge(const PropertyBagImpl &newstuff, bool overwrite)
{
	for(PropertyMap::const_iterator newiter = newstuff.data.begin();
	    newiter != newstuff.data.end();
	    ++newiter)
	{
		// alias the iterator's pair
		const string &tagName = newiter->first;
		const PropertyBagItem *tagItem = newiter->second;

		const bool tagAlreadyExists = data.find(tagName)!=data.end();

		const PropertyBagString *pStr =
				dynamic_cast<const PropertyBagString*>(tagItem);
		const PropertyBagImpl *pBag = dynamic_cast<const PropertyBagImpl*>(tagItem);

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

		if(pBag) // if the item is a PropertyBagImpl
		{
			// if it doesn't exist, just add the bag (easy!)
			if(!tagAlreadyExists)
			{
				add(tagName, *pBag);
			}
			else
			{
				PropertyBagItem *originalItem = data.find(tagName)->second;
				ASSERT(originalItem != 0, "originalItem was null!");

				/*
				The tag we are trying to add is a PropertyBagImpl,
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
					(dynamic_cast<PropertyBagImpl&>(*originalItem)).merge(*pBag,
																	overwrite);
				}
			}
		}
	} // loop
}

bool PropertyBagImpl::operator==(const PropertyBagImpl &r) const
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

bool PropertyBagImpl::operator==( const PropertyBagItem &r ) const
{
	return(  (*this) == dynamic_cast<const PropertyBagImpl &>(r)  );
}

PropertyBagImpl & PropertyBagImpl::operator=( const PropertyBagImpl &r )
{
	copy(r);
	return(*this);
}

} // namespace Engine
