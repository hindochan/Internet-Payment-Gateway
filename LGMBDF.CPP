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
 *           Synopsis:  
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */

#include "LGmbdf.h"
#include "LGstrcts.h"
#include "LGerrors.h"
#include <iostream.h>
#include <fstream.h>

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MBDF::MBDF
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
MBDF::MBDF()
{
	// Initialise variables
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MBDF::~MBDF
 *
 *           Synopsis:  Destructor for class object
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
MBDF::~MBDF()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MBDF::Open
 *
 *           Synopsis:  Open connection
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS MBDF::Open(RWCString MBDFFileName)
{
	// Init routines set up the file descriptor by opening file
	// so don't open if InitFlag set
	STATUS nStatus = OK;

	m_oMBDFFileID.open (MBDFFileName.data(), ios::in);

#ifdef sadf
	if (m_oMBDFFileID == NULL)
	{
		nStatus = MBDF_FILE_ERROR;
	}
#endif

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MBDF::Read
 *
 *           Synopsis:  Read line
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS MBDF::Read()
{
	STATUS nStatus = OK;

    if (!m_strCurrentLine.readLine (m_oMBDFFileID, TRUE))
	{
		nStatus = END_OF_FILE;
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MBDF::Write
 *
 *           Synopsis:  Write line
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS MBDF::Write()
{
	STATUS nStatus = OK;

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MBDF::Close
 *
 *           Synopsis:  Close line
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS MBDF::Close()
{
	STATUS nStatus = OK;

	m_oMBDFFileID.close();

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  MBDF::SearchForMerchant
 *
 *           Synopsis:  Searches for a specified merchant
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	STATUS (0 = OK)
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS MBDF::SearchForMerchant(	RWCString strMerchant,
								MerchantDetails * pstDetails)
{
	STATUS		nStatus = OK;
	RWCString	strMerchantID;
	RWCString	strMerchantData;
 
	/* Rewind the File Pointer to the beginning of the merchant file */
	m_oMBDFFileID.seekg (0);

	while ((nStatus = Read()) != END_OF_FILE)
	{
    RWCTokenizer next (m_strCurrentLine);

        if (m_strCurrentLine (0) == '#')
        {
            continue;
        }
 
        strMerchantID = next("=");
        strMerchantData = next ("=");
 
		if (strMerchantID == strMerchant)
		{
			RWCTokenizer next (strMerchantData);
			RWCString	strToken;
			int iCount = 0;
 
            while (!(strToken = next(":")).isNull())
            {
                switch (iCount)
                {
                case 0: // Bank Sort Code
					m_stMerchant.strBankSortCode = strToken;
                    iCount++;
                    break;
                case 1: // Account Number
					m_stMerchant.strAccountNumber = strToken;
                    iCount++;
                    break;
                case 2: // Merchant Name
					m_stMerchant.strMerchantName = strToken;
                    iCount++;
                    break;
                default:
                    break;
                }
            }
			if (iCount < 2)
			{
				nStatus = MERCHANT_NOT_FOUND;
			}
			break;
		}
	}

	if (nStatus == END_OF_FILE)
	{
		nStatus = MERCHANT_NOT_FOUND;
	}
	else if (nStatus == OK)
	{
		*pstDetails = m_stMerchant;
	}

	return nStatus;
}
