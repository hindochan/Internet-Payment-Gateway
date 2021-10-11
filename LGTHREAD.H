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
 *           Synopsis:  class definition for base thread class
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */
#ifndef LGTHREAD_H
#define LGTHREAD_H

#include "LGtypes.h"
#include "LGport.h"
#include "LGmsg.h"

enum ThreadStatus
{
	UNINITIALISED,
	OKIDLE,
	BUSYLINE,
	BUSYTRAN,
	LINEDOWN,
	DEAD
};

/************************************************************************
 *
 *   CLASS DECLARATION
 *
 *               Name:  PThread
 *
 *           Synopsis:  .
 *
 *              Notes:  .
 *
 ************************************************************************
 */

class PThread
{

// Attributes

public:

protected:

	RWCString m_strInterfaceName;

	THREAD m_nThreadId;
	THREAD_ATTR m_attrThr;
	CONDVAR m_cndCondVar;
	CONDVAR_ATTR m_attrCnd;
	MUTEX m_mtxCondMutex;
	MUTEX_ATTR m_attrCndMtx;

	Port *m_poPort;
	Message *m_poMsg;

private:

	ThreadStatus m_enStatus;

// Methods

public:
	// Constructors
	PThread();

	// Destructors
	virtual ~PThread();

	// Reference methods
	Port* & Port();
	Message* & Message();

	// Get methods
	WORD GetThreadId();
	RWCString InterfaceName();
	ThreadStatus Status();

	int Create(void * (*start_func)(void *), void *arg);
	int Join();

	int Lock();
	int Lock(MUTEX *pmtx);
	int Unlock();
	int Unlock(MUTEX *pmtx);
	int Signal();
	int Signal(CONDVAR *pcndCondVar);
	int Wait();

	int Cancel();
	void Exit();

	void SetStatusOKIdle();
	void SetStatusBusyTran();
	void SetStatusLineDown();
	void SetStatusDead();

	BOOL OKIdle();
	BOOL BusyTran();
	BOOL LineDown();
	BOOL Dead();

	int SetCancelTypeAsync();

protected:

private:

};

#endif // LGTHREAD_H
