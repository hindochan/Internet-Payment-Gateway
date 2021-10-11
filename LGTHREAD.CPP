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
 *           Synopsis:  POSIX thread class
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */

#include "LGthread.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::PThread
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
PThread::PThread()
{
	// Initialise variables
	m_enStatus = UNINITIALISED;

	::pthread_attr_init(&m_attrThr);
	::pthread_condattr_init(&m_attrCnd);
	::pthread_cond_init(&m_cndCondVar, &m_attrCnd);
	::pthread_mutexattr_init(&m_attrCndMtx);
	::pthread_mutex_init(&m_mtxCondMutex, &m_attrCndMtx);

	m_poPort = NULL;
	m_poMsg = NULL;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::~PThread
 *
 *           Synopsis:  Destructor for class object
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
PThread::~PThread()
{
	if (m_poPort)
	{
		delete m_poPort;
		m_poPort = NULL;
	}
	if (m_poMsg)
	{
		delete m_poMsg;
		m_poMsg = NULL;
	}
	::pthread_attr_destroy(&m_attrThr);
	::pthread_condattr_destroy(&m_attrCnd);
	::pthread_cond_destroy(&m_cndCondVar);
	::pthread_mutexattr_destroy(&m_attrCndMtx);
	::pthread_mutex_destroy(&m_mtxCondMutex);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::ReferenceMethods
 *
 *           Synopsis:  Reference Member to get and set variable
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  Reference to variable
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
Port* & PThread::Port()
{
	return m_poPort;
}

Message* & PThread::Message()
{
	return m_poMsg;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::GetMethods
 *
 *           Synopsis:  Returns value of hidden data type
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  value of data item
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
WORD PThread::GetThreadId()
{
	return m_nThreadId;
}

RWCString PThread::InterfaceName()
{
	return m_strInterfaceName;
}

ThreadStatus PThread::Status()
{
	return m_enStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::Create
 *
 *           Synopsis:  Create Method start thread running
 *
 *   Input parameters:  Start routine and argument
 *
 *  Output parameters:  As for pthread_create
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
int PThread::Create(void * (*start_func)(void *), void *arg)
{
	return ::pthread_create(&m_nThreadId, &m_attrThr, start_func, arg);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::Join
 *
 *           Synopsis:  Create Method start thread running
 *
 *   Input parameters:  As for pthread_join
 *
 *  Output parameters:  As for pthread_join
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
int PThread::Join()
{
	int iStatus = 0;

	return ::pthread_join(m_nThreadId, (void**)iStatus);
	//return ::pthread_join(m_nThreadId, (void**)m_enStatus);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::Lock
 *
 *           Synopsis:  Locks my mutex
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  int from function call
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
int PThread::Lock()
{
	return ::pthread_mutex_lock(&m_mtxCondMutex);
}

int PThread::Lock(MUTEX *pmtx)
{
	return ::pthread_mutex_lock(pmtx);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::Unlock
 *
 *           Synopsis:  Unlocks my mutex
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  int from function call
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
int PThread::Unlock()
{
	return ::pthread_mutex_unlock(&m_mtxCondMutex);
}

int PThread::Unlock(MUTEX *pmtx)
{
	return ::pthread_mutex_unlock(pmtx);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::Signal
 *
 *           Synopsis:  Signals this thread - will be called externally to
 *						wake me up
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  int from function call
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
int PThread::Signal()
{
	return ::pthread_cond_signal(&m_cndCondVar);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::Signal
 *
 *           Synopsis:  Signals a conditional variable
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  int from function call
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
int PThread::Signal(CONDVAR *pcndCondVar)
{
	return ::pthread_cond_signal(pcndCondVar);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::Wait
 *
 *           Synopsis:  Wait on my cond variable for signal
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  int from function call
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
int PThread::Wait()
{
	return ::pthread_cond_wait(&m_cndCondVar, &m_mtxCondMutex);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::Cancel
 *
 *           Synopsis:  Cancels this thread
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  int from function call
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
int PThread::Cancel()
{
	return ::pthread_cancel(m_nThreadId);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::Exit
 *
 *           Synopsis:  Terminates thread
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  None
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void PThread::Exit()
{
	::pthread_exit(&m_enStatus);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::SetStatusValue
 *
 *           Synopsis:  Sets port status
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  None
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void PThread::SetStatusOKIdle()
{
	m_enStatus = OKIDLE;
}

void PThread::SetStatusBusyTran()
{
	m_enStatus = BUSYTRAN;
}

void PThread::SetStatusLineDown()
{
	m_enStatus = LINEDOWN;
}

void PThread::SetStatusDead()
{
	m_enStatus = DEAD;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::CheckState
 *
 *           Synopsis:  check line in this state
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  BOOL
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
BOOL PThread::OKIdle()
{
	return (m_enStatus == OKIDLE);
}

BOOL PThread::BusyTran()
{
	return (m_enStatus == BUSYTRAN);
}

BOOL PThread::LineDown()
{
	return (m_enStatus == LINEDOWN);
}

BOOL PThread::Dead()
{
	return (m_enStatus == DEAD);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  PThread::SetCancelTypeAsync
 *
 *           Synopsis:  Sets cancel type to ASYNC
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  int
 *
 *        Description:	Will allow thread to be cancelled immediately
 *
 *              Notes:
 *
 ****************************************************************************
 */
int PThread::SetCancelTypeAsync()
{
	int iOldType = 0;
	int iNewType = PTHREAD_CANCEL_ASYNCHRONOUS;
	return ::pthread_setcanceltype(iNewType, &iOldType);
}
