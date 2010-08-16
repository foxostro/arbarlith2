#include <boost/lexical_cast.hpp>

#include "PropertyBag.h"
#include "PropertyBag_XML.h"

#define STUB throw PropertyBagException("STUB")

namespace Engine {

PropertyBag::~PropertyBag(void)
{
	clear();
}

PropertyBag::PropertyBag(void) : xml(NULL)
{
	clear();
}

PropertyBag::PropertyBag(const PropertyBag &copyMe)
{
	copy(copyMe);
}

void PropertyBag::clear(void)
{
	delete xml;
	xml = new PropertyBag_XML();
}

void PropertyBag::copy(const PropertyBag & copyMe)
{
	clear();
	ASSERT(copyMe.xml, "copyMe.xml was NULL which is never expected");
	(*xml) = (*copyMe.xml);
}

PropertyBag PropertyBag::clone(void)
{
	return PropertyBag(*this);
}

void PropertyBag::removeAll(const std::string &key)
{
	ASSERT(xml, "xml was NULL which is never expected");
	xml->remove(key);
}

void PropertyBag::remove(const std::string &key, size_t idx)
{
	ASSERT(xml, "xml was NULL which is never expected");
	xml->remove(key, idx);
}

void PropertyBag::saveToFile(const std::string & fileName) const
{
	ASSERT(xml, "xml was NULL which is never expected");
	xml->saveToFile(fileName);
}

void PropertyBag::loadFromFile(const std::string & fileName)
{
	ASSERT(xml, "xml was NULL which is never expected");
	xml->loadFromFile(fileName);
}

size_t PropertyBag::count(const std::string &key) const
{
	ASSERT(xml, "xml was NULL which is never expected");
	return xml->getNumInstances(key);
}

bool PropertyBag::exists(const std::string & k) const
{
	return count(k) > 0;
}

bool PropertyBag::operator==(const PropertyBag &r) const
{
	ASSERT(xml, "xml was NULL which is never expected");
	ASSERT(r.xml, "r.xml was NULL which is never expected");
	return (*r.xml) == (*xml);
}

PropertyBag & PropertyBag::operator=(const PropertyBag &r)
{
	copy(r);
	return(*this);
}

void PropertyBag::add(const std::string & k, const PropertyBag & p)
{
	ASSERT(xml, "xml was NULL; should never be NULL.");
	xml->add(k, *p.xml);
}

bool PropertyBag::get(const std::string & k, PropertyBag & p, size_t idx) const
{
	ASSERT(xml, "xml was NULL; should never be NULL.");
	return xml->get(k, *p.xml, idx);
}

void PropertyBag::add(const string & k, bool p)
{
	ASSERT(xml, "xml was NULL which is never expected");
	xml->add(k, p);
}

bool PropertyBag::get(const string& k, bool & p, size_t idx) const
{
	ASSERT(xml, "xml was NULL which is never expected");
	return xml->get(k, p, idx);
}

void PropertyBag::add(const string & k, const std::string & p)
{
	ASSERT(xml, "xml was NULL which is never expected");
	xml->add(k, p);
}

bool PropertyBag::get(const string& k, std::string & p, size_t idx) const
{
	ASSERT(xml, "xml was NULL which is never expected");
	return xml->get(k, p, idx);
}

} // namespace Engine

