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
#ifndef LGSTATS_H
#define LGSTATS_H

#include "LGtypes.h"

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

class LGStatistics
{

// Attributes

public:

protected:

private:

	DWORD m_lDateStarted;

	MUTEX m_mtxLggrStats;
	DWORD m_lNbrSuccessTrans;
	DWORD m_lNbrFailedTrans;

	MUTEX m_mtxSrvrStats;
	DWORD m_lNbrServerTrans;
	DWORD m_lTotalServerRespTime;

	MUTEX m_mtxMrchStats;
	DWORD m_lNbrMerchantTrans;
	DWORD m_lTotalMerchantRespTime;

// Methods

public:
	// Constructors
	LGStatistics();

	// Destructors
	~LGStatistics();

	// Get methods
	DWORD GetStartDate();
	DWORD GetSuccessTrans();
	DWORD GetFailedTrans();
	DWORD GetAverageSrvrResp();
	DWORD GetAverageMrchResp();

	void UpdateSrvrStats(DWORD lRespTime);
	void UpdateMrchStats(DWORD lRespTime);
	void UpdateLggrStats(BOOL bIsOK);

protected:

private:

	void LockLggrStats();
	void LockSrvrStats();
	void LockMrchStats();
	void UnlockLggrStats();
	void UnlockSrvrStats();
	void UnlockMrchStats();

};

#endif // LGSTATS_H
