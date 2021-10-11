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
#include "LGport.h"
#include "LGglobls.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Port::Port
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
Port::Port()
{
	// Initialise variables
	m_bInitFlag = FALSE;
	m_nMsgHdrInLen = 0;
	m_nMsgTrlInLen = 0;
	m_nMsgHdrOutLen = 0;
	m_nMsgTrlOutLen = 0;
	m_nType = 0;

	memset(m_stDataBuffer.cMessage, '\0', sizeof(m_stDataBuffer.cMessage));
	memset(m_stCommsBuffer.cMessage, '\0', sizeof(m_stCommsBuffer.cMessage));
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Port::~Port
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
Port::~Port()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Port::ReferenceMethods
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
DataMsg & Port::DataBuffer()
{
	return m_stDataBuffer;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Port::DumpDataBuffer
 *
 *           Synopsis:  Dumps raw data message
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	None
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void Port::DumpDataBuffer()
{
	WORD lIndex = 0;
	WORD lPos = 0;

	while(lIndex < m_stDataBuffer.nMessageLen)
	{
		// print out hex offset of bytes to be displayed
		fprintf(stderr, " %04x-%04x  ", lPos, lPos + 15);

		for(lIndex = lPos; lIndex < (lPos + 16); lIndex++)
		{
			// print out 16 bytes of data in hex with spacing
			if (lIndex % 4 == 0 && lIndex != lPos)
				fprintf(stderr, " ");

			if (!(lIndex < m_stDataBuffer.nMessageLen))
				fprintf(stderr, "  ");
			else
				fprintf(stderr,
						"%02x",
						(unsigned char)m_stDataBuffer.cMessage[lIndex]);
		}

		fprintf(stderr, "     | ");

		for(lIndex = lPos; lIndex < (lPos + 16); lIndex++)
		{
			// print out 16 bytes of ascii (non-printable = '.')
			if (!(lIndex < m_stDataBuffer.nMessageLen))
				fprintf(stderr, " ");
			else
				if ((m_stDataBuffer.cMessage[lIndex] < ' ') ||
					(m_stDataBuffer.cMessage[lIndex] > '~'))
					fprintf(stderr, "."); // non-printable
				else
					fprintf(stderr, "%c", m_stDataBuffer.cMessage[lIndex]);
		}

		fprintf(stderr, " |\n");

		lPos += 16;
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Port::WaitForInput
 *
 *           Synopsis:  Read data
 *
 *   Input parameters:  fd (File Descriptor)
 *						timeout (in milliseconds)
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
STATUS Port::WaitForInput (int fd, WORD timeout)
{
STATUS			nStatus = OK;
pollfd_t		stFds;
int				nFds = 1;
int				nReturn;

	stFds.fd		= fd;
	stFds.events	= POLLIN;

	nReturn = poll (&stFds, nFds, timeout);
	switch (nReturn)
	{
	case 0:					// Timeout condition on file descriptor
		if (TRACE)
		{
			cout << "Timeout condition reached on file descriptor\n";
			cout.flush ();
		}
		nStatus = TIMEOUT_CONDITION;
		break;
	case -1:				// Error Condition
		nStatus = ConvertSystemError ();
		break;
	default:				// Data received on the file descriptor
		if (nReturn != nFds)
		{
			nStatus = INVALID_POLLFD_ERROR;
		}
		else
		{
			switch (stFds.revents)
			{
			case POLLIN:		// Valid Data exists
			case POLLPRI:
			case POLLRDNORM:
				break;
			case POLLERR:		// Error conditions on file descriptor
			case POLLHUP:
				nStatus = POLL_ERROR;
				cout << "Poll Error:" << errno << "\n";
				break;
			}
		}
		break;
	}
	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Port::Delay
 *
 *           Synopsis:  Function called to delay the port by a number of
 *                      speficied milliseconds
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  pointer
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS Port::Delay (WORD timeout)
{
STATUS          nStatus = OK;
int             nReturn;
pollfd_t *      pFds = NULL;
 
	if (TRACE)
	{
    	cout << "Delaying for " << timeout << "msecs\n";
    	cout.flush();
	}
    nReturn = poll (NULL, 0, timeout);
    switch (nReturn)
    {
    case 0:                 // Timeout condition on file descriptor
		if (TRACE)
		{
        	cout << "Timeout condition reached on file descriptor\n";
        	cout.flush ();
		}
        nStatus = TIMEOUT_CONDITION;
        break;
    case -1:                // Error Condition
        nStatus = ConvertSystemError();
        break;
    default:                // Data received on the file descriptor
        break;
    }
    return nStatus;
}
 
/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Port::ConvertSystemError
 *
 *           Synopsis:  Converts system errors into STATUS type
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
STATUS Port::ConvertSystemError ()
{
	STATUS nStatus = OK;

	switch (errno)
	{
	default:
		cout << "Errno is: " << errno << "\n";
		nStatus = SYSTEM_ERROR;
		break;
	}

	return nStatus;
}
