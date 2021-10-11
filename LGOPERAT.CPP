/***********************************************************************
 *  LAG - Source file header
 *
 *              Layer:  
 *             Module: 
 *           $Logfile:   $
 *         Originator:  Nilesh Hindocha
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

#include "LGoperator.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorMessage::OperatorMessage
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
OperatorMessage::OperatorMessage(WORD nMsgType)
{
	// Initialise variables
	m_nMsgType = nMsgType;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorMessage::~OperatorMessage
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
OperatorMessage::~OperatorMessage()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorMessage::DecodeMessage
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
STATUS OperatorMessage::DecodeMessage()
{
	STATUS nStatus = OK;

	memcpy (&m_stCommand, m_stDataMsg.cMessage, sizeof (m_stCommand));

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorMessage::EncodeMessage
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
STATUS OperatorMessage::EncodeMessage()
{
	STATUS nStatus = OK;

	memcpy (m_stDataMsg.cMessage,
			&m_stResponse,
			sizeof (m_stResponse));

	m_stDataMsg.nMessageLen = sizeof (m_stResponse);

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorMessage::ValidateMessage
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
STATUS OperatorMessage::ValidateMessage()
{
	STATUS nStatus = OK;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorMessage::ConvertToInternal
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
void OperatorMessage::ConvertToInternal()
{
	if (IsOutputInput())
	{
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorMessage::ConvertFromInternal
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
void OperatorMessage::ConvertFromInternal()
{
	if (IsOutputInput())
	{
	}
}
