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

#include "LGfield.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Field::Field
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  Data length
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
Field::Field(WORD nDataLength)
{
	// Initialise variables
	Init();

	AllocBuffer(nDataLength);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Field::~Field
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
Field::~Field()
{
	if (m_pcData)
	{
		delete m_pcData;
		m_pcData = NULL;
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Field::Set
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
void Field::Set(char **ppcData)
{
	memcpy(m_pcData, *ppcData, m_nDataLength);
	*ppcData += m_nDataLength;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Field::Get
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
void Field::Get(char **ppcData)
{
	memcpy(*ppcData, m_pcData, m_nDataLength);
	*ppcData += m_nDataLength;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Field::GetMethods
 *
 *           Synopsis:  returns variable
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	value of variable
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
char* Field::Data()
{
	return m_pcData;
}

WORD Field::Length()
{
	return m_nDataLength;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Field::Operators
 *
 *           Synopsis:  Operators for class
 *
 *   Input parameters:  Various
 *
 *  Output parameters:  Various
 *
 *       Return value:	Various
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
const Field & Field::operator = (const char *pcData)
{
	WORD nMaxLen;

	nMaxLen = min(SafeStrLen(pcData), m_nAllocLength);

	AssignCopy(nMaxLen, pcData);

	return *this;
}

const Field & Field::operator = (const RWCString & strValue)
{
	WORD nMaxLen;

	nMaxLen = min(strValue.length(), m_nAllocLength);

	AssignCopy(nMaxLen, strValue.data());

	return *this;
}

const Field & Field::operator = (WORD nData)
{
	char cTemp(m_nDataLength);
	WORD nTemp = 0;

	sprintf(&cTemp, "%d", nData);

	nTemp = SafeStrLen(&cTemp);

	if (nTemp < m_nDataLength)
	{
		memset(m_pcData, '0', (m_nDataLength - nTemp));
		memcpy(m_pcData + (m_nDataLength - nTemp), &cTemp, nTemp);
	}
	else
		memcpy(m_pcData, &cTemp, m_nDataLength);

	return *this;
}

BOOL Field::operator == (const char *pcData)
{
	return Compare(pcData) == 0;
}

BOOL Field::operator == (const RWCString & strValue)
{
	return Compare(strValue.data()) == 0;
}

BOOL Field::operator == (WORD nData)
{
	char cTemp(m_nDataLength);
	BOOL bTemp = FALSE;

	sprintf(&cTemp, "%d", nData);
	bTemp = Compare(&cTemp) == 0;

	return bTemp;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Field::OperatorHelpers
 *
 *           Synopsis:  routines to aid operators
 *
 *   Input parameters:  Various
 *
 *  Output parameters:  Various
 *
 *       Return value:	Various
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void Field::Init()
{
	m_nDataLength = m_nAllocLength = 0;
	m_pcData = NULL;
}

void Field::Empty()
{
	SafeDelete(m_pcData);
	Init();
}

void Field::AssignCopy(WORD nSrcLen, const char *pcSrcData)
{
	// check if it will fit
	if (nSrcLen > m_nAllocLength)
	{
		// it won't fit, allocate another one
		Empty();
		AllocBuffer(nSrcLen);
	}

	if (nSrcLen != 0)
	{
		memcpy(m_pcData, pcSrcData, nSrcLen);
		m_pcData[nSrcLen] = '\0';
	}

	m_nDataLength = nSrcLen;
}

void Field::AllocBuffer(WORD nLen)
{
	if (nLen)
	{
		m_pcData = new char[nLen + 1];
		if (!m_pcData)
		{
			m_nDataLength = 0;
			m_nAllocLength = 0;
		}
		else
		{
			m_pcData[nLen] = '\0';
			m_nDataLength = nLen;
			m_nAllocLength = nLen;
		}
	}
}

void Field::SafeDelete(char *pc)
{
	if (pc != NULL)
	{
		delete pc;
		pc = NULL;
	}
}

WORD Field::SafeStrLen(const char *pc)
{
	return (pc == NULL) ? NULL : strlen(pc);
}

int Field::Compare(const char * pcData) const
{
	return memcmp(m_pcData, pcData, m_nDataLength);
}
