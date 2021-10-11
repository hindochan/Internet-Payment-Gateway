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

#include <rw/rwdate.h>

#include "LGstats.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGStatistics::LGStatistics
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
LGStatistics::LGStatistics()
{
	// Initialise variables
	RWDate oDate; // current system date
	m_lDateStarted = (10000 * oDate.year());
	m_lDateStarted += (100 * oDate.month());
	m_lDateStarted += oDate.dayOfMonth();

	m_lNbrSuccessTrans = 0;
	m_lNbrFailedTrans = 0;
	m_lNbrServerTrans = 0;
	m_lNbrMerchantTrans = 0;

	m_lTotalServerRespTime = 0;
	m_lTotalMerchantRespTime = 0;

	pthread_mutex_init(&m_mtxLggrStats, NULL);
	pthread_mutex_init(&m_mtxSrvrStats, NULL);
	pthread_mutex_init(&m_mtxMrchStats, NULL);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGStatistics::~LGStatistics
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
LGStatistics::~LGStatistics()
{
	pthread_mutex_destroy(&m_mtxLggrStats);
	pthread_mutex_destroy(&m_mtxSrvrStats);
	pthread_mutex_destroy(&m_mtxMrchStats);
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGStatistics::GetMethods
 *
 *           Synopsis:  Returns values of private data members
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	Value of variable
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
DWORD LGStatistics::GetStartDate()
{
	return m_lDateStarted;
}

DWORD LGStatistics::GetSuccessTrans()
{
	return m_lNbrSuccessTrans;
}

DWORD LGStatistics::GetFailedTrans()
{
	return m_lNbrFailedTrans;
}

DWORD LGStatistics::GetAverageSrvrResp()
{
	if (m_lNbrServerTrans)
		return (m_lTotalServerRespTime / m_lNbrServerTrans);
	else
		return 0;
}

DWORD LGStatistics::GetAverageMrchResp()
{
	if (m_lNbrMerchantTrans)
		return (m_lTotalMerchantRespTime / m_lNbrMerchantTrans);
	else
		return 0;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGStatistics::UpdateSrvrStats
 *
 *           Synopsis:  Updates Server stats
 *
 *   Input parameters:  Response Time
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
void LGStatistics::UpdateSrvrStats(DWORD lRespTime)
{
	LockSrvrStats();

	m_lTotalServerRespTime += lRespTime;
	m_lNbrServerTrans ++;

	UnlockSrvrStats();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGStatistics::UpdateMrchStats
 *
 *           Synopsis:  Updates Merchant stats
 *
 *   Input parameters:  Response Time
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
void LGStatistics::UpdateMrchStats(DWORD lRespTime)
{
	LockMrchStats();

	m_lTotalMerchantRespTime += lRespTime;
	m_lNbrMerchantTrans ++;

	UnlockMrchStats();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGStatistics::UpdateLggrStats
 *
 *           Synopsis:  Updates Logger stats
 *
 *   Input parameters:  Transaction status
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
void LGStatistics::UpdateLggrStats(BOOL bIsOK)
{
	LockLggrStats();

	if (bIsOK)
		m_lNbrSuccessTrans++;
	else
		m_lNbrFailedTrans++;

	UnlockLggrStats();
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGStatistics::Lock and Unlock Routines
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
void LGStatistics::LockLggrStats()
{
	pthread_mutex_lock(&m_mtxLggrStats);
}

void LGStatistics::LockSrvrStats()
{
	pthread_mutex_lock(&m_mtxSrvrStats);
}

void LGStatistics::LockMrchStats()
{
	pthread_mutex_lock(&m_mtxMrchStats);
}

void LGStatistics::UnlockLggrStats()
{
	pthread_mutex_unlock(&m_mtxLggrStats);
}

void LGStatistics::UnlockSrvrStats()
{
	pthread_mutex_unlock(&m_mtxSrvrStats);
}

void LGStatistics::UnlockMrchStats()
{
	pthread_mutex_unlock(&m_mtxMrchStats);
}
