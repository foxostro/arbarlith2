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
	return pimpl->count(key);
}

bool PropertyBag::exists(const std::string & k) const
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	return pimpl->exists(k);
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

void PropertyBag::get(const std::string & k, PropertyBag & p, size_t idx) const
{
	ASSERT(pimpl, "pimpl was NULL; should never be NULL.");
	if(!pimpl->get(k, *p.pimpl, idx)) {
		throw PropertyBag_PropertyNotFound_Exception(k);
	}
}

void PropertyBag::add(const string & k, bool p)
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	pimpl->add(k, p);
}

void PropertyBag::get(const string& k, bool & p, size_t idx) const
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	if(!pimpl->get(k, p, idx)) {
		throw PropertyBag_PropertyNotFound_Exception(k);
	}
}

void PropertyBag::add(const string & k, const std::string & p)
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	pimpl->add(k, p);
}

void PropertyBag::get(const string& k, std::string & p, size_t idx) const
{
	ASSERT(pimpl, "pimpl was NULL which is never expected");
	if(!pimpl->get(k, p, idx)) {
		throw PropertyBag_PropertyNotFound_Exception(k);
	}
}

} // namespace Engine

