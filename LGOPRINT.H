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
#ifndef LGOPRINT_H
#define LGOPRINT_H

#include <rw/tpdlist.h>

#include "LGintfce.h"
#include "LGoprthr.h"
#include "LGoperator.h"
#include "LGcommand.h"

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

class OperatorInterface : public Interface
{

// Attributes

public:

protected:

private:

	WORD m_nInterfaceNbr;
	OperatorThread *m_poOprThrd;
	RWTPtrDlist<OperatorThread> *m_pllOprThrds;

 
// Methods

public:
	// Constructors
	OperatorInterface();
	OperatorInterface(RWTPtrDlist<OperatorThread> *pllOprThrds);

	// Destructors
	~OperatorInterface();

	STATUS Open();
	STATUS Close();

protected:

private:

    STATUS CancelThreads();
    STATUS ProcessRequest(OperatorCommand *pOperatorRequest,
							OperatorResponse *pOperatorResponse,
							WORD *pnLength);
    STATUS ProcessOperatorCommands();
};

#endif // LGOPRINT_H
