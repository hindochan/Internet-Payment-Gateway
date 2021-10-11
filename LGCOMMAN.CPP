#include <string.h>
#include <stream.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <rw/cstring.h>

#include "LGlagapp.h"
#include "LGtypes.h"
#include "LGstrcts.h"
#include "LGconfig.h"
#include "LGtcpprt.h"
#include "LGoprint.h"
#include "LGoperator.h"
#include "LGcommand.h"

LAGApplication oTheApplication;

#ifdef SELECTING
STATUS WaitForInput (int fd, WORD t)
{
STATUS			nStatus = OK;
fd_set			stFds;
int				nFds = 1;
int				nReturn;
struct timeval	timeout = {4,500};


     //int select(int nfds, fd_set *readfds, fd_set *writefds,
       //   fd_set *exceptfds, struct timeval *timeout);

	FD_ZERO (&stFds);
	FD_SET (fd, &stFds);
     nReturn = select(nFds, NULL, &stFds, NULL, &timeout);

    //void FD_SET(int fd, fd_set &fdset);
    //void FD_CLR(int fd, fd_set &fdset);
    //int FD_ISSET(int fd, fd_set &fdset);
    //void FD_ZERO(fd_set &fdset);

	//nReturn = poll (&stFds, nFds, timeout);
	switch (nReturn)
	{
	case 0:					// Timeout condition on file descriptor
		if (TRACE)
		{
			cout << "Timeout condition reached on file descriptor\n";
			cout.flush ();
		}
		nStatus = TIMEOUT_CONDITION;
		break;
	case -1:				// Error Condition
		nStatus = LINEDOWN_CONDITION;
		break;
	default:				// Data received on the file descriptor
		if (nReturn != nFds)
		{
			nStatus = INVALID_POLLFD_ERROR;
		}
		else
		{
			if ((nReturn = FD_ISSET (fd, &stFds)))
			{
				printf ("File Descriptor M_READY\n");
			}
		}
		break;
	}
	return nStatus;
}
#endif
#ifdef POLLING
STATUS WaitForInput (int fd, WORD timeout)
{
STATUS			nStatus = OK;
pollfd_t		stFds;
int				nFds = 1;
int				nReturn;

	stFds.fd		= fd;
	stFds.events	= POLLOUT;

	nReturn = poll (&stFds, nFds, timeout);
	switch (nReturn)
	{
	case 0:					// Timeout condition on file descriptor
		if (TRACE)
		{
			cout << "Timeout condition reached on file descriptor\n";
			cout.flush ();
		}
		nStatus = TIMEOUT_CONDITION;
		break;
	case -1:				// Error Condition
		nStatus = LINEDOWN_CONDITION;
		break;
	default:				// Data received on the file descriptor
		if (nReturn != nFds)
		{
			nStatus = INVALID_POLLFD_ERROR;
		}
		else
		{
			switch (stFds.revents)
			{
			case POLLIN:		// Valid Data exists
			case POLLPRI:
			case POLLRDNORM:
				break;
			case POLLERR:		// Error conditions on file descriptor
			case POLLHUP:
				nStatus = POLL_ERROR;
				cout << "Poll Error:" << errno << "\n";
				cout.flush();
				break;
			}
		}
		break;
	}
	return nStatus;
}
#endif

void ProcessResponse (OperatorResponse *pOperatorResponse)
{
	printf ("Operator Command: %d\n", pOperatorResponse->nCmd);
	printf ("LAG Status: %d\n", pOperatorResponse->nStatus);
	switch (pOperatorResponse->nCmd)
	{
	case LAG_STOP:
		if (pOperatorResponse->nStatus)
			printf ("Error code is %lx\n", pOperatorResponse->nStatus);
		else
			printf ("STOP SUCCESSFUL\n");
		break;
	case LAG_STATUS:
	{
	LAGStatus	*pstStatus = (LAGStatus*)&pOperatorResponse->uData.stStatus;
	int			i;

		printf ("LAG Status:\n");
		printf ("ThreadName\tThread ID\tThreadStatus\n");
		for (i = 0; i < pstStatus->nNbrThreads; i++)
		{
			printf ("%s\t\t%d\t\t%s\n",
			pstStatus->ThreadDetails[i].cThread,
			pstStatus->ThreadDetails[i].nThreadID,
			ThreadStatusToText[pstStatus->ThreadDetails[i].enThreadStatus].cText);
		}
		break;
	}
	case LAG_STATISTICS:
	{
	LAGStatistics	*stStatistics = &pOperatorResponse->uData.stStatistics;

		printf ("LAG Statistics:\n");

		printf ("Date Started:\t\t\t%d\n",
				stStatistics->nDateStarted);
		printf ("Number of Transactions OK:\t\t\t%d\n",
				stStatistics->nNoTransOK);
		printf ("Number of Transactions NOK:\t\t\t%d\n",
				stStatistics->nNoTransNOK);
		printf ("Average Server Response Time:\t\t\t%d\n",
				stStatistics->nAvgServResponse);
		printf ("Average Merchant Response Time:\t\t\t%d\n",
				stStatistics->nAvgMrchResponse);
		break;
	}
	case LAG_CUTOVER:
		printf ("LAG Cutover:\n");
		if (pOperatorResponse->nStatus)
			printf ("Error code is %lx\n", pOperatorResponse->nStatus);
		else
			printf ("CUTOVER SUCCESSFUL\n");
		break;
	default:
		printf ("Unknown Operator Command\n");
		break;
	}
}

int main( int argc, char* argv[] )
{
LGConfiguration oConfiguration;
int i;
TCPIPPort			*port;
BOOL				OperatorFound = FALSE;
STATUS				nStatus = OK;
DataMsg				stMyBuffer;
OperatorInterface	*poOperInt;
OperatorCommand		stOprCommand;
OperatorResponse	stOprResponse;
RWCString			strParam;

	if (argc != 2)
	{
		fprintf (stderr,
				"usage: %s [stop | statistics | cutover | status]\n",
				argv[0]);
		exit (1);
	}
	stOprCommand.nCmd = LAG_STATUS;
	strParam = argv[1];
	if (strParam == "stop")
	{
		stOprCommand.nCmd = LAG_STOP;
	}
	if (strParam == "status")
	{
		stOprCommand.nCmd = LAG_STATUS;
	}
	if (strParam == "statistics")
	{
		stOprCommand.nCmd = LAG_STATISTICS;
	}
	if (strParam == "cutover")
	{
		stOprCommand.nCmd = LAG_CUTOVER;
	}
	if (strParam != "stop" &&
		strParam != "status" &&
		strParam != "statistics" &&
		strParam != "cutover")
	{
		fprintf (stderr,
				"usage: %s [stop | statistics | cutover | status]\n",
				argv[0]);
		exit(1);
	}

	stMyBuffer.nMessageLen = sizeof (OperatorCommand);

	nStatus = oConfiguration.LoadConfiguration();

	if (nStatus)
	{
		cerr << "Error Loading Config File\n";
		fprintf (stderr, "Error Code :- %lx\n", nStatus);
		cerr.flush();
		exit(nStatus);
	}

	poOperInt = oConfiguration.GetCommandInterface();

	if (TRACE)
	{
		cout << "CMDOperatorInterface "  << " is:\n";
		cout << "Name:"  << poOperInt->Name() << "\n";
		cout << "MsgType:"  << poOperInt->MsgType() << "\n";
		cout << "CommType:"  << poOperInt->CommType() << "\n";
		cout << "NbrPorts:"  << poOperInt->NbrPorts() << "\n";
		cout << "CommsConfig:"  << poOperInt->CommsConfig() << "\n";
		cout << "\n";
	}

	/* Open a TCPIP Port for listening */
	port = new TCPIPPort (1, poOperInt->CommsConfig());

	if (nStatus == OK)
	{
		nStatus = port->Open();
		if (TRACE)
			printf ("nStatus from Open is: %lx\n", nStatus);
		if (nStatus == OK)
		{
			nStatus = port->WaitForConnection();
			if (TRACE)
				printf ("nStatus from WaitForConnection is: %lx\n", nStatus);
			if (nStatus == OK)
			{
				memcpy (stMyBuffer.cMessage,
						&stOprCommand,
						sizeof (stOprCommand));
				port->DataBuffer() = stMyBuffer;
				nStatus = port->Write();

				if (nStatus == OK)
				{
					nStatus = port->Read();
					if (TRACE)
							printf ("nStatus from Read is: %lx\n", nStatus);
					if (nStatus == OK)
					{
						stMyBuffer = port->DataBuffer();
						memcpy (&stOprResponse,
								stMyBuffer.cMessage,
								stMyBuffer.nMessageLen);
						ProcessResponse(&stOprResponse);
						if (TRACE)
						{
							cout << "Data len is: ";
							cout << port->DataBuffer().nMessageLen << "\n";
						}
						port->Close();
					}
				}
			}
		}
	}
	cout.flush();

	return nStatus;
}
