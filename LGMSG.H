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
#ifndef LGMSG_H
#define LGMSG_H

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

class Message
{

// Attributes

public:

protected:

	WORD m_nMsgType;

	DataMsg m_stDataMsg;
	IntMsg m_stInternalMsg;

private:

	enum MsgSequence
	{
		UNDEFINED,
		INPUTOUTPUT,
		OUTPUTINPUT,
		OUTPUTONLY
	};

	MsgSequence m_enMsgSequence;

// Methods

public:

	// Constructors
	Message();

	// Destructors
	virtual ~Message();

	// Reference methods
	DataMsg & DataMsg();
	IntMsg & InternalMsg();

	// Pure virtuals
	virtual STATUS DecodeMessage() = 0;
	virtual STATUS EncodeMessage() = 0;
	virtual STATUS ValidateMessage() = 0;

	virtual void ConvertToInternal() = 0;
	virtual void ConvertFromInternal() = 0;

	// Set methods
	void SetInputOutput();
	void SetOutputInput();
	void SetOutputOnly();

	// Tester methods
	BOOL IsInputOutput();
	BOOL IsOutputInput();
	BOOL IsOutputOnly();

	void DumpInternal();

protected:

private:

};

#endif // LGMSG_H
