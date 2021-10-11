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

#include "LGvrfld.h"
#include "LGstrcts.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  VariableField::VariableField
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
VariableField::VariableField() :
	Field(0)
{
	memset(&m_cFS[0], FS, 1);
	memset(&m_cFS[1], '\0', 1);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  VariableField::~VariableField
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
VariableField::~VariableField()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  VariableField::Set
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
void VariableField::Set(char **ppcData)
{
	WORD nIndex;

	for (nIndex = 0; nIndex < MAX_MSG_LEN; nIndex++)
	{
		if (*(*ppcData+nIndex) == m_cFS[0])
			break;
	}

	Empty();
	AllocBuffer(nIndex);
 
	Field::Set(&*ppcData);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  VariableField::Operators
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
const VariableField & VariableField::operator = (const char *pcData)
{
	AssignCopy(SafeStrLen(pcData), pcData);

	return *this;
}

const VariableField & VariableField::operator = (const RWCString & strValue)
{
	AssignCopy(strValue.length(), strValue.data());

	return *this;
}

const VariableField & VariableField::operator = (WORD nData)
{
	char cTemp(5);

	sprintf(&cTemp, "%d", nData);

	AssignCopy(SafeStrLen(&cTemp), &cTemp);

	return *this;
}

const VariableField & VariableField::operator += (const char *pcData)
{
	ConcatInPlace(SafeStrLen(pcData), pcData);

	return *this;
}

const VariableField & VariableField::operator += (const RWCString & strValue)
{
	ConcatInPlace(strValue.length(), strValue.data());

	return *this;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  VariableField::OperatorHelpers
 *
 *           Synopsis:  routines to aid operators
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
void VariableField::ConcatCopy(WORD nSrcLen1, const char *pcData1,
								WORD nSrcLen2, const char *pcData2)
{
	WORD nNewLen = nSrcLen1 + nSrcLen2;
	AllocBuffer(nNewLen);
	memcpy(m_pcData, pcData1, nSrcLen1);
	memcpy(&m_pcData[nSrcLen1], pcData2, nSrcLen2);
}

void VariableField::ConcatInPlace(WORD nSrcLen, const char *pcData)
{
	if (m_nDataLength + nSrcLen > m_nAllocLength)
	{
		char *pcOldData = m_pcData;
		ConcatCopy(m_nDataLength, pcOldData, nSrcLen, pcData);
		SafeDelete(pcOldData);
	}
	else
	{
		memcpy(&m_pcData[m_nDataLength], pcData, nSrcLen);
		m_nDataLength += nSrcLen;
	}

	m_pcData[m_nDataLength] = '\0';
}
