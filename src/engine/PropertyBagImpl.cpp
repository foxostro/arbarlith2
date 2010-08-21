#include <boost/lexical_cast.hpp>
#include <string>
#include <memory> // std::auto_ptr

#include "tinyxml/tinyxml.h"

#include "stdafx.h"
#include "file.h"
#include "profile.h"
#include "PropertyBagImpl.h"

using std::string;

namespace Engine {

static string getElementContentsAsString(TiXmlElement * el)
{
	ASSERT(el, "el was NULL");

	TiXmlNode * child = NULL;
	string s;
	
	for(child = el->FirstChild(); child; child = child->NextSibling())
	{
		TiXmlPrinter printer;
		printer.SetStreamPrinting();
		child->Accept(&printer);
		s += printer.CStr();
	}
	
	return s;
}

PropertyBagImpl::~PropertyBagImpl() { /* do nothing */ }

PropertyBagImpl::PropertyBagImpl(void)
{
	clear();
}

PropertyBagImpl::PropertyBagImpl(const PropertyBagImpl &r)
{
	copy(r);
}

void PropertyBagImpl::add(const string & key, const string & contents)
{
	TiXmlElement * el_key = new TiXmlElement(key);
	TiXmlText * el_value = new TiXmlText(contents);
	el_key->LinkEndChild(el_value);
	xml->LinkEndChild(el_key);
}

void PropertyBagImpl::add(const string& key, bool data)
{
	add(key, data ? string("true") : string("false"));
}

void PropertyBagImpl::add(const string & key, const PropertyBagImpl & bag)
{
	TiXmlNode * child = NULL;
	TiXmlNode * container = new TiXmlElement(key);

	// clone all children of the other bag and insert into a container
	for(child = bag.xml->FirstChild(); child; child = child->NextSibling())
	{
		container->LinkEndChild(child->Clone());
	}

	xml->LinkEndChild(container);
}

void PropertyBagImpl::remove(const string & key)
{
	xml->RemoveChild(xml->FirstChild(key));
	ASSERT(count(key)==0, "Failed to remove items sharing key: " + key);
}

TiXmlNode * PropertyBagImpl::findChild(const string & key, int n) const
{
	int i = 0;
	TiXmlNode * root = xml.get();
	TiXmlNode * child = NULL;

	if(!root) {
		return NULL;
	}

	// Find the child by linearly iterating until we get the n-th child
	for(i = 0, child = root->FirstChild(key); child;
	    child = child->NextSibling())
	{
		if(key == child->Value()) {
			if(i++ == n) {
				return child;
			}
		}
	}

	return NULL;
}

void PropertyBagImpl::remove(const string & key, int n)
{
	xml->RootElement()->RemoveChild(findChild(key, n));
}

void PropertyBagImpl::saveToFile(const string &fileName) const
{
	xml->SaveFile(fileName);
}

string PropertyBagImpl::save(void) const
{
	TiXmlPrinter printer;
	xml->Accept(&printer);
	return printer.Str();
}

void PropertyBagImpl::loadFromFile(const string &fileName)
{
	xml.reset(new TiXmlDocument);
	xml->LoadFile(fileName);
	while(resolveInheritTags());
}

bool PropertyBagImpl::get(const string & key,
                          string & dest,
                          size_t instance) const
{
	TiXmlNode * node = NULL;
	TiXmlElement * child = NULL;
	TiXmlNode * valNode = NULL;
	
	node = findChild(key, instance);
	if(!node) {
		return false;
	}

	child = node->ToElement();
	if(!child) {
		return false;
	}

	valNode = child->FirstChild();
	if(!valNode) {
		return false;
	}
	
	dest = valNode->Value();
	return true;
}

bool PropertyBagImpl::get(const string & key,
                          bool & dest,
                          size_t instance) const
{
	string str;

	if (!get(key, str, instance)) {
		return false;
	}

	str = toLowerCase(str);

	dest = (str == "true");

	return true;
}

bool PropertyBagImpl::get(const string & key,
                          PropertyBagImpl & dest,
                          size_t instance) const
{
	TiXmlNode * node = NULL;
	TiXmlElement * el = NULL;

	node = findChild(key, instance);
	if(!node) {
		return false;
	}

	el = node->ToElement();
	if(!el) {
		return false;
	}

	dest.clear();
	dest.xml->Parse(getElementContentsAsString(el).c_str());

	return true;
}

size_t PropertyBagImpl::count(const string & key) const
{
	size_t i = 0;
	TiXmlNode * child = NULL;
	
	// Find the child by linearly iterating until we count them all
	for(i = 0, child = xml->FirstChild(key); child;
	    child = child->NextSibling())
	{
		if(key == child->ValueStr()) {
			++i;
		}
	}

	return i;
}

bool PropertyBagImpl::exists(const std::string &key) const
{
	return xml->FirstChild(key) != NULL;
}

void PropertyBagImpl::clear(void)
{
	xml.reset(new TiXmlDocument);
}

void PropertyBagImpl::copy(const PropertyBagImpl &copyMe)
{
	xml.reset(new TiXmlDocument(*copyMe.xml));
	ASSERT(copyMe == (*this), "Failed to copy bag");
}

bool PropertyBagImpl::operator==(const PropertyBagImpl &r) const
{
	throw std::logic_error("Oh? Is someone actually calling this?");
	return true;
}

PropertyBagImpl & PropertyBagImpl::operator=(const PropertyBagImpl &r)
{
	copy(r);
	return(*this);
}

bool PropertyBagImpl::findFirstDescendent(const string & key,
                                          TiXmlNode * root,
                                          TiXmlNode ** parent,
                                          TiXmlNode ** node) const
{
	TiXmlNode * n = NULL;

	assert(root);
	assert(parent);
	assert(node);

	for(n = root->FirstChild(); n; n = n->NextSibling())
	{
		if(key == n->Value()) {
			//found it!
			(*parent) = root;
			(*node) = n;
			return true;
		} else if(findFirstDescendent(key, n, parent, node)) {
			// found it through recursion
			return true;
		}
	}
	
	// did find it in this branch
	return false;
}

bool PropertyBagImpl::resolveInheritTags(void)
{
	// XXX: The entire concept of the INHERIT tag seems to be placed at too low of a level. It's been here forever, though, so I can't really get rid of it without a major effort. :(

	TiXmlNode * parent = NULL;
	TiXmlNode * node = NULL;

	// find the first inherit tag
	if(!findFirstDescendent("INHERIT", xml.get(), &parent, &node)) {
		return false;
	}

	// what filename is this referencing?
	TiXmlNode * fileNameNode = node->FirstChild();
	if(!fileNameNode) {
		throw std::runtime_error("Format of INHERIT tag was unexpected (1)");
	}
	TiXmlText * fileNameTextNode = fileNameNode->ToText();
	if(!fileNameTextNode) {
		throw std::runtime_error("Format of INHERIT tag was unexpected (2)");
	}

	// load the data referenced from the other file
	string fileName = fileNameTextNode->Value();
	TiXmlDocument subdoc;
	subdoc.LoadFile(fileName);

	// now replace the inherit tag with the data we just loaded
	parent->RemoveChild(node);
	
	{
		TiXmlNode * child = NULL;
		TiXmlNode * cursor = parent->FirstChild();

		assert(cursor);

		// insert children which are not present in this context
		for(child = subdoc.FirstChild(); child; child = child->NextSibling())
		{
			if(!parent->FirstChild(child->Value())) {
				parent->InsertBeforeChild(cursor, *child);
			}
		}
	}
	
	return true;
}

} // namespace Engine
