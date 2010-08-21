/*
Modified by Andrew Fox in 2003-2007,2010
E-Mail: mailto:foxostro@gmail.com

Original Source:
McCuskey, Mason. "Game Programming Tricks of the Trade".
	"Trick 15: Serialization Using XML Property Bags".
	Premier Press. 2002.
*/

#ifndef _PROPERTY_BAG_IMPL_H_
#define _PROPERTY_BAG_IMPL_H_

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
	virtual std::string save(int indentlevel=0) const = 0;

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
	PropertyBagString(const std::string &data, bool convert = true);

	/**
	Constructor
	@param name The name of the item
	@param data The string data to accept
	@param convert If true, the string should be made safe for an XML file
	*/
	PropertyBagString(const std::string &name, const std::string &data, bool convert = true);

	/**
	Return a string representation of some XML structure that in turn
	represents this item.
	@param indentlevel Indentation level of the resultant XML code
	*/
	virtual std::string save(int indentlevel=0) const;

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
	std::string getData(void) const;

	/**
	Sets data for the item
	@param data The data to accept
	@param convert If true, then it is necessary to convert the string to
	a form safe for XML.
	@return The actual data stored for this item
	*/
	const std::string& setData(const std::string &data, bool convert);

	/**
	Sets the name of the item
	@param name The new name of the item
	@return The name actually set for the item
	*/
	const std::string& setName(const std::string &name);

	/** Gets the name of the item */
	const std::string& getName(void) const;

	/**
	Makes a string safe for insertion into an XML file by replacing special
	characters like '>' and '&' with the appropriate special entity.
	@param str String parameter
	@return Safe string
	*/
	static std::string makeStringSafe(const std::string &str);

	/**
	Takes a string made safe for an XML file and replaces the entities like
	"&amp;" and "&quot;" with the appropriate characters.
	@param str String parameter
	@return Original string
	*/
	static std::string restoreFromSafeString(const std::string &str);

private:
	/** Has the item been converted to a safe form? */
	bool itemHasBeenConverted;

	/** Name of the item */
	std::string itemName;

	/** Data stored for this item*/
	std::string itemData;
};

/** Contains property bag items */
class PropertyBagImpl : public PropertyBagItem
{
private:
	/** string -> PropertyBagItem */
	typedef std::multimap<std::string, PropertyBagItem *> PropertyMap;

	/** Property bag content */
	PropertyMap data;

public:
	/** Destructor */
	virtual ~PropertyBagImpl(void);

	/** Constructor */
	PropertyBagImpl(void);

	/** Copy constructor */
	PropertyBagImpl(const PropertyBagImpl &r);

	/**
	Construct from a string
	@param s String from which to accept data
	*/
	PropertyBagImpl(const std::string &s);

	/** Clear out the property bag */
	void clear(void);

	/** Assignment operator */
	PropertyBagImpl &operator=(const PropertyBagImpl &r);

	/** Equality operator against CPropBag */
	virtual bool operator==(const PropertyBagImpl &r) const;

	/**
	Equality operator
	@param r Item to test
	@return true when the objects are equal
	*/
	virtual bool operator==(const PropertyBagItem &r) const;

	/**
	Return a string representation of some XML structure that in turn
	represents this item.
	@param indentlevel Indentation level of the resultant XML code
	*/
	virtual std::string save(int indentlevel=0) const;

	/**
	Saves to file some XML structure that in turn represents this item.
	@param fileName Name of the file to write
	@param indentlevel Indentation level of the resultant XML code
	*/
	void saveToFile(const std::string &fileName, int indentLevel=0) const;

	/**
	Loads the contents of the property bag from file.  It is possible to
	merge the contents of the file with data that is already loaded.
	Otherwise, the currently loaded data is completely discarded and
	replaced by data loaded from file.
	@param fileName Name of the file from which to load
	@param merge If true, then the contents of the file are merged with any
	existing contents of this bag.
	*/
	void loadFromFile(const std::string &fileName, bool merge = false);

	/**
	Remove all instances of the item
	@param key Name of the key to remove
	*/
	void remove(const std::string &key);

	/**
	Remove all instances of the item
	@param key Name of the key to remove
	@param instance Index of the instance to remove
	*/
	void remove(const std::string &key, int instance);

	/**
	Gets the number of instances of data elements paired with the given key
	@param key Key value
	@return The number of times the key is present in the data
	*/
	size_t count(const std::string &key) const;

	/** Determines whether the key exists or not. */
	inline bool exists(const std::string &key) const
	{
		return data.find(key) != data.end();
	}

	/** Adds a string */
	void add(const std::string &key, const std::string &data, bool convert = true);

	/** Adds a bool */
	void add(const std::string &key, bool data);

	/** Adds a PropertyBagImpl */
	void add(const std::string &key, const PropertyBagImpl &contents);

	/** Gets a string */
	bool get(const std::string &key, std::string &dest, size_t instance = 0) const;

	/** Gets a bool */
	bool get(const std::string &key, bool &dest, size_t instance = 0) const;

	/** Gets a PropertyBagImpl */
	bool get(const std::string &key, PropertyBagImpl &dest, size_t instance = 0) const;

private:
	/**
	Inserts a tag into the property bag
	@param tagName Name of the tag
	@param tagContent Content if the tag
	*/
	void insertTag(const std::string &tagName, const std::string &tagContent);

	/**
	Interprets the contents of the string as property bag contents and
	merges with the existing contents.
	@param newStuff String containing additional data
	@param allowInheritance If true, allows @inherit tags to evaluate
	@return true if successful, false otherwise
	*/
	bool loadMergeFromString(const std::string &newStuff, bool allowInheritance);

	/**
	Merges the contents of another PropertyBagImpl with this one
	@param newStuff Contains additional data
	@param overwrite If true, then conflicts are resolved by overwriting
	the existing elements.
	*/
	void merge(const PropertyBagImpl &newStuff, bool overwrite = true);

	/**
	Copies the contents of this bag from another
	@param r Bag to copy
	*/
	void copy(const PropertyBagImpl &r);
};

} // namespace Engine

#endif
