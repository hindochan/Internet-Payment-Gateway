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
#include <rw/rwdate.h>
#include <rw/rwtime.h>

#include "LGlgrint.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerInterface::LoggerInterface
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
LoggerInterface::LoggerInterface()
{
	// Initialise variables
	m_bMultiThreaded = TRUE;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerInterface::LoggerInterface
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  Address of global thread linked list
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
LoggerInterface::LoggerInterface(RWTPtrDlist<LoggerThread> *pllLggrThrds)
{
	LoggerInterface();
	m_pllLggrThrds = pllLggrThrds;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerInterface::~LoggerInterface
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
LoggerInterface::~LoggerInterface()
{

}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerInterface::Open
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
STATUS LoggerInterface::Open()
{
	WORD nIndex = 0;
	STATUS nStatus = 0;
	void *pVoid = NULL;

	for (nIndex = 0; nIndex < m_nNbrPorts && !nStatus; nIndex++)
	{
		m_poLggrThrd = new LoggerThread(m_strName);

		if (m_poLggrThrd)
		{
			m_poLggrThrd->Lock();
			nStatus = m_poLggrThrd->Create();

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

					m_poLggrThrd->Port() = m_poPort;
					// Now get Message
					m_poMsg = GetNewMsg();
					if (m_poMsg)
					{
						m_poLggrThrd->Message() = m_poMsg;
						// Since all OK add to list
						m_pllLggrThrds->insert(m_poLggrThrd);
					}
					else
						nStatus = MEMORY_ERROR;
				}
				else
					nStatus = MEMORY_ERROR;
			}

			m_poLggrThrd->Unlock();
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
 *               Name:  LoggerInterface::Close
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
STATUS LoggerInterface::Close()
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
 *               Name:  LoggerInterface::RenameLogFile
 *
 *           Synopsis:  Adds datetime to log file
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
STATUS LoggerInterface::RenameLogFile()
{
	char cTemp[5];
	STATUS nStatus = 0;
	WORD nIndex = 0;
	RWCTokenizer tknStr(m_strCommsConfig);
	RWCString strValue;
	RWCString strLogFileName;
	RWCString strDateTimeName;
	RWDate oDate;
	RWTime oTime;

	memset(cTemp, '\0', sizeof(cTemp));

	// Get file name
	strValue = tknStr(",");
	if (strValue.index("wrr") != RW_NPOS)
	{
		strLogFileName = strValue(3, strValue.length() - 3);
		nIndex = strLogFileName.index("CURRENT");
		if (nIndex)
		{
			strDateTimeName = strLogFileName(0, nIndex);
			sprintf(cTemp, "%4.4d", oDate.year());
			strDateTimeName += cTemp;
			sprintf(cTemp, "%2.2d", oDate.month());
			strDateTimeName += cTemp;
			sprintf(cTemp, "%2.2d", oDate.dayOfMonth());
			strDateTimeName += cTemp;
			sprintf(cTemp, "%2.2d", oTime.hour());
			strDateTimeName += cTemp;
			sprintf(cTemp, "%2.2d", oTime.minute());
			strDateTimeName += cTemp;
			sprintf(cTemp, "%2.2d", oTime.second());
			strDateTimeName += cTemp;
			nStatus = ::rename(strLogFileName, strDateTimeName);
		}
		else
			nStatus = NO_CURRENT_LOGFILE;
	}
	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerInterface::CancelThreads
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
STATUS LoggerInterface::CancelThreads()
{
	STATUS nStatus = 0;
	WORD nCount;
	WORD nIndex = 0;

	nCount = m_pllLggrThrds->entries();

	while (nIndex < nCount && !nStatus)
	{
		m_poLggrThrd = m_pllLggrThrds->get();

		if (m_poLggrThrd)
		{
			if (m_poLggrThrd->InterfaceName() == Name())
			{
				if (m_poLggrThrd->BusyTran())
					nStatus = TRANSACTION_IN_SYSTEM;
				else
				{
					if (!m_poLggrThrd->Dead())
					{
						nStatus = m_poLggrThrd->Cancel();
						if (!nStatus)
						{
							m_poLggrThrd->Join();
							m_poLggrThrd->SetStatusDead();
						}
					}
				}
			}
			m_pllLggrThrds->insert(m_poLggrThrd);
		}
		nIndex++;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LoggerInterface::DeleteThreads
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
void LoggerInterface::DeleteThreads()
{
	WORD nCount;
	WORD nIndex = 0;

	nCount = m_pllLggrThrds->entries();

	while (nIndex < nCount)
	{
		m_poLggrThrd = m_pllLggrThrds->get();

		if (m_poLggrThrd)
		{
			if (m_poLggrThrd->InterfaceName() == Name())
			{
				delete m_poLggrThrd;
				m_poLggrThrd = NULL;
			}
		}
		nIndex++;
	}
}
