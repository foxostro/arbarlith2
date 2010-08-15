#include <boost/lexical_cast.hpp>

#include "PropertyBag.h"
#include "PropertyBag_XML.h"

#define STUB throw PropertyBagException("STUB")

namespace Engine {

PropertyBag::~PropertyBag(void)
{}

PropertyBag::PropertyBag(void)
{
	clear();
}

PropertyBag::PropertyBag(const PropertyBag &copyMe)
{
	copy(copyMe);
}

void PropertyBag::clear(void)
{
	STUB;
}

void PropertyBag::copy(const PropertyBag & copyMe)
{
	STUB;
}

PropertyBag PropertyBag::clone(void)
{
	STUB;
	return *((PropertyBag *)NULL);
}

void PropertyBag::removeAll(const std::string &key)
{
	STUB;
}

void PropertyBag::remove(const std::string &key, size_t idx)
{
	STUB;
}

void PropertyBag::saveToFile(const std::string & fileName) const
{
	STUB;
}

void PropertyBag::loadFromFile(const std::string & filename)
{
	STUB;
}

size_t PropertyBag::count(const std::string &key) const
{
	STUB;
	return 0;
}

bool PropertyBag::exists(const std::string & k) const
{
	return count(k) > 0;
}

bool PropertyBag::operator==(const PropertyBag &r) const
{
	STUB;
	return false;
}

PropertyBag & PropertyBag::operator=(const PropertyBag &r)
{
	STUB;
	return(*this);
}

bool PropertyBag::get_any(const string & k, boost::any & p, size_t idx) const
{
	STUB;
	return false;
}

void PropertyBag::set_any(const string & k, boost::any p)
{
	STUB;
}

} // namespace Engine

