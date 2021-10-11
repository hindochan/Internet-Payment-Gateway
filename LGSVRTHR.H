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
#ifndef LGSVRTHR_H
#define LGSVRTHR_H

#include "LGthread.h"
#include "LGmchthr.h"
#include "LGlgrthr.h"
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

class ServerThread : public PThread
{

// Attributes

public:

protected:

private:

	MerchantThread *m_poCurrentMrchThrd;
	LoggerThread *m_poLggrThrd;

	LAGTransaction m_oMyTran;

// Methods

public:
	// Constructors
	ServerThread(RWCString strInterfaceName);

	// Destructors
	~ServerThread();

	// Reference methods
	MerchantThread* & CurrentMrchThrd();
	LoggerThread* & LggrThrd();

	// Get methods
	RWCString CurrentMrchName();

	STATUS Create();

	void GetTransaction();
	void PutTransaction();

	STATUS ValidateTransaction();

	STATUS CallMerchant();

	STATUS LogTransaction(STATUS nStatus);

protected:

private:

};

// Server Thread entry point
 
void * ServerThreadMain(void *oSrvrThrd);

#endif // LGSVRTHR_H
