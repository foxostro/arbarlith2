#ifndef _PROPERTY_BAG_IMPL_H_
#define _PROPERTY_BAG_IMPL_H_

class TiXmlDocument;
class TiXmlNode;

namespace Engine {

/** Contains property bag items */
class PropertyBagImpl
{
private:
	// auto_ptr ensures proper deletion on destruction
	std::auto_ptr<TiXmlDocument> xml; 

public:
	/** Destructor */
	~PropertyBagImpl();

	/** Constructor */
	PropertyBagImpl(void);

	/** Copy constructor */
	PropertyBagImpl(const PropertyBagImpl &r);

	/** Clear out the property bag */
	void clear(void);

	/** Assignment operator */
	PropertyBagImpl &operator=(const PropertyBagImpl &r);

	/**	Equality operator */
	bool operator==(const PropertyBagImpl &r) const;

	/**
	Return a string representation of some XML structure that in turn
	represents this item.
	*/
	std::string save(void) const;

	/**
	Saves to file some XML structure that in turn represents this item.
	@param fileName Name of the file to write
	*/
	void saveToFile(const std::string &fileName) const;

	/**
	Loads the contents of the property bag from file.  It is possible to
	merge the contents of the file with data that is already loaded.
	Otherwise, the currently loaded data is completely discarded and
	replaced by data loaded from file.
	@param fileName Name of the file from which to load
	*/
	void loadFromFile(const std::string &fileName);

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
	bool exists(const std::string &key) const;

	/** Adds a string */
	void add(const std::string &key, const std::string &data);

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
	Copies the contents of this bag from another
	@param r Bag to copy
	*/
	void copy(const PropertyBagImpl &r);

	/** Search for the N-th child with the specified key */
	TiXmlNode * findChild(const string & key, int n) const;

	/** Search for the 1st descendent with the specified key */
	bool findFirstDescendent(const string & key, TiXmlNode * root, TiXmlNode ** parent, TiXmlNode ** node) const;

	/** These data files can include text from other data files with an
	@inherit element. Handles and removes the first @inherit tag it finds.
	Returns false if none were found in any descendent.
	*/
	bool resolveInheritTags(void);
};

} // namespace Engine

#endif
