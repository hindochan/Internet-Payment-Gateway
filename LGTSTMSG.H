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
#ifndef LGTSTMSG_H
#define LGTSTMSG_H

#include "LGmsg.h"
#include "LGfield.h"

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

class TestMessage : public Message
{

// Attributes

public:

protected:

private:

	char *m_pcPos;

	Field m_oDelim;

	Field m_oCardNumber;
	Field m_oAmount;
	Field m_oAuthCode;
	Field m_oResponseCode;

// Methods

public:
	// Constructors
	TestMessage(WORD nMsgType);

	// Destructors
	~TestMessage();

	STATUS DecodeMessage();
	STATUS EncodeMessage();
	STATUS ValidateMessage();

	void ConvertToInternal();
	void ConvertFromInternal();

protected:

private:

};

#endif // LGTSTMSG_H
