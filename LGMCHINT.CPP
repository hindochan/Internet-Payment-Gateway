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

#include "LGmchint.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantInterface::MerchantInterface
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
MerchantInterface::MerchantInterface()
{
	// Initialise variables
	m_bMultiThreaded = TRUE;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantInterface::MerchantInterface
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  Address of global merchant thread linked list
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
MerchantInterface::MerchantInterface(RWTPtrDlist<MerchantThread> *pllMrchThrds)
{
	m_bMultiThreaded = TRUE;
	m_pllMrchThrds = pllMrchThrds;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantInterface::~MerchantInterface
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
MerchantInterface::~MerchantInterface()
{

}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantInterface::Open
 *
 *           Synopsis:  Opens interface
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS MerchantInterface::Open()
{
	STATUS nStatus = 0;
	WORD nIndex = 0;
	void *pVoid = NULL;

	for (nIndex = 0; nIndex < m_nNbrPorts && !nStatus; nIndex++)
	{
		m_poMrchThrd = new MerchantThread(m_strName);

		if (m_poMrchThrd)
		{
			m_poMrchThrd->Lock();
			nStatus = m_poMrchThrd->Create();

			if (!nStatus)
			{
				// Get Port
				m_poPort = GetNewPort();
				if (m_poPort)
				{
					// If first one then call PreThreadInit ?? handle nStatus
					// the return from this is then passed to subsequent ones
					if (nIndex == 0)
						pVoid = m_poPort->PreThreadInit();
					else
						nStatus = m_poPort->Init(pVoid);

					m_poMrchThrd->Port() = m_poPort;
					// Now get Message
					m_poMsg = GetNewMsg();
					if (m_poMsg)
					{
						m_poMrchThrd->Message() = m_poMsg;
						// Since all OK add to list
						m_pllMrchThrds->insert(m_poMrchThrd);
					}
					else
						nStatus = MEMORY_ERROR;
				}
				else
					nStatus = MEMORY_ERROR;
			}

			m_poMrchThrd->Unlock();
		}
		else
			nStatus = MEMORY_ERROR;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantInterface::Close
 *
 *           Synopsis:  Closes interface
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS MerchantInterface::Close()
{
	STATUS nStatus = 0;

	nStatus = CancelThreads();

	if (!nStatus)
		DeleteThreads();

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantInterface::CancelThreads
 *
 *           Synopsis:  Cancels each thread
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS MerchantInterface::CancelThreads()
{
	STATUS nStatus = 0;
	WORD nCount;
	WORD nIndex = 0;

	nCount = m_pllMrchThrds->entries();

	while (nIndex < nCount && !nStatus)
	{
		m_poMrchThrd = m_pllMrchThrds->get();

		if (m_poMrchThrd)
		{
			if (m_poMrchThrd->InterfaceName() == Name())
			{
				if (m_poMrchThrd->BusyTran())
					nStatus = TRANSACTION_IN_SYSTEM;
				else
				{
					if (!m_poMrchThrd->Dead())
					{
						nStatus = m_poMrchThrd->Cancel();
						if (!nStatus)
						{
							m_poMrchThrd->Join();
							m_poMrchThrd->SetStatusDead();
						}
					}
				}
			}
			m_pllMrchThrds->insert(m_poMrchThrd);
		}
		nIndex++;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MerchantInterface::DeleteThreads
 *
 *           Synopsis:  Deletes each thread
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  None
 *
 *        Description:
 *
 *              Notes:	Serious function to call
 *
 ****************************************************************************
 */
void MerchantInterface::DeleteThreads()
{
	WORD nCount;
	WORD nIndex = 0;

	nCount = m_pllMrchThrds->entries();

	while (nIndex < nCount)
	{
		m_poMrchThrd = m_pllMrchThrds->get();

		if (m_poMrchThrd)
		{
			if (m_poMrchThrd->InterfaceName() == Name())
			{
				delete m_poMrchThrd;
				m_poMrchThrd = NULL;
			}
		}
		nIndex++;
	}
}
