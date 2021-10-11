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
#ifndef LGLAGTRN_H
#define LGLAGTRN_H

#include "LGtran.h"
#include "LGmsg.h"
#include "LGmbdf.h"

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

class LAGTransaction : public Transaction
{

// Attributes

public:

protected:

private:

	STATUS m_nTranStatus;
	IntMsg m_stInternalMsg;
	RWCString m_strMBDFFileName;
	MBDF m_oMBDFFile;
	MerchantDetails m_stMerchantDetails;

// Methods

public:
	// Constructors
	LAGTransaction();

	// Destructors
	~LAGTransaction();

	// Reference methods
	IntMsg & InternalMsg();

	// Get methods
	RWCString MerchantInterfaceName();

	BOOL IsTranOK();

	void ResetTranStatus();

	STATUS ValidateRequest();
	void SetTranStatus(STATUS nStatus);
	void CheckTranStatus();

protected:

private:

	void SetResultCode();

};

#endif // LGLAGTRN_H
