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
#ifndef LGMCHTHR_H
#define LGMCHTHR_H

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

class MerchantThread : public PThread
{

// Attributes

public:

protected:

private:

	CONDVAR *m_pcndReqServerCondVar;
	MUTEX *m_pmtxReqServerMutex;

	LAGTransaction *m_poMyTran;

// Methods

public:
	// Constructors
	MerchantThread(RWCString strInterfaceName);

	// Destructors
	~MerchantThread();

	// Reference methods
	LAGTransaction* & MyTran();
	CONDVAR* & ReqServerCondVar();
	MUTEX* & ReqServerMutex();

	STATUS Create();

	void GetTransaction();
	void PutTransaction(STATUS nStatus);

	STATUS SignalRequestor();

protected:

private:

};

// Merchant Thread entry point

void * MerchantThreadMain(void *oMrchThrd);

#endif // LGMCHTHR_H
