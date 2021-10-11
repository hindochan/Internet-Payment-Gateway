/***********************************************************************
 *  LAG - Source file header
 *
 *              Layer:  Server Main module
 *             Module:  LGsvrstt.cpp
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

BOOL SCheck(ServerThread *poMyThread, RWCString *pstrText,
			STATUS *pnStatus, STATUS *pnSaveStatus);

void SAbort(ServerThread *poMyThread);

void * ServerThreadMain(void *oSrvrThrd)
{
	ServerThread *poMyThread;
	Port *poMyPort;
	Message *poMyMessage;
	RWCString strText;
	STATUS nStatus = 0;
	STATUS nSaveStatus = 0;
	time_t lBeginTime = 0;
	time_t lEndTime = 0;

	poMyThread = (ServerThread*)oSrvrThrd;

	if (poMyThread)
	{
		strText = "Locking Myself";
		nStatus = poMyThread->Lock();
		SCheck(poMyThread, &strText, &nStatus, &nSaveStatus);

		strText = "Changing State";
		nStatus = poMyThread->SetCancelTypeAsync();
		SCheck(poMyThread, &strText, &nStatus, &nSaveStatus);

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
	cout << "This is Server :- " << poMyThread->InterfaceName();
	cout << "\n";
	cout << "and thread nbr :- " << poMyThread->GetThreadId();
	cout << "\n";
	cout << "*******************";
	cout << "\n";
	cout << "\n";
	cout.flush();
 
	strText = "Unlocking Myself";
	nStatus = poMyThread->Unlock();
	SCheck(poMyThread, &strText, &nStatus, &nSaveStatus);

	strText = "Opening Port";
	nStatus = poMyPort->Open();
	SCheck(poMyThread, &strText, &nStatus, &nSaveStatus);

	nStatus = 0;

	// Main processing loop

	while (TRUE)
	{
		if (poMyThread->LineDown())
		{
			nStatus = poMyPort->WaitForConnection();
			if (SCheck(poMyThread, &strText, &nStatus, &nSaveStatus))
				continue;
		}

		oTheApplication.LockSrvrLL();
		poMyThread->SetStatusOKIdle();
		oTheApplication.UnlockSrvrLL();

		strText = "Reading Port";
		nStatus = poMyPort->Read();

		time(&lBeginTime);

		oTheApplication.LockSrvrLL();
		poMyThread->SetStatusBusyTran();
		oTheApplication.UnlockSrvrLL();

		if (SCheck(poMyThread, &strText, &nStatus, &nSaveStatus))
			continue;

		// Get the message read and reset codes as begining of tran
		nStatus = 0;
		nSaveStatus = 0;

		if (TRACE)
			poMyPort->DumpDataBuffer();

		poMyMessage->DataMsg() = poMyPort->DataBuffer();

		strText = "Decoding Message";
		nStatus = poMyMessage->DecodeMessage();

		if (SCheck(poMyThread, &strText, &nStatus, &nSaveStatus))
			continue;

		strText = "Validating Message";
		nStatus = poMyMessage->ValidateMessage();

		if (SCheck(poMyThread, &strText, &nStatus, &nSaveStatus))
			continue;

		poMyMessage->ConvertToInternal();

		// We have a valid message now so set transaction up
		poMyThread->PutTransaction();

		// and check merchant etc.
		strText = "Validating Transaction";
		nStatus = poMyThread->ValidateTransaction();

		if (SCheck(poMyThread, &strText, &nStatus, &nSaveStatus))
			continue;

		if (nStatus != LOG_ONLY)
		{
			nStatus = 0;

			oTheApplication.LockMrchLL();

			// Find available Merchant thread
			poMyThread->CurrentMrchThrd() =
				 oTheApplication.GetFreeMrchThrd(poMyThread->CurrentMrchName());

			oTheApplication.UnlockMrchLL();

			if (poMyThread->CurrentMrchThrd())
			{
				strText = "Calling Merchant";
				nStatus = poMyThread->CallMerchant();

				if (SCheck(poMyThread, &strText, &nStatus, &nSaveStatus))
					continue;
			}
			else
			{
				nStatus = MERCHANT_UNAVAILABLE;
				strText = "No merchant threads available";
				SCheck(poMyThread, &strText, &nStatus, &nSaveStatus);
			}
		}

		poMyThread->GetTransaction();

		poMyMessage->ConvertFromInternal();

		strText = "Encoding reply";
		nStatus =  poMyMessage->EncodeMessage();
		if (SCheck(poMyThread, &strText, &nStatus, &nSaveStatus))
			continue;

		poMyPort->DataBuffer() = poMyMessage->DataMsg();

		strText = "Writing Reply";
		nStatus = poMyPort->Write();
		if (SCheck(poMyThread, &strText, &nStatus, &nSaveStatus))
			continue;

		time(&lEndTime);

		if (TRACE)
			poMyPort->DumpDataBuffer();

		oTheApplication.GetStatistics()->UpdateSrvrStats(lEndTime - lBeginTime);

		oTheApplication.LockLggrLL();

		poMyThread->LggrThrd() = oTheApplication.GetLggrThrd();

		oTheApplication.UnlockLggrLL();

		if (poMyThread->LggrThrd())
		{
			strText = "Logging Transaction";
			nStatus = poMyThread->LogTransaction(nSaveStatus);
			if (SCheck(poMyThread, &strText, &nStatus, &nSaveStatus))
				continue;
		}
		else
		{
			nStatus = LOGGER_UNAVAILABLE;
			strText = "No logger threads available";
			strText += "\n SERIOUS ERROR \n";
			if (SCheck(poMyThread, &strText, &nStatus, &nSaveStatus))
				continue;
		}

		SCheck(poMyThread, &strText, &nStatus, &nSaveStatus);
	}

	poMyThread->Exit();

	return NULL;
}

BOOL SCheck(ServerThread *poMyThread, RWCString *pstrText,
			STATUS *pnStatus, STATUS *pnSaveStatus)
{
	BOOL bBool = FALSE;

	if (*pnStatus && *pnStatus != LOG_ONLY)
	{
		if (*pnStatus != LINEDOWN_CONDITION || TRACE)
		{
			cerr << "######################\n";
			cerr << *pstrText << "\n";
			fprintf (stderr, "Error Code :- %lx\n", *pnStatus);
			cerr << "Error Code :- " << *pnStatus << "\n";
			cerr << "######################\n";
			cerr.flush();
		}
		if (CHECK_SEVERITY(*pnStatus) == TERMINATE_ERROR)
			SAbort(poMyThread);
		else
		{
			*pnSaveStatus = *pnStatus;

			if (*pnStatus == LINEDOWN_CONDITION)
			{
				oTheApplication.LockSrvrLL();
				poMyThread->SetStatusLineDown();
				oTheApplication.UnlockSrvrLL();
			}

			if (CHECK_CLASS(*pnStatus) == CLASS_LAGTRN)
				*pnStatus = LOG_ONLY;
			else
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

void SAbort(ServerThread *poMyThread)
{
	cerr << "THREAD ABORTING \n";
	cerr.flush();

	oTheApplication.LockSrvrLL();
	poMyThread->SetStatusDead();
	oTheApplication.UnlockSrvrLL();

	poMyThread->Exit();
}
