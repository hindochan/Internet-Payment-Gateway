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
 *           Synopsis:  General stuff
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */

#ifndef LGSTRCTS_H
#define LGSTRCTS_H

/////////////////////////////////////////////////////////////////////////////
// Internal message format

typedef struct internalmessage
{
	RWCString strDate;						// YYYYMMDD
	RWCString strTime;						// HHMMSS
	RWCString strTranType;					// 0100 etc..
	RWCString strMerchantInterfaceName;		// e.g. BMS
	RWCString strMerchantNumber;			// 000 + 7digit merch id
	RWCString strResultCode;
	RWCString strAuthCode;
	RWCString strCardNumber;				// 0 filled from left
	RWCString strExpiryDate;				// YYMM
	RWCString strAmount;					// e.g $11.99 = 000000001199
	RWCString strTranLogId;					// e.g 000000000014
	RWCString strBusinessDate;				// MMDD - file logged to
	RWCString strRetrievalRefNbr;			// Terminal Id + msg no
	RWCString strResponseCode;
	RWCString strTerminalId;
	RWCString strTextMessage;				// Arbitary text
	RWCString strSystemTraceAudit;			// Trace audit number
	// Any additions here should be include in the
	// Message::DumpInternal function in LGmsg.h
} IntMsg;


/////////////////////////////////////////////////////////////////////////////
// Data message type

#define MAX_MSG_LEN		1024
#define MAX_MSG_HDR_LEN	10
#define MAX_MSG_TRL_LEN	10

typedef struct datamessage
{
	WORD	nMessageLen;
	char	cMessage[MAX_MSG_LEN];
} DataMsg;

typedef struct commsmessage
{
	WORD	nMessageLen;
	char	cMessage[	MAX_MSG_LEN
						+MAX_MSG_HDR_LEN
						+MAX_MSG_TRL_LEN];
} CommsMsg;

typedef struct statistics
{
	int		nDateStarted;
	int		nNoTransOK;
	int		nNoTransNOK;
	int		nAvgServResponse;
	int		nAvgMrchResponse;
} LAGStatistics;

#endif // LGSTRCTS_H
