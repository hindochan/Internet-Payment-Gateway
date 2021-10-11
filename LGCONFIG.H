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
 *           Synopsis:  class definition for the LAG configuration
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */
#ifndef LGCONFIG_H
#define LGCONFIG_H

#include <rw/cstring.h>
#include <rw/ctoken.h>
#include <rw/tpdlist.h>
#include <fstream.h>

#include "LGtypes.h"
#include "LGsvrint.h"
#include "LGmchint.h"
#include "LGlgrint.h"
#include "LGoprint.h"

/************************************************************************
 *
 *   CLASS DECLARATION
 *
 *               Name:  LGConfiguration
 *
 *           Synopsis:  Class to store the configuration data
 *
 *              Notes:  .
 *
 ************************************************************************
 */

class LGConfiguration
{

// Attributes

public:

protected:

private:
	RWCString							m_strAsfPrefix;
	RWCString							m_strTracingFlag;
	WORD								m_nResponseTimeout;
	RWCString							m_strOperatorName;
	RWCString							m_strMBDFFileName;
	RWTPtrDlist<Interface>				m_llServerInterface;
	RWTPtrDlist<Interface>				m_llMerchantInterface;
	RWTPtrDlist<Interface>				m_llLoggerInterface;
	RWTPtrDlist<Interface>				m_llOperatorInterface;
	WORD								m_nNbrServerInterfaces;
	WORD								m_nNbrMerchantInterfaces;
	WORD								m_nNbrLoggerInterfaces;
	WORD								m_nNbrOperatorInterfaces;
	RWTPtrDlist<ServerThread>			*m_pllServerThrdLL;
	RWTPtrDlist<MerchantThread>			*m_pllMerchantThrdLL;
	RWTPtrDlist<LoggerThread>			*m_pllLoggerThrdLL;
	RWTPtrDlist<OperatorThread>			*m_pllOperatorThrdLL; // frig for now

// Methods

public:
	// Constructors
	LGConfiguration();
	LGConfiguration(RWTPtrDlist<ServerThread> *pllSrvrThrds,
                    RWTPtrDlist<MerchantThread> *pllMrchThrds,
                    RWTPtrDlist<LoggerThread> *pllLggrThrds,
                    RWTPtrDlist<OperatorThread> *pllOprThrds);

	// Destructors
	~LGConfiguration();

	RWCString				GetASFPrefix ();
	RWCString				GetTracingFlag ();
	WORD					GetResponseTimeout ();
	RWCString				GetOperatorName ();
	RWCString				GetMBDFFileName ();
	ServerInterface			*GetServerInterface (WORD i);
	MerchantInterface		*GetMerchantInterface (WORD i);
	LoggerInterface			*GetLoggerInterface (WORD i);
	OperatorInterface		*GetOperatorInterface ();
	OperatorInterface		*GetCommandInterface ();
	WORD 					GetNbrServerInterfaces ();
	WORD 					GetNbrMerchantInterfaces ();
	WORD 					GetNbrLoggerInterfaces ();
	WORD 					GetNbrOperatorInterfaces ();

	STATUS					LoadConfiguration ();

protected:

private:

};

#endif // LGCONFIG_H
