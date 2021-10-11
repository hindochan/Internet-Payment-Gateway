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

#include "LGoprthr.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorThread::OperatorThread
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
OperatorThread::OperatorThread(RWCString strInterfaceName)
{
	// Initialise variables
	m_strInterfaceName = strInterfaceName;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorThread::~OperatorThread
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
OperatorThread::~OperatorThread()
{

}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorThread::ReferenceMethods
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
LoggerThread* & OperatorThread::LggrThrd()
{
	return m_poLggrThrd;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorThread::Create
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
STATUS OperatorThread::Create()
{
	return PThread::Create(OperatorThreadMain, this);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorThread::GetTransaction
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
void OperatorThread::GetTransaction()
{
	m_poMsg->InternalMsg() = m_oMyTran.InternalMsg();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorThread::PutTransaction
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
void OperatorThread::PutTransaction()
{
	m_oMyTran.InternalMsg() = m_poMsg->InternalMsg();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorThread::ValidateTransaction
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
STATUS OperatorThread::ValidateTransaction()
{
	return m_oMyTran.ValidateRequest();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorThread::LogTransaction
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
STATUS OperatorThread::LogTransaction(STATUS nNewStatus)
{
	STATUS nStatus = 0;

	// Overwite result code if thread experienced problem
	m_oMyTran.SetTranStatus(nNewStatus);

	// Give logger thread the transaction and signal
	// A lock/unlock on this thread's mutex ensures it is ready to receive
	// a signal for sure
	m_poLggrThrd->Lock();

	m_poLggrThrd->MyTran() = &m_oMyTran;
	// Put this in if you need to get a signal back from logger
	// m_poLggrThrd->ReqOperatorCondVar() = &m_cndCondVar;

	m_poLggrThrd->Unlock();

	nStatus = m_poLggrThrd->Signal();

	if (!nStatus)
	{
		// Now lock myself and wait for signal back from logger

		Lock();

		nStatus = Wait();

		Unlock();
	}

	return nStatus;
}
