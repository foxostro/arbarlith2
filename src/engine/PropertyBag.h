#ifndef _PROPERTY_BAG_H_
#define _PROPERTY_BAG_H_

#include <string>
#include <map>
#include <boost/lexical_cast.hpp>

#include "myassert.h"

namespace Engine {

class PropertyBagImpl; // class protoype

/** Base exception type for errors encountered by PropertyBag */
class PropertyBagException : public std::exception
{
private:
	const std::string s;

public:
	PropertyBagException(const std::string & _s) : s(_s) {}
	virtual ~PropertyBagException() throw() {}

	char const * what() const throw()
	{
		return s.c_str();
	}
};

/** The specified property could not be found. */
class PropertyBag_PropertyNotFound_Exception : public PropertyBagException
{
public:
	PropertyBag_PropertyNotFound_Exception(const std::string & _s)
		: PropertyBagException(_s) {}
};

/** Maps from string names to arbitrary values. Used for configuration data. */
class PropertyBag
{
private:
	PropertyBagImpl * pimpl;

public:
	~PropertyBag(void);
	PropertyBag(void);
	PropertyBag(const PropertyBag & copyMe);
	
	PropertyBag & operator=(const PropertyBag & r);
	bool operator==(const PropertyBag & r) const;

	/** Resets the bag and destroys all data currently stored in the bag. */
	void clear(void);

	/** Sets the contents of this bag to be identical to the specified bag. */
	void copy(const PropertyBag & copyMe);

	/** Clones a new bag that is identical to this one. */
	PropertyBag clone(void);

	void saveToFile(const std::string & fileName) const;
	void loadFromFile(const std::string & fileName);

	/**	Remove all instances of the indicated property. */
	void removeAll(const std::string & key);

	/**
	Remove a single instance of the indicated property.
	@param key Name of the property to remove
	@param idx Index of the instance to remove
	*/
	void remove(const std::string & key, size_t idx);

	/**	Returns the number of properties that share the given name. */
	size_t count(const std::string & key) const;
	
	/** Returns true if any number of properties exist with the given name. */
	bool exists(const std::string & k) const;

	// For adding and getting PropertyBag objects
	void add(const std::string & k, const PropertyBag & p);
	void get(const std::string & k, PropertyBag & p, size_t idx = 0) const;
	
	// For adding and getting boolean objects
	void add(const std::string & k, bool p);
	void get(const std::string & k, bool & p, size_t idx = 0) const;
	
	// For adding and getting string objects
	void add(const std::string & k, const std::string & p);
	void get(const std::string & k, std::string & p, size_t idx = 0) const;

	// For adding generic-typed objects: anything that can be lexical_cast'd
	template<typename T>
	void add(const std::string & k, const T & p)
	{
		try
		{
			add(k, boost::lexical_cast<std::string>(p));
		}
		catch(boost::bad_lexical_cast & e)
		{
			std::string what = std::string("Could not add \"") + k
				+ std::string("\" to bag: ")
				+ std::string(e.what());
			throw PropertyBagException(what);
		}
	}

	// For getting generic-typed objects: anything that can be lexical_cast'd
	template<typename T>
	void get(const std::string & k, T & p, size_t idx = 0) const
	{
		std::string str;

		get(k, str, idx);

		try
		{
			p = boost::lexical_cast<T>(str);
		}
		catch(boost::bad_lexical_cast & e)
		{
			std::string what = std::string("For key \"") + k
				+ std::string("\" got value \"")
				+ std::string("\", but don't know what to do with it: ")
				+ std::string(e.what());
			throw PropertyBagException(what);
		}
	}

	// Sometimes, we don't really care if the property is missing
	template<typename T>
	bool get_optional(const std::string & k, T & p, size_t idx = 0) const
	{
		if(!exists(k)) {
			return false;
		} else {
			get(k, p, idx);
			return true;
		}
	}
};

} // namespace Engine

#endif

