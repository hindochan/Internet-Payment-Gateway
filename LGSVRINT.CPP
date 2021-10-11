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

#include "LGsvrint.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerInterface::ServerInterface
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
ServerInterface::ServerInterface()
{
	// Initialise variables
	m_bMultiThreaded = TRUE;
	m_nInterfaceNbr = 0;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerInterface::ServerInterface
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  Address of global server thread linked list
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
ServerInterface::ServerInterface(RWTPtrDlist<ServerThread> *pllSrvrThrds)
{
	//ServerInterface();

	m_bMultiThreaded = TRUE;
	m_nInterfaceNbr = 0;

	m_pllSrvrThrds = pllSrvrThrds;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerInterface::~ServerInterface
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
ServerInterface::~ServerInterface()
{

}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerInterface::Open
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
STATUS ServerInterface::Open()
{
	STATUS nStatus = 0;
	WORD nIndex = 0;
	void *pVoid = NULL;

	for (nIndex = 0; nIndex < m_nNbrPorts && !nStatus; nIndex++)
	{
		m_poSrvrThrd = new ServerThread(m_strName);

		if (m_poSrvrThrd)
		{
			m_poSrvrThrd->Lock();
			nStatus = m_poSrvrThrd->Create();

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

					m_poSrvrThrd->Port() = m_poPort;
					// Now get Message
					m_poMsg = GetNewMsg();
					if (m_poMsg)
					{
						m_poSrvrThrd->Message() = m_poMsg;
						// Since all OK add to list
						m_pllSrvrThrds->insert(m_poSrvrThrd);
					}
					else
						nStatus = MEMORY_ERROR;
				}
				else
					nStatus = MEMORY_ERROR;
			}

			m_poSrvrThrd->Unlock();
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
 *               Name:  ServerInterface::Close
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
STATUS ServerInterface::Close()
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
 *               Name:  ServerInterface::CancelThreads
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
STATUS ServerInterface::CancelThreads()
{
	STATUS nStatus = 0;
	WORD nCount;
	WORD nIndex = 0;

	nCount = m_pllSrvrThrds->entries();

	while (nIndex < nCount && !nStatus)
	{
		m_poSrvrThrd = m_pllSrvrThrds->get();

		if (m_poSrvrThrd)
		{
			if (m_poSrvrThrd->InterfaceName() == Name())
			{
				if (m_poSrvrThrd->BusyTran())
					nStatus = TRANSACTION_IN_SYSTEM;
				else
				{
					if (!m_poSrvrThrd->Dead())
					{
						nStatus = m_poSrvrThrd->Cancel();
						if (!nStatus)
						{
							m_poSrvrThrd->Join();
							m_poSrvrThrd->SetStatusDead();
						}
					}
				}
			}
			m_pllSrvrThrds->insert(m_poSrvrThrd);
		}
		nIndex++;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  ServerInterface::DeleteThreads
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
void ServerInterface::DeleteThreads()
{
	WORD nCount;
	WORD nIndex = 0;

	nCount = m_pllSrvrThrds->entries();

	while (nIndex < nCount)
	{
		m_poSrvrThrd = m_pllSrvrThrds->get();

		if (m_poSrvrThrd)
		{
			if (m_poSrvrThrd->InterfaceName() == Name())
			{
				delete m_poSrvrThrd;
				m_poSrvrThrd = NULL;
			}
		}
		nIndex++;
	}
}
