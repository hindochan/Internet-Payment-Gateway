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
#ifndef LGAPACS_H
#define LGAPACS_H

#include "LGmsg.h"
#include "LGfield.h"
#include "LGvrfld.h"

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

#define STX 0x02
#define ETX 0x03
#define US 0x1f
#define FS 0x1c

class APACS30Message : public Message
{

// Attributes

public:

protected:

private:

	char *m_pcPos;

	RWCString m_strRequestKey;
	RWCString m_strResponseKey;
	int m_iMessageNum;

	char m_cSTX[2];
	char m_cETX[2];
	char m_cUS[2];
	char m_cFS[2];
	Field m_oSTX;
	Field m_oETX;
	Field m_oFS;
	Field m_oTempSep;

	Field m_oMessageTypeId;
	
	Field m_oDIndicator;
	
	Field m_oTerminalId;	

	Field m_oMessageNum;

	Field m_oTerminalType;

	VariableField m_oMerchantNum;

	Field m_oResponseCode;

	Field m_oConfirmationReq;

	VariableField m_oAuthorisationCode;

	VariableField m_oMessage;

	VariableField m_oReferralTelNum;

	VariableField m_oFloorLimit;

	Field m_oDate;

	VariableField m_oCardDetails;

	VariableField m_oTransAmount;

// Methods

public:
	// Constructors
	APACS30Message(WORD nMsgType);

	// Destructors
	~APACS30Message();

	STATUS DecodeMessage();
	STATUS EncodeMessage();
	STATUS ValidateMessage();

	void ConvertToInternal();
	void ConvertFromInternal();

protected:

private:

	void InitialiseFields();
	void MaskDataMsg();

};

#endif // LGAPACS_H
