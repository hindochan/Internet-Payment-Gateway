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

#include "LGintfce.h"

#include "LGfleprt.h"
#include "LGtcpprt.h"
#include "LGx25prt.h"
#include "LGomprt.h"

#include "LGtstmsg.h"
#include "LGlogrec.h"
#include "LGomiso.h"
#include "LGapacs.h"
#include "LGoperator.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Interface::Interface
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
Interface::Interface()
{
	// Initialise variables
	m_nInterfaceNbr;
	m_bMultiThreaded = FALSE;
    m_nCommType = 0;
    m_nMsgType = 0;
    m_nNbrPorts = 0;

	m_poMsg = NULL;
	m_poPort = NULL;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Interface::~Interface
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
Interface::~Interface()
{

}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Interface::ReferenceMethods
 *
 *           Synopsis:  Reference Member to get and set variable
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  Reference to variable
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
WORD & Interface::InterfaceNbr()
{
	return m_nInterfaceNbr;
}

WORD & Interface::CommType()
{
	return m_nCommType;
}

WORD & Interface::MsgType()
{
	return m_nMsgType;
}

WORD & Interface::NbrPorts()
{
	return m_nNbrPorts;
}

RWCString & Interface::Key()
{
	return m_strKey;
}

RWCString & Interface::Name()
{
	return m_strName;
}

RWCString & Interface::CommsConfig()
{
	return m_strCommsConfig;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Interface::GetNewPort
 *
 *           Synopsis:  Creates a new port depending on type
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  Address of port object
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
Port* Interface::GetNewPort()
{
	switch (m_nCommType)
	{
		case FILE:
			return new FilePort(m_nCommType, m_strCommsConfig);

		case TCPIP:
			return new TCPIPPort(m_nCommType, m_strCommsConfig);

		case X25:
			return new X25Port(m_nCommType, m_strCommsConfig);

		case OMTCPIP:
			return new OMTCPIPPort(m_nCommType, m_strCommsConfig);

		default:
			return NULL;
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  Interface::GetNewMsg
 *
 *           Synopsis:  Creates a new message depending on type
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:  Address of message object
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
Message* Interface::GetNewMsg()
{
	switch (m_nMsgType)
	{
		case TEST:
			return new TestMessage(m_nMsgType);

		case LOGFORMAT:
			return new LogRecord(m_nMsgType);

		case OMISO8583:
			return new OMISO8583Message(m_nMsgType);

		case APACS30:
			return new APACS30Message(m_nMsgType);

		case OPERATORFORMAT:
			return new OperatorMessage(m_nMsgType);

		default:
			return NULL;
	}
}
