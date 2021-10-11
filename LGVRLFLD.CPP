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
 *           Synopsis:  
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */

#include "LGvrlfld.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  VariableLenField::VariableLenField
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
VariableLenField::VariableLenField(WORD nSizeOfLength) :
	m_oLength(nSizeOfLength)
{
	// Initialise variables
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  VariableLenField::~VariableLenField
 *
 *           Synopsis:  Destructor for class object
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
VariableLenField::~VariableLenField()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  VariableLenField::Set
 *
 *           Synopsis:  Set up data
 *
 *   Input parameters:  pointer to data
 *
 *  Output parameters:  pointer to data
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void VariableLenField::Set(char **ppcData)
{
	m_oLength.Set(&*ppcData);

	Empty();
	AllocBuffer(atoi(m_oLength));

	Field::Set(&*ppcData);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  VariableLenField::Get
 *
 *           Synopsis:  Pass back data
 *
 *   Input parameters:  pointer to buffer
 *
 *  Output parameters:  pointer to buffer
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void VariableLenField::Get(char **ppcData)
{
	m_oLength.Get(&*ppcData);

	Field::Get(&*ppcData);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  VariableLenField::Operators
 *
 *           Synopsis:  Operators for class
 *
 *   Input parameters:  Various
 *
 *  Output parameters:  Various
 *
 *       Return value:  Various
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
const VariableLenField & VariableLenField::operator = (const char *pcData)
{
	AssignCopy(SafeStrLen(pcData), pcData);

	m_oLength = SafeStrLen(pcData);

	return *this;
}

const VariableLenField& VariableLenField::operator =(const RWCString & strValue)
{
	AssignCopy(strValue.length(), strValue.data());

	m_oLength = m_nDataLength;

	return *this;
}

const VariableLenField & VariableLenField::operator = (WORD nData)
{
	char cTemp(5);

	sprintf(&cTemp, "%d", nData);

	AssignCopy(SafeStrLen(&cTemp), &cTemp);

	m_oLength = m_nDataLength;

	return *this;
}

const VariableLenField & VariableLenField::operator += (const char *pcData)
{
	ConcatInPlace(SafeStrLen(pcData), pcData);

	m_oLength = m_nDataLength;

	return *this;
}

const VariableLenField & VariableLenField::operator += (const RWCString & strValue)
{
	ConcatInPlace(strValue.length(), strValue.data());

	m_oLength = m_nDataLength;

	return *this;
}
