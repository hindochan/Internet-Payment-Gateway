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
#include <rw/rwdate.h>
#include <rw/ctoken.h>

#include "LGomiso.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OMISO8583Message::OMISO8583Message
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
OMISO8583Message::OMISO8583Message(WORD nMsgType) :
	m_oIdentifier(2),
	m_oDPCNumber(4),
	m_oMsgTypeId(4),
	m_oPrimaryBitMap(8),
	m_oSecondaryBitMap(8),
	m_oProcessingCode(6),
	m_oAmount(12),
	m_oTransmitDateTime(10),
	m_oSystemTraceAudit(6),
	m_oLocalTranTime(6),
	m_oLocalTranDate(4),
	m_oBusinessDate(4),
	m_oRetrievalReferenceNbr(9),
	m_oAuthorisationCode(6),
	m_oResponseCode(2),
	m_oSPDHHeaderFields(3),
	m_oDeviceAddress(2),
	m_oTransmissionNbr(2),
	m_oMerchantNumber(10),
	m_oRealTermId(4),
	m_oSpaces1(2),
	m_oClerkId(6),
	m_oDate(6),
	m_oTime(6),
	m_oRequestType(1),
	m_oRequestSubType(1),
	m_oTransactionCode(2),
	m_oProcessingFlags(3),
	m_oB24ResponseCode(3),
	m_oPOSConditionCode(3),
	m_oField61(3),
	m_oTerminalDisplay(3),
	m_oOptionalData(3),
	m_oSpaces2(4),
	m_oTranLogId (12),
	m_oFiller (64)
{
	// Initialise variables
	WORD nIndex;

	m_pcPos = NULL;
	m_nMsgType = nMsgType;

	m_oAmount = "000000000000";
	m_oRetrievalReferenceNbr = "000000000";
	m_oAuthorisationCode = "999999";
	m_oResponseCode = "05";
	m_oB24ResponseCode = "050";
	m_oField61 = " ";

	// Set up Array
	for (nIndex = 0; nIndex < NBR_ISO_FIELDS; nIndex++)
		m_poFields[nIndex] = NULL;

	m_poFields[1] = &m_oSecondaryBitMap;
	m_poFields[3] = &m_oProcessingCode;
	m_poFields[4] = &m_oAmount;
	m_poFields[7] = &m_oTransmitDateTime;
	m_poFields[11] = &m_oSystemTraceAudit;
	m_poFields[12] = &m_oLocalTranTime;
	m_poFields[13] = &m_oLocalTranDate;
	m_poFields[17] = &m_oBusinessDate;
	m_poFields[35] = &m_oTrackTwo;
	m_poFields[37] = &m_oRetrievalReferenceNbr;
	m_poFields[38] = &m_oAuthorisationCode;
	m_poFields[39] = &m_oResponseCode;
	m_poFields[44] = &m_oCashierLangCode;
	m_poFields[48] = &m_oSPDHHeaderFields;
	m_poFields[60] = &m_oPOSConditionCode;
	m_poFields[61] = &m_oField61;
	m_poFields[122] = &m_oTerminalDisplay;
	m_poFields[127] = &m_oOptionalData;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OMISO8583Message::~OMISO8583Message
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
OMISO8583Message::~OMISO8583Message()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OMISO8583Message::DecodeMessage
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
STATUS OMISO8583Message::DecodeMessage()
{
	WORD nIndex;
	STATUS nStatus = 0;

	if (m_stDataMsg.nMessageLen >= 18)
	{
		// Set pointer and decode header fields
		m_pcPos = &m_stDataMsg.cMessage[0];

		m_oIdentifier.Set(&m_pcPos);
		m_oDPCNumber.Set(&m_pcPos);
		m_oMsgTypeId.Set(&m_pcPos);
		m_oPrimaryBitMap.Set(&m_pcPos);

		// Decode primary fields
		for (nIndex = 0;
			nIndex < (8 * m_oPrimaryBitMap.Length()) && !nStatus;
			nIndex++)
		{
			if (m_oPrimaryBitMap.IsBitSet(nIndex + 1))
			{
				if (m_poFields[nIndex + 1])
					m_poFields[nIndex + 1]->Set(&m_pcPos);
				else
					nStatus = INVALID_FIELD;
			}
		}

		// Decode secondary fields if any present
		if (m_oPrimaryBitMap.IsBitSet(1))
		{
			for (nIndex = 0;
				nIndex < (8 * m_oSecondaryBitMap.Length()) && !nStatus;
				nIndex++)
			{
				if (m_oSecondaryBitMap.IsBitSet(nIndex + 1))
				{
					if (m_poFields[nIndex + (NBR_ISO_FIELDS / 2) + 1])
						m_poFields[nIndex + (NBR_ISO_FIELDS / 2) + 1]->Set(&m_pcPos);
					else
						nStatus = INVALID_FIELD;
				}
			}
		}
	}
	else
		nStatus = INVALID_MESSAGE;

	// Check length decoded is correct
	if (!nStatus)
	{
		nIndex = (m_pcPos - &m_stDataMsg.cMessage[0]);
		if (nIndex != m_stDataMsg.nMessageLen)
			nStatus = MESSAGE_LENGTH_ERROR;
	}

	if (!nStatus)
	{
		// Set up sub fields
		if (m_oPrimaryBitMap.IsBitSet(48))
		{
			// Need to check length of this var length field ??
			m_pcPos = m_oSPDHHeaderFields.Data();

			m_oDeviceAddress.Set(&m_pcPos);
			m_oTransmissionNbr.Set(&m_pcPos);
			m_oMerchantNumber.Set(&m_pcPos);
			m_oRealTermId.Set(&m_pcPos);
			m_oSpaces1.Set(&m_pcPos);
			m_oClerkId.Set(&m_pcPos);
			m_oDate.Set(&m_pcPos);
			m_oTime.Set(&m_pcPos);
			m_oRequestType.Set(&m_pcPos);
			m_oRequestSubType.Set(&m_pcPos);
			m_oTransactionCode.Set(&m_pcPos);
			m_oProcessingFlags.Set(&m_pcPos);
			m_oB24ResponseCode.Set(&m_pcPos);
		}

		if (m_oSecondaryBitMap.IsBitSet(63))
		{
			// Again check var length ??
			m_pcPos = m_oOptionalData.Data();
			m_oSpaces2.Set(&m_pcPos);
			m_oTranLogId.Set(&m_pcPos);
		}
	}

	m_pcPos = NULL;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OMISO8583Message::EncodeMessage
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
STATUS OMISO8583Message::EncodeMessage()
{
	WORD nIndex;
	STATUS nStatus = 0;

	if (IsInputOutput())
	{
		if(m_oMsgTypeId == "0100")
			m_oMsgTypeId = "0110";
		else
		{
			if(m_oMsgTypeId == "0200")
				m_oMsgTypeId = "0210";
			else
			{
				if(m_oMsgTypeId == "0220")
					m_oMsgTypeId = "0230";
			}
		}

		m_oProcessingCode = "203000";

		m_oPrimaryBitMap.ClearBit(35);
		m_oPrimaryBitMap.ClearBit(44);
		m_oPrimaryBitMap.ClearBit(60);

		m_oPrimaryBitMap.SetBit(17);
		m_oPrimaryBitMap.SetBit(37);
		m_oPrimaryBitMap.SetBit(38);
		m_oPrimaryBitMap.SetBit(39);

		if (m_oMsgTypeId == "0110" || m_oMsgTypeId == "0210")
		{
			if (m_oPrimaryBitMap.IsBitSet(1))
				m_oSecondaryBitMap.SetBit(58);
			if (m_oMsgTypeId == "0110")
				m_oPrimaryBitMap.SetBit(61);
		}
	}

	m_oSPDHHeaderFields = m_oDeviceAddress;
	m_oSPDHHeaderFields += m_oTransmissionNbr;
	m_oSPDHHeaderFields += m_oMerchantNumber;
	m_oSPDHHeaderFields += m_oRealTermId;
	m_oSPDHHeaderFields += m_oSpaces1;
	m_oSPDHHeaderFields += m_oClerkId;
	m_oSPDHHeaderFields += m_oDate;
	m_oSPDHHeaderFields += m_oTime;
	m_oSPDHHeaderFields += m_oRequestType;
	m_oSPDHHeaderFields += m_oRequestSubType;
	m_oSPDHHeaderFields += m_oTransactionCode;
	m_oSPDHHeaderFields += m_oProcessingFlags;
	m_oSPDHHeaderFields += m_oB24ResponseCode;

	m_oOptionalData = m_oSpaces2;
	m_oOptionalData += m_oTranLogId;

	// Set pointer and encode fields
	m_pcPos = &m_stDataMsg.cMessage[0];

	m_oIdentifier.Get(&m_pcPos);
	m_oDPCNumber.Get(&m_pcPos);

	m_oMsgTypeId.Get(&m_pcPos);
	m_oPrimaryBitMap.Get(&m_pcPos);

	for (nIndex = 0;
		nIndex < (8 * m_oPrimaryBitMap.Length()) && !nStatus;
		nIndex++)
	{
		if (m_oPrimaryBitMap.IsBitSet(nIndex + 1))
		{
			if (m_poFields[nIndex + 1])
				m_poFields[nIndex + 1]->Get(&m_pcPos);
			else
				nStatus = INVALID_FIELD;
		}
	}

	for (nIndex = 0;
		nIndex < (8 * m_oSecondaryBitMap.Length()) && !nStatus;
		nIndex++)
	{
		if (m_oSecondaryBitMap.IsBitSet(nIndex + 1))
		{
			if (m_poFields[nIndex + (NBR_ISO_FIELDS / 2) + 1])
				m_poFields[nIndex + (NBR_ISO_FIELDS / 2) + 1]->Get(&m_pcPos);
			else
				nStatus = INVALID_FIELD;
		}
	}

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
 *               Name:  OMISO8583Message::ValidateMessage
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
STATUS OMISO8583Message::ValidateMessage()
{
	STATUS nStatus = INVALID_MESSAGE;

	if (IsInputOutput())
	{
		if (m_oMsgTypeId == "0100" ||
			m_oMsgTypeId == "0200" ||
			m_oMsgTypeId == "0220")
			nStatus = 0;
	}

	if (IsOutputInput())
	{
		if (m_oMsgTypeId == "0110" ||
			m_oMsgTypeId == "0210" ||
			m_oMsgTypeId == "0230")
			nStatus = 0;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OMISO8583Message::ConvertToInternal
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
void OMISO8583Message::ConvertToInternal()
{
	int nIndex = 0;
	char cTemp[2];
	char cTemp2[4];

	RWDate oDate; // current system date
	UINT iYear = oDate.year();
	UINT iMonth = oDate.month();
	UINT iMonth2 = 0;

	if (IsInputOutput())
	{
		memcpy(cTemp, m_oLocalTranDate, sizeof(cTemp));
		iMonth2 = atoi(cTemp);

		// If month in tran date is greater than current
		// then year has changed so we want to log previous year
		if (iMonth < iMonth2)
			iYear--;

		sprintf(cTemp2, "%d", iYear);
		m_stInternalMsg.strDate = cTemp2;
		m_stInternalMsg.strDate += m_oLocalTranDate;

		m_stInternalMsg.strTime = m_oLocalTranTime;

		m_stInternalMsg.strTranType = m_oMsgTypeId;

		m_stInternalMsg.strMerchantNumber = m_oMerchantNumber;
		// Now shorten to first space if it has one
		nIndex = m_stInternalMsg.strMerchantNumber.index(":");
		if (nIndex != RW_NPOS)
			m_stInternalMsg.strMerchantNumber =
			m_stInternalMsg.strMerchantNumber(0, nIndex);

		// Set up dummy values in case of comms failure
		m_stInternalMsg.strRetrievalRefNbr = "111";
		m_stInternalMsg.strRetrievalRefNbr += m_stInternalMsg.strTime;

		m_stInternalMsg.strTextMessage = "INTERNAL";

		if (m_oMsgTypeId == "0220")
		{
			m_stInternalMsg.strResultCode = "0000";
			m_stInternalMsg.strAuthCode = m_oAuthorisationCode;
			m_stInternalMsg.strResponseCode = "00";
			m_stInternalMsg.strTextMessage += " OK";
		}
		else
		{
			// Set up result code as UNAUTHORISED
			m_stInternalMsg.strResultCode = "9999";
			m_stInternalMsg.strAuthCode = "999999";
			m_stInternalMsg.strResponseCode = "05";
		}

		RWCString strTrackII(m_oTrackTwo);
		RWCTokenizer tknDelim(strTrackII);

		m_stInternalMsg.strCardNumber = tknDelim("=");
		m_stInternalMsg.strExpiryDate = tknDelim("=");

		m_stInternalMsg.strAmount = m_oAmount;

		m_stInternalMsg.strTranLogId = m_oTranLogId;

		// Take this out later ??
		m_stInternalMsg.strBusinessDate = m_oLocalTranDate;

		m_stInternalMsg.strTerminalId = "0103";
		m_stInternalMsg.strTerminalId += m_oRealTermId;

		m_stInternalMsg.strSystemTraceAudit = m_oSystemTraceAudit;
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OMISO8583Message::ConvertFromInternal
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
void OMISO8583Message::ConvertFromInternal()
{
	if (IsInputOutput())
	{
		m_oBusinessDate = m_stInternalMsg.strBusinessDate;
		m_oRetrievalReferenceNbr = m_stInternalMsg.strRetrievalRefNbr;
		m_oAuthorisationCode = m_stInternalMsg.strAuthCode;
		m_oResponseCode = m_stInternalMsg.strResponseCode;

		if (m_stInternalMsg.strResultCode != "0000" && m_oResponseCode == "00")
			m_oResponseCode = "05";

		if (m_oResponseCode == "00")
			m_oB24ResponseCode = "000";
		else
			m_oB24ResponseCode = "050";

		m_oTerminalDisplay = m_stInternalMsg.strTextMessage;
	}
}
