/***********************************************************************
 *  LAG - Source file header
 *
 *              Layer:  
 *             Module:  
 *           $Logfile:   $
 *         Originator:  Nilesh Hindocha
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
#ifndef LGcommand_h
#define LGcommand_h

static struct
{
	ThreadStatus enThreadStatus;
	char *cText;
} ThreadStatusToText[] = 
	{
		UNINITIALISED,	"UNINITIALISED",
		OKIDLE,			"OKIDLE",
		BUSYLINE,		"BUSYLINE",
		BUSYTRAN,		"BUSYTRAN",
		LINEDOWN,		"LINEDOWN",
		DEAD,			"DEAD"
	};

#endif // LGcommand_h
