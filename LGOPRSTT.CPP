/***********************************************************************
 *  LAG - Source file header
 *
 *              Layer:  Operator Main module
 *             Module:  LGoprstt.cpp
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
 
#include "LGtypes.h"

#include "LGglobls.h"

BOOL OCheck(OperatorThread *poMyThread, RWCString *pstrText,
			STATUS *pnStatus, STATUS *pnSaveStatus);

void OAbort(OperatorThread *poMyThread);

void * OperatorThreadMain(void *oOperThrd)
{
	OperatorThread *poMyThread;
	Port *poMyPort;
	Message *poMyMessage;
	RWCString strText;
	STATUS nStatus = 0;
	STATUS nSaveStatus = 0;

	poMyThread = (OperatorThread*)oOperThrd;

	if (poMyThread)
	{
		strText = "Locking Myself";
		nStatus = poMyThread->Lock();
		OCheck(poMyThread, &strText, &nStatus, &nSaveStatus);

		strText = "Changing State";
		nStatus = poMyThread->SetCancelTypeAsync();
		OCheck(poMyThread, &strText, &nStatus, &nSaveStatus);

		// Could just use thread calls as these pointers are members ??
		poMyPort = poMyThread->Port();
		poMyMessage = poMyThread->Message();
		poMyMessage->SetInputOutput();
	}
	else
	{
		// Just kill everything
		cerr << "NULL SERVER THREAD \n";
		cerr << "PROCESS TERMINATING \n";
		cerr.flush();
		exit(-1);
	}

	cout << "*******************";
	cout << "\n";
	cout << "This is Operator :- " << poMyThread->InterfaceName();
	cout << "\n";
	cout << "and thread nbr :- " << poMyThread->GetThreadId();
	cout << "\n";
	cout << "*******************";
	cout << "\n";
	cout << "\n";
	cout.flush();
 
	strText = "Unlocking Myself";
	nStatus = poMyThread->Unlock();
	OCheck(poMyThread, &strText, &nStatus, &nSaveStatus);

	strText = "Opening Port";
	nStatus = poMyPort->Open();
	OCheck(poMyThread, &strText, &nStatus, &nSaveStatus);

	nStatus = 0;

	// Main processing loop

	while (TRUE)
	{
		strText = "Locking Myself Again";
		nStatus = poMyThread->Lock();

		strText = "Locking Myself Again";
		nStatus = poMyThread->Lock();
	}

	poMyThread->Exit();

	return NULL;
}

BOOL OCheck(OperatorThread *poMyThread, RWCString *pstrText,
			STATUS *pnStatus, STATUS *pnSaveStatus)
{
	BOOL bBool = FALSE;

	if (*pnStatus && *pnStatus != LOG_ONLY)
	{
		cerr << *pstrText << "\n";
		fprintf (stderr, "Error Code :- %lx\n", *pnStatus);
		cerr << "Error Code :- " << *pnStatus << "\n";
		cerr.flush();
		if (CHECK_SEVERITY(*pnStatus) == TERMINATE_ERROR)
			OAbort(poMyThread);
		else
		{
			if (CHECK_CLASS(*pnStatus) == CLASS_LAGTRN)
			{
				*pnSaveStatus = *pnStatus;
				*pnStatus = LOG_ONLY;
			}
			else
			{
				*pnSaveStatus = *pnStatus;
				bBool = TRUE;
			}
		}
	}
	else
	{
		if (TRACE)
		{
			cout << *pstrText << " OK\n";
			cout.flush();
		}
	}

	*pstrText = " ";

	return bBool;
}

void OAbort(OperatorThread *poMyThread)
{
	cerr << "THREAD ABORTING \n";
	cerr.flush();
	poMyThread->SetStatusDead();
	poMyThread->Exit();
}
