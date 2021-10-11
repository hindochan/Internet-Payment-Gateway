/***********************************************************************
 *  LAG - Source file header
 *
 *              Layer:	Main module
 *             Module:	LGmain.cpp
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
#include "LGlagapp.h"

LAGApplication oTheApplication;

int main( int argc, char* argv[] )
{
	STATUS nStatus = 0;

	cout << "LOGICA ACQUIRER GATEWAY STARTING....";
	cout << "\n";
	cout << "\n";
	cout.flush();

	// Read Config file in here

	nStatus = oTheApplication.LoadConfigFile();

	if (nStatus)
	{
		cerr << "Error Loading Config File\n";
		fprintf (stderr, "Error Code :- %lx\n", nStatus);
		cerr.flush();
		exit(nStatus);
	}

	nStatus = oTheApplication.OpenLoggers();

	if (nStatus)
	{
		cerr << "Error Opening Logger Interfaces \n";
		fprintf (stderr, "Error Code :- %lx\n", nStatus);
		cerr.flush();
		exit(nStatus);
	}

	nStatus = oTheApplication.OpenMerchants();

	if (nStatus)
	{
		cerr << "Error Opening Merchant Interfaces \n";
		fprintf (stderr, "Error Code :- %lx\n", nStatus);
		cerr.flush();
		exit(nStatus);
	}

	nStatus = oTheApplication.OpenServers();

	if (nStatus)
	{
		cerr << "Error Opening Server Interfaces \n";
		fprintf (stderr, "Error Code :- %lx\n", nStatus);
		cerr.flush();
		exit(nStatus);
	}

	nStatus = oTheApplication.OpenOperator();

	if (nStatus)
	{
		cerr << "Error Opening Operator Interface \n";
		fprintf (stderr, "Error Code :- %lx\n", nStatus);
		cerr.flush();
	}

	nStatus = oTheApplication.CloseServers();

	if (nStatus)
	{
		cerr << "Error Closing Servers \n";
		fprintf (stderr, "Error Code :- %lx\n", nStatus);
		cerr.flush();
	}

	nStatus = oTheApplication.CloseMerchants();

	if (nStatus)
	{
		cerr << "Error Closing Merchants \n";
		fprintf (stderr, "Error Code :- %lx\n", nStatus);
		cerr.flush();
	}

	nStatus = oTheApplication.CloseLoggers();

	if (nStatus)
	{
		cerr << "Error Closing Loggers \n";
		fprintf (stderr, "Error Code :- %lx\n", nStatus);
		cerr.flush();
	}

	nStatus = oTheApplication.CloseOperator();

	if (nStatus)
	{
		cerr << "Error Closing Operator Interface \n";
		fprintf (stderr, "Error Code :- %lx\n", nStatus);
		cerr.flush();
	}

	return nStatus;
}
