/*
Modified by Andrew Fox in 2003-2007
E-Mail: mailto:andrewfox@cmu.edu

Original Source:
McCuskey, Mason. "Game Programming Tricks of the Trade".
	"Trick 15: Serialization Using XML Property Bags".
	Premier Press. 2002.
*/

#ifndef _PROPBAG_H_
#define _PROPBAG_H_

#include <map>

#include "PrimitiveDataTypes.h"

namespace Engine {

/** Item stored in a property bag */
class PropertyBagItem
{
public:
	/** Destructor */
	virtual ~PropertyBagItem(void);

	/** Constructor */
	PropertyBagItem(void);

	/**
	Return a string representation of some XML structure that in turn
	represents this item.
	@param indentlevel Indentation level of the resultant XML code
	*/
	virtual _tstring save(int indentlevel=0) const = 0;

	/**
	Equality operator
	@param r Item to test
	@return true when the objects are equal
	*/
	virtual bool operator==(const PropertyBagItem &r) const = 0;

	/**
	Inequality operator
	@param r Item to test
	@return true when the objects are not equal
	*/
	bool operator!=(const PropertyBagItem &r);
};

/** String stored in a property bag */
class PropertyBagString : public PropertyBagItem
{
public:
	/** Destructor */
	virtual ~PropertyBagString(void);

	/**
	Constructor
	@param data The string data to accept
	@param convert If true, the string should be made safe for an XML file
	*/
	PropertyBagString(const _tstring &data, bool convert = true);

	/**
	Constructor
	@param name The name of the item
	@param data The string data to accept
	@param convert If true, the string should be made safe for an XML file
	*/
	PropertyBagString(const _tstring &name, const _tstring &data, bool convert = true);

	/**
	Return a string representation of some XML structure that in turn
	represents this item.
	@param indentlevel Indentation level of the resultant XML code
	*/
	virtual _tstring save(int indentlevel=0) const;

	/**
	Equality operator
	@param r The item with which to compare
	@return true if the items are equal
	*/
	virtual bool operator==(const PropertyBagItem &r) const;

	/**
	Gets the data from the item, converted as appropriate.
	@return data from this item
	*/
	_tstring getData(void) const;

	/**
	Sets data for the item
	@param data The data to accept
	@param convert If true, then it is necessary to convert the string to
	a form safe for XML.
	@return The actual data stored for this item
	*/
	const _tstring& setData(const _tstring &data, bool convert);

	/**
	Sets the name of the item
	@param name The new name of the item
	@return The name actually set for the item
	*/
	const _tstring& setName(const _tstring &name);

	/** Gets the name of the item */
	const _tstring& getName(void) const;

	/**
	Makes a string safe for insertion into an XML file by replacing special
	characters like '>' and '&' with the appropriate special entity.
	@param str String parameter
	*/
	static const _tstring& makeStringSafe(const _tstring &str);

	/**
	Takes a string made safe for an XML file and replaces the entities like
	"&amp;" and "&quot;" with the appropriate characters.
	@param str String parameter
	*/
	static const _tstring& restoreFromSafeString(const _tstring &str);

private:
	/** Has the item been converted to a safe form? */
	bool itemHasBeenConverted;

	/** Name of the item */
	_tstring itemName;

	/** Data stored for this item*/
	_tstring itemData;
};

/** _tstring -> PropertyBagItem */
typedef multimap<_tstring, PropertyBagItem *> PropertyMap;

/** Contains property bag items */
class PropertyBag : public PropertyBagItem
{
public:
	/** Destructor */
	virtual ~PropertyBag(void);

	/** Constructor */
	PropertyBag(void);

	/** Copy constructor */
	PropertyBag(const PropertyBag &r);

	/**
	Construct from a string
	@param s String from which to accept data
	*/
	PropertyBag(const _tstring &s);

	/** Clear out the property bag */
	void clear(void);

	/**
	Inserts a tag into the property bag
	@param tagName Name of the tag
	@param tagContent Content if the tag
	*/
	void insertTag(const _tstring &tagName, const _tstring &tagContent);

	/** Assignment operator */
	PropertyBag &operator=(const PropertyBag &r);

	/** Equality operator against CPropBag */
	virtual bool operator==(const PropertyBag &r) const;

	/**
	Return a string representation of some XML structure that in turn
	represents this item.
	@param indentlevel Indentation level of the resultant XML code
	*/
	virtual _tstring save(int indentlevel=0) const;

	/**
	Equality operator
	@param r Item to test
	@return true when the objects are equal
	*/
	virtual bool operator==(const PropertyBagItem &r) const;

	/**
	Remove all instances of the item
	@param key Name of the key to remove
	*/
	void remove(const _tstring &key);

	/**
	Remove all instances of the item
	@param key Name of the key to remove
	@param instance Index of the instance to remove
	*/
	void remove(const _tstring &key, int instance);
	
	/**
	Gets the number of instances of data elements paired with the given key
	@param key Key value
	@return The number of times the key is present in the data
	*/
	size_t getNumInstances(const _tstring &key) const;

	inline PropertyMap& getData(void)
	{
		return(data);
	}

	inline const PropertyMap& getData(void) const
	{
		return(data);
	}

	inline void add(const _tstring &key, const XmlDataType &data)
	{
		add(key, (const XmlDataType*)(&data));
	}

	inline bool get(const _tstring &key, XmlDataType &dest, size_t instance = 0) const
	{
		return get(key, (XmlDataType*)(&dest), instance);
	}

	void saveToFile(const _tstring &fileName, int indentLevel=0) const;

	bool Load(const _tstring &filename);

	bool LoadFromString(const _tstring &data);

	bool LoadMerge(const _tstring &filename);

	bool LoadMergeFromString(const _tstring &data);

	void merge(const PropertyBag &newstuff, bool overwrite = true);

	void add(const _tstring &key, const _tstring &data, bool convert = true);
	void add(const _tstring &key, int data);
	void add(const _tstring &key, size_t data);
	void add(const _tstring &key, double data);
	void add(const _tstring &key, float data);
	void add(const _tstring &key, bool data);
	void add(const _tstring &key, const PropertyBag &contents);
	void add(const _tstring &key, const XmlDataType *data);

	bool get(const _tstring &key, _tstring &dest, size_t instance = 0) const;
	bool get(const _tstring &key, int &dest, size_t instance = 0) const;
	bool get(const _tstring &key, size_t &dest, size_t instance = 0) const;
	bool get(const _tstring &key, double &dest, size_t instance = 0) const;
	bool get(const _tstring &key, float &dest, size_t instance = 0) const;
	bool get(const _tstring &key, bool &dest, size_t instance = 0) const;
	bool get(const _tstring &key, PropertyBag &dest, size_t instance = 0) const;
	bool get(const _tstring &key, XmlDataType *dest, size_t instance = 0) const;

protected:
	/**
	Copies the contents of this bag from another
	@param r Bag to copy
	*/
	void copy(const PropertyBag &r);
	
	/** Property bag content */
	PropertyMap data;
};

} // namespace Engine



// Gets a value from XML when the tag name and symbol name are the same
#define getSym(SYMBOL) get(STRINGIFY(SYMBOL), (SYMBOL))

// Puts a value from XML when the tag name and symbol name are the same
#define addSym(SYMBOL) add(STRINGIFY(SYMBOL), (SYMBOL))



#endif
