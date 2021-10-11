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
 *           Synopsis:  class definition for 
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */
#ifndef LGOMISO_H
#define LGOMISO_H

#define NBR_ISO_FIELDS 128

#include "LGmsg.h"
#include "LGfield.h"
#include "LGbitfld.h"
#include "LGvrlfld.h"

/************************************************************************
 *
 *   CLASS DECLARATION
 *
 *               Name:  
 *
 *           Synopsis:  .
 *
 *              Notes:  .
 *
 ************************************************************************
 */

class OMISO8583Message : public Message
{

// Attributes

public:

protected:

private:

	char *m_pcPos;

	Field *m_poFields[NBR_ISO_FIELDS];

	// Header Fields
	Field m_oIdentifier;
	Field m_oDPCNumber;
	Field m_oMsgTypeId;

	BitMapField m_oPrimaryBitMap;

	// Field 001
	BitMapField m_oSecondaryBitMap;

	// Field 003
	Field m_oProcessingCode;

	// Field 004
	Field m_oAmount;

	// Field 007
	Field m_oTransmitDateTime;

	// Field 011
	Field m_oSystemTraceAudit;

	// Field 012
	Field m_oLocalTranTime;

	// Field 013
	Field m_oLocalTranDate;

	// Field 017
	Field m_oBusinessDate;

	// Field 035
	VariableLenField m_oTrackTwo;

	// Field 037
	Field m_oRetrievalReferenceNbr;

	// Field 038
	Field m_oAuthorisationCode;

	// Field 039
	Field m_oResponseCode;

	// Field 044
	VariableLenField m_oCashierLangCode;

	// Field 048
	VariableLenField m_oSPDHHeaderFields;

	Field m_oDeviceAddress;
	Field m_oTransmissionNbr;

	//These are a breakdown of the Terminal Id field
	Field m_oMerchantNumber;
	Field m_oRealTermId;
	Field m_oSpaces1;

	Field m_oClerkId;
	Field m_oDate;
	Field m_oTime;
	Field m_oRequestType;
	Field m_oRequestSubType;
	Field m_oTransactionCode;
	Field m_oProcessingFlags;
	Field m_oB24ResponseCode;

	// Field 060
	VariableLenField m_oPOSConditionCode;

	// Field 061
	VariableLenField m_oField61;

	// Field 122
	VariableLenField m_oTerminalDisplay;

	// Field 127
	VariableLenField m_oOptionalData;

	Field m_oSpaces2;
	Field m_oTranLogId;
	Field m_oFiller;

// Methods

public:
	// Constructors
	OMISO8583Message(WORD nMsgType);

	// Destructors
	~OMISO8583Message();

	STATUS DecodeMessage();
	STATUS EncodeMessage();
	STATUS ValidateMessage();
 
	void ConvertToInternal();
	void ConvertFromInternal();
 
protected:

private:

};

#endif // LGOMISO_H
