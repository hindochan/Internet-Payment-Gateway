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
#ifndef LGOPERATOR_H
#define LGOPERATOR_H

#include "LGmsg.h"
#include "LGstrcts.h"
#include "LGthread.h"

#define MAX_LAG_THREADS	40

typedef enum
{

	LAG_STOP,
	LAG_STATUS,
	LAG_STATISTICS,
	LAG_CUTOVER

} OPR_CMD;

typedef struct
{
	char			nThreadID;
	char			cThread[12];
	ThreadStatus	enThreadStatus;
} LAGThreadDetails;

typedef struct
{

	WORD				nNbrThreads;
	LAGThreadDetails	ThreadDetails [MAX_LAG_THREADS];

} LAGStatus;

typedef struct operatorcommand
{

	OPR_CMD	nCmd;

} OperatorCommand;

typedef struct operatorresponse
{

	WORD			nCmd;
	STATUS			nStatus;
	union
	{

		LAGStatistics	stStatistics;
		LAGStatus		stStatus;

	} uData;

} OperatorResponse;

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

class OperatorMessage : public Message
{

// Attributes

public:

protected:

private:

	OperatorCommand		m_stCommand;
	OperatorResponse	m_stResponse;


// Methods

public:
	// Constructors
	OperatorMessage(WORD nMsgType);

	// Destructors
	~OperatorMessage();

	STATUS DecodeMessage();
	STATUS EncodeMessage();
	STATUS ValidateMessage();

	void ConvertToInternal();
	void ConvertFromInternal();

protected:

private:

};

#endif // LGOPERATOR_H
