/***********************************************************************
 *  LAG - Source file header
 *
 *              Layer:  
 *             Module:  
 *           $Logfile:   $
 *         Originator:  Adam Hawley
 *          $Revision:   $
 *       on     $Date:   $
 *       by   $Author:   $
 *
 *           Synopsis:  class definition for 
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */
#ifndef LGFLEPRT_H
#define LGFLEPRT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

#include <rw/ctoken.h>

#include "LGport.h"

/************************************************************************
 *
 *   CLASS DECLARATION
 *
 *               Name:  
 *
 *           Synopsis:  .
 *
 *              Notes:  .
 *
 ************************************************************************
 */

#define READFILE m_pstFileDescs->fdReadFile
#define WRITEFILE m_pstFileDescs->fdWriteFile

class FilePort : public Port
{

// Attributes

public:

protected:

private:

	typedef struct filedescs
	{
		int fdReadFile;
		int fdWriteFile;
	} FileDescs;

	FileDescs *m_pstFileDescs; // File descriptors

	BOOL m_bReadFilePresent;
	RWCString m_strReadFileName;
	BOOL m_bWriteFilePresent;
	RWCString m_strWriteFileName;

	char m_cNewLine[1];

// Methods

public:
	// Constructors
	FilePort();
	FilePort(WORD nType, RWCString strCommsConfig);

	// Destructors
	~FilePort();

	void * PreThreadInit();
	STATUS Init(void *pVoid);
	STATUS Open();
	STATUS Read();
	STATUS Write();
	STATUS Close();

	STATUS WaitForConnection();

protected:

private:

};

#endif // LGFLEPRT_H
