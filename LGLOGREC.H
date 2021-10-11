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
#ifndef LGLOGREC_H
#define LGLOGREC_H

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

class LogRecord : public Message
{

// Attributes

public:

protected:

private:

	char *m_pcPos;

	Field m_oDelim;

	RWCString m_strLogRecord;

// Methods

public:
	// Constructors
	LogRecord(WORD nMsgType);

	// Destructors
	~LogRecord();

	STATUS DecodeMessage();
	STATUS EncodeMessage();
	STATUS ValidateMessage();

	void ConvertToInternal();
	void ConvertFromInternal();

protected:

	RWCString JulianDate();

private:

};

#endif // LGLOGREC_H
