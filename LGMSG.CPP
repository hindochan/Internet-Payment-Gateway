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

#include "LGmsg.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Message::Message
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
Message::Message()
{
	// Initialise variables
	m_nMsgType = 0; // Override in derived class ?? remove later
	m_enMsgSequence = UNDEFINED;
	m_stDataMsg.nMessageLen = 0;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Message::~Message
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
Message::~Message()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Message::ReferenceMethods
 *
 *           Synopsis:  Reference Member to get and set variable
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	Reference to variable
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
DataMsg & Message::DataMsg()
{
	return m_stDataMsg;
}

IntMsg & Message::InternalMsg()
{
	return m_stInternalMsg;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Message::SetMethods
 *
 *           Synopsis:  Member to set variable
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
void Message::SetInputOutput()
{
	m_enMsgSequence = INPUTOUTPUT;
}

void Message::SetOutputInput()
{
	m_enMsgSequence = OUTPUTINPUT;
}

void Message::SetOutputOnly()
{
	m_enMsgSequence = OUTPUTONLY;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Message::TesterMethods
 *
 *           Synopsis:  Member to test variable
 *
 *   Input parameters:  None
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
BOOL Message::IsInputOutput()
{
	return (m_enMsgSequence == INPUTOUTPUT);
}

BOOL Message::IsOutputInput()
{
	return (m_enMsgSequence == OUTPUTINPUT);
}

BOOL Message::IsOutputOnly()
{
	return (m_enMsgSequence == OUTPUTONLY);
}

void Message::DumpInternal()
{
	cout << "Date is :- ";
	cout << m_stInternalMsg.strDate << "\n";
	cout << "Time is :- ";
	cout << m_stInternalMsg.strTime << "\n";
	cout << "Tran Type is :- ";
	cout << m_stInternalMsg.strTranType << "\n";
	cout << "Interface name is :- ";
	cout << m_stInternalMsg.strMerchantInterfaceName << "\n";
	cout << "Merchant number is :- ";
	cout << m_stInternalMsg.strMerchantNumber << "\n";
	cout << "Result code is :- ";
	cout << m_stInternalMsg.strResultCode << "\n";
	cout << "Auth code is :- ";
	cout << m_stInternalMsg.strAuthCode << "\n";
	cout << "Card number is :- ";
	cout << m_stInternalMsg.strCardNumber << "\n";
	cout << "Expiry date is :- ";
	cout << m_stInternalMsg.strExpiryDate << "\n";
	cout << "Amount is :- ";
	cout << m_stInternalMsg.strAmount << "\n";
	cout << "Tran log id is :- ";
	cout << m_stInternalMsg.strTranLogId << "\n";
	cout << "Business date is :- ";
	cout << m_stInternalMsg.strBusinessDate << "\n";
	cout << "RRN is :- ";
	cout << m_stInternalMsg.strRetrievalRefNbr << "\n";
	cout << "Response code is :- ";
	cout << m_stInternalMsg.strResponseCode << "\n";
	cout << "Terminal Id is :- ";
	cout << m_stInternalMsg.strTerminalId << "\n";
	cout << "Text message is :- ";
	cout << m_stInternalMsg.strTextMessage << "\n";
	cout << "STAN is :- ";
	cout << m_stInternalMsg.strSystemTraceAudit << "\n";
	cout.flush();
}
