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
#ifndef LGOPRTHR_H
#define LGOPRTHR_H

#include "LGthread.h"
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

class OperatorThread : public PThread
{

// Attributes

public:

protected:

private:

	LoggerThread *m_poLggrThrd;

	LAGTransaction m_oMyTran;

// Methods

public:
	// Constructors
	OperatorThread(RWCString strInterfaceName);

	// Destructors
	~OperatorThread();

	// Reference methods
	LoggerThread* & LggrThrd();

	STATUS Create();

	void GetTransaction();
	void PutTransaction();

	STATUS ValidateTransaction();

	STATUS LogTransaction(STATUS nStatus);

protected:

private:

};

// Operator Thread entry point
 
void * OperatorThreadMain(void *oOprThrd);

#endif // LGOPRTHR_H
