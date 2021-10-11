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
 *           Synopsis:  
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */

#include "LGlgrthr.h"
#include "LGglobls.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerThread::LoggerThread
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  Interface name
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
LoggerThread::LoggerThread(RWCString strInterfaceName)
{
	// Initialise variables
	m_strInterfaceName = strInterfaceName;
	m_bMainFlag = FALSE;
	m_pcndReqServerCondVar = NULL;
	m_pmtxReqServerMutex = NULL;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerThread::~LoggerThread
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
LoggerThread::~LoggerThread()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerThread::ReferenceMethods
 *
 *           Synopsis:  Reference Member to get and set variable
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	Reference to variable
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
LAGTransaction* & LoggerThread::MyTran()
{
	return m_poMyTran;
}

CONDVAR* & LoggerThread::ReqServerCondVar()
{
	return m_pcndReqServerCondVar;
}

MUTEX* & LoggerThread::ReqServerMutex()
{
	return m_pmtxReqServerMutex;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerThread::Create
 *
 *           Synopsis:  Overload from Parent
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS 0 if OK
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LoggerThread::Create()
{
	// If first logger interface i.e. name = TLF then this is the main one
	if (m_strInterfaceName == "TLF")
		m_bMainFlag = TRUE;

	return PThread::Create(LoggerThreadMain, this);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerThread::IsMain
 *
 *           Synopsis:  Checks if main logger thread
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS 0 if OK
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
BOOL LoggerThread::IsMain()
{
	if (m_bMainFlag)
		return TRUE;
	else
		return FALSE;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerThread::GetTransaction
 *
 *           Synopsis:  Copies internal message from transaction object
 *						to my message
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
void LoggerThread::GetTransaction()
{
	m_poMsg->InternalMsg() = m_poMyTran->InternalMsg();
	m_poMyTran->CheckTranStatus();
	oTheApplication.GetStatistics()->UpdateLggrStats(m_poMyTran->IsTranOK());
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerThread::SignalRequestor
 *
 *           Synopsis:  Signals the requesting server thread
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS 0 if OK
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LoggerThread::SignalRequestor()
{
	// Do this first to ensure Server is waiting for signal
	Lock(m_pmtxReqServerMutex);
	Unlock(m_pmtxReqServerMutex);

	return Signal(m_pcndReqServerCondVar);
}
