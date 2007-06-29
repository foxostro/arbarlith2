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

#ifndef _TEXTURE_HANDLE_H_
#define _TEXTURE_HANDLE_H_


namespace Engine { 


/** Handle to a texture object */
class TextureHandle
{
private:
	/** file name of the texture */
	_tstring fileName;

	/** width of texture */
	int width;

	/** height of texture */
	int height;
	
	/** whether the texture has an alpha component */
	bool alpha;

	/** the texture ID */
	unsigned int id;

public:
	/** Constructor */
	TextureHandle(void);

	/**
	Constructor
	@param fileName The name of the image file
	@param width The width of the texture
	@param height The height of the texture
	@param alpha Indicates the texture has an alpha component
	@param id
	*/
	TextureHandle(const _tstring &fileName, int width, int height, bool alpha, unsigned int id);

	/** Gets the file name of the texture */
	_tstring getFileName(void) const
	{
		return fileName;
	}

	/** Gets the width of texture */
	int getWidth(void) const
	{
		return width;
	}

	/** Gets the height of texture */
	int getHeight(void) const
	{
		return height;
	}
	
	/** Gets whether the texture has an alpha component */
	bool getAlpha(void) const
	{
		return alpha;
	}

	/** Gets the texture ID */
	unsigned int getID(void) const
	{
		return id;
	}

	/**
	Determines whether the texture is currently valid.  If
	a texture is invalid, it may need to be reaquired or it
	may be in error.
	@return true if the texture is valid, false otherwise
	*/
	bool isValid(void) const
	{
		return id>0;
	}

	/** Releases the texture */
	void release(void);

	/** Reloads the texture */
	void reaquire(void);
};


}; // namespace


#endif
