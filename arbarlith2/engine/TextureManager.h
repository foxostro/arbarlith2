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

#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "TextureHandle.h"



namespace Engine { 



class TextureManager
{
private:
	/** the texture list type */
	typedef std::list<TextureHandle> TListType;
	
	/** the texture list */
	TListType tlist;

	/** present texture */
	unsigned int prestex;

public:
	/** Constructor */
	TextureManager();
	
	/** Destructor */
	~TextureManager();

	/**
	Load a texture
	@param fileName texture to load
	@return TextureHandle
	*/
	TextureHandle* Load(const _tstring &fileName);

	/**
	Create a texture from an image
	@param img image class to create from
	@return TextureHandle
	*/
	TextureHandle* Create(Image *img);

	/**
	Create a texture from an image
	@param img image class to create from
	@return TextureHandle
	*/
	TextureHandle* Create(Image &img);

	/**
	Set the current texture
	@param texID The texture ID
	*/
	void Set(unsigned int texID);

	/**
	Get info by texure id
	@param texID The texture ID
	*/
	TextureHandle* getHandle(unsigned int texID);

	/**
	Get info by texture name
	@param name Texture name
	*/
	TextureHandle* getHandle(const _tstring &name);

	/**
	Delete a single texture
	@param texID The texture ID
	*/
	void Delete(unsigned int texID);

	/**
	Delete a single texture
	@param name The texture name
	*/
	void Delete(const _tstring &name);
	
	/** Release all textures */
	void release(void);
	
	/** Reaquire all textures */
	void reaquire(void);
};


}; // namespace


#endif
