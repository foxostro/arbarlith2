/*
Modified in 2003-2006 by Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu
*/

/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		TextureManager Manager	

	Version History:
		v1.00	New class (GA 31/10/02)

	Licence:

	This SDK/library is free software, written primarily for
	teaching purposes. You can redistribute and modify it to 
	your heart's content. However, any use of this library is 
	at the user's own risk. There is no warranty. The authors 
	will not be held responsible for ANYTHING that happens as 
	a result of the use of this software. All financial/emotional/health 
	problems or anything else that happens after using this 
	software is therefore the user's own responsibility. 
	Nevertheless, if the user finds it useful, an email or a 
	mention in any credits would be appreciated, but is not 
	necessary.
*/

#include "stdafx.h"
#include "gl.h"
#include "WaitScreen.h"
#include "Effect.h"
#include "TextureManager.h"


namespace Engine {




bool CheckID(TextureHandle &tex, unsigned chkid)
{
	return tex.getID()==chkid;
}

bool CheckStr(TextureHandle &tex, _tstring chkstr)
{
	return tex.getFileName()==chkstr;
}





TextureManager::TextureManager()
{
	tlist.clear();
	prestex=0U;
}

TextureManager::~TextureManager()
{
	while(!tlist.empty())
	{
		// get the first object in the list
		TListType::iterator tex = tlist.begin();

		// remove the texture from the hardware
		tex->release();

		// and from the list
		tlist.erase(tex);
	}
}

TextureHandle* TextureManager::Load(const _tstring &fileName)
{
	Image img;
	TextureHandle tex;

	// do we already have the texture?
	if(!tlist.empty())
	{
		// find by the filename
		TListType::iterator found = find_if(tlist.begin(), tlist.end(), bind(CheckStr, _1, fileName));

		// is it already in the list?
		if(found!=tlist.end())
			return &(*found);
	}

	// Create the texture
	Image image(fileName);

	// Return the texture handle
	return Create(image);
}

TextureHandle* TextureManager::Create(Image *img)
{
	ASSERT(img!=0, _T("TextureManager::Create - NULL pointer passed."));
	return Create(*img);
}	

TextureHandle* TextureManager::Create(Image &img)
{
	const int &depth = img.getDepth();
	unsigned int id=0;

	// get a new id
	glGenTextures(1, &id);

	// and bind it as the present texture
	glBindTexture(GL_TEXTURE_2D, id);

	// Set the texture filtering according to global performance settings
	Effect::setTextureFilters();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// now build mipmaps from the texture data
	gluBuild2DMipmaps(GL_TEXTURE_2D,
					  depth,
					  img.getWidth(),
					  img.getHeight(),
					  depth==4 ? GL_RGBA : GL_RGB,
					  GL_UNSIGNED_BYTE,
					  img.getImage());

	// Create a texture handle
	const _tstring &fileName = img.getFileName();
	const int &width = img.getWidth();
	const int &height = img.getHeight();

	TextureHandle tex(fileName, width, height, depth==4, id);
	
	// Cache the texture handle
	tlist.push_back(tex);

	// finally return the texture handle
	return getHandle(tex.getID());
}

void TextureManager::Set(unsigned texID)
{
	// do we need to change the texture
	if(texID != prestex)
	{
		// set to the id
		glBindTexture(GL_TEXTURE_2D, texID);

		// Set the texture filtering according to global performance settings
		Effect::setTextureFilters();

		// and set as present texture
		prestex=texID;
	}
}

TextureHandle* TextureManager::getHandle(unsigned int texid)
{
	ASSERT(!tlist.empty(), _T("No textured allocated"));

	// find the id
	TListType::iterator found = find_if(tlist.begin(), tlist.end(), bind(CheckID, _1, texid));

	if(found != tlist.end())
	{
		return &(*found); // return the found texture
	}
	else
	{
		FAIL(_T("id not found"));
		return 0;
	}
}

TextureHandle* TextureManager::getHandle(const _tstring &str)
{
	ASSERT(!tlist.empty(), _T("No textures allocated."));

	// find the name
	TListType::iterator found = find_if(tlist.begin(), tlist.end(), bind(CheckStr, _1, str));

	if(found != tlist.end())
	{
		return &(*found); // return the found texture
	}
	else
	{
		FAIL(_T("id not found"));
		return 0;
	}
}

void TextureManager::Delete(unsigned int texid)
{
	// anything to do?
	if(!tlist.empty())
	{
		// find the id
		TListType::iterator found = find_if(tlist.begin(), tlist.end(), bind(CheckID, _1, texid));

		// was it found?
		if(found!=tlist.end())
		{
			found->release();
			
			// remove the texture
			tlist.erase(found);
		}
	}
}

void TextureManager::Delete(const _tstring &fileName)
{
	// anything to do?
	if(!tlist.empty())
	{
		// find the name
		TListType::iterator found = find_if(tlist.begin(), tlist.end(), bind(CheckStr, _1, fileName));

		// was it found?
		if(found != tlist.end())
		{
			found->release();
			
			// remove the texture from the list
			tlist.erase(found);
		}
	}
}

void TextureManager::release(void)
{
	for_each(tlist.begin(), tlist.end(), bind(&TextureHandle::release, _1));
}

void TextureManager::reaquire(void)
{
	for_each(tlist.begin(), tlist.end(), bind(&TextureHandle::reaquire, _1));
}


}; // namespace
