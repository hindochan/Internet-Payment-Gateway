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

#include "LGfleprt.h"
#include "LGstrcts.h"
#include "LGglobls.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  FilePort::FilePort
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
FilePort::FilePort()
{
	// Initialise variables
	m_pstFileDescs = NULL;
	m_bReadFilePresent = FALSE;
	m_bWriteFilePresent = FALSE;
	m_cNewLine[0] = '\n';
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  FilePort::FilePort
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  Comm type & config string
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
FilePort::FilePort(WORD nType, RWCString strCommsConfig)
{
	m_strCommsConfig = strCommsConfig;

	RWCTokenizer tknStr(m_strCommsConfig);
	RWCString strValue;

	// Initialise variables

	m_pstFileDescs = NULL;
	m_bReadFilePresent = FALSE;
	m_bWriteFilePresent = FALSE;
	m_cNewLine[0] = '\n';

	m_nType = nType;

	// Get first file name
	strValue = tknStr(",");
	if (strValue.index("rdr") != RW_NPOS)
	{
		m_strReadFileName = strValue(3, strValue.length() - 3);
		m_bReadFilePresent = TRUE;
	}
	else
	{
		if (strValue.index("wrr") != RW_NPOS)
		{
			m_strWriteFileName = strValue(3, strValue.length() - 3);
			m_bWriteFilePresent = TRUE;
		}
	}

	// Get next one if its there
	strValue = tknStr(",");
	if (!strValue.isNull())
	{
		if (strValue.index("rdr") != RW_NPOS)
		{
			m_strReadFileName = strValue(3, strValue.length() - 3);
			m_bReadFilePresent = TRUE;
		}
		else
		{
			if (strValue.index("wrr") != RW_NPOS)
			{
				m_strWriteFileName = strValue(3, strValue.length() - 3);
				m_bWriteFilePresent = TRUE;
			}
		}
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  FilePort::~FilePort
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
FilePort::~FilePort()
{
	Close();

	if (m_pstFileDescs)
	{
		delete m_pstFileDescs;
		m_pstFileDescs = NULL;
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  FilePort::PreThreadInit
 *
 *           Synopsis:  Function called before thread creation is complete
 *						to enable multiple threads to share data
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	pointer
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void * FilePort::PreThreadInit()
{
	if (TRACE)
	{
		cout << "File PreThreadInit \n";
		cout.flush();
	}

	Open();

	return m_pstFileDescs;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  FilePort::Init
 *
 *           Synopsis:  Comms initialisation
 *
 *   Input parameters:  pointer
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS FilePort::Init(void *pVoid)
{
	STATUS nStatus;

	if (TRACE)
	{
		cout << "In FILE type Init method \n";
		cout << "My config string is :- " << m_strCommsConfig <<"\n";
		cout.flush();
	}

	if (!m_pstFileDescs)
		m_pstFileDescs = (FileDescs*)pVoid;

	if (m_pstFileDescs)
	{
		m_bInitFlag = TRUE;
		nStatus = 0;
	}
	else
	{
		m_bInitFlag = FALSE;
		nStatus = PORT_INIT_ERROR;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  FilePort::Open
 *
 *           Synopsis:  Open connection
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS FilePort::Open()
{
	// Init routines set up the file descriptor by opening file
	// so don't open if InitFlag set
	STATUS nStatus = 0;
	int iFlags = 0;

	if (!m_bInitFlag)
	{
		if (m_pstFileDescs)
		{
			delete m_pstFileDescs;
			m_pstFileDescs = NULL;
		}

		m_pstFileDescs = new FileDescs;

		if (m_pstFileDescs)
		{
			READFILE = 0;
			WRITEFILE = 0;

			if (m_bReadFilePresent)
			{
				iFlags = O_RDONLY | O_RSYNC;
				READFILE = ::open(m_strReadFileName, iFlags);
				if (!READFILE)
					nStatus = PORT_INIT_ERROR;
			}

			if (m_bWriteFilePresent && !nStatus)
			{
				iFlags = O_WRONLY | O_APPEND | O_CREAT | O_SYNC;
				WRITEFILE = ::open(m_strWriteFileName, iFlags, 0660);
				if (!WRITEFILE)
					nStatus = PORT_INIT_ERROR;
			}

			if (!nStatus && (m_bWriteFilePresent || m_bReadFilePresent))
			{
				m_bInitFlag = TRUE;
				if (TRACE)
					cout << "File Port Opened successfully \n";

				if (m_bReadFilePresent)
				{
					if (TRACE)
					{
						cout << "Read File descriptor is :- " << READFILE;
						cout << "\n";
						cout << "Read File name is :- " << m_strReadFileName;
						cout << "\n";
					}
				}
				if (m_bWriteFilePresent)
				{
					if (TRACE)
					{
						cout << "Write File descriptor is :- " << WRITEFILE;
						cout << "\n";
						cout << "Write File name is :- " << m_strWriteFileName;
						cout << "\n";
					}
				}
					
				if (TRACE)
					cout.flush();
			}
			else
				nStatus = PORT_INIT_ERROR;
		}
		else
			nStatus = PORT_INIT_ERROR;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  FilePort::Read
 *
 *           Synopsis:  Read line
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS FilePort::Read()
{
	WORD nBytesRead;
	STATUS nStatus = 0;

	if (m_bReadFilePresent)
	{
		for (nBytesRead = 0;
			read(READFILE, &m_stDataBuffer.cMessage[nBytesRead], 1);
			nBytesRead++)
		{
			if (m_stDataBuffer.cMessage[nBytesRead] == m_cNewLine[0])
				break;
		}

		if (nBytesRead)
			m_stDataBuffer.nMessageLen = nBytesRead;
		else
			nStatus = END_OF_FILE;
	}
	else
		nStatus = READ_ERROR;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  FilePort::Write
 *
 *           Synopsis:  Write line
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS FilePort::Write()
{
	STATUS nStatus;
	int iBytesWritten = 0;

	if (m_bWriteFilePresent)
	{
		if (m_stDataBuffer.nMessageLen < MAX_MSG_LEN)
		{
			// Add newline character
			memcpy(&m_stDataBuffer.cMessage[m_stDataBuffer.nMessageLen],
					m_cNewLine, sizeof(m_cNewLine));

			m_stDataBuffer.nMessageLen ++;

			iBytesWritten =
				 write(WRITEFILE, &m_stDataBuffer.cMessage[0],
						m_stDataBuffer.nMessageLen);

			if (iBytesWritten == m_stDataBuffer.nMessageLen)
				nStatus = OK;
			else
				nStatus = FILE_WRITE_ERROR;
		}
		else
			nStatus = FILE_WRITE_ERROR;
	}
	else
		nStatus = FILE_WRITE_ERROR;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  FilePort::Close
 *
 *           Synopsis:  Close line
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS FilePort::Close()
{
	STATUS nStatus = 0;

	m_bInitFlag = FALSE;

	if (m_bReadFilePresent)
	{
		if (READFILE)
		{
			nStatus = ::close(READFILE);
			READFILE = 0;
		}
	}

	if (m_bWriteFilePresent && !nStatus)
	{
		if (WRITEFILE)
		{
			nStatus = ::close(WRITEFILE);
			WRITEFILE = 0;
		}
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  FilePort::WaitForConnection
 *
 *           Synopsis:  Wait for line connect
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS FilePort::WaitForConnection()
{
	STATUS nStatus = 0;

	return nStatus;
}
