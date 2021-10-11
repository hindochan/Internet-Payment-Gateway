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
#ifndef LGLGRTHR_H
#define LGLGRTHR_H

#include "LGthread.h"
#include "LGlagtrn.h"

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

class LoggerThread : public PThread
{

// Attributes

public:

protected:

private:

	BOOL m_bMainFlag;
	CONDVAR *m_pcndReqServerCondVar;
	MUTEX *m_pmtxReqServerMutex;

	LAGTransaction *m_poMyTran;

// Methods

public:
	// Constructors
	LoggerThread(RWCString strInterfaceName);

	// Destructors
	~LoggerThread();

	// Reference methods
	LAGTransaction* & MyTran();
	CONDVAR* & ReqServerCondVar();
	MUTEX* & ReqServerMutex();

	STATUS Create();

	BOOL IsMain();

	void GetTransaction();

	STATUS SignalRequestor();

protected:

private:

};

// Logger Thread entry point
 
void * LoggerThreadMain(void *oLggrThrd);

#endif // LGLGRTHR_H
