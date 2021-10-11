#include <string.h>
#include <stream.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

#include "LGtypes.h"
#include "LGconfig.h"
#include "LGerrors.h"
#include "LGglobls.h"
#include "LGport.h"
#include "LGtcpprt.h"
#include "LGx25prt.h"

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
		cout << "Timeout condition reached on file descriptor\n";
		cout.flush ();
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
	return (nStatus);
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
		cout << "Timeout condition reached on file descriptor\n";
		cout.flush ();
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
	return (nStatus);
}
#endif

int main( int argc, char* argv[] )
{
LGConfiguration oConfiguration;
int i;
TCPIPPort	*port;
X25Port		*xport;
BOOL		ServerFound = FALSE;
BOOL		MerchantFound = FALSE;
STATUS		nStatus;
int			*pSocket;
int			*pPort;
DataMsg		stMyBuffer;
ServerInterface *server;
MerchantInterface *merchant;


	oConfiguration.LoadConfiguration();


	cout << "LAGASFPrefix is: "  << oConfiguration.GetASFPrefix ();
	cout << "\n";
	cout << "LAGTracingFlag is: "  << oConfiguration.GetTracingFlag ();
	cout << "\n";
	cout << "LAGResponseTimeout is: "  << oConfiguration.GetResponseTimeout ();
	cout << "\n";
	cout << "LAGOperatorName is: "  << oConfiguration.GetOperatorName ();
	cout << "\n";
	
	cout << "Servers:\n";
	for (i=0; i<oConfiguration.GetNbrServerInterfaces();i++)
	{
		server = oConfiguration.GetServerInterface(i);
		//cout << "LAGServerInterface"  << i <<  "is:"  << oConfiguration.GetServerInterface (i);
		cout << "LAGServerInterface "  << i <<  " is:\n";
		cout << "Name:"  << server->Name() << "\n";
		cout << "MsgType:"  << server->MsgType() << "\n";
		cout << "CommType:"  << server->CommType() << "\n";
		cout << "NbrPorts:"  << server->NbrPorts() << "\n";
		cout << "CommsConfig:"  << server->CommsConfig() << "\n";
		cout << "\n";
		ServerFound = TRUE;
	}
	cout << "Merchants:\n";
	for (i=0; i<oConfiguration.GetNbrMerchantInterfaces();i++)
	{
		merchant = oConfiguration.GetMerchantInterface(i);

		// cout << "LAGMerchantInterface"  << i <<  "is:"  << oConfiguration.GetMerchantInterface (i);
		cout << "LAGMerchantInterface "  << i <<  " is:\n";
		cout << "Name:"  << merchant->Name() << "\n";
		cout << "MsgType:"  << merchant->MsgType() << "\n";
		cout << "CommType:"  << merchant->CommType() << "\n";
		cout << "NbrPorts:"  << merchant->NbrPorts() << "\n";
		cout << "CommsConfig:"  << merchant->CommsConfig() << "\n";
		cout << "\n";
		MerchantFound = TRUE;
	}
#ifdef MERCHANT
	if (MerchantFound)
	{
		xport = new X25Port (1, merchant->CommsConfig());
		/* Open a X25Port for writing */
		cout << "port = " << xport << "\n";
		pPort = (int*) xport->PreThreadInit();
		cout << "Socket id is: " << socket << "\n";
		while ((nStatus = xport->Init((void*)pPort)) == OK)
		{
			cout << "nStatus from Init is: " << nStatus << "\n";
			if (nStatus == OK)
			{
				nStatus = xport->Open();
				cout << "nStatus from Open is: " << nStatus << "\n";
				if (nStatus == OK)
				{
					//nStatus = xport->WaitForConnection();
					//cout << "nStatus from WaitForConnection is: " << nStatus << "\n";
					if (nStatus == OK)
					{
						stMyBuffer = xport->DataBuffer();
						strcpy (stMyBuffer.cMessage, "Hello World");
						stMyBuffer.nMessageLen = strlen (stMyBuffer.cMessage);
						xport->DataBuffer() = stMyBuffer;
						nStatus = xport->Write();
						cout << "nStatus from Write is: " << nStatus << "\n";
						if (nStatus == OK)
						{
							cout << "Data len is: " << xport->DataBuffer().nMessageLen << "\n";
							cout << "Data is: " << xport->DataBuffer().cMessage << "\n";
							// Wait for reply
							xport->Read();
						}
					}
				}
				xport->Close();
			}
		}
		cout.flush();
	}
#endif /* SERVER */
#ifdef SERVER
	if (ServerFound)
	{
		port = new TCPIPPort (1, server->CommsConfig());
		/* Open a TCPIP Port for listening */
		cout << "port = " << port << "\n";
		pSocket = (int*) port->PreThreadInit();
		cout << "Socket id is: " << socket << "\n";
		while ((nStatus = port->Init((void*)pSocket)) == OK)
		{
			cout << "nStatus from Init is: " << nStatus << "\n";
			if (nStatus == OK)
			{
				nStatus = port->Open();
				cout << "nStatus from Open is: " << nStatus << "\n";
				if (nStatus == OK)
				{
					nStatus = port->WaitForConnection();
					cout << "nStatus from WaitForConnection is: " << nStatus << "\n";
					if (nStatus == OK)
					{
						while (1)
						{
							if ((nStatus = WaitForInput(*(port->m_pMsgSocket), 500)) == OK)
							{
								nStatus = port->Write();

								if (nStatus == OK)
								{
									nStatus = port->Read();
									cout << "nStatus from Read is: " << nStatus << "\n";
									if (nStatus == OK)
									{
										cout << "Data len is: " << port->DataBuffer().nMessageLen << "\n";
										//cout << "Data is: " << port->DataBuffer().pMessagePtr << "\n";
										//port->Close();
									}
								}
							}
							else
							{
								cout << "Wait for Input Error: " << nStatus << "\n";
								cout.flush ();
							}
						}
					}
				}
			}
		}
		cout.flush();
	}
#endif /* SERVER */

	return 0;
}
