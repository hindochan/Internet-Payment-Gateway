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
 *           Synopsis:  class definition for 
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */
#ifndef LGX25PRT_H
#define LGX25PRT_H

#include "LGport.h"
#include <netx25/uint.h>
#include <netx25/x25_proto.h>

typedef enum
{
	LINE_DOWN_STATE = 0,
	DEVICE_OPEN_STATE,
	CONNECTION_REQUEST_SENT_STATE,
	DATA_TRANSFER_STATE,
	EXPEDITED_DATA_SENT_STATE,
	RESET_RESPONSE_SENT_STATE,
	DATA_REQUEST_SENT_STATE,
	CLEAR_CONF_SENT_STATE,
	CLEAR_REQUEST_SENT_STATE,
	TIMEOUT_ON_READ_STATE
} X25STATE;

typedef enum
{
	OPEN_EVENT = 0,
	CLOSE_EVENT,
	WAIT_FOR_CONNECTION_EVENT,
	READ_EVENT,
	WRITE_EVENT,

	CONNECTION_CONF_RCVD_EVENT,
	DISCONNECT_CONF_RCVD_EVENT,
	EXPEDITED_DATA_RCVD_EVENT,
	RESET_IND_EVENT,
	NETWORK_DATA_EVENT,
	DISCONNECT_INDICATION_RCVD_EVENT,
	ABORT_RCVD_EVENT,
	CLEAR_CONF_RCVD_EVENT,
	NETWORK_ERROR_EVENT,
	TIMEOUT_EVENT,
	UNKNOWN_EVENT

} X25EVENT;

#define MAX_CUD_LEN				14
#define MAX_STATES				10
#define MAX_EVENTS				17
#define DEFAULT_WAIT_TIMEOUT	100

typedef struct X25EventFSM
{
	STATUS		nStatus;
	X25STATE	nNextState;
} X25EVENTFSM[MAX_EVENTS][MAX_STATES];

/************************************************************************
 *
 *   CLASS DECLARATION
 *
 *               Name:  
 *
 *           Synopsis:  .
 *
 *              Notes:  .
 *
 ************************************************************************
 */

class X25Port : public Port
{

// Attributes

public:

protected:

private:
	BOOL			m_bListen;
	BOOL			m_bSVC;

	char			m_cCud[MAX_CUD_LEN]; // Call User Data
	int				m_nCudLen;
	int				m_nX25fd;
	int				m_RecovWaitTime;
	int				m_RetryDelay;
	RWCString		m_strHostName;
	RWCString		m_strDeviceName;
	struct xaddrf	m_stRemotePeer;
	struct xaddrf	m_stLocalHost;

	X25STATE		m_nState;
	X25EVENT		m_nEvent;

	union {
		struct xhdrf	xMsgHdr;
		struct xdataf	xDataHdr;
		struct xedataf	xEdataHdr;
		struct xrstf	xRstHdr;
		struct xrscf	xRscHdr;
		struct xdiscf	xDiscHdr;
		struct xdcnff	xdcnfHdr;
		struct xabortf	xAbortHdr;
		struct xccnff	xCcnfHdr;
	} m_uX25CtlMsg;

	//S_X25_HDR		m_stX25CtlMsg;

    static const X25EVENTFSM     m_stEventFSM;
	 
// Methods

public:
	// Constructors
	X25Port();
	X25Port(WORD nType, RWCString strCommsConfig);

	// Destructors
	~X25Port();

	void		*PreThreadInit();
	STATUS		Init(void *pVoid);
	STATUS		Open();
	STATUS		Read();
	STATUS		Write();
	STATUS		Close();

	STATUS		WaitForConnection();

protected:
	void		CommonConstructorInitialisation ();
	X25EVENT	GetNetworkEvent (WORD nTimeout);
	STATUS		ConvertSystemError ();
	STATUS		SendConnectionRequest ();
	STATUS		SendClearRequest ();
	STATUS		SendData ();
	STATUS		SendExpeditedDataAck ();
	STATUS		SendResetConfirmation ();
	STATUS		SendClearConfirmation ();
	STATUS		ValidateEvent();
	STATUS		ProcessUnexpectedOpenEvent();
	STATUS		ProcessUnexpectedCloseEvent();
	STATUS		ProcessInvalidInternalEvent();
	STATUS		ProcessInvalidNetworkEvent();
	STATUS		ProcessInvalidEvent();

private:

};

#endif // LGX25PRT_H
