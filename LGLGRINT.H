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
#ifndef LGLGRINT_H
#define LGLGRINT_H

#include <rw/tpdlist.h>

#include "LGintfce.h"
#include "LGlgrthr.h"

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

class LoggerInterface : public Interface
{

// Attributes

public:

protected:

private:

	LoggerThread *m_poLggrThrd;
	RWTPtrDlist<LoggerThread> *m_pllLggrThrds;

// Methods

public:
	// Constructors
	LoggerInterface();
	LoggerInterface(RWTPtrDlist<LoggerThread> *pllLggrThrds);

	// Destructors
	~LoggerInterface();

	STATUS Open();
	STATUS Close();

	STATUS RenameLogFile();

protected:

private:

	STATUS CancelThreads();
	void DeleteThreads();

};

#endif // LGLGRINT_H
