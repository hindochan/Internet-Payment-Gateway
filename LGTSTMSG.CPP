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

#include "LGtstmsg.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TestMessage::TestMessage
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
TestMessage::TestMessage(WORD nMsgType) :
	m_oDelim(1),
	m_oCardNumber(16),
	m_oAmount(12),
	m_oAuthCode(6),
	m_oResponseCode(2)
{
	// Initialise variables
	m_nMsgType = nMsgType;
	m_pcPos = NULL;
	m_oDelim = "|";
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TestMessage::~TestMessage
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
TestMessage::~TestMessage()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TestMessage::DecodeMessage
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
STATUS TestMessage::DecodeMessage()
{
	STATUS nStatus = 0;

	m_pcPos = &m_stDataMsg.cMessage[0];

	if (IsOutputInput())
	{
		m_oAuthCode.Set(&m_pcPos);
		m_oResponseCode.Set(&m_pcPos);
	}

	m_pcPos = NULL;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TestMessage::EncodeMessage
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
STATUS TestMessage::EncodeMessage()
{
	STATUS nStatus = 0;

	m_pcPos = &m_stDataMsg.cMessage[0];

	if (IsOutputInput())
	{
		m_oCardNumber.Get(&m_pcPos);
		m_oDelim.Get(&m_pcPos);
		m_oAmount.Get(&m_pcPos);
	}

	m_stDataMsg.nMessageLen = (m_pcPos - &m_stDataMsg.cMessage[0]);

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TestMessage::ValidateMessage
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
STATUS TestMessage::ValidateMessage()
{
	STATUS nStatus = 0;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TestMessage::ConvertToInternal
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
void TestMessage::ConvertToInternal()
{
	if (IsOutputInput())
	{
		m_stInternalMsg.strAuthCode = m_oAuthCode;
		m_stInternalMsg.strResponseCode = m_oResponseCode;
		if (m_oResponseCode == "00")
			m_stInternalMsg.strResultCode = "0000";
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TestMessage::ConvertFromInternal
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
void TestMessage::ConvertFromInternal()
{
	if (IsOutputInput())
	{
		m_oCardNumber = m_stInternalMsg.strCardNumber;
		m_oAmount = m_stInternalMsg.strAmount;
	}
}
