#ifndef _PROPERTY_BAG_H_
#define _PROPERTY_BAG_H_

#include <string>
#include <map>
#include <boost/any.hpp>

namespace Engine {

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

/** Maps from string names to arbitrary values. Used for configuration data. */
class PropertyBag
{
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
	Remove a single instances of the indicated property.
	@param key Name of the property to remove
	@param idx Index of the instance to remove
	*/
	void remove(const std::string & key, size_t idx);

	/**	Returns the number of properties that share the given name. */
	size_t count(const std::string & key) const;
	
	/** Returns true if any number of properties exist with the given name. */
	bool exists(const std::string & k) const;

	template<typename T>
	void set(const std::string & k, const T & p)
	{
		set_any(k, boost::any(p));
	}

	template<typename T>
	void add(const std::string & k, const T & p)
	{
		set_any(k, boost::any(p));
	}

	/** Return true if the property was found. */
	template<typename T>
	bool get(const std::string & k, T & p, size_t idx = 0) const
	{
		boost::any a;

		if(get_any(k, a, idx)) {
			p = boost::any_cast<T>(a);
			return true;
		}

		return false;
	}
	
	bool get_any(const std::string & k, boost::any & p, size_t idx=0) const;
	void set_any(const std::string & k, boost::any p);
};

} // namespace Engine



// Gets a value from XML when the tag name and symbol name are the same
#define getSym(SYMBOL) get(STRINGIFY(SYMBOL), (SYMBOL))

// Puts a value from XML when the tag name and symbol name are the same
#define addSym(SYMBOL) add(STRINGIFY(SYMBOL), (SYMBOL))


#endif

