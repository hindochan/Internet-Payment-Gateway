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

#include <rw/ctoken.h>

#include "LGglobls.h"
#include "LGx25prt.h"
#include "LGtypes.h"
#include "LGerrors.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stream.h>
#include <stropts.h>
#include <unistd.h>
#include <netx25/uint.h>
#include <netx25/timer.h>
#include <netdlc/ll_proto.h>
#include <netdlc/ll_control.h>
#include <netx25/xnetdb.h>
#include <netx25/x25_control.h>
#include <netx25/x32_control.h>
#include <netx25/gen_snmp.h>
#include <netx25/x25_mib.h>
#include <netx25/x25db.h>
#include <netx25/system.h>
#include <netx25/x25_proto.h>

extern "C" int  stox25(unsigned char *cp, struct xaddrf *xad, int lookup);

#define E1	OPEN_EVENT
#define E2	CLOSE_EVENT
#define E3	WAIT_FOR_CONNECTION_EVENT
#define E4	READ_EVENT
#define E5	WRITE_EVENT
#define E6	CONNECTION_CONF_RCVD_EVENT
#define E7	DISCONNECT_CONF_RCVD_EVENT
#define E8	EXPEDITED_DATA_RCVD_EVENT
#define E9	RESET_IND_EVENT
#define E10	NETWORK_DATA_EVENT
#define E11	DISCONNECT_INDICATION_RCVD_EVENT
#define E12	ABORT_RCVD_EVENT
#define E13	CLEAR_CONF_RCVD_EVENT
#define E14	NETWORK_ERROR_EVENT
#define E15	TIMEOUT_EVENT
#define E16	UNKNOWN_EVENT

#define S1	LINE_DOWN_STATE
#define S2	DEVICE_OPEN_STATE
#define S3	CONNECTION_REQUEST_SENT_STATE
#define S4	DATA_TRANSFER_STATE
#define S5	EXPEDITED_DATA_SENT_STATE
#define S6	RESET_RESPONSE_SENT_STATE
#define S7	DATA_REQUEST_SENT_STATE
#define S8	CLEAR_CONF_SENT_STATE
#define S9	CLEAR_REQUEST_SENT_STATE
#define S10	TIMEOUT_ON_READ_STATE

#define NOK	INVALID_EVENT
 
const X25EVENTFSM X25Port::m_stEventFSM =	// Index = Event/State
{
/*		{{S1},   {S2},   {S3},   {S4},   {S5+},  {S6*},  {S7},   {S8*},  {S9},   {S10*}, },*/

/* E1 */{{OK,0}, {NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E2 */{{NOK,0},{OK,0}, {NOK,0},{OK,0}, {NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E3 */{{OK,0}, {OK,0}, {OK,0}, {OK,0}, {NOK,0},{NOK,0},{OK,0}, {OK,0}, {OK,0}, {NOK,0},},
/* E4 */{{NOK,0},{OK,0}, {NOK,0},{OK,0}, {NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E5 */{{NOK,0},{OK,0}, {NOK,0},{OK,0}, {NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E6 */{{NOK,0},{NOK,0},{OK,0}, {NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E7 */{{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E8 */{{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E9 */{{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E10*/{{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E11*/{{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E12*/{{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E13*/{{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{OK,0}, {NOK,0},},
/* E14*/{{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E15*/{{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E16*/{{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},{NOK,0},},
/* E0 */{{0,0},  {0,0},  {0,0},  {0,0},  {0,0},  {0,0},  {0,0},  {0,0},  {0,0},  {0,0}}
};

void TraceEntry (char *cText)
{
	if (TRACE)
	{
		cout << cText;
		cout.flush();
	}
}

void TraceExit (char *cText)
{
	if (TRACE)
	{
		cout << cText;
		cout.flush();
	}
}

void TraceWrite (char *cText)
{
	if (TRACE)
	{
		cout << cText;
		cout.flush();
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name: X25Port::CommonConstructorInitialisation
 *
 *           Synopsis:  Performs the common constructor initialisation
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

void X25Port::CommonConstructorInitialisation ()
{
	m_bListen = FALSE;

	m_RecovWaitTime = oTheApplication.GetConfiguration().GetResponseTimeout();
	m_nMsgHdrInLen = 0;		// STX
	m_nMsgTrlInLen = 0;		// ETX
	m_nMsgHdrOutLen = 0;	// STX
	m_nMsgTrlOutLen = 0;	// ETX
    m_cMsgHdrIn[0] = 0x02;
    m_cMsgTrlIn[0] = 0x03;
    m_cMsgHdrOut[0] = 0x02;
    m_cMsgTrlOut[0] = 0x03;
	m_RetryDelay = 1000;

	m_nState = LINE_DOWN_STATE;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::X25Port
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
X25Port::X25Port()
{
	// Initialise variables
	CommonConstructorInitialisation();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::X25Port
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  CommType & config string
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description: Comms config format as below:
 *	#LAGMerchantInterface=BMS:2:30:8:0.1120032399189,CALL,SVC,,/dev/x25
 *
 *              Notes:
 *
 ****************************************************************************
 */
X25Port::X25Port(WORD nType, RWCString strCommsConfig)
{
RWCTokenizer	tknStr(m_strCommsConfig);
RWCString		strValue;
char			buffer[128];

	CommonConstructorInitialisation();

	// Initialise variables
	m_nType = nType;
	m_strCommsConfig = strCommsConfig;

	sprintf (buffer, "X25Port::Config: %s\n", strCommsConfig.data());
	TraceWrite (buffer);

	/* Get remote host name */
	m_strHostName = tknStr (",");

	/* Get connection mode */
	strValue = tknStr(",");
	if (strValue == "CALL")
		m_bListen = FALSE;
	else
		m_bListen = TRUE;

	/* Get SVC type */
	strValue = tknStr(",");
	if (strValue == "SVC")
		m_bSVC = TRUE;
	else
		m_bSVC = TRUE;

	/* Set up m_cCud */
	//strncpy (m_cCud , m_strHostName.data(), MAX_CUD_LEN);
	//m_nCudLen = strlen (m_cCud);
	m_nCudLen = 0;

	/* Get device_name */
	m_strDeviceName = tknStr (",");

	/* Convert NUA into an X.25 Address */
	if (stox25 ((unsigned char*)"0.234222200308", &m_stLocalHost, 0) == -1)
	{
		sprintf (buffer, "X.25 Local Address Error\n");
		TraceWrite (buffer);
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::~X25Port
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
X25Port::~X25Port()
{

}

#ifdef NOPOLL
/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::Delay
 *
 *           Synopsis:  Function called to delay the port by a number of
 *						speficied milliseconds
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
STATUS X25Port::Delay (WORD timeout)
{
STATUS			nStatus = OK;
int				nReturn;
pollfd_t *		pFds = NULL;
char			buffer[128];

	sprintf (buffer, "Delaying for %d msecs\n", timeout);
	TraceWrite (buffer);

	nReturn = poll (NULL, 0, timeout);
	switch (nReturn)
	{
	case 0:					// Timeout condition on file descriptor
		TraceWrite ("Timeout condition reached on file descriptor\n");
		nStatus = TIMEOUT_CONDITION;
		break;
	case -1:				// Error Condition
		nStatus = ConvertSystemError();
		break;
	default:				// Data received on the file descriptor
		break;
	}
	return nStatus;
}
#endif

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::PreThreadInit
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
void * X25Port::PreThreadInit()
{
	TraceEntry ("X25Port::PreThreadInit: ENTRY\n");

	TraceExit ("X25Port::PreThreadInit: EXIT\n");
	return NULL;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::Init
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
STATUS X25Port::Init(void *pVoid)
{
STATUS	nStatus = OK;
char	buffer[128];

	TraceEntry ("X25Port::Init: ENTRY\n");

	sprintf (buffer, "X25Port::My config string is :- %s\n",
			m_strCommsConfig.data());
	TraceWrite (buffer);

	TraceExit ("X25Port::Init: EXIT\n");
	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::Open
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
STATUS X25Port::Open()
{
STATUS	nStatus = OK;
int		nReturn;
char	buffer[128];

	TraceEntry ("X25Port::Open: ENTRY\n");

	m_nEvent = OPEN_EVENT;
	if ((nStatus = ValidateEvent()) != OK)
	{
		nStatus = ProcessInvalidEvent ();
		//return nStatus;
	}

	/* Convert NUA into an X.25 Address */
	sprintf (buffer, "X25Port::strHostName: %s\n", m_strHostName.data());
	TraceWrite (buffer);

	if (stox25 ((unsigned char*)m_strHostName.data(), &m_stRemotePeer, 0) == -1)
	{
		sprintf (buffer, "X25Port::Errno: %d\n", errno);
		TraceWrite (buffer);
		nStatus = X25_ADDRESS_ERROR;
		return nStatus;
	}

	if ((nReturn = ::open (m_strDeviceName.data(), O_RDWR)) == -1)
	{
		nStatus = ConvertSystemError ();
		m_nX25fd = -1;
		return nStatus;
	}
	else
	{
		m_nState = DEVICE_OPEN_STATE;
		m_nX25fd = nReturn;
		if ((nReturn = ::fcntl (m_nX25fd, F_SETFD, O_NONBLOCK)) == -1)
		{
			Close();
			nStatus = ConvertSystemError();
		}
	}

	if (nStatus == OK)
	{
		nStatus = WaitForConnection ();
	}

	TraceExit ("X25Port::Open: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::Read
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
STATUS X25Port::Read()
{
	STATUS		nStatus = OK;

	TraceEntry ("X25Port::Read: ENTRY\n");

	m_nEvent = READ_EVENT;
	if ((nStatus = ValidateEvent()) != OK)
	{
		nStatus = ProcessInvalidEvent ();
		//return nStatus;
	}

	if ((nStatus = WaitForConnection()) == OK)
	{
		// Reset the event as the WaitForConnection will have set it
		m_nEvent = READ_EVENT;
		/*
		 * Wait for a response.  Normally we expect a Data packet.
		 */
		m_nEvent = GetNetworkEvent (m_RecovWaitTime);
		switch (m_nEvent)
		{
		case TIMEOUT_EVENT:
			nStatus = TIMEOUT_CONDITION;
			//m_nState = TIMEOUT_ON_READ_STATE;
			break;
		case NETWORK_DATA_EVENT:
			nStatus = OK;
			// Copy the data part into the data message
			m_stDataBuffer.nMessageLen =  m_stCommsBuffer.nMessageLen -
										m_nMsgHdrInLen - m_nMsgTrlInLen;
			memcpy (m_stDataBuffer.cMessage,
					&m_stCommsBuffer.cMessage[m_nMsgHdrInLen],
					m_stCommsBuffer.nMessageLen
					- m_nMsgHdrInLen - m_nMsgTrlInLen);
			break;
		default:
			/* Something unusual happened, deal with it */
			nStatus = ProcessInvalidEvent();
			if (m_nState == LINE_DOWN_STATE)
			{
				nStatus = X25_LINE_DOWN;
			}
			break;
		}
	}

	TraceExit ("X25Port::Read: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::Write
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
STATUS X25Port::Write()
{
	STATUS nStatus = OK;

	TraceEntry ("X25Port::Write: ENTRY\n");

	m_nEvent = WRITE_EVENT;
	if ((nStatus = ValidateEvent()) != OK)
	{
		nStatus = ProcessInvalidEvent ();
		//return nStatus;
	}

	m_nEvent = WRITE_EVENT;
	if ((nStatus = WaitForConnection()) == OK)
	{
		while ((m_nEvent=GetNetworkEvent(DEFAULT_WAIT_TIMEOUT))
				!= TIMEOUT_EVENT)
		{
			nStatus = ProcessInvalidEvent();
			if (nStatus == X25_LINE_DOWN)
			{
				nStatus = LINEDOWN_CONDITION;
				break;
			}
		}

		if (nStatus == OK)
		{
			// Reset the event as the WaitForConnection will have set it
			m_nEvent = WRITE_EVENT;

			if (m_nMsgHdrOutLen)
			{
				memcpy (m_stCommsBuffer.cMessage,
						m_cMsgHdrOut,
						m_nMsgHdrOutLen);
			}
			// Copy the data part into the data message
			memcpy (&m_stCommsBuffer.cMessage[m_nMsgHdrOutLen],
					m_stDataBuffer.cMessage,
					m_stDataBuffer.nMessageLen);
			m_stCommsBuffer.nMessageLen =  m_stDataBuffer.nMessageLen;

			if (m_nMsgTrlOutLen)
			{
				memcpy (&m_stCommsBuffer.cMessage[
						m_nMsgHdrOutLen + m_stDataBuffer.nMessageLen],
						m_cMsgTrlOut,
						m_nMsgTrlOutLen);
			}

			nStatus = SendData();
			if (nStatus == OK)
			{
				// No Ack'ing needed as no D-bit support, so don't wait for it
				m_nState = DATA_TRANSFER_STATE;
			}
		}
	}

	TraceExit ("X25Port::Write: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::Close
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
STATUS X25Port::Close()
{
	STATUS nStatus = OK;

	TraceEntry ("X25Port::Close: ENTRY\n");

	m_nEvent = CLOSE_EVENT;
 
	// Need to validate the event here
	if ((nStatus = ValidateEvent()) != OK)
	{
		nStatus = ProcessInvalidEvent ();
		//return nStatus;
	}

	if (m_nState != LINE_DOWN_STATE)
	{
		nStatus = SendClearRequest();
		if (nStatus == OK)
		{
			m_nState = CLEAR_REQUEST_SENT_STATE;
			/*
			 * Wait for a response.  Normally we expect a Clear Confirm packet.
			 */
			m_nEvent = GetNetworkEvent (DEFAULT_WAIT_TIMEOUT);
			if (m_nEvent != CLEAR_CONF_RCVD_EVENT)
			{
				/* Something unusual happened, deal with it */
				/* Unwind the driver until a stable state is reached */
				nStatus = ProcessInvalidEvent();
			}
			else
			{
				m_nState = LINE_DOWN_STATE;
			}
		}
	}

	::close (m_nX25fd);
	m_nX25fd = 0;

	TraceExit ("X25Port::Close: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::WaitForConnection
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
STATUS X25Port::WaitForConnection()
{
	STATUS nStatus = OK;

	TraceEntry ("X25Port::WaitForConnection: ENTRY\n");

	m_nEvent = WAIT_FOR_CONNECTION_EVENT;
	if ((nStatus = ValidateEvent()) != OK)
	{
		nStatus = ProcessInvalidEvent ();
		//return nStatus;
	}


	if (m_nState != DEVICE_OPEN_STATE && m_nState != DATA_TRANSFER_STATE)
	{
		nStatus = Open();
	}

	while (nStatus == OK && m_nState != DATA_TRANSFER_STATE)
	{
		if (nStatus == OK && m_nState != CONNECTION_REQUEST_SENT_STATE)
		{
			nStatus = SendConnectionRequest ();
			if (nStatus == OK)
			{
				m_nState = CONNECTION_REQUEST_SENT_STATE;
			}
		}
		/*
		 * Wait for a response. Expect a Connection Conf packet.
		 */
		if (nStatus == OK)
		{
			m_nEvent = GetNetworkEvent (DEFAULT_WAIT_TIMEOUT);
			if (m_nEvent != CONNECTION_CONF_RCVD_EVENT)
			{
				/* Something unusual happened, deal with it */
				/* Unwind the driver until a stable state is reached */
				nStatus = ProcessInvalidEvent();
				/* Wait a while before retrying the connection */
				Delay (m_RetryDelay);
				m_nState = DEVICE_OPEN_STATE;
			}
			else
			{
				m_nState = DATA_TRANSFER_STATE;
			}
		}
	}

	TraceExit ("X25Port::WaitForConnection: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::GetNetworkEvent
 *
 *           Synopsis:  Wait on the port for an event from the X.25 network
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
X25EVENT X25Port::GetNetworkEvent(WORD nTimeout)
{
	STATUS			nStatus = OK;
	X25EVENT		nEvent = UNKNOWN_EVENT;
    struct strbuf   stDataBuf;
    struct strbuf   stCtlBuf;
    char            cCtlBlk[1024];
	int				nFlags = 0;
	S_X25_HDR		*pX25CtlMsg;
	int				nReturn;
	char			buffer[128];

	TraceEntry ("X25Port::GetNetworkEvent: ENTRY\n");

    stCtlBuf.len = 0;
    stCtlBuf.maxlen = 1024;
    stCtlBuf.buf = (char *) cCtlBlk;
 
    stDataBuf.len = 0;
    stDataBuf.maxlen = sizeof (m_stCommsBuffer.cMessage);
    stDataBuf.buf = m_stCommsBuffer.cMessage;
 
	sprintf (buffer, "X25Port::m_nEvent: %d\n", m_nEvent);
	TraceWrite (buffer);
	sprintf (buffer, "X25Port::m_nState: %d\n", m_nState);
	TraceWrite (buffer);

    /*
     * Wait for a response.
     */
	if ((nStatus = WaitForInput (m_nX25fd, nTimeout)) == OK)
	{
		while ((nReturn = getmsg(m_nX25fd, &stCtlBuf, &stDataBuf, &nFlags))
				== (MORECTL | MOREDATA));
		if (nReturn < 0)
		{
			nEvent = NETWORK_ERROR_EVENT;
			return (nEvent);
		}

		pX25CtlMsg = (S_X25_HDR *) stCtlBuf.buf;
		// Store it in case it is needed.
		memcpy (&m_uX25CtlMsg, pX25CtlMsg, sizeof (m_uX25CtlMsg));
	 
		if (pX25CtlMsg->xl_type == XL_DAT)
		{
			m_stCommsBuffer.nMessageLen = stDataBuf.len;
			sprintf (buffer, "X25Port::DATA event: ");
			TraceWrite (buffer);
			switch (pX25CtlMsg->xl_command)
			{
			case N_Data:                 /* Data           */
				nEvent = NETWORK_DATA_EVENT;
				sprintf (buffer, "N_Data event\n");
				TraceWrite (buffer);
				m_stCommsBuffer.nMessageLen = stDataBuf.len;
				break;
			case N_EData:                /* Expedited data */
				nEvent = EXPEDITED_DATA_RCVD_EVENT;
				sprintf (buffer, "N_EData event\n");
				TraceWrite (buffer);
				break;
			// case N_EAck:              /* expedited ack  */
			default:
				nEvent = UNKNOWN_EVENT;
				sprintf (buffer, "Unknown event\n");
				TraceWrite (buffer);
				break;
			}
		}
		else  if (pX25CtlMsg->xl_type == XL_CTL)
		{
			sprintf (buffer, "X25Port::CONTROL event: ");
			TraceWrite (buffer);
			switch (pX25CtlMsg->xl_command)
			{
			//case N_CI:			/* Connect */
			case N_CC:				/* Connect conf */
			{
			struct xccnff   *pCallConfirm = (struct xccnff*) pX25CtlMsg;

    			sprintf (buffer,
						"N_CC: xl_command: %d\n",
						pCallConfirm->xl_command);
				TraceWrite (buffer);
    			sprintf (buffer,
						"N_CC: xl_type: %d\n",
						pCallConfirm->xl_type);
				TraceWrite (buffer);
    			sprintf (buffer,
						"N_CC: xl_conn_id: %d\n",
						pCallConfirm->conn_id);
				TraceWrite (buffer);
    			sprintf (buffer,
						"N_CC: xl_CONS_call: %d\n",
						pCallConfirm->CONS_call);
				TraceWrite (buffer);
    			sprintf (buffer,
						"N_CC: xl_negotiate_qos: %d\n",
						pCallConfirm->negotiate_qos);
				TraceWrite (buffer);
    			sprintf (buffer,
						"N_CC: xl_responder: 0x%x\n",
						pCallConfirm->responder);
				TraceWrite (buffer);
 
				nEvent = CONNECTION_CONF_RCVD_EVENT;
				sprintf (buffer, "N_CC event\n");
				TraceWrite (buffer);
				break;
			}
			case N_DI:				/* Disconnect */
				nEvent = DISCONNECT_INDICATION_RCVD_EVENT;
				sprintf (buffer, "N_DI event\n");
				TraceWrite (buffer);
				break;
			case N_Abort:			/* Abort - DI but no resource */
				nEvent = ABORT_RCVD_EVENT;
				sprintf (buffer, "N_Abort event\n");
				TraceWrite (buffer);
				break;
			case N_RI:				/* Reset */
				nEvent = RESET_IND_EVENT;
				sprintf (buffer, "N_RI event\n");
				TraceWrite (buffer);
				break;
			case N_RC:				/* Reset conf  */
				nEvent = CLEAR_CONF_RCVD_EVENT;
				sprintf (buffer, "N_RC event\n");
				TraceWrite (buffer);
				break;
			case N_DC:				/* Disconnect Confirm */
				nEvent = DISCONNECT_CONF_RCVD_EVENT;
				sprintf (buffer, "N_DC event\n");
				TraceWrite (buffer);
				break;
			case N_DAck:			/* data ack */
				sprintf (buffer, "N_DAck event\n");
				TraceWrite (buffer);
				break;
			case N_DIsafe:			/* "Safe" disconnect (used by x25mod) */
			case N_DC_TMO:			/* did not recv DC in time */
			default:
				nEvent = UNKNOWN_EVENT;
				sprintf (buffer, "Unknown event: %d\n", pX25CtlMsg->xl_command);
				TraceWrite (buffer);
				break;
			}
		}
	}
	else if (nStatus == TIMEOUT_CONDITION)
	{
		nEvent = TIMEOUT_EVENT;
		sprintf (buffer, "Timeout event\n");
		TraceWrite (buffer);
	}

	TraceExit ("X25Port::GetNetworkEvent: EXIT\n");

	return nEvent;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::ConvertSystemError
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
STATUS X25Port::ConvertSystemError ()
{
STATUS	nStatus = OK;
char	buffer[128];

	TraceEntry ("X25Port::ConvertSystemError: ENTRY\n");

	sprintf (buffer, "Errno: %d\n", errno);
	TraceWrite (buffer);

	switch (errno)
	{
	default:
		nStatus = X25_COMMS_ERROR;
		break;
	}

	TraceExit ("X25Port::ConvertSystemError: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::SendConnectionRequest
 *
 *           Synopsis:  Sends a Connection request to the network
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
STATUS X25Port::SendConnectionRequest()
{
	STATUS			nStatus = OK;
    struct strbuf   stDataBuf;
    struct strbuf   stCtlBuf;
    char        	cDataBlk[1024];
    char            cCtlBlk[1024];
    int             nReturn;
    struct xcallf   *pCallRequest;
	int				nFlags;

	TraceEntry ("X25Port::SendConnectionRequest: ENTRY\n");

    pCallRequest =  new struct xcallf;
    memset((char *) pCallRequest, 0, sizeof(struct xcallf));
 
    memset((char *) cCtlBlk, 0, 1024);
    memset((char *) cDataBlk, 0, 1024);
 
    pCallRequest->xl_command = N_CI;
    pCallRequest->xl_type = XL_CTL;
    pCallRequest->conn_id = 0;
    pCallRequest->CONS_call = 0;
    pCallRequest->negotiate_qos = 0;
    pCallRequest->calledaddr = m_stRemotePeer;
    pCallRequest->callingaddr = m_stLocalHost;
    memset (&pCallRequest->qos, 0, sizeof (pCallRequest->qos)) ;
 
    /* All other fields of N_CI are left = 0. */
 
    stCtlBuf.len = sizeof(struct xcallf);
    stCtlBuf.buf = (char *) pCallRequest;
 
	/* Set up the Call User Data: the Comms config will do */
    stDataBuf.len = strlen (m_cCud);
    stDataBuf.buf = m_cCud;
 
    /*
     * Send a Call Request packet.
     */
    if ((nReturn = putmsg(m_nX25fd, &stCtlBuf, &stDataBuf, 0)) < 0)
	{
		nStatus = ConvertSystemError ();
		return nStatus;
    }
	Delay (1000);

	TraceExit ("X25Port::SendConnectionRequest: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::SendClearRequest
 *
 *           Synopsis:  Sends a Clear Request to the network
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
STATUS X25Port::SendClearRequest()
{
	STATUS			nStatus = OK;
    struct strbuf   stDataBuf;
    struct strbuf   stCtlBuf;
    char        	cDataBlk[1024];
    char            cCtlBlk[1024];
    int             nReturn;
    struct xdiscf   *pDisc;
    struct xdcnff   *pDiscConf;
	int				nFlags;

	TraceEntry ("X25Port::SendClearRequest: ENTRY\n");

    pDisc = (struct xdiscf *) new struct xdiscf;
    memset((char *) pDisc, 0, sizeof(struct xdiscf));
 
    memset((char *) cCtlBlk, 0, 1024);
    memset((char *) cDataBlk, 0, 1024);
 
    pDisc->xl_command = N_DI;
    pDisc->xl_type = XL_CTL;
 
    /* All other fields of N_DI are left = 0. */
 
    stCtlBuf.len = sizeof(struct xdiscf);
    stCtlBuf.buf = (char *) pDisc;
 
    /*
     * Send a Clear Request packet.
     */
    if ((nReturn = putmsg(m_nX25fd, &stCtlBuf, 0, 0)) < 0)
	{
		nStatus = ConvertSystemError ();
		return nStatus;
    }

	TraceExit ("X25Port::SendClearRequest: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::SendData
 *
 *           Synopsis:  Sends a Data to the network
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
STATUS X25Port::SendData()
{
	STATUS			nStatus = OK;
    struct strbuf   stDataBuf;
    struct strbuf   stCtlBuf;
    char            cCtlBlk[1024];
    int             nReturn;
    struct xdataf   *pData;
	int				nFlags;

	TraceEntry ("X25Port::SendData: ENTRY\n");

    pData = (struct xdataf *) new struct xdataf;
    memset((char *) pData, 0, sizeof(struct xdataf));
 
    memset((char *) cCtlBlk, 0, 1024);

    stDataBuf.maxlen = sizeof ((char *) m_stCommsBuffer.cMessage);
    stDataBuf.len = m_stCommsBuffer.nMessageLen
					+ m_nMsgHdrOutLen
					+ m_nMsgTrlOutLen;
    stDataBuf.buf = (char *) m_stCommsBuffer.cMessage;
 
    pData->xl_command = N_Data;
    pData->xl_type = XL_DAT;
    pData->More = 0;	// No More Bit support
    pData->setQbit = 0;	// No Q Bit support
    pData->setDbit = 0;	// No D Bit support
 
    stCtlBuf.len = sizeof(struct xdataf);
    stCtlBuf.buf = (char *) pData;
    stCtlBuf.maxlen = sizeof(struct xdataf);
 
    /*
     * Send the Data packet packet.
     */
    if ((nReturn = putmsg(m_nX25fd, &stCtlBuf, &stDataBuf, 0)) < 0)
	{
		nStatus = ConvertSystemError ();
		return nStatus;
	}

	TraceExit ("X25Port::SendData: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::SendExpeditedDataAck
 *
 *           Synopsis:  Sends an Ack for Expedited data message received
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes: SENDING EXPEDITED DATA NOT REQUIRED FOR NOW.
 *
 ****************************************************************************
 */
STATUS X25Port::SendExpeditedDataAck()
{
	STATUS			nStatus = OK;
    struct strbuf   stDataBuf;
    struct strbuf   stCtlBuf;
    char            cCtlBlk[1024];
    int             nReturn;
    struct xdataf   *pData;
	int				nFlags;

	TraceEntry ("X25Port::SendExpeditedDataAck: ENTRY\n");

    pData = (struct xdataf *) new struct xdataf;
    memset((char *) pData, 0, sizeof(struct xdataf));
 
    memset((char *) cCtlBlk, 0, 1024);

    stDataBuf.len = sizeof(struct xdataf);
	// What is the data to send in Expedited data??
    stDataBuf.buf = (char *) m_stCommsBuffer.cMessage;
 
    pData->xl_command = N_EAck;
    pData->xl_type = XL_DAT;
 
    stCtlBuf.len = sizeof(struct xdataf);
    stCtlBuf.buf = (char *) pData;
 
    /*
     * Send the Data packet packet.
     */
    if ((nReturn = putmsg(m_nX25fd, &stCtlBuf, &stDataBuf, 0)) < 0)
	{
		nStatus = ConvertSystemError ();
		return nStatus;
    }

	TraceExit ("X25Port::SendExpeditedDataAck: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::SendClearConfirmation
 *
 *           Synopsis:  Sends a Clear confirmation to the network
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
STATUS X25Port::SendClearConfirmation()
{
	STATUS			nStatus = OK;
    struct strbuf   stCtlBuf;
    char            cCtlBlk[1024];
    int             nReturn;
    struct xdcnff  *pClearConf;
	int				nFlags;

	TraceEntry ("X25Port::SendClearConfirmation: ENTRY\n");

    pClearConf = (struct xdcnff*) new struct xdcnff;
    memset((char *) pClearConf, 0, sizeof(struct xdcnff));
 
    memset((char *) cCtlBlk, 0, 1024);
 
    pClearConf->xl_command = N_DC;
    pClearConf->xl_type = XL_CTL;
 
    stCtlBuf.len = sizeof(struct xdcnff);
    stCtlBuf.buf = (char *) pClearConf;
 
    /*
     * Send a Clear Confirmation packet.
     */
    if ((nReturn = putmsg(m_nX25fd, &stCtlBuf, 0, 0)) < 0)
	{
		nStatus = ConvertSystemError ();
		return nStatus;
    }

	TraceExit ("X25Port::SendClearConfirmation: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::SendResetConfirmation
 *
 *           Synopsis:  Sends a Reset confirmation to the network
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
STATUS X25Port::SendResetConfirmation()
{
	STATUS			nStatus = OK;
    struct strbuf   stCtlBuf;
    char            cCtlBlk[1024];
    int             nReturn;
    struct xrscf   *pResetConf;
	int				nFlags;

	TraceEntry ("X25Port::SendResetConfirmation: ENTRY\n");

    pResetConf = (struct xrscf *) new struct xrscf;
    memset((char *) pResetConf, 0, sizeof(struct xrscf));
 
    memset((char *) cCtlBlk, 0, 1024);
 
    pResetConf->xl_command = N_RC;
    pResetConf->xl_type = XL_CTL;
 
    stCtlBuf.len = sizeof(struct xrscf);
    stCtlBuf.buf = (char *) pResetConf;
 
    /*
     * Send a Clear Request packet.
     */
    if ((nReturn = putmsg(m_nX25fd, &stCtlBuf, 0, 0)) < 0)
	{
		nStatus = ConvertSystemError ();
		return nStatus;
    }

	TraceExit ("X25Port::SendResetConfirmation: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::ProcessConnectionConfirmation
 *
 *           Synopsis:  Processes an incoming Connection confirmation event
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
//STATUS X25Port::ProcessConnectionConfirmation()
//{
	//STATUS	nStatus = OK;
//
	//return nStatus;
//}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::ValidateEvent
 *
 *           Synopsis:  Sends a Reset confirmation to the network
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
STATUS X25Port::ValidateEvent()
{
STATUS	nStatus = OK;
char	buffer[128];

	TraceEntry ("X25Port::ValidateEvent: ENTRY\n");

	sprintf (buffer, "X25Port::m_nEvent: %d\n", m_nEvent);
	TraceWrite (buffer);
	sprintf (buffer, "X25Port::m_nState: %d\n", m_nState);
	TraceWrite (buffer);

	nStatus = m_stEventFSM [m_nEvent][m_nState].nStatus;

	TraceExit ("X25Port::ValidateEvent: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::ProcessInternalEvent
 *
 *           Synopsis:  Processes an unexpected internal event
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
STATUS X25Port::ProcessInvalidInternalEvent()
{
	STATUS	nStatus = OK;

	TraceEntry ("X25Port::ProcessInvalidInternalEvent: ENTRY\n");

	switch (m_nEvent)
	{
	case OPEN_EVENT:
	case CLOSE_EVENT:
	case WAIT_FOR_CONNECTION_EVENT:
	case READ_EVENT:
	case WRITE_EVENT:
		/* Return OK assume the application calling sequence is correct */
		break;
	}

	TraceExit ("X25Port::ProcessInvalidInternalEvent: EXIT\n");

	return nStatus;
}
/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::ProcessUnexpectedOpenEvent
 *
 *           Synopsis:  Processes an unexpected open event
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
STATUS X25Port::ProcessUnexpectedOpenEvent()
{
	STATUS	nStatus = OK;

	TraceEntry ("X25Port::ProcessUnexpectedOpenEvent: ENTRY\n");

	TraceExit ("X25Port::ProcessUnexpectedOpenEvent: EXIT\n");

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::ProcessUnexpectedCloseEvent
 *
 *           Synopsis:  Processes an unexpected close event
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
STATUS X25Port::ProcessUnexpectedCloseEvent()
{
	STATUS	nStatus = OK;

	TraceEntry ("X25Port::ProcessUnexpectedCloseEvent: ENTRY\n");

	TraceExit ("X25Port::ProcessUnexpectedCloseEvent: EXIT\n");

	return nStatus;
}

void DumpQos (struct xdiscf *pCtlData)
{
char	buffer[128];

	struct qosformat stQosFormat = pCtlData->qos;
	struct extraformat xtras = pCtlData->qos.xtras;

	sprintf (buffer,"reqtclass is: 0x%x\n", stQosFormat.reqtclass);
	TraceWrite (buffer);
	sprintf (buffer,"reqtclass is: 0x%x\n", stQosFormat.	reqtclass);
	TraceWrite (buffer);
	sprintf (buffer,"locthroughput, remthroughput is: 0x%x 0x%x\n",
		stQosFormat.locthroughput, 	stQosFormat.remthroughput);
	TraceWrite (buffer);
	sprintf (buffer,"reqminthruput is: 0x%x\n", stQosFormat.reqminthruput);
	TraceWrite (buffer);
	sprintf (buffer,"locminthru, remminthru is: 0x%x 0x%x\n",
			stQosFormat.locminthru, stQosFormat.remminthru);
	TraceWrite (buffer);
	sprintf (buffer,"reqtransitdelay is: 0x%x\n", stQosFormat.reqtransitdelay);
	TraceWrite (buffer);
	sprintf (buffer,"transitdelay is: 0x%x\n", stQosFormat.transitdelay);
	TraceWrite (buffer);
	sprintf (buffer,"reqmaxtransitdelay is: 0x%x\n", stQosFormat.reqmaxtransitdelay);
	TraceWrite (buffer);
	sprintf (buffer,"acceptable is: 0x%x\n", stQosFormat.acceptable);
	TraceWrite (buffer);
	sprintf (buffer,"reqpriority is: 0x%x\n", stQosFormat.reqpriority);
	TraceWrite (buffer);
	sprintf (buffer,"reqprtygain is: 0x%x\n", stQosFormat.reqprtygain);
	TraceWrite (buffer);
	sprintf (buffer,"reqprtykeep is: 0x%x\n", stQosFormat.reqprtykeep);
	TraceWrite (buffer);
	sprintf (buffer,"prtydata is: 0x%x\n", stQosFormat.prtydata);
	TraceWrite (buffer);
	sprintf (buffer,"prtygain is: 0x%x\n", stQosFormat.prtygain);
	TraceWrite (buffer);
	sprintf (buffer,"prtykeep is: 0x%x\n", stQosFormat.prtykeep);
	TraceWrite (buffer);
	sprintf (buffer,"reqlowprtydata is: 0x%x\n", stQosFormat.reqlowprtydata);
	TraceWrite (buffer);
	sprintf (buffer,"reqlowprtygain is: 0x%x\n", stQosFormat.reqlowprtygain);
	TraceWrite (buffer);
	sprintf (buffer,"reqlowprtykeep is: 0x%x\n", stQosFormat.reqlowprtykeep);
	TraceWrite (buffer);
	sprintf (buffer,"lowprtydata is: 0x%x\n", stQosFormat.lowprtydata);
	TraceWrite (buffer);
	sprintf (buffer,"lowprtygain is: 0x%x\n", stQosFormat.lowprtygain);
	TraceWrite (buffer);
	sprintf (buffer,"lowprtykeep is: 0x%x\n", stQosFormat.lowprtykeep);
	TraceWrite (buffer);
	sprintf (buffer,"protection_type is: 0x%x\n", stQosFormat.protection_type);
	TraceWrite (buffer);
	sprintf (buffer,"prot_len is: 0x%x\n", stQosFormat.prot_len);
	TraceWrite (buffer);
	sprintf (buffer,"lowprot_len is: 0x%x\n", stQosFormat.lowprot_len);
	TraceWrite (buffer);
	sprintf (buffer,"protection is: %s\n", stQosFormat.protection);
	TraceWrite (buffer);
	sprintf (buffer,"lowprotection is: %s\n", stQosFormat.lowprotection);
	TraceWrite (buffer);
	sprintf (buffer,"reqexpedited is: 0x%x\n", stQosFormat.reqexpedited);
	TraceWrite (buffer);
	sprintf (buffer,"reqackservice is: 0x%x\n", stQosFormat.reqackservice);
	TraceWrite (buffer);

	sprintf (buffer,"fastselreq is: 0x%x\n", xtras.fastselreq);
	TraceWrite (buffer);
	sprintf (buffer,"restrictresponse, xtras.reversecharges is: 0x%x 0x%x\n", xtras.restrictresponse, xtras.reversecharges);
	TraceWrite (buffer);
	sprintf (buffer,"pwoptions is: 0x%x\n", xtras.pwoptions);
	TraceWrite (buffer);
	sprintf (buffer,"locpacket, xtras.rempacket is: 0x%x 0x%x\n", xtras.locpacket, xtras.rempacket);
	TraceWrite (buffer);
	sprintf (buffer,"locwsize , xtras.remwsize is: 0x%x 0x%x\n", xtras.locwsize , xtras.remwsize);
	TraceWrite (buffer);
	sprintf (buffer,"nsdulimit is: %d\n", xtras.nsdulimit);
	TraceWrite (buffer);
	sprintf (buffer,"nui_len is: 0x%x\n", xtras.nui_len);
	TraceWrite (buffer);
	sprintf (buffer,"nui_field is: %s\n", xtras.nui_field);
	TraceWrite (buffer);
	sprintf (buffer,"rpoa_len is: %d\n", xtras.rpoa_len);
	TraceWrite (buffer);
	sprintf (buffer,"rpoa_field is: %s\n", xtras.rpoa_field);
	TraceWrite (buffer);
	sprintf (buffer,"cug_type is: 0x%x\n", xtras.cug_type);
	TraceWrite (buffer);
	sprintf (buffer,"cug_field is: %s\n", xtras.cug_field);
	TraceWrite (buffer);
	sprintf (buffer,"reqcharging is: 0x%x\n", xtras.reqcharging);
	TraceWrite (buffer);
	sprintf (buffer,"chg_cd_len is: 0x%x\n", xtras.chg_cd_len);
	TraceWrite (buffer);
	sprintf (buffer,"chg_cd_field is: %s\n", xtras.chg_cd_field);
	TraceWrite (buffer);
	sprintf (buffer,"chg_sc_len is: 0x%x\n", xtras.chg_sc_len);
	TraceWrite (buffer);
	sprintf (buffer,"chg_sc_field is: %s\n", xtras.chg_sc_field);
	TraceWrite (buffer);
	sprintf (buffer,"chg_mu_len is: 0x%x\n", xtras.chg_mu_len);
	TraceWrite (buffer);
	sprintf (buffer,"chg_mu_field is: %s\n", xtras.chg_mu_field);
	TraceWrite (buffer);
	sprintf (buffer,"called_add_mod is: 0x%x\n", xtras.called_add_mod);
	TraceWrite (buffer);
	sprintf (buffer,"call_redirect is: 0x%x\n", xtras.call_redirect);
	TraceWrite (buffer);
	sprintf (buffer,"called is: 0x%x\n", xtras.called);
	TraceWrite (buffer);
	sprintf (buffer,"call_deflect is: 0x%x\n", xtras.call_deflect);
	TraceWrite (buffer);
	sprintf (buffer,"x_fac_len is: 0x%x\n", xtras.x_fac_len);
	TraceWrite (buffer);
	sprintf (buffer,"cg_fac_len is: 0x%x\n", xtras.cg_fac_len);
	TraceWrite (buffer);
	sprintf (buffer,"cd_fac_len is: 0x%x\n", xtras.cd_fac_len);
	TraceWrite (buffer);
	sprintf (buffer,"fac_field is: %s\n", xtras.fac_field);
	TraceWrite (buffer);

}

/****************************************************************************
*
*   MEMBER FUNCTION DEFINITION
*
 *               Name:  X25Port::ProcessInvalidNetworkEvent
 *
 *           Synopsis:  Processes an unexpected network event
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
STATUS X25Port::ProcessInvalidNetworkEvent()
{
STATUS	nStatus = OK;
char	buffer[128];

	TraceEntry ("X25Port::ProcessInvalidNetworkEvent: ENTRY\n");

	switch (m_nState)
	{
	case DATA_TRANSFER_STATE:
	case DATA_REQUEST_SENT_STATE:
		/* Assume that we are expecting a response from the remote end */
		switch (m_nEvent)
		{
		case EXPEDITED_DATA_RCVD_EVENT:
		{
		struct xedataf *pCtlData = (struct xedataf*)&m_uX25CtlMsg;
			nStatus = SendExpeditedDataAck();
			break;
		}
		case RESET_IND_EVENT:
		{
		struct xrstf *pCtlData = (struct xrstf*)&m_uX25CtlMsg;
			sprintf (buffer, "ResetInd: originator = %d\n", pCtlData->originator);
			TraceWrite (buffer);
			sprintf (buffer, "ResetInd: reason = %d\n", pCtlData->reason);
			TraceWrite (buffer);
			sprintf (buffer, "ResetInd: cause = %d\n", pCtlData->cause);
			TraceWrite (buffer);
			sprintf (buffer, "ResetInd: diag = %d\n", pCtlData->diag);
			TraceWrite (buffer);
			if ((nStatus = SendResetConfirmation()) == OK)
			{
				nStatus = Close();
			}
			m_nState = LINE_DOWN_STATE;
			nStatus = X25_LINE_DOWN;
			break;
		}
		case DISCONNECT_INDICATION_RCVD_EVENT:
		{
		struct xdiscf *pCtlData = (struct xdiscf*)&m_uX25CtlMsg;
			sprintf (buffer,"DiscInd: originator = %x\n", pCtlData->originator);
			TraceWrite (buffer);
			sprintf (buffer,"DiscInd: reason = %x\n", pCtlData->reason);
			TraceWrite (buffer);
			sprintf (buffer,"DiscInd: cause = %x\n", pCtlData->cause);
			TraceWrite (buffer);
			sprintf (buffer,"DiscInd: diag = %x\n", pCtlData->diag);
			TraceWrite (buffer);
			sprintf (buffer,"DiscInd: conn_id = %x\n", pCtlData->conn_id);
			TraceWrite (buffer);
			sprintf (buffer,"DiscInd: indicated_qos = %x\n", pCtlData->indicated_qos);
			TraceWrite (buffer);
			if (pCtlData->indicated_qos) DumpQos(pCtlData);
			fflush (stdout);
			m_nState = LINE_DOWN_STATE;
			if ((nStatus = SendClearConfirmation()) == OK)
			{
				nStatus = Close();
			}
			nStatus = X25_LINE_DOWN;
			break;
		}
		case ABORT_RCVD_EVENT:
		{
			nStatus = Close();
			m_nState = LINE_DOWN_STATE;
			nStatus = X25_LINE_DOWN;
			break;
		}
		default:
			break;
		}
	case CONNECTION_REQUEST_SENT_STATE:
		switch (m_nEvent)
		{
		case EXPEDITED_DATA_RCVD_EVENT:
		{
		struct xedataf *pCtlData = (struct xedataf*)&m_uX25CtlMsg;
			nStatus = SendExpeditedDataAck();
			break;
		}
		case RESET_IND_EVENT:
		{
		struct xrstf *pCtlData = (struct xrstf*)&m_uX25CtlMsg;
			sprintf (buffer, "ResetInd: originator = %d\n",
					pCtlData->originator);
			TraceWrite (buffer);
			sprintf (buffer, "ResetInd: reason = %d\n", pCtlData->reason);
			TraceWrite (buffer);
			sprintf (buffer, "ResetInd: cause = %d\n", pCtlData->cause);
			TraceWrite (buffer);
			sprintf (buffer, "ResetInd: diag = %d\n", pCtlData->diag);
			TraceWrite (buffer);
			if ((nStatus = SendResetConfirmation()) == OK)
			{
				nStatus = Close();
				Delay (m_RetryDelay);
				nStatus = Open();
			}
			m_nState = LINE_DOWN_STATE;
			nStatus = X25_LINE_DOWN;
			break;
		}
		case DISCONNECT_INDICATION_RCVD_EVENT:
		{
		struct xdiscf *pCtlData = (struct xdiscf*)&m_uX25CtlMsg;
			sprintf (buffer,"DiscInd: originator = %x\n", pCtlData->originator);
			TraceWrite (buffer);
			sprintf (buffer,"DiscInd: reason = %x\n", pCtlData->reason);
			TraceWrite (buffer);
			sprintf (buffer,"DiscInd: cause = %x\n", pCtlData->cause);
			TraceWrite (buffer);
			sprintf (buffer,"DiscInd: diag = %x\n", pCtlData->diag);
			TraceWrite (buffer);
			sprintf (buffer,"DiscInd: conn_id = %x\n", pCtlData->conn_id);
			TraceWrite (buffer);
			sprintf (buffer,"DiscInd: indicated_qos = %x\n", pCtlData->indicated_qos);
			TraceWrite (buffer);
			if (pCtlData->indicated_qos) DumpQos(pCtlData);
			if ((nStatus = SendClearConfirmation()) == OK)
			{
				nStatus = Close();
				Delay (m_RetryDelay);
				nStatus = Open();
			}
			m_nState = LINE_DOWN_STATE;
			nStatus = X25_LINE_DOWN;
			break;
		}
		case ABORT_RCVD_EVENT:
		{
			nStatus = Close();
			m_nState = LINE_DOWN_STATE;
			nStatus = X25_LINE_DOWN;
			break;
		}
		case TIMEOUT_EVENT:
		{
			m_nState = DEVICE_OPEN_STATE;
			break;
		}
		default:
			break;
		}

		break;
	default:
		break;
	}

	TraceExit ("X25Port::ProcessInvalidNetworkEvent: EXIT\n");

	return nStatus;
}
/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  X25Port::ProcessInvalidEvent
 *
 *           Synopsis:  Processes an invalid event to reach a stable state
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
STATUS X25Port::ProcessInvalidEvent()
{
STATUS	nStatus = OK;
char	buffer[128];

	TraceEntry ("X25Port::ProcessInvalidEvent: ENTRY\n");

	sprintf (buffer,"X25Port::Invalid Event received: %d\n", m_nEvent);
	TraceWrite (buffer);
	sprintf (buffer,"X25Port::Errno: %d\n", errno);
	TraceWrite (buffer);
	sprintf (buffer,"X25Port::xl_type: %x\n", m_uX25CtlMsg.xMsgHdr.xl_type);
	TraceWrite (buffer);
	sprintf (buffer,"X25Port::xl_command: %x\n", m_uX25CtlMsg.xMsgHdr.xl_command);
	TraceWrite (buffer);

	/* This should really be a finite state machine, but this will do */

	switch (m_nEvent)
	{
	case OPEN_EVENT:
	case CLOSE_EVENT:
	case WAIT_FOR_CONNECTION_EVENT:
	case READ_EVENT:
	case WRITE_EVENT:
		nStatus = ProcessInvalidInternalEvent ();
		break;
	case CONNECTION_CONF_RCVD_EVENT:
	case DISCONNECT_CONF_RCVD_EVENT:
	case EXPEDITED_DATA_RCVD_EVENT:
	case RESET_IND_EVENT:
	case NETWORK_DATA_EVENT:
	case DISCONNECT_INDICATION_RCVD_EVENT:
	case ABORT_RCVD_EVENT:
	case CLEAR_CONF_RCVD_EVENT:
	case NETWORK_ERROR_EVENT:
	case UNKNOWN_EVENT:
		nStatus = ProcessInvalidNetworkEvent();
		break;
	case TIMEOUT_EVENT:
		nStatus = ProcessInvalidNetworkEvent ();
		break;
	default:
		break;
	}

	TraceExit ("X25Port::ProcessInvalidEvent: EXIT\n");

	return nStatus;
}
