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

#include "LGlagtrn.h"
#include "LGglobls.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGTransaction::LAGTransaction
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
LAGTransaction::LAGTransaction()
{
	// Initialise variables
	m_nTranStatus = 0;
	m_strMBDFFileName = oTheApplication.GetConfiguration().GetMBDFFileName();
	m_oMBDFFile.Open (m_strMBDFFileName);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGTransaction::~LAGTransaction
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
LAGTransaction::~LAGTransaction()
{
	m_oMBDFFile.Close();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGTransaction::ReferenceMethods
 *
 *           Synopsis:  Reference Member to get and set variable
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	Reference to variable
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
IntMsg & LAGTransaction::InternalMsg()
{
	return m_stInternalMsg;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGTransaction::GetMethods
 *
 *           Synopsis:  Returns value of char field
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	converted value
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
RWCString LAGTransaction::MerchantInterfaceName()
{
	return m_stInternalMsg.strMerchantInterfaceName;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGTransaction::IsTranOK
 *
 *           Synopsis:  Tests Tran status and result code
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	BOOL
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
BOOL LAGTransaction::IsTranOK()
{
	if ((m_nTranStatus == 0) && (m_stInternalMsg.strResultCode == "0000"))
		return TRUE;
	else
		return FALSE;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGTransaction::ResetTranStatus
 *
 *           Synopsis:  Sets back to zero for new transaction
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	None
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void LAGTransaction::ResetTranStatus()
{
	m_nTranStatus = 0;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGTransaction::Validate
 *
 *           Synopsis:  Validate various fields in transaction
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
STATUS LAGTransaction::ValidateRequest()
{
	STATUS nStatus = INVALID_TRAN;

	if (!m_oMBDFFile.SearchForMerchant(m_stInternalMsg.strMerchantNumber,
										&m_stMerchantDetails))
	{
		m_stInternalMsg.strMerchantInterfaceName =
		m_stMerchantDetails.strMerchantName;
		nStatus = 0;
	}
	else
	{
		m_oMBDFFile.Close();
		m_oMBDFFile.Open(m_strMBDFFileName);
		m_stInternalMsg.strMerchantInterfaceName = "UNKNOWN";
		nStatus = INVALID_MERCHANT;
	}

	if (!nStatus)
	{
		if (m_stInternalMsg.strTranType == "0220")
			nStatus = LOG_ONLY;
	}
	else
		SetTranStatus(nStatus);

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGTransaction::SetTranStatus
 *
 *           Synopsis:  Sets Tran status
 *
 *   Input parameters:  Status to be set
 *
 *  Output parameters:  None
 *
 *       Return value:	None
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void LAGTransaction::SetTranStatus(STATUS nStatus)
{
	m_nTranStatus = nStatus;
	SetResultCode();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGTransaction::CheckTranStatus
 *
 *           Synopsis:  Checks tran status for action
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	None
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void LAGTransaction::CheckTranStatus()
{
	if (CHECK_ACTION(m_nTranStatus) == OPERATOR)
	{
		RWCString strMail("mail -tw ");
		strMail += oTheApplication.GetConfiguration().GetOperatorName();
		strMail += " <<!\n";
		strMail += "LAG system error - Attention required\n";
		if (m_nTranStatus == INVALID_MERCHANT)
			strMail += "Transaction with invalid merchant ID received\n";
		if (m_nTranStatus == MERCHANT_UNAVAILABLE)
			strMail += "Transaction declined - no merchant lines were available\n";
		strMail += "Error code is ";
		strMail += m_stInternalMsg.strResultCode;
		strMail += "\n";
		strMail += "!";
		system(strMail);
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LAGTransaction::SetResultCode
 *
 *           Synopsis:  Sets result code for logging from status
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	None
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
void LAGTransaction::SetResultCode()
{
	if (m_nTranStatus)
	{
		char cResCode[9];

		sprintf(cResCode, "%lx", m_nTranStatus);
		m_stInternalMsg.strResultCode = cResCode;
	}
}
