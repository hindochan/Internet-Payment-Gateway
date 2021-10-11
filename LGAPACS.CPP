/***********************************************************************
 *  LAG - Source file header
 *
 *              Layer:  
 *             Module: 
 *           $Logfile:   $
 *         Originator:  Adam Hawley
 *          $Revision:   $
 *       on     $Date:  31st October 1996 
 *       by   $Author:  Deborah Samuel 
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

#include "LGapacs.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  APACS30Message::APACS30Message
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
APACS30Message::APACS30Message(WORD nMsgType) :
	
	m_oTempSep(1),
	m_oFS(1),
	m_oSTX(1),
	m_oETX(1),
	m_oMessageTypeId(2),
	m_oDIndicator(1),
	m_oTerminalId(8),
	m_oMessageNum(4),
	m_oTerminalType(4),
	m_oResponseCode(2),
	m_oConfirmationReq(1),
	m_oDate(4)

{
	// Initialise variables
	m_pcPos = NULL;
	m_nMsgType = nMsgType;

	m_iMessageNum = -1;   
	memset(&m_cSTX[0], STX, 1);
	memset(&m_cSTX[1], '\0', 1);
	memset(&m_cETX[0], ETX, 1);
	memset(&m_cETX[1], '\0', 1);
	memset(&m_cUS[0], US, 1);
	memset(&m_cUS[1], '\0', 1);
	memset(&m_cFS[0], FS, 1);
	memset(&m_cFS[1], '\0', 1);
	m_oSTX = m_cSTX;
	m_oETX = m_cETX;
	m_oFS = m_cFS;

	InitialiseFields();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  APACS30Message::~APACS30Message
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
APACS30Message::~APACS30Message()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  APACS30Message::DecodeMessage
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
STATUS APACS30Message::DecodeMessage()
{
	WORD nIndex;
	STATUS nStatus = 0;

	if (m_stDataMsg.nMessageLen >= 13)
	{
		// Temp function to mask out top bit ??
		MaskDataMsg();

		// Set pointer 
		m_pcPos = &m_stDataMsg.cMessage[0];
		
		// Decode fields
		m_oTempSep.Set(&m_pcPos); // STX
		m_oDIndicator.Set(&m_pcPos);
		m_oTerminalId.Set(&m_pcPos);

		m_oMessageNum.Set(&m_pcPos);

		if (IsOutputInput())
		{
			m_oMessageTypeId.Set(&m_pcPos);
			m_oResponseCode.Set(&m_pcPos);
			m_oConfirmationReq.Set(&m_pcPos);
			m_oAuthorisationCode.Set(&m_pcPos);
			m_oTempSep.Set(&m_pcPos);
			m_oTransAmount.Set(&m_pcPos);
			m_oTempSep.Set(&m_pcPos);
			m_oMessage.Set(&m_pcPos);
			m_oTempSep.Set(&m_pcPos);

			// Now check for optional fields
			nIndex = (m_pcPos - &m_stDataMsg.cMessage[0]);	
			if (nIndex <  m_stDataMsg.nMessageLen)
			{
				m_oReferralTelNum.Set(&m_pcPos);
				m_oTempSep.Set(&m_pcPos);
				m_oFloorLimit.Set(&m_pcPos);	
				m_oTempSep.Set(&m_pcPos);
				m_oDate.Set(&m_pcPos);
				m_oTempSep.Set(&m_pcPos);
			}
		}
		m_oTempSep.Set(&m_pcPos); // ETX
	}
	else
		nStatus = INVALID_MESSAGE;

	// Set up key for validation
	m_strResponseKey = m_oDIndicator;
	m_strResponseKey += m_oTerminalId;
	m_strResponseKey += m_oMessageNum;

	// Check decoded length is correct
	if (!nStatus)
	{
		nIndex = (m_pcPos - &m_stDataMsg.cMessage[0]);	
		if (nIndex != m_stDataMsg.nMessageLen)
			nStatus = MESSAGE_LENGTH_ERROR;
	}

	m_pcPos = NULL;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  APACS30Message::EncodeMessage
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
STATUS APACS30Message::EncodeMessage()
{
	WORD nIndex;
	STATUS nStatus = 0;

	// Set pointer
	m_pcPos = &m_stDataMsg.cMessage[0];

	// Set values
	if (m_iMessageNum >= 9999)
		m_iMessageNum = 0;
	else
		m_iMessageNum ++; 

	m_oMessageNum = m_iMessageNum;	

	// Encode fields

	m_oSTX.Get(&m_pcPos);
	m_oDIndicator.Get(&m_pcPos);
	m_oTerminalId.Get(&m_pcPos);
	m_oMessageNum.Get(&m_pcPos);

	if (IsOutputInput())
	{
		m_oTerminalType.Get(&m_pcPos);
		m_oMessageTypeId.Get(&m_pcPos);
		m_oMerchantNum.Get(&m_pcPos);
		m_oFS.Get(&m_pcPos);
		m_oCardDetails.Get(&m_pcPos);
		m_oFS.Get(&m_pcPos);
		m_oTransAmount.Get(&m_pcPos);
		m_oFS.Get(&m_pcPos);
	}
	m_oETX.Get(&m_pcPos);

	// Set up key for validation
	m_strRequestKey = m_oDIndicator;
	m_strRequestKey += m_oTerminalId;
	m_strRequestKey += m_oMessageNum;

	if (!nStatus)
		m_stDataMsg.nMessageLen = (m_pcPos - &m_stDataMsg.cMessage[0]);
	else
		m_stDataMsg.nMessageLen = 0;

	m_pcPos = NULL;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  APACS30Message::ValidateMessage
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
STATUS APACS30Message::ValidateMessage()
{
	STATUS nStatus = INVALID_MESSAGE;

	if (IsOutputInput())
	{
		if (m_strRequestKey == m_strResponseKey)
		{
			if (m_oMessageTypeId == "12")
				nStatus = 0;
			else
				nStatus = INVALID_MESSAGE_ID;
		}
		else
			nStatus = RESPONSE_MISMATCH;
	}

	if (IsInputOutput())
	{
		if (m_oMessageTypeId == "09" ||
		    m_oMessageTypeId == "47")
			nStatus = 0;
	}

	return nStatus;
} 

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  APACS30Message::ConvertToInternal
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
void APACS30Message::ConvertToInternal()
{
	if(IsOutputInput())
	{
		m_stInternalMsg.strResponseCode = m_oResponseCode;

		// Check length as Barclays dont send auth code on declines
		if (m_oAuthorisationCode.Length())
			m_stInternalMsg.strAuthCode = m_oAuthorisationCode;
		else
			m_stInternalMsg.strAuthCode = "REJECT";

		m_stInternalMsg.strRetrievalRefNbr = m_stInternalMsg.strTerminalId(3,5);
		m_stInternalMsg.strRetrievalRefNbr += m_oMessageNum;	

		if (m_oResponseCode == "00")
			m_stInternalMsg.strResultCode = "0000";

		m_stInternalMsg.strTextMessage = m_oMessage;
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  APACS30Message::ConvertFromInternal
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
void APACS30Message::ConvertFromInternal()
{
	if (IsOutputInput())
	{
		InitialiseFields();

		m_oTerminalId = m_stInternalMsg.strTerminalId;

		m_oMerchantNum = m_stInternalMsg.strMerchantNumber;

		m_oCardDetails = m_cUS;
		m_oCardDetails += m_stInternalMsg.strCardNumber;
		m_oCardDetails += m_cUS;
		m_oCardDetails += m_stInternalMsg.strExpiryDate;

		m_oTransAmount = m_stInternalMsg.strAmount(1, 11);
		
		if (m_stInternalMsg.strTranType == "0100")    
			m_oMessageTypeId = "09";
	 	else
			if (m_stInternalMsg.strTranType == "0200")
				m_oMessageTypeId = "47";
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  APACS30Message::InitialiseFields
 *
 *           Synopsis:  Initialises Static Field values
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
void APACS30Message::InitialiseFields()
{
	m_oMessageTypeId = "00";
	m_oDIndicator = "4";
	m_oTerminalId = "00000000";
	m_oMessageNum = "0000";
	m_oTerminalType = "4101";
	m_oMerchantNum = "0000000";
	m_oResponseCode = "05";
	m_oAuthorisationCode = "999999";
	m_oMessage = "NO MESSAGE";
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  APACS30Message::MaskDataMsg
 *
 *           Synopsis:  Removes first bit of each char in Data Msg
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
void APACS30Message::MaskDataMsg()
{
	WORD nIndex;
 
	for (nIndex = 0; nIndex < m_stDataMsg.nMessageLen; nIndex++)
		m_stDataMsg.cMessage[nIndex] = (m_stDataMsg.cMessage[nIndex] & 0x7f);
}
