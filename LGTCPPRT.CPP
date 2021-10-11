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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <unistd.h>

#include <rw/ctoken.h>

#include "LGtypes.h"
#include "LGtcpprt.h"
#include "LGglobls.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::CommonConstructorInitialisation
 *
 *           Synopsis:  Performs the common contructor initialisation
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

void TCPIPPort::CommonConstructorInitialisation ()
{
	m_bListen = FALSE;

	m_pMainSocket = 0;
	m_pMsgSocket = 0;
	m_Port = 0;
	m_RecovWaitTime = 0;
	m_nMsgHdrInLen = 2; // 2 bytes for the length
	m_nMsgTrlInLen = 0;
	m_nMsgHdrOutLen = 2; // 2 bytes for the length
	m_nMsgTrlOutLen = 0;
	m_nInitialReadLength = 2;
	m_RetryDelay = 1000;

}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::TCPIPPort
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
TCPIPPort::TCPIPPort()
{
	// Initialise variables
	CommonConstructorInitialisation ();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::TCPIPPort
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
TCPIPPort::TCPIPPort(WORD nType, RWCString strCommsConfig)
{
RWCTokenizer tknStr(m_strCommsConfig);
RWCString strValue;

	CommonConstructorInitialisation();

	// Initialise variables
	m_nType = nType;
	m_strCommsConfig = strCommsConfig;

	/* Get hostname */
	m_strHostName = tknStr (",");

	/* Get port number */
	strValue = tknStr(",");
	m_Port = atoi(strValue.data());

	/* Get LISTEN BOOL */
	if (m_strCommsConfig.index("LISTEN") == RW_NPOS)
		m_bListen = FALSE;
	else
		m_bListen = TRUE;

}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::~TCPIPPort
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
TCPIPPort::~TCPIPPort()
{
	if (m_pMainSocket)
		CloseSocket (&m_pMainSocket);
	if (m_pMsgSocket)
		CloseSocket (&m_pMsgSocket);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::PreThreadInit
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
void * TCPIPPort::PreThreadInit()
{
	if (TRACE)
	{
		cout << "TCPIP PreThreadInit \n";
		cout.flush();
	}

	Open();

	return m_pMainSocket;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::Init
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
STATUS TCPIPPort::Init(void *pVoid)
{
	STATUS nStatus = OK;

	if (TRACE)
	{
		cout << "In TCPIP type Init method \n";
		cout << "My config string is :- " << m_strCommsConfig <<"\n";
		cout.flush();
	}

	if (!m_pMainSocket)
		m_pMainSocket = (int*)pVoid;

	if (m_pMainSocket && *m_pMainSocket)
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
 *               Name:  TCPIPPort::Open
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
STATUS TCPIPPort::Open()
{
	// Init routines set up the file descriptor by opening socket
	// so don't open if InitFlag set
	STATUS nStatus = OK;

	if (!m_bInitFlag)
	{
		if (m_pMainSocket)
			delete m_pMainSocket;

		m_pMainSocket = new int;

		if (m_pMainSocket)
		{
			*m_pMainSocket = -1;
			if ((*m_pMainSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
			{
				nStatus = PORT_INIT_ERROR;
				cerr << "Call to socket() failed \n";
				cerr.flush();
			}
			if ((nStatus = SetSocketOptions()))
			{
				//nStatus = PORT_INIT_ERROR;
				cerr << "SetSocketOptions() failed \n";
				cerr.flush();
			}
			if ((nStatus = SetAddrInfo()))
			{
				//nStatus = PORT_INIT_ERROR;
				cerr << "SetAddrInfo() failed \n";
				cerr.flush();
			}
		}
		else
			nStatus = MEMORY_ERROR;
	}

	if (!nStatus)
		m_bInitFlag = TRUE;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::ValidateMessagefromLength
 *
 *           Synopsis:  Validates if all the data has been read
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	BOOL 
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */

BOOL TCPIPPort::ValidateMessageFromLength ()
{
	BOOL	bReturnValue = TRUE;
	WORD	len;

	if (m_stCommsBuffer.nMessageLen < m_nInitialReadLength)
		bReturnValue =  FALSE;

	memcpy (&len, m_stCommsBuffer.cMessage, m_nInitialReadLength);
	//len = ntohs (len);
	if (m_stCommsBuffer.nMessageLen  < len)
		bReturnValue =  FALSE;

	return bReturnValue;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::ReadMsgPort
 *
 *           Synopsis:  Read data
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

STATUS TCPIPPort::ReadMsgPort (	char *cBuffer,
								int nBufferLen,
								int *pBytesRead)
{
	STATUS	nStatus = OK;
	int		nBytesRead;

	if (!m_pMsgSocket)
	{
		nStatus = WaitForConnection ();
	}
	if (nStatus == OK)
	{
		nStatus = WaitForInput (*m_pMsgSocket, 500); // Timeout in milliseconds
		if (nStatus == OK)
		{
			/* Read the line */
			nBytesRead = recv (*m_pMsgSocket, cBuffer, nBufferLen, 0);
			switch (nBytesRead)
			{
			case -1:			// Error Condition
				nStatus = ConvertSystemError ();
				break;
			case 0:				// Zero data read
				/* Treat this as a line down condition as WaitForInput */
				/* indicated that there was data to read, yet no data arrived */
				nStatus = TCPIP_LINE_DOWN;
				CloseSocket (&m_pMsgSocket);
				break;
			default:			// Valid Data read
				/* Data read, return it back to the caller */
				*pBytesRead = nBytesRead;
				if (TRACE)
				{
					cout << "Bytes Read: " << nBytesRead << "\n";
					cout.flush();
				}
			}
		}
	}
	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::Read
 *
 *           Synopsis:  Read data
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
STATUS TCPIPPort::Read()
{
	STATUS	nStatus = OK;
	int		BytesRead = 0, BytesToRead = 0, TotalRead = 0;

	/******************************************************************/
	/* Read the initial header bytes. If OK, then proceed to read the */
	/* remainder of the datastream. The Validate method is called to  */
	/* decide whether any more reads are necessary                    */
	/******************************************************************/

	BytesToRead = m_nInitialReadLength;
	while ((nStatus = ReadMsgPort (&m_stCommsBuffer.cMessage[TotalRead],
									BytesToRead,
									&BytesRead)) == TIMEOUT_CONDITION);
	if (nStatus == OK)
	{
		TotalRead += BytesRead;
		BytesToRead -= BytesRead;
	}

	/* Read the header, now for the data */
	m_stCommsBuffer.nMessageLen = TotalRead;
	while (nStatus == OK &&!ValidateMessageFromLength())
	{
		BytesToRead = sizeof (m_stCommsBuffer.cMessage) - TotalRead;

		nStatus = ReadMsgPort (	&m_stCommsBuffer.cMessage[TotalRead],
								BytesToRead,
								&BytesRead);
		if (nStatus == OK)
		{
			TotalRead += BytesRead;
			m_stCommsBuffer.nMessageLen = TotalRead;
		}
	}
	if (nStatus == OK)
	{
		m_stCommsBuffer.nMessageLen -= m_nMsgHdrInLen;
		m_stCommsBuffer.nMessageLen -= m_nMsgTrlInLen;

		// Copy the data part into the data message
		memcpy (m_stDataBuffer.cMessage,
				&m_stCommsBuffer.cMessage[m_nMsgHdrInLen],
				m_stCommsBuffer.nMessageLen);
		m_stDataBuffer.nMessageLen =  m_stCommsBuffer.nMessageLen;
	}

	if (nStatus == POLL_ERROR)
	{
		/* There is an error reading the port */
		CloseSocket (&m_pMsgSocket);
		nStatus = LINEDOWN_CONDITION;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::Write
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
STATUS TCPIPPort::Write()
{
	STATUS	nStatus = OK;
	int		BytesSent = 0, BytesToTransmit = 0, TotalSent = 0;
	WORD	MessageHdr;


	if (!m_pMsgSocket)
	{
		nStatus = WaitForConnection ();
	}

	if (nStatus == OK)
	{
		// Copy the data part into the data message
		memcpy (&m_stCommsBuffer.cMessage[m_nMsgHdrOutLen],
				m_stDataBuffer.cMessage,
				m_stDataBuffer.nMessageLen);
		m_stCommsBuffer.nMessageLen =  m_stDataBuffer.nMessageLen;

		MessageHdr = htons (m_stCommsBuffer.nMessageLen
							+ m_nMsgHdrOutLen
							+ m_nMsgTrlOutLen);
		memcpy (m_stCommsBuffer.cMessage, (char*)&MessageHdr, m_nMsgHdrOutLen);
		if (m_nMsgTrlOutLen)
		{
			memcpy (&m_stCommsBuffer.cMessage[m_stCommsBuffer.nMessageLen
												+ m_nMsgHdrOutLen],
					&m_cMsgTrlOut, m_nMsgTrlOutLen);
		}
		BytesToTransmit =	m_stCommsBuffer.nMessageLen
							+ m_nMsgHdrOutLen
							+ m_nMsgTrlOutLen;

		while (BytesToTransmit && (BytesSent = send (	*m_pMsgSocket,
									&m_stCommsBuffer.cMessage[TotalSent],
									BytesToTransmit, 0)) != BytesToTransmit)
		{
			if (BytesSent == -1
				&& errno != EWOULDBLOCK
				&& errno != EAGAIN
				&& errno != ENOBUFS)
			{
				/* Error Sending the data */
				nStatus = ConvertSystemError ();
				break;
			}
			else
			{
				TotalSent += BytesSent;
				BytesToTransmit -= BytesSent;
			}
		}
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::CloseSocket
 *
 *           Synopsis:  Close socket
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
STATUS TCPIPPort::CloseSocket(int **ppSocket)
{
	STATUS nStatus = OK;

	if (*ppSocket)
	{
		if (**ppSocket != -1)
			(void) close (**ppSocket);
		delete *ppSocket;
		*ppSocket = 0;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::Close
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
STATUS TCPIPPort::Close()
{
	STATUS nStatus = OK;

	if (m_pMsgSocket)
	{
		CloseSocket (&m_pMsgSocket);
	}
	if (m_pMainSocket && m_bInitFlag)
	{
		CloseSocket (&m_pMainSocket);
	}

	m_bInitFlag = FALSE;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::WaitForConnection
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
STATUS TCPIPPort::WaitForConnection()
{
	STATUS	nStatus = OK;
	int		nReturn = 0;
	int		nLength;

	if (m_bListen)
	{
		/************************************/
		/*        SERVER PROCESSING         */
		/************************************/

		/* Wait for input on the main socket */
		if (nStatus == OK)
		{
			if ((nReturn = listen (*m_pMainSocket, 8)) == -1)
			{
				nStatus = TCPIP_LISTEN_ERROR;
				cerr << "Listen error - " << errno << "\n";
				cerr.flush();
				// Close everything
				Close ();
			}
			else
			{
				if (TRACE)
				{
					cout << "Listen returned: " << nReturn << "\n";
					cout.flush();
				}

				nLength = sizeof (m_stClntAddr);

				/* Close the socket and allocate a new socket */
				CloseSocket (&m_pMsgSocket);
				m_pMsgSocket = new int;
				*m_pMsgSocket = -1;
				if ((*m_pMsgSocket = accept (	*m_pMainSocket,
												(struct sockaddr*)&m_stClntAddr,
												&nLength))
					== -1)
				{
					nStatus = ConvertSystemError ();
					// Override the system error
					cerr << "Accept error - " << errno << "\n";
					cerr.flush();
					CloseSocket (&m_pMsgSocket);
				}
			}
		}
	}
	else
	{
		/************************************/
		/*        CLIENT PROCESSING         */
		/************************************/

		while( connect(*m_pMainSocket,
						(struct sockaddr *) &m_stServAddr, 
						sizeof(m_stServAddr)) == -1) 
		{
				m_bInitFlag = 0;
				CloseSocket (&m_pMainSocket); /* Ensure this socket is closed */
				Open (); /* re-open it */
				Delay (m_RetryDelay); /* Wait a while before retrying */
		}

		CloseSocket (&m_pMsgSocket);
		m_pMsgSocket = new int;
		*m_pMsgSocket = *m_pMainSocket;

		if ((nStatus = SetSocketOptions()) != OK)
		{
			Close();
		}
    }

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::SetSocketOptions
 *
 *           Synopsis:  Sets various flags for socket
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
STATUS TCPIPPort::SetSocketOptions()
{
	STATUS	nStatus = OK;
	int		opt;
	struct linger	linger_options = {1,5};
	int		nReturn;

	opt = 1;
	if (setsockopt (	*m_pMainSocket,
						IPPROTO_TCP,
						TCP_NODELAY,
						(char*)&opt,
						sizeof(opt))
		== -1)
	{
		nStatus = ConvertSystemError ();
		return nStatus;
	}

	if (setsockopt (	*m_pMainSocket,
						SOL_SOCKET,
						SO_KEEPALIVE,
						(char*)&opt,
						sizeof(opt))
		== -1)
	{
		nStatus = ConvertSystemError ();
		return nStatus;
	}

	if (setsockopt (	*m_pMainSocket,
						SOL_SOCKET,
						SO_REUSEADDR,
						(char*)&opt,
						sizeof(opt))
		== -1)
	{
		nStatus = ConvertSystemError ();
		return nStatus;
	}

	if (setsockopt (	*m_pMainSocket,
						SOL_SOCKET,
						SO_LINGER,
						(char*)&linger_options,
						sizeof(linger_options))
		== -1)
	{
		nStatus = ConvertSystemError ();
		return nStatus;
	}

#ifdef NONBLOCKING
	if (!m_bListen)
	{
		if ((nReturn = ::fcntl (*m_pMainSocket, F_SETFL, O_NONBLOCK)) == -1)
		{
			nStatus = ConvertSystemError();
			return nStatus;
		}
	}
#endif /* NONBLOCKING */

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  TCPIPPort::SetAddrInfo
 *
 *           Synopsis:  Sets up socket address information
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
STATUS TCPIPPort::SetAddrInfo()
{
	STATUS nStatus = OK;
	char	cBuffer[200];
	int		nReturn;
	int		nErrno;

	if (gethostbyname_r (m_strHostName.data(), &m_stHostEntry,
						cBuffer,
						sizeof (cBuffer),
						&nErrno) == (HOSTENTRY *) NULL)

	{
		nStatus = GETHOSTENT_ERROR;
		return nStatus;
	}

	m_stServAddr.sin_family = AF_INET;
	m_stServAddr.sin_port = htons (m_Port);
	m_stServAddr.sin_addr.s_addr =
					((struct in_addr *)(m_stHostEntry.h_addr))->s_addr;
	if (m_bListen)
	{
		/************************************/
		/*        SERVER PROCESSING         */
		/************************************/

		/* Bind the socket */
		if ((nReturn = bind (	*m_pMainSocket,
								(struct sockaddr*)&m_stServAddr,
								sizeof (SOCKADDR))) == -1)
		{
			nStatus = BIND_ERROR;
			cerr << "Error: errno=" << errno << "\n" ;
			return nStatus;			
		}
	}
	else
	{
		/************************************/
		/*        CLIENT PROCESSING         */
		/************************************/
	}

	return nStatus;
}
