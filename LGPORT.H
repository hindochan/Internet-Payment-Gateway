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
#ifndef LGPORT_H
#define LGPORT_H

#include <sys/poll.h>
#include <errno.h>
#include <rw/cstring.h>

#include "LGtypes.h"
#include "LGstrcts.h"

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

class Port
{

// Attributes

public:

protected:

	BOOL		m_bInitFlag;

	WORD		m_nType;
	RWCString	m_strCommsConfig;
	DataMsg		m_stDataBuffer;
	CommsMsg	m_stCommsBuffer;
	WORD		m_nMsgHdrInLen;
	WORD		m_nMsgTrlInLen;
	WORD		m_nMsgHdrOutLen;
	WORD		m_nMsgTrlOutLen;
	char		m_cMsgHdrIn[MAX_MSG_HDR_LEN];
	char		m_cMsgTrlIn[MAX_MSG_HDR_LEN];
	char		m_cMsgHdrOut[MAX_MSG_HDR_LEN];
	char		m_cMsgTrlOut[MAX_MSG_HDR_LEN];

private:

// Methods

public:
	// Constructors
	Port();

	// Destructors
	virtual ~Port();

	// Reference methods
	DataMsg & DataBuffer();

	// Pure virtuals
	virtual void * PreThreadInit() = 0;
	virtual STATUS Init(void *pVoid) = 0;
	virtual STATUS Open() = 0;
	virtual STATUS Read() = 0;
	virtual STATUS Write() = 0;
	virtual STATUS Close() = 0;
	virtual STATUS WaitForConnection() = 0;

	void DumpDataBuffer();

protected:

	STATUS WaitForInput(int nSd, WORD nTimeout);
	STATUS ConvertSystemError ();
	STATUS Delay (WORD msecs);

private:

};

#endif // LGPORT_H
