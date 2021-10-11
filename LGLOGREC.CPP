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

#include "LGlogrec.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LogRecord::LogRecord
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
LogRecord::LogRecord(WORD nMsgType) :
	m_oDelim(1)
{
	// Initialise variables
	m_nMsgType = nMsgType;
	m_oDelim = "|";
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LogRecord::~LogRecord
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
LogRecord::~LogRecord()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LogRecord::DecodeMessage
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
STATUS LogRecord::DecodeMessage()
{
	STATUS nStatus = 0;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LogRecord::EncodeMessage
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
STATUS LogRecord::EncodeMessage()
{
	STATUS nStatus = 0;

	m_pcPos = &m_stDataMsg.cMessage[0];

	memcpy(m_pcPos, m_strLogRecord.data(), m_strLogRecord.length());

	m_stDataMsg.nMessageLen = m_strLogRecord.length();

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LogRecord::ValidateMessage
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
STATUS LogRecord::ValidateMessage()
{
	STATUS nStatus = 0;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LogRecord::ConvertToInternal
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
void LogRecord::ConvertToInternal()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LogRecord::ConvertFromInternal
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
void LogRecord::ConvertFromInternal()
{
	if (IsOutputOnly())
	{
		m_strLogRecord = m_stInternalMsg.strDate;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strTime;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strTranType;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strMerchantInterfaceName;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strMerchantNumber;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strResultCode;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strAuthCode;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strCardNumber;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strExpiryDate;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strAmount;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strTranLogId;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += JulianDate();
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strRetrievalRefNbr;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strResponseCode;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strTerminalId;
		m_strLogRecord += m_oDelim;
		m_strLogRecord += m_stInternalMsg.strTextMessage;
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LogRecord::JulianDate
 *
 *           Synopsis:  Returns the date as a Julian date
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	String
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
RWCString LogRecord::JulianDate()
{
	char cTemp[8];
	char cYear[5];
	char cMonth[3];
	char cDay[3];
	char cJulDate[5];

	memset(cTemp, '\0', sizeof(cTemp));
	memset(cYear, '\0', sizeof(cYear));
	memset(cMonth, '\0', sizeof(cMonth));
	memset(cDay, '\0', sizeof(cDay));
	memset(cJulDate, '\0', sizeof(cJulDate));

	memcpy(cTemp, m_stInternalMsg.strDate.data(), sizeof(cTemp));
	memcpy(cYear, cTemp, 4);
	memcpy(cMonth, cTemp + 4, 2);
	memcpy(cDay, cTemp + 6, 2);

	RWDate oDate(atoi(cDay), atoi(cMonth), atoi(cYear));

	memcpy(cJulDate, cYear + 2, 2);
	sprintf(cJulDate + 2, "%3.3d", oDate.day());

	RWCString oJulDate(cJulDate);

	return oJulDate;
}
