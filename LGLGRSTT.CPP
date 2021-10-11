/***********************************************************************
 *  LAG - Source file header
 *
 *              Layer:  Logger Main module
 *             Module:  LGlgrstt.cpp
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

BOOL LCheck(LoggerThread *poMyThread, RWCString *pstrText,
			STATUS *pnStatus);

void LAbort(LoggerThread *poMyThread);
 
void * LoggerThreadMain(void *oLggrThrd)
{
	LoggerThread *poMyThread;
	Port *poMyPort;
	Message *poMyMessage;
	RWCString strText;
	STATUS nStatus = 0;

	poMyThread = (LoggerThread*)oLggrThrd;

	if (poMyThread)
	{
		strText = "Locking Myself";
		nStatus = poMyThread->Lock();
		LCheck(poMyThread, &strText, &nStatus);

		strText = "Changing State";
		nStatus = poMyThread->SetCancelTypeAsync();
		LCheck(poMyThread, &strText, &nStatus);

		// Could just use thread calls as these pointers are members ??
		poMyPort = poMyThread->Port();
		poMyMessage = poMyThread->Message();
		poMyMessage->SetOutputOnly();
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
	cout << "This is Logger :- " << poMyThread->InterfaceName();
	cout << "\n";
	cout << "and thread nbr :- " << poMyThread->GetThreadId();
	cout << "\n";
	cout << "*******************";
	cout << "\n";
	cout << "\n";
	cout.flush();

	strText = "Opening Port";
	nStatus = poMyPort->Open();
	LCheck(poMyThread, &strText, &nStatus);

	// Main processing loop

	while (TRUE)
	{
		if (poMyThread->LineDown())
		{
			nStatus = poMyPort->WaitForConnection();
			if (LCheck(poMyThread, &strText, &nStatus))
				continue;
		}

		oTheApplication.LockLggrLL();
		poMyThread->SetStatusOKIdle();
		oTheApplication.UnlockLggrLL();

		strText = "Waiting for Signal";
		nStatus = poMyThread->Wait();
		if (LCheck(poMyThread, &strText, &nStatus))
			continue;

		if (TRACE)
		{
			cout << "Got a request to log \n";
			cout.flush();
		}

		poMyThread->GetTransaction();

		poMyMessage->ConvertFromInternal();

		strText = "Encoding log record";
		nStatus = poMyMessage->EncodeMessage();
		if (LCheck(poMyThread, &strText, &nStatus))
			continue;

		poMyPort->DataBuffer() = poMyMessage->DataMsg();

		strText = "Writing log record";
		nStatus = poMyPort->Write();
		if (LCheck(poMyThread, &strText, &nStatus))
			continue;

		strText = "Signal back to server";
		nStatus = poMyThread->SignalRequestor();
		LCheck(poMyThread, &strText, &nStatus);
	}

	// Unlock here because wait will unlock and lock again when signalled
	strText = "Unlocking myself";
	nStatus = poMyThread->Unlock();
	LCheck(poMyThread, &strText, &nStatus);

	poMyThread->Exit();

	return NULL;
}

BOOL LCheck(LoggerThread *poMyThread, RWCString *pstrText,
			STATUS *pnStatus)
{
	BOOL bBool = FALSE;
	Message *poMyMessage;

	if (*pnStatus)
	{
		cerr << "######################\n";
		cerr << *pstrText << "\n";
		fprintf (stderr, "Error Code :- %lx\n", *pnStatus);
		cerr << "Error Code :- " << *pnStatus << "\n";
		cerr << "######################\n";
		cerr.flush();
		if (CHECK_SEVERITY(*pnStatus) == TERMINATE_ERROR)
		{
			poMyMessage = poMyThread->Message();
			poMyMessage->DumpInternal();
			LAbort(poMyThread);
		}
		else
		{
			if (*pnStatus == LINEDOWN_CONDITION)
			{
				oTheApplication.LockLggrLL();
				poMyThread->SetStatusLineDown();
				oTheApplication.UnlockLggrLL();
			}
			bBool = TRUE;
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

void LAbort(LoggerThread *poMyThread)
{
	cerr << "THREAD ABORTING \n";
	cerr.flush();

	oTheApplication.LockLggrLL();
	poMyThread->SetStatusDead();
	oTheApplication.UnlockLggrLL();

	poMyThread->Exit();
}
