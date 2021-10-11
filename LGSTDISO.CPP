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

#include "LGstdiso.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ISO8583Message::ISO8583Message
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
ISO8583Message::ISO8583Message()
{
	// Initialise variables
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ISO8583Message::ISO8583Message
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  Message type
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
ISO8583Message::ISO8583Message(WORD nMsgType)
{
	// Initialise variables
	m_nMsgType = nMsgType;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ISO8583Message::~ISO8583Message
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
ISO8583Message::~ISO8583Message()
{

}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ISO8583Message::DecodeMessage
 *
 *           Synopsis:  Decode raw buffer into type fields
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS ISO8583Message::DecodeMessage()
{
	return 0;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ISO8583Message::EncodeMessage
 *
 *           Synopsis:  Encode type fields into raw buffer
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS ISO8583Message::EncodeMessage()
{
	return 0;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ISO8583Message::ValidateMessage
 *
 *           Synopsis:  Test validity of message
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS ISO8583Message::ValidateMessage()
{
	return 0;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ISO8583Message::ConvertToInternal
 *
 *           Synopsis:  Maps type fields to internal format
 *
 *   Input parameters:  None
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
void ISO8583Message::ConvertToInternal()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ISO8583Message::ConvertFromInternal
 *
 *           Synopsis:  Maps type fields from internal format
 *
 *   Input parameters:  None
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
void ISO8583Message::ConvertFromInternal()
{
}
