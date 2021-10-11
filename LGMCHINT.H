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
#ifndef LGMCHINT_H
#define LGMCHINT_H

#include <rw/tpdlist.h>

#include "LGintfce.h"
#include "LGmchthr.h"

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

class MerchantInterface : public Interface
{

// Attributes

public:

protected:

private:

	MerchantThread *m_poMrchThrd;
	RWTPtrDlist<MerchantThread> *m_pllMrchThrds;

// Methods

public:
	// Constructors
	MerchantInterface();
	MerchantInterface(RWTPtrDlist<MerchantThread> *pllMrchThrds);

	// Destructors
	~MerchantInterface();

	STATUS Open();
	STATUS Close();

protected:

private:

	STATUS CancelThreads();
	void DeleteThreads();

};
#endif // LGMCHINT_H
