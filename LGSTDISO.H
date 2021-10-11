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
#ifndef LGSTDISO_H
#define LGSTDISO_H

#include "LGmsg.h"

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

class ISO8583Message : public Message
{

// Attributes

public:

protected:

private:

// Methods

public:
	// Constructors
	ISO8583Message();
	ISO8583Message(WORD nMsgType);

	// Destructors
	~ISO8583Message();

	STATUS DecodeMessage();
	STATUS EncodeMessage();
	STATUS ValidateMessage();

	void ConvertToInternal();
	void ConvertFromInternal();

protected:

private:

};

#endif // LGSTDISO_H
