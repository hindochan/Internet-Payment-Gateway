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

#include "LGsvrthr.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerThread::ServerThread
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
ServerThread::ServerThread(RWCString strInterfaceName)
{
	// Initialise variables
	m_strInterfaceName = strInterfaceName;
	m_poCurrentMrchThrd = NULL;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerThread::~ServerThread
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
ServerThread::~ServerThread()
{

}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerThread::ReferenceMethods
 *
 *           Synopsis:  Get Member to get and set ariable
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
MerchantThread* & ServerThread::CurrentMrchThrd()
{
	return m_poCurrentMrchThrd;
}

LoggerThread* & ServerThread::LggrThrd()
{
	return m_poLggrThrd;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerThread::GetMethods
 *
 *           Synopsis:  Get Member to get variable
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  Variable value
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
RWCString ServerThread::CurrentMrchName()
{
	return m_oMyTran.MerchantInterfaceName();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerThread::Create
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
STATUS ServerThread::Create()
{
	return PThread::Create(ServerThreadMain, this);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerThread::GetTransaction
 *
 *           Synopsis:  Gets back internal message from transaction
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
void ServerThread::GetTransaction()
{
	m_poMsg->InternalMsg() = m_oMyTran.InternalMsg();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerThread::PutTransaction
 *
 *           Synopsis:  Sets up transaction object
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
void ServerThread::PutTransaction()
{
	m_oMyTran.ResetTranStatus();
	m_oMyTran.InternalMsg() = m_poMsg->InternalMsg();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerThread::ValidateTransaction
 *
 *           Synopsis:  Validates fields
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
STATUS ServerThread::ValidateTransaction()
{
	return m_oMyTran.ValidateRequest();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerThread::CallMerchant
 *
 *           Synopsis:  Gives transaction to merchant thread and waits
 *						for completion signal
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
STATUS ServerThread::CallMerchant()
{
	STATUS nStatus = 0;

	// Give merchant thread the transaction and signal
	// A lock/unlock on this thread's mutex ensures it is ready to receive
	// a signal for sure
	m_poCurrentMrchThrd->Lock();

	m_poCurrentMrchThrd->MyTran() = &m_oMyTran;
	m_poCurrentMrchThrd->ReqServerCondVar() = &m_cndCondVar;
	m_poCurrentMrchThrd->ReqServerMutex() = &m_mtxCondMutex;

	m_poCurrentMrchThrd->Unlock();

	// Now lock myself and wait for signal back from merchant

	Lock();

	nStatus = m_poCurrentMrchThrd->Signal();

	if (!nStatus)
		nStatus = Wait();

	Unlock();

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerThread::LogTransaction
 *
 *           Synopsis:  Gives transaction to main logger thread and waits
 *						for completion signal
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
STATUS ServerThread::LogTransaction(STATUS nNewStatus)
{
	STATUS nStatus = 0;

	// Overwite result code if thread experienced problem
	m_oMyTran.SetTranStatus(nNewStatus);

	// Give logger thread the transaction and signal
	// A lock/unlock on this thread's mutex ensures it is ready to receive
	// a signal for sure
	m_poLggrThrd->Lock();

	m_poLggrThrd->MyTran() = &m_oMyTran;
	m_poLggrThrd->ReqServerCondVar() = &m_cndCondVar;
	m_poLggrThrd->ReqServerMutex() = &m_mtxCondMutex;

	m_poLggrThrd->Unlock();

	// Now lock myself and wait for signal back from logger

	Lock();

	nStatus = m_poLggrThrd->Signal();

	if (!nStatus)
		nStatus = Wait();

	Unlock();

	return nStatus;
}
