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
#ifndef LGOMPRT_H
#define LGOMPRT_H

#include "LGtcpprt.h"
#include "LGfield.h"
#include "LGbitfld.h"
#include "LGvrlfld.h"

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

class OMTCPIPPort : public TCPIPPort
{

// Attributes

public:

protected:

private:

	BitMapField m_oBitMapField;
	VariableLenField m_oStdVarField;
	VariableLenField m_o3DigVarField;

// Methods

public:
	// Constructors
	OMTCPIPPort(WORD nType, RWCString strCommsConfig);

	// Destructors
	~OMTCPIPPort();

protected:

	BOOL ValidateMessageFromLength ();

private:

};

#endif // LGOMPRT_H
