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
#ifndef LGTCPPRT_H
#define LGTCPPRT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "LGport.h"

typedef struct sockaddr_in SOCKADDR;
typedef struct hostent HOSTENTRY;

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

class TCPIPPort : public Port
{

// Attributes

public:
	int			*m_pMainSocket;	// Socket used for accept
	int			*m_pMsgSocket;	// Socket used for messages

protected:
	WORD		m_nInitialReadLength;

private:

	BOOL		m_bListen;

	int			m_Port;
	int			m_RecovWaitTime;
	int			m_RetryDelay;
	RWCString	m_strHostName;

	SOCKADDR	m_stServAddr;
	SOCKADDR	m_stClntAddr;
	HOSTENTRY	m_stHostEntry;

// Methods

public:
	// Constructors
	TCPIPPort();
	TCPIPPort(WORD nType, RWCString strCommsConfig);

	// Destructors
	~TCPIPPort();

	void * PreThreadInit();
	STATUS Init(void *pVoid);
	STATUS Open();
	STATUS Read();
	STATUS Write();
	STATUS Close();

	STATUS WaitForConnection();

protected:

	STATUS SetSocketOptions();
	STATUS SetAddrInfo();
	virtual BOOL ValidateMessageFromLength ();
	STATUS ReadMsgPort(char *cBuffer, int nBytesToRead, int *pBytesRead);
	STATUS TCPIPPort::CloseSocket(int **ppSocket);

private:
	void CommonConstructorInitialisation ();

};

#endif // LGTCPPRT_H
