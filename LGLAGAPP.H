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
 *           Synopsis:  class definition for an acquirer gateway
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */
#ifndef LGLAGAPP_H
#define LGLAGAPP_H

#include "LGtypes.h"
#include "LGconfig.h"
#include "LGuxapp.h"
#include "LGstats.h"

/************************************************************************
 *
 *   CLASS DECLARATION
 *
 *               Name:  LAGApplication
 *
 *           Synopsis:  Main LAG Object.
 *
 *              Notes:  One per process only.
 *
 ************************************************************************
 */

class LAGApplication : public CUnixApp
{

// Attributes

public:

protected:

private:

	RWTPtrDlist<MerchantThread> m_llMrchThrds;
	MUTEX m_mtxMrchLL;

	RWTPtrDlist<ServerThread> m_llSrvrThrds;
	MUTEX m_mtxSrvrLL;
	
	RWTPtrDlist<LoggerThread> m_llLggrThrds;
	MUTEX m_mtxLggrLL;
	
	RWTPtrDlist<OperatorThread> m_llOprThrds;
	MUTEX m_mtxOprLL;
	
	// Get methods
	LGConfiguration *m_poConfiguration;
	LGStatistics *m_poStatistics;

// Methods

public:
	// Constructors
	LAGApplication();

	// Destructor
	~LAGApplication();

	MerchantThread* GetFreeMrchThrd(RWCString strMrchIntName);
	LoggerThread* GetLggrThrd();

	STATUS LoadConfigFile();

	LGConfiguration GetConfiguration();
	LGStatistics* GetStatistics();

	BOOL IsTracingOn();

	STATUS OpenLogger(WORD nIndex);
	STATUS OpenLoggers();
	STATUS OpenMerchant(WORD nIndex);
	STATUS OpenMerchants();
	STATUS OpenServer(WORD nIndex);
	STATUS OpenServers();

	STATUS OpenOperator();

	STATUS CloseLogger(WORD nIndex);
	STATUS CloseLoggers();
	STATUS CloseMerchant(WORD nIndex);
	STATUS CloseMerchants();
	STATUS CloseServer(WORD nIndex);
	STATUS CloseServers();

	STATUS CloseOperator();

	void LockMrchLL();
	void UnlockMrchLL();
	void LockSrvrLL();
	void UnlockSrvrLL();
	void LockLggrLL();
	void UnlockLggrLL();

	void LoadThreadInfo(LAGStatus *pstThreadInfo);
	STATUS CutoverLogFile();

private:

};

// Linked list stand alone tester functions

RWBoolean FreeMerchant(MerchantThread* poMrchThrd, void* strMrchIntName);
RWBoolean MainLogger(LoggerThread* poLggrThrd, void* d);

#endif // LGLAGAPP_H
