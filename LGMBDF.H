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
 *           Synopsis:  class definition for MBDF
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */
#ifndef LGMBDF_H
#define LGMBDF_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream.h>

#include <rw/ctoken.h>

#include "LGtypes.h"

typedef struct merchantdetails
{
	RWCString	strBankSortCode;
	RWCString	strAccountNumber;
	RWCString	strMerchantName;
} MerchantDetails;

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

class MBDF
{

// Attributes

public:

protected:

private:

	BOOL m_bReadFilePresent;
	RWCString m_strReadFileName;
	BOOL m_bWriteFilePresent;
	RWCString m_strWriteFileName;

	MerchantDetails	m_stMerchant;

	RWCString		m_strCurrentLine;

	RWCString		m_strMBDFFileName;
	ifstream		m_oMBDFFileID;

// Methods

public:
	// Constructors
	MBDF();
	//MBDF(WORD nType, RWCString strCommsConfig);

	// Destructors
	~MBDF();

	STATUS Open(RWCString MBDFFileName);
	STATUS Read();
	STATUS SearchForMerchant(RWCString strMerchant,
							MerchantDetails *pstDetails);
	STATUS Write();
	STATUS Close();


protected:

private:

};

#endif // LGMBDF_H
