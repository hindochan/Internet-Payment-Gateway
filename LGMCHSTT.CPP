/***********************************************************************
 *  LAG - Source file header
 *
 *              Layer:  Merchant Main module
 *             Module:  LGmchstt.cpp
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

BOOL MCheck(MerchantThread *poMyThread, RWCString *pstrText,
			STATUS *pnStatus, BOOL *pbNeedToReply);

void MAbort(MerchantThread *poMyThread);

void * MerchantThreadMain(void *oMrchThrd)
{
	MerchantThread *poMyThread;
	Port *poMyPort;
	Message *poMyMessage;
	BOOL bNeedToReply;
	BOOL bHaveRetried = FALSE;
	RWCString strText;
	STATUS nStatus = 0;
	time_t lBeginTime = 0;
	time_t lEndTime = 0;

	poMyThread = (MerchantThread*)oMrchThrd;

	if (poMyThread)
	{
		strText = "Locking Myself";
		nStatus = poMyThread->Lock();
		MCheck(poMyThread, &strText, &nStatus, &bNeedToReply);

		strText = "Changing State";
		nStatus = poMyThread->SetCancelTypeAsync();
		MCheck(poMyThread, &strText, &nStatus, &bNeedToReply);

		// Could just use thread calls as these pointers are members ??
		poMyPort = poMyThread->Port();
		poMyMessage = poMyThread->Message();
		poMyMessage->SetOutputInput();
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
	cout << "This is Merchant :- " << poMyThread->InterfaceName();
	cout << "\n";
	cout << "and thread nbr :- " << poMyThread->GetThreadId();
	cout << "\n";
	cout << "*******************";
	cout << "\n";
	cout << "\n";
	cout.flush();

	strText = "Opening Port";
	nStatus = poMyPort->Open();
	MCheck(poMyThread, &strText, &nStatus, &bNeedToReply);

	bNeedToReply = FALSE;
	// Main processing loop

	while (TRUE)
	{
		if (bNeedToReply)
		{
			bNeedToReply = FALSE;

			poMyMessage->ConvertToInternal();

			poMyThread->PutTransaction(nStatus);

			strText = "Signal back to server";
			nStatus = poMyThread->SignalRequestor();
			MCheck(poMyThread, &strText, &nStatus, &bNeedToReply);
		}

		if (poMyThread->LineDown())
		{
			strText = "Linedown waiting for connection";
			nStatus = poMyPort->WaitForConnection();
			if (MCheck(poMyThread, &strText, &nStatus, &bNeedToReply))
				continue;
		}

		oTheApplication.LockMrchLL();
		poMyThread->SetStatusOKIdle();
		oTheApplication.UnlockMrchLL();

		strText = "Waiting for Signal";
		nStatus = poMyThread->Wait();
		if (MCheck(poMyThread, &strText, &nStatus, &bNeedToReply))
			continue;

		if (TRACE)
		{
			cout << "***********************\n";
			cout << "Merchant thread number :-  " << poMyThread->GetThreadId();
			cout << "\nReceived transaction\n";
			cout << "***********************\n";
			cout.flush();
		}

		poMyThread->GetTransaction();

		poMyMessage->ConvertFromInternal();

		strText = "Encoding Message";
		nStatus = poMyMessage->EncodeMessage();
		if (MCheck(poMyThread, &strText, &nStatus, &bNeedToReply))
			continue;

		poMyPort->DataBuffer() = poMyMessage->DataMsg();

		strText = "Writing request";

		if (TRACE)
			poMyPort->DumpDataBuffer();

		bHaveRetried = FALSE;

		time(&lBeginTime);

		RETRY:

		nStatus = poMyPort->Write();

		if (nStatus == LINEDOWN_CONDITION && !bHaveRetried)
		{
			poMyPort->DataBuffer() = poMyMessage->DataMsg();
			strText = "Re-writing request";
			bHaveRetried = TRUE;
			goto RETRY;
		}

		if (MCheck(poMyThread, &strText, &nStatus, &bNeedToReply))
			continue;

		strText = "Reading response";
		nStatus = poMyPort->Read();

		if (nStatus == LINEDOWN_CONDITION && !bHaveRetried)
		{
			poMyPort->DataBuffer() = poMyMessage->DataMsg();
			strText = "Re-writing request";
			bHaveRetried = TRUE;
			goto RETRY;
		}

		if (MCheck(poMyThread, &strText, &nStatus, &bNeedToReply))
			continue;

		time(&lEndTime);

		oTheApplication.GetStatistics()->UpdateMrchStats(lEndTime - lBeginTime);

		if (TRACE)
			poMyPort->DumpDataBuffer();

		poMyMessage->DataMsg() = poMyPort->DataBuffer();

		strText = "Decoding reply";
		nStatus = poMyMessage->DecodeMessage();
		if (MCheck(poMyThread, &strText, &nStatus, &bNeedToReply))
			continue;

		strText = "Validating reply";
		nStatus = poMyMessage->ValidateMessage();
		if (MCheck(poMyThread, &strText, &nStatus, &bNeedToReply))
			continue;

		bNeedToReply = TRUE;
	}

	// Unlock here because wait will unlock and lock again when signalled
	strText = "Unlocking myself";
	nStatus = poMyThread->Unlock();
	MCheck(poMyThread, &strText, &nStatus, &bNeedToReply);

	poMyThread->Exit();

	return NULL;
}

BOOL MCheck(MerchantThread *poMyThread, RWCString *pstrText,
			STATUS *pnStatus, BOOL *pbNeedToReply)
{
	BOOL bBool = FALSE;

	if (*pnStatus)
	{
		cerr << "######################\n";
		cerr << *pstrText << "\n";
		fprintf (stderr, "Error Code :- %lx\n", *pnStatus);
		cerr << "Error Code :- " << *pnStatus << "\n";
		cerr << "######################\n";
		cerr.flush();
		if (CHECK_SEVERITY(*pnStatus) == TERMINATE_ERROR)
			MAbort(poMyThread);
		else
		{
			if (*pnStatus == LINEDOWN_CONDITION)
			{
				oTheApplication.LockMrchLL();
				poMyThread->SetStatusLineDown();
				oTheApplication.UnlockMrchLL();
			}
			*pbNeedToReply = TRUE;
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

void MAbort(MerchantThread *poMyThread)
{
	cerr << "THREAD ABORTING \n";
	cerr.flush();

	oTheApplication.LockMrchLL();
	poMyThread->SetStatusDead();
	oTheApplication.UnlockMrchLL();

	poMyThread->Exit();
}
