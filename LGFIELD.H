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
#ifndef LGFIELD_H
#define LGFIELD_H

#include <rw/cstring.h>

#include "LGtypes.h"

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

class Field
{

// Attributes

public:

protected:

	char *m_pcData;
	WORD m_nDataLength;
	WORD m_nAllocLength;

private:

// Methods

public:

	// Constructors
	Field(WORD nDataLength);

	// Destructors
	virtual ~Field();

	// Virtuals
	virtual void Set(char **ppcData);
	virtual void Get(char **ppcData);

	// Get methods
	char* Data();
	WORD Length();

	// operators
	const Field & operator = (const char *pcData);
	const Field & operator = (const RWCString & strValue);
	const Field & operator = (WORD nData);
	BOOL operator == (const char *pcData);
	BOOL operator == (const RWCString & strValue);
	BOOL operator == (WORD nData);
	operator const char*() const {return m_pcData;}

protected:

	// implementation helpers
	void Init();
	void Empty();
	void AssignCopy(WORD nSrcLen, const char *pcSrcData);
	void AllocBuffer(WORD nLen);
	static void SafeDelete(char *pc);
	static WORD SafeStrLen(const char *pc);
	int Compare(const char * pcData) const;

private:

};

#endif // LGFIELD_H
