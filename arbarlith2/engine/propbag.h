/*
Modified by Andrew Fox in 2003-2006
Original Source: McCuskey, Mason. "Game Programming Tricks of the Trade". "Trick 15: Serialization Using XML Property Bags". Premier Press. 2002.
E-Mail: mailto:andrewfox@cmu.edu
*/

#ifndef PROPBAG_H__INCLUDED
#define PROPBAG_H__INCLUDED

// abstract base class
#include <map>
#include "PrimitiveDataTypes.h"

using namespace std;

namespace Engine { 

/** Item for XML bag */
class CPropItem
{
public:
	/** Constructor */
	CPropItem(void)
	{}

#if 0
	/** Destructor */
	virtual ~CPropItem(void)
	{}
#endif

	/**
	Save _tstring
	@param indentlevel Indentation level of the resultant xml code
	*/
	virtual _tstring Save(int indentlevel=0) const = 0;
	
	/**
	Equality operator
	@param r Item to test
	@return true when the objects are equal
	*/
	virtual bool operator==(const CPropItem &r) = 0;
	
	/**
	Inequality operator
	@param r Item to test
	@return true when the objects are not equal
	*/
	bool operator!=(const CPropItem &r)
	{
		return(!((*this) == r));
	}
};

class CPropString : public CPropItem
{
public:
	static void MakeStringSafeForPropBag(_tstring &str);
	static void RestoreOrigFromSafeString(_tstring &str);

	virtual ~CPropString()
	{}

	CPropString(_tstring data, bool convert = true)
	{
		SetData(data, convert); 
	}

	_tstring GetData() const
	{ 
		_tstring str = m_Data; 
		if (m_Converted)
			RestoreOrigFromSafeString(str); 

		return(str); 
	}

	void SetData(_tstring d, bool convert)
	{ 
		if(convert)
			MakeStringSafeForPropBag(d);

		m_Data = d; 
		m_Converted = convert;
	}

	_tstring Save(int indentlevel=0) const;

	void SetName(_tstring name)
	{
		m_strName = name;
	}

	_tstring GetName() { return(m_strName); }

	bool operator==(const CPropItem &r)
	{
		try
		{
			const CPropString &rStr = (const CPropString &)(r);
			return(m_Data == rStr.m_Data);
		} catch(...) { return(false); }
	}

protected:
	bool m_Converted;
	_tstring m_strName;
	_tstring m_Data;
};

typedef multimap<_tstring, CPropItem *> PropertyMap;

class CPropBag : public CPropItem
{
public:
  virtual ~CPropBag();
  CPropBag();
  CPropBag(const CPropBag &r) { Init(); Copy(r); }
  CPropBag(const _tstring &r) { Init(); LoadFromString(r); }

  
  void Clear();

  bool PutTagIntoBag(const _tstring &tagname, const _tstring &tagvalue);

  CPropBag &operator=(const CPropBag &r) { Copy(r); return(*this); }
  bool operator==(const CPropBag &r);
  bool operator==(const CPropItem &r) {
	try {
	  const CPropBag &rBag = (const CPropBag &)(r);
	  return((*this) == rBag);
	} catch(...) { return(false); }
  }

  inline void Add(const _tstring &key, const XmlDataType &data)
  {
	  Add(key, (const XmlDataType*)(&data));
  }

  void Add(const _tstring &key, _tstring data, bool convert = true);
  void Add(const _tstring &key, int data);
  void Add(const _tstring &key, double data);
  void Add(const _tstring &key, float data);
  void Add(const _tstring &key, bool data);
  void Add(const _tstring &key, CPropBag data);
  void Add(const _tstring &key, const XmlDataType *data);
 
  void Remove(const _tstring &key, int instance = -1); // -1 = all instances

  _tstring Save (int indentlevel=0) const;
  void saveToFile(const _tstring &fileName, int indentLevel=0) const;

  bool Load(const _tstring &filename);
  bool LoadFromString(const _tstring &data);

  // Loads a file and merges the contents
  bool LoadMerge(const _tstring &filename);
  bool LoadMergeFromString(const _tstring &data);
  
  int GetNumInstances(_tstring key);

  inline bool Get(const _tstring &key, XmlDataType &dest, int instance = 0)
  {
	  return Get(key, (XmlDataType*)(&dest), instance);
  }

  bool Get(const _tstring &key, _tstring &dest, int instance = 0);
  bool Get(const _tstring &key, int &dest, int instance = 0);
  bool Get(const _tstring &key, double &dest, int instance = 0);
  bool Get(const _tstring &key, float &dest, int instance = 0);
  bool Get(const _tstring &key, bool &dest, int instance = 0);
  bool Get(const _tstring &key, CPropBag &dest, int instance = 0);
  bool Get(const _tstring &key, XmlDataType *dest, int instance = 0);

  PropertyMap &GetData()
  {
	  return(m_Data);
  }

  void Merge(const CPropBag &newstuff, bool overwrite = true);

protected:
  void Init();
  void Copy(const CPropBag &r);
  PropertyMap m_Data;
};

}; //namespace



// Gets a value from XML when the tag name and symbol name are the same
#define getSym(SYMBOL) Get(STRINGIFY(SYMBOL), (SYMBOL))

// Puts a value from XML when the tag name and symbol name are the same
#define addSym(SYMBOL) Add(STRINGIFY(SYMBOL), (SYMBOL))



#endif
