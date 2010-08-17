#include <boost/lexical_cast.hpp>

#include "PropertyBag.h"
#include "PropertyBagImpl.h"

namespace Engine {

PropertyBag::~PropertyBag(void)
{
	clear();
}

PropertyBag::PropertyBag(void) : pimpl(NULL)
{
	clear();
}

PropertyBag::PropertyBag(const PropertyBag &copyMe)
{
	copy(copyMe);
}

void PropertyBag::clear(void)
{
	delete pimpl;
	pimpl = new PropertyBagImpl();
}

void PropertyBag::copy(const PropertyBag & copyMe)
{
	clear();
	ASSERT(copyMe.pimpl, "copyMe.pimpl was NULL which is never expected");
	(*pimpl) = (*copyMe.pimpl);
}

PropertyBag PropertyBag::clone(void)
{
	return PropertyBag(*this);
}

void PropertyBag::removeAll(const std::string &key)
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	pimpl->remove(key);
}

void PropertyBag::remove(const std::string &key, size_t idx)
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	pimpl->remove(key, idx);
}

void PropertyBag::saveToFile(const std::string & fileName) const
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	pimpl->saveToFile(fileName);
}

void PropertyBag::loadFromFile(const std::string & fileName)
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	pimpl->loadFromFile(fileName);
}

size_t PropertyBag::count(const std::string &key) const
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	return pimpl->getNumInstances(key);
}

bool PropertyBag::exists(const std::string & k) const
{
	return count(k) > 0;
}

bool PropertyBag::operator==(const PropertyBag &r) const
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	ASSERT(r.pimpl, "r.pimpl was NULL which is never expected");
	return (*r.pimpl) == (*pimpl);
}

PropertyBag & PropertyBag::operator=(const PropertyBag &r)
{
	copy(r);
	return(*this);
}

void PropertyBag::add(const std::string & k, const PropertyBag & p)
{
	ASSERT(pimpl, "pimpl was NULL; should never be NULL.");
	pimpl->add(k, *p.pimpl);
}

bool PropertyBag::get(const std::string & k, PropertyBag & p, size_t idx) const
{
	ASSERT(pimpl, "pimpl was NULL; should never be NULL.");
	return pimpl->get(k, *p.pimpl, idx);
}

void PropertyBag::add(const string & k, bool p)
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	pimpl->add(k, p);
}

bool PropertyBag::get(const string& k, bool & p, size_t idx) const
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	return pimpl->get(k, p, idx);
}

void PropertyBag::add(const string & k, const std::string & p)
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	pimpl->add(k, p);
}

bool PropertyBag::get(const string& k, std::string & p, size_t idx) const
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	return pimpl->get(k, p, idx);
}

} // namespace Engine

