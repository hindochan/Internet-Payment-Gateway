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
#ifndef LGVRFLD_H
#define LGVRFLD_H

#include "LGfield.h"

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

#define FS 0x1c

class VariableField : public Field
{

// Attributes

public:

protected:

private:

	char m_cFS[2];

// Methods

public:
	// Constructors
	VariableField();

	// Destructors
	~VariableField();

	virtual void Set(char **ppcData);

	// operators
	const VariableField & operator = (const char *pcData);
	const VariableField & operator = (const RWCString & strValue);
	const VariableField & operator = (WORD nData);
	const VariableField & operator += (const char *pcData);
	const VariableField & operator += (const RWCString & strValue);

protected:

	// implementation helpers
	void ConcatCopy(WORD nSrcLen1, const char *pcData1,
					WORD nSrcLen2, const char *pcData2);
	void ConcatInPlace(WORD nSrcLen, const char *pcData);


private:

};

#endif // LGVRFLD_H
