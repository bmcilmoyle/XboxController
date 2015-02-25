#ifndef _CTextureFromBMP_HEADER_GUARD_
#define _CTextureFromBMP_HEADER_GUARD_

// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//

#include <fstream>
#include <string>
#include "C24BitBMPpixel.h"
//#include <gl\glext.h>		// OpenGL Extensions (for cube mapping)
#include <GL\glew.h>
#include <gl\freeglut.h>

typedef unsigned char uchar;

class CTextureFromBMP
{
public:
	CTextureFromBMP();
	~CTextureFromBMP();
	// Returns true if able to load texture and store it
	// Updated: December 2010 for ATI cards, too!
	// CHANGE: Now pass the texture unit, not the texture number...
	// (texture number is created automatically). Pass GL_TEXTURE0, GL_TEXTURE1, etc. 
	bool CreateNewTextureFromBMPFile( std::string fileName, GLenum textureUnit );		
	bool CreateNewTextureFromBMPFile2( std::string fileName, GLenum textureUnit, bool bGenerateMIPMap );		
	bool CreateNewTextureFromBMPFile_OLD(std::string fileName, GLuint textureNumber);		

	// _____  _     _                        _     _                         
	//|_   _|| |_  (_) ___  _ __  __ _  _ _ | |_  (_) ___  _ _   ___ __ __ __
	//  | |  | ' \ | |(_-< | '_ \/ _` || '_||  _| | |(_-< | ' \ / -_)\ V  V /
	//  |_|  |_||_||_|/__/ | .__/\__,_||_|   \__| |_|/__/ |_||_|\___| \_/\_/ 
	//                     |_|                                               
	bool CreateNewCubeTextureFromBMPFiles( std::string posX_fileName, std::string negX_fileName, 
		                                   std::string posY_fileName, std::string negY_fileName, 
										   std::string posZ_fileName, std::string negZ_fileName, 
										   GLenum textureUnit, 
										   GLenum &errorEnum, std::string &errorString, std::string &errorDetails );
	bool CreateNewBMPFromCurrentTexture( int mipMapLevel );
	bool LoadBMP( std::string fileName );
	bool LoadBMP2( std::string fileName );		// Faster loader
	bool SaveBMP( std::string fileName );
	// Deletes the data (and the array) - used after calling LoadBMP
	bool ClearBMP( void );	
	void SetDebug_cout_output( bool bHave_cout_output );

	bool ResizeBitmap(int DesiredHeight, int DesiredWidth);
	// Returns true if texture exists and can be applied
	bool MakeTextureActive(void);
	//	new Texture replacement mode.
	// Call this to set a new replacement mode: GL_MODULATE, GL_DECAL, GL_REPLACE, GL_BLEND
	bool SetTextureEnvironmentMode(GLint textureReplacementMode);
	// Call this after you are done with the texture replacement mode
	bool RestoreOldTextureEnvironmentMode(void);
	// This is for displaying the image as a raster image...
	void DisplayTextureAsRasterBitmap(GLfloat xOrigin, GLfloat yOrigin, GLfloat xMove, GLfloat yMove);
	// This is for error handling
	int GetLastErrorNumber(void);
	// Reutn false if everything is OK
	bool bWasThereAnOpenGLError(void);
	bool bWasThereAnOpenGLError(GLenum &errorEnum);
	bool bWasThereAnOpenGLError(GLenum &errorEnum, std::string &errorString);
	bool bWasThereAnOpenGLError(GLenum &errorEnum, std::string &errorString, std::string &errorDetails);
	std::string DecodeLastError(int errorNum);
	// Some getters.
	unsigned long GetHeight(void);
	unsigned long GetWidth(void);
	unsigned long GetOriginalHeight(void);
	unsigned long GetOriginalWidth(void);
	unsigned long GetFileSize(void);
	unsigned long GetFileSizeInBytes(void);
	unsigned short GetReserved1(void);
	unsigned short GetReserved2(void);
	unsigned long GetBitsPerPixel(void);
	unsigned long GetOffsetInBits(void);
	unsigned long GetImageSizeInBytes(void);
	unsigned long GetCompressionMode(void);
	unsigned long GetPixelsPerMeterX(void);
	unsigned long GetPixelsPerMeterY(void);
	unsigned long GetNumberOfLookUpTables(void);
	unsigned long GetNumberOfImportantColours(void);
	unsigned long m_numberOfRows;
	unsigned long m_numberOfColumns;
	//
	GLuint getTextureNumber(void);
private:
	// The actual image information
	C24BitBMPpixel* m_p_theImages;	
	//C32BitBMPpixel* m_p_theImages;	
	GLuint m_textureNumber;
	GLint m_oldTextureEnvironmentMode;
	unsigned long m_Height;
	unsigned long m_Width;
	unsigned long m_OriginalHeight;
	unsigned long m_OriginalWidth;
	unsigned short m_reserved1;
	unsigned short m_reserved2;
	unsigned long m_FileSize;
	unsigned long m_offsetInBits;
	unsigned long m_headerSize;
	unsigned short m_numberOfPlanes;
	unsigned short m_bitPerPixel;	// 1, 4, 8, 15, 24, 32, or 64
	unsigned long m_compressionMode;	// 0 if NOT compressed (i.e. 24 bit bitmap)
	unsigned long m_imageSizeInBytes;	// Total bytes in image - may be zero
	unsigned long m_PixelsPerMeterX;
	unsigned long m_PixelsPerMeterY;
	unsigned long m_numberOfLookUpTableEntries;	// Could be 0 for 24 bit format
	unsigned long m_numberOfImportantColours;	// 0 if all colours are important
	// These functions shift numbers by one, two, and three bytes.
	unsigned long ShiftAndAdd_Byte_to_ULong(unsigned long theULong, char theByte, int bytesToShift);
	unsigned short ShiftAndAdd_Byte_to_UShort(unsigned short theUShort, char theByte, int bytesToShift);
	// These are used to read the data from a file (LoadBMP method).
	unsigned long ReadAnUnsignedLong(std::ifstream& theStream);
	unsigned short ReadAnUnsignedShort(std::ifstream& theStream);
	char ReadAByte(std::ifstream& theStream);
	// These are used to read the data from a file (LoadBMP2 method).
	// * index is incremented appropriate amount (to next char)
	// * no bounds checking for array
	unsigned long ReadNextUnsignedLong(char* data, unsigned long &index);
	unsigned short ReadNextUnsignedShort(char* data, unsigned long &index);
	char ReadNextChar(char* data, unsigned long &index);
	//
	void WriteAsUnsignedShort( unsigned short value, std::ofstream& theStream );
	void WriteAsUnsignedLong( unsigned long value, std::ofstream& theStream );


	// This is for error handling:
	int m_lastErrorNum;
	enum ERORRCONSTANTS
	{
		ERROR_NO_ERROR = 0,
		ERORR_FILE_WONT_OPEN = 1,
		ERROR_NOT_A_BMP_FILE = 2,
		ERROR_NOT_A_24_BIT_BITMAP = 3,
		ERROR_NOT_ENOUGHT_MEMORY_FOR_BITMAP = 4
	};
	//
	bool m_bHave_cout_output;
};

#endif
