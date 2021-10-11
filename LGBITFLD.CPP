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

#include <math.h>

#include "LGbitfld.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  BitMapField::BitMapField
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
BitMapField::BitMapField(WORD nDataLength)
	: Field(nDataLength)
{
	// Initialise variables
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  BitMapField::~BitMapField
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
BitMapField::~BitMapField()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  BitMapField::SetBit
 *
 *           Synopsis:  Sets specified bit in string
 *
 *   Input parameters:  Bit number to set - starts at 1
 *
 *  Output parameters:  None
 *
 *       Return value:	None
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void BitMapField::SetBit(WORD nIndex)
{
	if (0 < nIndex && nIndex <= (m_nDataLength * 8))
	{
		char *pcTemp;

		pcTemp  = &m_pcData[(nIndex - 1)/m_nDataLength];

		*pcTemp = *pcTemp | (char)pow(2,(7 - (nIndex -1)%8));
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  BitMapField::ClearBit
 *
 *           Synopsis:  Clears specified bit in string
 *
 *   Input parameters:  Bit number to clear - starts at 1
 *
 *  Output parameters:  None
 *
 *       Return value:	None
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void BitMapField::ClearBit(WORD nIndex)
{
	if (0 < nIndex && nIndex <= (m_nDataLength * 8))
	{
		char *pcTemp;

		pcTemp  = &m_pcData[(nIndex - 1)/m_nDataLength];

		*pcTemp = *pcTemp & (255 - (char)pow(2,(7 - (nIndex -1)%8)));
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  BitMapField::IsBitSet
 *
 *           Synopsis:  Tests a particular bit
 *
 *   Input parameters:  Bit number to test - starts at 1
 *
 *  Output parameters:  None
 *
 *       Return value:	BOOL
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
BOOL BitMapField::IsBitSet(WORD nIndex)
{
	BOOL bBool = FALSE;

	if (0 < nIndex && nIndex <= (m_nDataLength * 8))
	{
		if ((m_pcData[(nIndex - 1)/m_nDataLength] << ((nIndex - 1)%8)) & 128)
			bBool = TRUE;
	}
	return bBool;
}
