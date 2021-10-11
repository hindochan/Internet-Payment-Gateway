#include <string.h>
#include <stream.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <errno.h>

#include "LGtypes.h"
#include "LGconfig.h"
#include "LGglobls.h"
#include "LGport.h"
#include "LGtcpprt.h"
#include "LGerrors.h"

STATUS WaitForInput (int fd, WORD timeout)
{
STATUS			nStatus = OK;
pollfd_t		stFds;
int				nFds = 1;
int				nReturn;
pollfd_t *		pFds = &stFds;

	stFds.fd		= fd;
	stFds.events	= POLLIN;

	if (fd ==0) {pFds = NULL; nFds = 0;}

	nReturn = poll (pFds, nFds, timeout);
	switch (nReturn)
	{
	case 0:					// Timeout condition on file descriptor
		cout << "Timeout condition reached on file descriptor\n";
		cout.flush ();
		nStatus = TIMEOUT_CONDITION;
		break;
	case -1:				// Error Condition
		nStatus = 12345;
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
				//nStatus = POLL_ERROR;
				nStatus = TERMINATE;
				cout << "Poll Error:" << errno << "\n";
				break;
			}
		}
		break;
	}
	return (nStatus);
}

int main( int argc, char* argv[] )
{

	printf ("error is: %lx\n", TESTLINE_DOWN_ERROR);
	printf ("Profile is:\n");
	switch (CHECK_SEVERITY(TESTLINE_DOWN_ERROR))
	{
	case INFORMATION_ERROR:
		printf ("Information Error\n");
		break;
	case NORMAL_ERROR:
		printf ("Normal Error\n");
		break;
	case CRITICAL_ERROR:
		printf ("Critical Error\n");
		break;
	case TERMINATE_ERROR:
		printf ("Terminate Error\n");
		break;
	}
	switch (CHECK_TYPE(TESTLINE_DOWN_ERROR))
	{
	case APPLICATION:
		printf ("Application Error\n");
		break;
	case SYSTEM:
		printf ("System Error\n");
		break;
	default:
		printf ("Unknown Error\n");
		break;
	}
	switch (CHECK_ACTION(TESTLINE_DOWN_ERROR))
	{
	case LOG:
		printf ("Log Action\n");
		break;
	case OPERATOR:
		printf ("Operator Action\n");
		break;
	default:
		printf ("Unknown Error\n");
		break;
	}
	while (1)
	{
		cout << "Waiting for input\n" ;
		cout.flush();
		WaitForInput (0, 100);
	}

	return 0;
}
