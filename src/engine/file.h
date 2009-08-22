/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2003-2007,2009 Game Creation Society
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Game Creation Society nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE Game Creation Society ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE Game Creation Society BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _FILE_H_
#define _FILE_H_

namespace Engine {

/**
Creates a directory
@param path Directory to create
*/
void createDirectory(const string &path);

/**
Use this command to set the current working directory
@param path The new drectory to set as the current working directory
@return true if the current working directory was changed, false otherwise
*/
bool setWorkingDirectory(const string &path);

/**
Gets the current working directory
@return The current working directory
*/
string getWorkingDirectory(void);

/**
Gets the home directory of the current user
@return The home directory
*/
string getAppDataDirectory(void);

/**
Gets the directory where the application is located
@return Application's directory
*/
string getApplicationDirectory(void);

/**
Appends a file name to a path
@param path directory name
@param fileName file name
@return The composite path
*/
string pathAppend(const string &path, const string &fileName);

/**
Wraps low-level file input / output operations
*/
class File
{
private:
	/** The raw data bytes of the file */
	unsigned char * data;

	/** The size of the file such as that the last accessible offset into the file is this->size-1 */
	size_t size;

	/** The read/write cursor's position in the file */
	size_t cursor;

	/**
	Gets the length of a file as a number of bytes on disk
	@param fileName file to examine
	@return Size of the file in bytes
	*/
	static streamsize getBytesOnDisk(const string &fileName);

protected:
	/** Location of the file */
	string fileName;

public:
	/** Status of a disk/file operation */
	enum FILE_STATUS
	{
		FILE_STATUS_SUCCESS=0,
		FILE_STATUS_NOT_FOUND,
		FILE_STATUS_CANNOT_OPEN,
		FILE_STATUS_CANNOT_ACCESS
	};

	/** Seek origin marker */
	enum FILE_SEEK
	{
		FILE_SEEK_BEGIN,
		FILE_SEEK_CURRENT,
		FILE_SEEK_END
	};

	/** Constructor */
	File(void)
	{
		clear();
	}

	/** Copy constructor */
	File(const File &file);

	/**
	Constructor
	@param fileName The file to open
	@param binary The file is binary data
	*/
	File(const string &fileName, bool binary)
	{
		clear();
		openFile(fileName, binary);
	}

	/** Destructor */
	~File(void)
	{
		destroy();
	}

	/**
	Clears all data members, resetting the file to a just-constructed state.
	Does this with no regard for the state of the data at the time of the call.
	*/
	void clear(void);

	/**
	Frees data associated with the file and then clears the file to a just-constructed state
	*/
	void destroy(void);

	/**
	Opens a file and reads its contents into the buffer
	@param fileName The file to open
	@param binary The file is binary data
	@return true if the file was opened and read correctly, false otherwise
	*/
	bool openFile(const string &fileName, bool binary);

	/**
	Saves the data to the specified file
	@param fileName The file to save as
	@param binary The file is binary data
	@return true if the file was opened and written correctly, false otherwise
	*/
	bool saveFile(const string &fileName, bool binary);

	/**
	Saves the data to the file specified in this->fileName
	@param binary The file is binary data
	@return true if the file was opened and written correctly, false otherwise
	*/
	bool saveFile(bool binary)
	{
		return saveFile(getFilename(), binary);
	}

	/**
	Gets the size of the file data
	@return File size in bytes
	*/
	size_t getSize(void) const
	{
		return size;
	}

	/**
	Gets a single character from the file, then increments the cursor
	@return Single character from the file
	*/
	unsigned char getChar(void);

	/**
	Gets a single character from the file, does not increment the cursor
	@return Single character from the file
	*/
	unsigned char peekChar(void);

	/**
	Gets a single character from the file, does not increment the cursor
	@param pos Position of the character within the file
	@return Single character from the file
	*/
	unsigned char peekChar(size_t pos);

	/**
	Gets the current read/write cursor of the file
	*/
	size_t tell(void) const
	{
		return cursor;
	}

	/**
	Determines whether the file has been loaded correctly.
	@return true if the file has been loaded correctly, false otherwise.
	*/
	bool loaded(void) const;

	/**
	Determines whether the curor has exceeded or met the end of the file.
	@return If EOF then true, otherwise false
	*/
	bool endOfFile(void) const;

	/**
	Sets the cursor position of the file
	@param offset The offset from the origin to seek towards
	@param origin The origin of the offset from some known point in the file
	@return The new cursor position
	*/
	size_t seek(size_t offset, FILE_SEEK origin);

	/**
	Seeks to the beginning of the file
	*/
	void rewind(void)
	{
		seek(0, FILE_SEEK_BEGIN);
	}

	/**
	Reads data from the file and copies it into the specified buffer
	@param buffer The destination buffer for the data
	@param count The number of bytes to read
	@return The number of bytes copied
	*/
	size_t read(unsigned char * buffer, size_t count);

	/**
	Reads data from the file and copies it into a second file
	@param file The destination file for the data
	@param count The number of bytes to read
	@return The number of bytes copied
	*/
	size_t read(File &file, size_t count);

	/**
	Reads data from the file and copies it into the specified buffer
	@param buffer The destination buffer for the data
	@param count The number of bytes to read
	@return The number of bytes copied
	*/
	size_t read(void *buffer, size_t count)
	{
		return read((unsigned char *)buffer, count);
	}

	/**
	Writes data from the buffer and copies it into the file from the current cursor position
	If the internal is not large enough to perform this write, then it will be expanded
	@param buffer The data buffer
	@param count The number of bytes to write
	@return The number of bytes actually written
	*/
	size_t write(unsigned char * buffer, size_t count);

	/**
	Writes a string to file
	@param s The string to write
	@return The number of bytes actually written
	*/
	size_t write(const string &s);

	/**
	Writes data from the buffer and copies it into the file from the current cursor position
	If the internal is not large enough to perform this write, then it will be expanded
	@param buffer The data buffer
	@param count The number of bytes to write
	@return The number of bytes actually written
	*/
	size_t write(void *buffer, size_t count)
	{
		return write((unsigned char *)buffer, count);
	}

	/**
	Increase the size of the buffer to the specified size.
	@param size the new size of the buffer, in bytes
	*/
	void reserve(size_t size);

	/**
	Gets the full file path and name
	@return file name and path
	*/
	const string& getFilename(void) const
	{
		return fileName;
	}

	/**
	Gets the full file path and name
	@return file name and path
	*/
    string getFilenameNoPath(void) const;

    /**
    Gets the full file path and name
    @param fileName File name
    @return file name and path
    */
    static string getFilenameNoPath(const string &fileName);

	/**
	Get the directory the file was located in
	@return file path
	*/
	string getPath(void) const;

	/**
	Get the directory the file was located in
	@param fileName File name
	@return file path
	*/
	static string getPath(const string &fileName);

	/**
	Get the file extension
	@return extension
	*/
	string getExtension(void) const;

	/**
	Transforms the given file name into all lower case with UNIX style slashes
	@param fileName File name to transform
	@return transformed filename
	*/
	static string fixFilename(const string &fileName);

	/**
	Finds the extension marker in a string.
	findExtensionDelimeter determines the index of the extension delimiter
	character in a given fileName, when possible. For example, calling with
	the fileName = "image.jpeg" will return 5 and "data/text.txt" will
	return 9.  Also be aware that "data/text.txt.bak" will return 13, as
	this is the last string that is clearly identifiable as a file
	extension.
	@param fileName Name of the file
	@return Index of the extension delimiter character in the fileName.
	If there is no extension, then the length of the filename is returned.
	*/
	static size_t findExtensionDelimeter(const string &fileName);

	/**
	Removes a file extension from the file name
	@param fileName The name of the file to extract the extension from
	@return Removes a file extension from a file path and returns the modified file name.
	*/
	static string stripExtension(const string &fileName);

	/**
	Gets the file extension from a file path
	@param fileName The name of the file to extract the extension from
	@return Returns the file extension
	*/
	static string getExtension(const string &fileName);

	/**
	Determine if a file exists on disk
	@param fileName Name of the file to check access permissions on
	@return true if the file is on disk, false otherwise
	*/
	static bool isFileOnDisk(const string &fileName);
};

} //namespace Engine

#endif
