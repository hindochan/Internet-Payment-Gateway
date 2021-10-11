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

#include "LGoprint.h"
#include "LGglobls.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorInterface::OperatorInterface
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
OperatorInterface::OperatorInterface()
{
	// Initialise variables
	m_bMultiThreaded = FALSE;
	m_nInterfaceNbr = 0;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorInterface::OperatorInterface
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
OperatorInterface::OperatorInterface(RWTPtrDlist<OperatorThread> *pllOprThrds)
{
	m_bMultiThreaded = FALSE;
	m_nInterfaceNbr = 0;
    m_pllOprThrds = pllOprThrds;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorInterface::~OperatorInterface
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
OperatorInterface::~OperatorInterface()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorInterface::Open
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
STATUS OperatorInterface::Open()
{
	STATUS nStatus = 0;
    WORD nIndex = 0;
    void *pVoid = NULL;
 
    for (nIndex = 0; nIndex < m_nNbrPorts && !nStatus; nIndex++)
    {
		if (m_bMultiThreaded)
		{
			m_poOprThrd = new OperatorThread(m_nInterfaceNbr);
			if (m_poOprThrd)
			{
				m_poOprThrd->Lock();
				nStatus = m_poOprThrd->Create();
			}
			else
				nStatus = MEMORY_ERROR;
		}
		else
		{
			// Ignore the NbrPorts setting and use the current thread
			m_nNbrPorts = 1;
		}

		if (nStatus == OK)
		{
			// Get Port
			m_poPort = GetNewPort();
			// Now get Message
			m_poMsg = GetNewMsg();
			if (m_poPort && m_poMsg)
			{
				// If first one then call PreThreadInit ?? handle nStatus
				// the return from this is then passed to subsequent ones
				if (m_bMultiThreaded)
				{
					if (nIndex == 0)
						pVoid = m_poPort->PreThreadInit();
					else
						nStatus = m_poPort->Init(pVoid);

					m_poOprThrd->Port() = m_poPort;
					m_poOprThrd->Message() = m_poMsg;
					// Since all OK add to list
					m_pllOprThrds->insert(m_poOprThrd);
				}
				else
					nStatus = ProcessOperatorCommands();
			}
			else
				nStatus = MEMORY_ERROR;
		}
		if (m_bMultiThreaded)
		{
            m_poOprThrd->Unlock();
        }
    }
	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorInterface::Close
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
STATUS OperatorInterface::Close()
{
	STATUS nStatus = 0;

	if (m_bMultiThreaded)
		nStatus = CancelThreads ();
	else
		nStatus = m_poPort->Close();

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorInterface::CancelThreads
 *
 *           Synopsis:  joins each thread to wait for termination
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
STATUS OperatorInterface::CancelThreads()
{
    STATUS nStatus = 0;
    WORD nIndex = 0;
 
    while (nIndex < m_nNbrPorts  && m_bMultiThreaded && !nStatus)
    {
        m_poOprThrd = m_pllOprThrds->get();
 
        if (m_poOprThrd->InterfaceName() == Name())
        {
            nStatus = m_poOprThrd->Join();
            nIndex++;
        }
        else
            m_pllOprThrds->insert(m_poOprThrd);
    }
 
    return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorInterface::ProcessRequest
 *
 *           Synopsis:  Handles actual request
 *
 *   Input parameters:  Request
 *
 *  Output parameters:  Response and it's length
 *
 *       Return value:  STATUS
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS OperatorInterface::ProcessRequest (OperatorCommand *pOperatorRequest,
						OperatorResponse	*pOperatorResponse,
						WORD				*pnLength)
{
STATUS	nStatus;

	pOperatorResponse->nCmd = pOperatorRequest->nCmd;
	switch (pOperatorRequest->nCmd)
	{
	case LAG_STOP:
	{
	LAGStatus	*pstStatus = (LAGStatus*)&pOperatorResponse->uData.stStatus;

		if (TRACE)
			cout << "LAG Stop\n";

		if ((nStatus = oTheApplication.CloseServers()) ||
			(nStatus = oTheApplication.CloseMerchants()) ||
			(nStatus = oTheApplication.CloseLoggers()))
		{
			pOperatorResponse->nStatus = nStatus;
		}
		else
		{
			nStatus = COMMIT_HARIKIRI;
		}
		*pnLength = (int)&pOperatorResponse->uData.stStatus
					- (int)pOperatorResponse;
		break;
	}
	case LAG_STATUS:
	{
	LAGStatus	*pstStatus = (LAGStatus*)&pOperatorResponse->uData.stStatus;

		if (TRACE)
			cout << "LAG Status\n";

		/* Setup the LAGStatus DataStructure */
		oTheApplication.LoadThreadInfo(pstStatus);

		*pnLength = (int)&pOperatorResponse->uData.stStatus.ThreadDetails
					- (int)pOperatorResponse
					+ (sizeof (LAGThreadDetails) * (pstStatus->nNbrThreads));
		pOperatorResponse->nStatus = 0;
		break;
	}
	case LAG_STATISTICS:
	{
	LAGStatistics	*stStatistics = &pOperatorResponse->uData.stStatistics;

		if (TRACE)
			cout << "LAG Statistics\n";

		/* Setup the LAGStatistics data structure */
		stStatistics->nDateStarted =
						oTheApplication.GetStatistics()->GetStartDate();
		stStatistics->nNoTransOK =
						oTheApplication.GetStatistics()->GetSuccessTrans();
		stStatistics->nNoTransNOK =
						oTheApplication.GetStatistics()->GetFailedTrans();
		stStatistics->nAvgServResponse =
						oTheApplication.GetStatistics()->GetAverageSrvrResp();
		stStatistics->nAvgMrchResponse =
						oTheApplication.GetStatistics()->GetAverageMrchResp();

		*pnLength = (int)&pOperatorResponse->uData.stStatus
					- (int)pOperatorResponse
					+ sizeof (LAGStatistics);
		pOperatorResponse->nStatus = 0;
		break;
	}
	case LAG_CUTOVER:

		if (TRACE)
			cout << "LAG cutover\n";

		pOperatorResponse->nStatus = oTheApplication.CutoverLogFile();
		*pnLength = (int)&pOperatorResponse->uData.stStatus
					 - (int)pOperatorResponse;
		break;
	default:
		if (TRACE)
			cout << "Unknown Operator Command\n";

		nStatus = UNKNOWN_OPERATOR_COMMAND;
		break;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OperatorInterface::ProcessOperatorCommands
 *
 *           Synopsis:  Obvious
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
STATUS OperatorInterface::ProcessOperatorCommands()
{
	STATUS				nStatus = OK;
	DataMsg				stMyBuffer;
	OperatorCommand		stOprRequest;
	OperatorResponse	stOprResponse;
	BOOL				bTerminate = FALSE;

	while (!bTerminate)
	{
		nStatus = m_poPort->Open();

		if (!nStatus)
		{
			nStatus = m_poPort->WaitForConnection();
			if (nStatus == OK)
			{
				nStatus = m_poPort->Read();
				if (nStatus == OK)
				{
					stMyBuffer = m_poPort->DataBuffer();
					memcpy (&stOprRequest,
							stMyBuffer.cMessage,
							stMyBuffer.nMessageLen);

					nStatus = ProcessRequest(	&stOprRequest,
												&stOprResponse,
												&stMyBuffer.nMessageLen);

					/* Write the response back */
					memcpy (stMyBuffer.cMessage,
							&stOprResponse,
							stMyBuffer.nMessageLen);

					m_poPort->DataBuffer() = stMyBuffer;
					if (nStatus ==  COMMIT_HARIKIRI)
					{
						bTerminate = TRUE;
					}
					nStatus = m_poPort->Write();

					if (nStatus != OK)
					{
						cout << "Error writing data\n";
					}
					m_poPort->Close();
				}
			}
		}
	}

	return nStatus;
}
