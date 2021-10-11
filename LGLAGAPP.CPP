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

#include "LGlagapp.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::LAGApplication
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
LAGApplication::LAGApplication()
{
	WORD nIndex = 0;

	// Initialise variables

	pthread_mutex_init(&m_mtxMrchLL, NULL);
	pthread_mutex_init(&m_mtxSrvrLL, NULL);
	pthread_mutex_init(&m_mtxLggrLL, NULL);
	pthread_mutex_init(&m_mtxOprLL, NULL);
	m_poConfiguration =
			new LGConfiguration(&m_llSrvrThrds,
								&m_llMrchThrds,
								&m_llLggrThrds,
								&m_llOprThrds);

	m_poStatistics = new LGStatistics;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::~LAGApplication
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
LAGApplication::~LAGApplication()
{
	pthread_mutex_destroy(&m_mtxMrchLL);
	pthread_mutex_destroy(&m_mtxSrvrLL);
	pthread_mutex_destroy(&m_mtxLggrLL);

	if (m_poConfiguration)
	{
		delete m_poConfiguration;
		m_poConfiguration = NULL;
	}

	if (m_poStatistics)
	{
		delete m_poStatistics;
		m_poStatistics = NULL;
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::GetFreeMrchThrd
 *
 *           Synopsis:  Searches linked list for free merchant thread
 *
 *   Input parameters:  Merchant interface number
 *
 *  Output parameters:  None
 *
 *       Return value:	pointer to merchant thread or null
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
MerchantThread* LAGApplication::GetFreeMrchThrd(RWCString strMrchIntName)
{
	MerchantThread *poFreeMrchThrd = NULL;

	poFreeMrchThrd = m_llMrchThrds.remove(FreeMerchant, (void*)&strMrchIntName);

	if (poFreeMrchThrd)
	{
		poFreeMrchThrd->SetStatusBusyTran();
		m_llMrchThrds.insert(poFreeMrchThrd);
	}

	return poFreeMrchThrd;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::GetLggrThrd
 *
 *           Synopsis:  Searches linked list for main logger thread
 *
 *   Input parameters:  Merchant interface number
 *
 *  Output parameters:  None
 *
 *       Return value:	pointer to logger thread or null
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
LoggerThread* LAGApplication::GetLggrThrd()
{
	LoggerThread *poMainLggrThrd = NULL;

	poMainLggrThrd = m_llLggrThrds.remove(MainLogger, (void*)NULL);

	if (poMainLggrThrd)
	{
		poMainLggrThrd->SetStatusBusyTran();
		m_llLggrThrds.insert(poMainLggrThrd);
	}

	return poMainLggrThrd;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::LoadConfigFile
 *
 *           Synopsis:  Reads in application configuration
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::LoadConfigFile()
{
	return m_poConfiguration->LoadConfiguration();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::GetMethods
 *
 *           Synopsis:  Returns the configuration to the caller
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
LGConfiguration LAGApplication::GetConfiguration()
{
	return *m_poConfiguration;
}

LGStatistics* LAGApplication::GetStatistics()
{
	return m_poStatistics;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::IsTracingOn
 *
 *           Synopsis:  Tests if trace flag is on
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
BOOL LAGApplication::IsTracingOn()
{
	return GetConfiguration().GetTracingFlag() == "Y";
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::OpenLogger
 *
 *           Synopsis:  Opens up a Logger Interface
 *
 *   Input parameters:  Logger Nbr 0 - MAX
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::OpenLogger(WORD nIndex)
{
	STATUS nStatus = 0;

	LockLggrLL();

	nStatus = (m_poConfiguration->GetLoggerInterface(nIndex))->Open();

	UnlockLggrLL();

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::OpenLoggers
 *
 *           Synopsis:  Opens all Logger Interfaces
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::OpenLoggers()
{
	WORD nIndex = 0;
	STATUS nStatus = 0;

	for (nIndex = 0;
		 nIndex < m_poConfiguration->GetNbrLoggerInterfaces() && !nStatus;
		 nIndex++)
	{
		nStatus = OpenLogger(nIndex);
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::OpenMerchant
 *
 *           Synopsis:  Opens up a Merchant Interface
 *
 *   Input parameters:  Merchant Nbr 0 - MAX
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::OpenMerchant(WORD nIndex)
{
	STATUS nStatus = 0;

	LockMrchLL();

	nStatus = m_poConfiguration->GetMerchantInterface(nIndex)->Open();

	UnlockMrchLL();

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::OpenMerchants
 *
 *           Synopsis:  Opens all Merchant Interfaces
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::OpenMerchants()
{
	WORD nIndex = 0;
	STATUS nStatus = 0;

	for (nIndex = 0;
		 nIndex < m_poConfiguration->GetNbrMerchantInterfaces() && !nStatus;
		 nIndex++)
	{
		nStatus = OpenMerchant(nIndex);
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::OpenServer
 *
 *           Synopsis:  Opens up a Server Interface
 *
 *   Input parameters:  Server Nbr 0 - MAX
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::OpenServer(WORD nIndex)
{
	STATUS nStatus = 0;

	LockSrvrLL();

	nStatus = m_poConfiguration->GetServerInterface(nIndex)->Open();

	UnlockSrvrLL();

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::OpenServers
 *
 *           Synopsis:  Opens up all Server Interfaces
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::OpenServers()
{
	WORD nIndex = 0;
	STATUS nStatus = 0;

	for (nIndex = 0;
		 nIndex < m_poConfiguration->GetNbrServerInterfaces() && !nStatus;
		 nIndex++)
	{
		nStatus = OpenServer(nIndex);
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::OpenOperator
 *
 *           Synopsis:  Opens operator command interface
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::OpenOperator()
{
	return m_poConfiguration->GetOperatorInterface()->Open();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::CloseLogger
 *
 *           Synopsis:  Closes logger merchant
 *
 *   Input parameters:  Logger Nbr 0 - MAX
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::CloseLogger(WORD nIndex)
{
	STATUS nStatus = TRANSACTION_IN_SYSTEM;

	while (nStatus == TRANSACTION_IN_SYSTEM)
	{
		LockLggrLL();

		nStatus = m_poConfiguration->GetLoggerInterface(nIndex)->Close();

		UnlockLggrLL();
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::CloseLoggers
 *
 *           Synopsis:  Closes all loggers
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::CloseLoggers()
{
	WORD nIndex = 0;
	STATUS nStatus = 0;

	for (nIndex = 0;
		 nIndex < m_poConfiguration->GetNbrLoggerInterfaces() && !nStatus;
		 nIndex++)
	{
		nStatus = CloseLogger(nIndex);
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::CloseMerchant
 *
 *           Synopsis:  Closes single merchant
 *
 *   Input parameters:  Merchant Nbr 0 - MAX
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::CloseMerchant(WORD nIndex)
{
	STATUS nStatus = TRANSACTION_IN_SYSTEM;

	while (nStatus == TRANSACTION_IN_SYSTEM)
	{
		LockMrchLL();

		nStatus = m_poConfiguration->GetMerchantInterface(nIndex)->Close();

		UnlockMrchLL();
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::CloseMerchants
 *
 *           Synopsis:  Closes all merchants
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::CloseMerchants()
{
	WORD nIndex = 0;
	STATUS nStatus = 0;

	for (nIndex = 0;
		 nIndex < m_poConfiguration->GetNbrMerchantInterfaces() && !nStatus;
		 nIndex++)
	{
		nStatus = CloseMerchant(nIndex);
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::CloseServer
 *
 *           Synopsis:  Closes single server
 *
 *   Input parameters:  Server Nbr 0 - MAX
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::CloseServer(WORD nIndex)
{
	STATUS nStatus = TRANSACTION_IN_SYSTEM;

	while (nStatus == TRANSACTION_IN_SYSTEM)
	{
		LockSrvrLL();

		nStatus = m_poConfiguration->GetServerInterface(nIndex)->Close();

		UnlockSrvrLL();
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::CloseServers
 *
 *           Synopsis:  Closes all servers
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::CloseServers()
{
	WORD nIndex = 0;
	STATUS nStatus = 0;

	for (nIndex = 0;
		 nIndex < m_poConfiguration->GetNbrServerInterfaces() && !nStatus;
		 nIndex++)
	{
		nStatus = CloseServer(nIndex);
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::CloseOperator
 *
 *           Synopsis:  Closes operator command interface
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  STATUS (0 - OK, !0 !OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LAGApplication::CloseOperator()
{
	return m_poConfiguration->GetOperatorInterface()->Close();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::LockMrchLL
 *
 *           Synopsis:  Locks global merchant linked list
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
void LAGApplication::LockMrchLL()
{
	pthread_mutex_lock(&m_mtxMrchLL);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::UnlockMrchLL
 *
 *           Synopsis:  Locks global merchant linked list
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
void LAGApplication::UnlockMrchLL()
{
	pthread_mutex_unlock(&m_mtxMrchLL);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::LockSrvrLL
 *
 *           Synopsis:  Locks global server linked list
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
void LAGApplication::LockSrvrLL()
{
	pthread_mutex_lock(&m_mtxSrvrLL);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::UnlockSrvrLL
 *
 *           Synopsis:  Locks global server linked list
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
void LAGApplication::UnlockSrvrLL()
{
	pthread_mutex_unlock(&m_mtxSrvrLL);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::LockLggrLL
 *
 *           Synopsis:  Locks global logger linked list
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
void LAGApplication::LockLggrLL()
{
	pthread_mutex_lock(&m_mtxLggrLL);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::UnlockLggrLL
 *
 *           Synopsis:  Locks global logger linked list
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
void LAGApplication::UnlockLggrLL()
{
	pthread_mutex_unlock(&m_mtxLggrLL);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::LoadThreadInfo
 *
 *           Synopsis:  Sets up thread info for operator command
 *
 *   Input parameters:  LAGStatus data structure
 *
 *  Output parameters:  LAGStatus data structure
 *
 *       Return value:  
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void LAGApplication::LoadThreadInfo(LAGStatus *pstThreadInfo)
{
	PThread *poThread;
	WORD nIndex;
	WORD nNbrInList = 0;
	WORD nTotal = 0;

	// Load Merchants
	LockMrchLL();

	nNbrInList = m_llMrchThrds.entries();

	for (nIndex = 0; nIndex < nNbrInList; nIndex++)
	{
		poThread = m_llMrchThrds.at(nIndex);
		strncpy(pstThreadInfo->ThreadDetails[nTotal].cThread, poThread->InterfaceName(), (sizeof(pstThreadInfo->ThreadDetails[nTotal].cThread) - 1));
		pstThreadInfo->ThreadDetails[nTotal].nThreadID = poThread->GetThreadId();
		pstThreadInfo->ThreadDetails[nTotal].enThreadStatus = poThread->Status();
		nTotal++;
	}

	UnlockMrchLL();

	// Load Servers
	LockSrvrLL();

	nNbrInList = m_llSrvrThrds.entries();

	for (nIndex = 0; nIndex < nNbrInList; nIndex++)
	{
		poThread = m_llSrvrThrds.at(nIndex);
		strncpy(pstThreadInfo->ThreadDetails[nTotal].cThread, poThread->InterfaceName(), (sizeof(pstThreadInfo->ThreadDetails[nTotal].cThread) - 1));
		pstThreadInfo->ThreadDetails[nTotal].nThreadID = poThread->GetThreadId();
		pstThreadInfo->ThreadDetails[nTotal].enThreadStatus = poThread->Status();
		nTotal++;
	}

	UnlockSrvrLL();

	// Load Loggers
	LockLggrLL();

	nNbrInList = m_llLggrThrds.entries();

	for (nIndex = 0; nIndex < nNbrInList; nIndex++)
	{
		poThread = m_llLggrThrds.at(nIndex);
		strncpy(pstThreadInfo->ThreadDetails[nTotal].cThread, poThread->InterfaceName(), (sizeof(pstThreadInfo->ThreadDetails[nTotal].cThread) - 1));
		pstThreadInfo->ThreadDetails[nTotal].nThreadID = poThread->GetThreadId();
		pstThreadInfo->ThreadDetails[nTotal].enThreadStatus = poThread->Status();
		nTotal++;
	}

	UnlockLggrLL();

	pstThreadInfo->nNbrThreads = nTotal;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGApplication::CutoverLogFile
 *
 *           Synopsis:  Stops servers and main logger and gets new log file
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
STATUS LAGApplication::CutoverLogFile()
{
	STATUS nStatus = 0;

	nStatus = CloseServers();

	if (!nStatus)
	{
		nStatus = CloseLogger(0);
		if (!nStatus)
		{
			nStatus = m_poConfiguration->GetLoggerInterface(0)->RenameLogFile();
			if (!nStatus)
			{
				nStatus = OpenLogger(0);
				if (!nStatus)
					nStatus = OpenServers();
			}
		}
	}

	return nStatus;
}

/****************************************************************************
 *
 *   FUNCTION DEFINITION
 *
 *               Name:  FreeMerchant
 *
 *           Synopsis:  Used to match idle merchant thread
 *
 *   Input parameters:  Merchant interface number
 *
 *  Output parameters:  None
 *
 *       Return value:	RWBoolean (int)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
RWBoolean FreeMerchant(MerchantThread* poMrchThrd, void* strMrchIntName)
{
	RWCString *pstrMrchIntName = (RWCString*)strMrchIntName;

	if (*pstrMrchIntName == poMrchThrd->InterfaceName() &&
		poMrchThrd->OKIdle())
		return TRUE;
	else
		return FALSE;
}

/****************************************************************************
 *
 *   FUNCTION DEFINITION
 *
 *               Name:  MainLogger
 *
 *           Synopsis:  Used to match main logger thread
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	RWBoolean (int)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
RWBoolean MainLogger(LoggerThread* poLggrThrd, void* d)
{
	if (poLggrThrd->IsMain() && poLggrThrd->OKIdle())
		return TRUE;
	else
		return FALSE;
}

