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
#ifndef LGVRLFLD_H
#define LGVRLFLD_H

#include "LGvrfld.h"

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

class VariableLenField : public VariableField
{

// Attributes

public:

protected:

private:

	Field m_oLength;

// Methods

public:
	// Constructors
	VariableLenField(WORD nSizeOfLength = 2);

	// Destructors
	~VariableLenField();

	void Set(char **ppcData);
	void Get(char **ppcData);

	// operators
	const VariableLenField & operator = (const char *pcData);
	const VariableLenField & operator = (const RWCString & strValue);
	const VariableLenField & operator = (WORD nData);
	const VariableLenField & operator += (const char *pcData);
	const VariableLenField & operator += (const RWCString & strValue);

protected:

private:

};

#endif // LGVRLFLD_H
