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
#ifndef LGSVRINT_H
#define LGSVRINT_H

#include <rw/tpdlist.h>

#include "LGintfce.h"
#include "LGsvrthr.h"

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

class ServerInterface : public Interface
{

// Attributes

public:

protected:

private:

	ServerThread *m_poSrvrThrd;
	RWTPtrDlist<ServerThread> *m_pllSrvrThrds;


// Methods

public:
	// Constructors
	ServerInterface();
	ServerInterface(RWTPtrDlist<ServerThread> *pllSrvrThrds);

	// Destructors
	~ServerInterface();

	STATUS Open();
	STATUS Close();

protected:

private:

	STATUS CancelThreads();
	void DeleteThreads();

};

#endif // LGSVRINT_H
