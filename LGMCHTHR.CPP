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

#include "LGmchthr.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantThread::MerchantThread
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
MerchantThread::MerchantThread(RWCString strInterfaceName)
{
	// Initialise variables
	m_strInterfaceName = strInterfaceName;
	m_pcndReqServerCondVar = NULL;
	m_pmtxReqServerMutex = NULL;
}
/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantThread::~MerchantThread
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
MerchantThread::~MerchantThread()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantThread::ReferenceMethods
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
LAGTransaction* & MerchantThread::MyTran()
{
	return m_poMyTran;
}

CONDVAR* & MerchantThread::ReqServerCondVar()
{
	return m_pcndReqServerCondVar;
}

MUTEX* & MerchantThread::ReqServerMutex()
{
	return m_pmtxReqServerMutex;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantThread::Create
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
STATUS MerchantThread::Create()
{
	return PThread::Create(MerchantThreadMain, this);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantThread::GetTransaction
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
void MerchantThread::GetTransaction()
{
	m_poMsg->InternalMsg() = m_poMyTran->InternalMsg();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantThread::PutTransaction
 *
 *           Synopsis:  Copies internal message from my message
 *						to transaction object
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
void MerchantThread::PutTransaction(STATUS nStatus)
{
	m_poMyTran->InternalMsg() = m_poMsg->InternalMsg();
	m_poMyTran->SetTranStatus(nStatus);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantThread::SignalRequestor
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
STATUS MerchantThread::SignalRequestor()
{
	// Do this first to ensure Server is waiting for signal
	Lock(m_pmtxReqServerMutex);
	Unlock(m_pmtxReqServerMutex);

	return Signal(m_pcndReqServerCondVar);
}
