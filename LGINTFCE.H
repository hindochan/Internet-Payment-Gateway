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
#ifndef LGINTFCE_H
#define LGINTFCE_H

#include <rw/cstring.h>

#include "LGtypes.h"
#include "LGmsg.h"
#include "LGport.h"

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

class Interface
{

// Attributes

public:

protected:

	WORD m_nInterfaceNbr;
	BOOL m_bMultiThreaded;
	WORD m_nCommType;
	WORD m_nMsgType;
	WORD m_nNbrPorts;
	RWCString m_strKey;
	RWCString m_strName;
	RWCString m_strCommsConfig;

	Message *m_poMsg;
	Port *m_poPort;

private:

	enum MsgType
	{
		TEST,
		LOGFORMAT,
		OMISO8583,
		APACS30,
		OPERATORFORMAT
	};

	enum CommsType
	{
		FILE,
		TCPIP,
		X25,
		OMTCPIP
	};

// Methods

public:
	// Constructors
	Interface();

	// Destructors
	virtual ~Interface();

	// Reference methods
	WORD & InterfaceNbr();
	WORD & CommType();
	WORD & MsgType();
	WORD & NbrPorts();
	RWCString & Key();
	RWCString & Name();
	RWCString & CommsConfig();

	// Pure virtuals
	virtual STATUS Open() = 0;
	virtual STATUS Close() = 0;

protected:

	Port* GetNewPort();
	Message* GetNewMsg();

private:

};

#endif // LGINTFCE_H
