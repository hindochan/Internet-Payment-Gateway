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
 *           Synopsis:  Error handling for LAG
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */
#ifndef LGERRORS_H
#define LGERRORS_H


/*************************************************************************/
/*                                                                       */
/*                                                                       */
/*   |               |               |               |               |   */
/*   -----------------------------------------------------------------   */
/*   | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |   */
/*   |c|c|c|c|c|c|c|c|a|a|a|t|t|s|s|s|e|e|e|e|e|e|e|e|e|e|e|e|e|e|e|e|   */
/*   -----------------------------------------------------------------   */
/*   | CLASS Base (8)|Action   (3)   |       Error Code (16 bits)    |   */
/*                    Type     (2)                                       */
/*                    Severity (3)                                       */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/*************************************************************************/


#define CLASS_APACS			(1 << 24)
#define CLASS_BITFLD		(2 << 24)
#define CLASS_CONFIG		(3 << 24)
#define CLASS_FIELD			(4 << 24)
#define CLASS_FLEPRT		(5 << 24)
#define CLASS_INTFCE		(6 << 24)
#define CLASS_LAGAPP		(7 << 24)
#define CLASS_LAGTRN		(8 << 24)
#define CLASS_LGRINT		(9 << 24)
#define CLASS_LGRSTT		(10 << 24)
#define CLASS_LGRTHR		(11 << 24)
#define CLASS_LOGREC		(12 << 24)
#define CLASS_MAIN			(13 << 24)
#define CLASS_MBDF			(14 << 24)
#define CLASS_MCHINT		(15 << 24)
#define CLASS_MCHSTT		(16 << 24)
#define CLASS_MCHTHR		(17 << 24)
#define CLASS_MSG			(18 << 24)
#define CLASS_OMISO			(19 << 24)
#define CLASS_OMPRT			(20 << 24)
#define CLASS_OPRINT		(21 << 24)
#define CLASS_PORT			(22 << 24)
#define CLASS_STDISO		(23 << 24)
#define CLASS_SVRINT		(24 << 24)
#define CLASS_SVRSTT		(25 << 24)
#define CLASS_SVRTHR		(26 << 24)
#define CLASS_TCPPRT		(27 << 24)
#define CLASS_THREAD		(28 << 24)
#define CLASS_TRAN			(29 << 24)
#define CLASS_TSTMSG		(30 << 24)
#define CLASS_UXAPP			(31 << 24)
#define CLASS_VRFLD			(32 << 24)
#define CLASS_VRLFLD		(33 << 24)
#define CLASS_X25PRT		(34 << 24)

/* ERROR ACTIONS */
#define	LOG					(1)
#define	OPERATOR			(2)
#define ACTION_LOG			(LOG << 21)
#define ACTION_OPERATOR		(OPERATOR << 21)

/* ERROR TYPES */
#define	APPLICATION			(1)
#define	SYSTEM				(2)
#define TYPE_APPLICATION	(APPLICATION << 19)
#define TYPE_SYSTEM			(SYSTEM << 19)

/* ERROR SEVERITY */
#define INFORMATION_ERROR	(1)
#define NORMAL_ERROR		(2)
#define CRITICAL_ERROR		(3)
#define TERMINATE_ERROR		(4)

#define INFORMATION(e)		((INFORMATION_ERROR << 16) | e)
#define NORMAL(e)			((NORMAL_ERROR << 16) | e)
#define CRITICAL(e)			((CRITICAL_ERROR << 16) | e)
#define TERMINATE(e)		((TERMINATE_ERROR << 16) | e)

#define TESTLINE_DOWN_ERROR ((CLASS_PORT|ACTION_LOG|TYPE_SYSTEM|CRITICAL(1)))

/* Check different bits of the error */
#define CHECK_SEVERITY(e)	((e & (7 << 16)) >> 16)
#define CHECK_TYPE(e)		((e & (3 << 19)) >> 19)
#define CHECK_ACTION(e)		((e & (7 << 21)) >> 21)
#define CHECK_CLASS(e)		((e & (0xff << 24)))

#define OK 0

/**************************************************************************/
/*    Error Codes for CLASS_APACS                                         */
/**************************************************************************/
#define INVALID_MESSAGE_ID \
			((CLASS_APACS|ACTION_LOG|TYPE_APPLICATION|NORMAL(1)))
#define RESPONSE_MISMATCH \
			((CLASS_APACS|ACTION_LOG|TYPE_APPLICATION|NORMAL(2)))

/**************************************************************************/
/*    Error Codes for CLASS_BITFLD                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_CONFIG                                        */
/**************************************************************************/
#define SRVRINT_MISSING \
			((CLASS_CONFIG|ACTION_LOG|TYPE_APPLICATION|TERMINATE(1)))
#define MRCHINT_MISSING \
			((CLASS_CONFIG|ACTION_LOG|TYPE_APPLICATION|TERMINATE(2)))
#define LGGRINT_MISSING \
			((CLASS_CONFIG|ACTION_LOG|TYPE_APPLICATION|TERMINATE(3)))
#define OPERINT_MISSING \
			((CLASS_CONFIG|ACTION_LOG|TYPE_APPLICATION|TERMINATE(4)))

/**************************************************************************/
/*    Error Codes for CLASS_FIELD                                         */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_FLEPRT                                        */
/**************************************************************************/
#define FILE_WRITE_ERROR \
			((CLASS_FLEPRT|ACTION_LOG|TYPE_APPLICATION|TERMINATE(1)))

/**************************************************************************/
/*    Error Codes for CLASS_INTFCE                                        */
/**************************************************************************/
#define MEMORY_ERROR ((CLASS_INTFCE|ACTION_LOG|TYPE_SYSTEM|TERMINATE(1)))
#define TRANSACTION_IN_SYSTEM \
			((CLASS_INTFCE|ACTION_LOG|TYPE_APPLICATION|INFORMATION(2)))

/**************************************************************************/
/*    Error Codes for CLASS_LAGAPP                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_LAGTRN                                        */
/**************************************************************************/
#define INVALID_TRAN ((CLASS_LAGTRN|ACTION_LOG|TYPE_APPLICATION|NORMAL(1)))
#define INVALID_MERCHANT ((CLASS_LAGTRN|ACTION_OPERATOR|TYPE_APPLICATION|NORMAL(2)))
#define LOG_ONLY ((CLASS_LAGTRN|ACTION_LOG|TYPE_APPLICATION|INFORMATION(3)))

/**************************************************************************/
/*    Error Codes for CLASS_LGRINT                                        */
/**************************************************************************/
#define NO_CURRENT_LOGFILE \
			((CLASS_LGRINT|ACTION_LOG|TYPE_APPLICATION|CRITICAL(1)))

/**************************************************************************/
/*    Error Codes for CLASS_LGRSTT                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_LGRTHR                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_LOGREC                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_MAIN                                          */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_MBDF                                          */
/**************************************************************************/
#define MERCHANT_NOT_FOUND ((CLASS_MBDF|ACTION_LOG|TYPE_APPLICATION|NORMAL(1)))

/**************************************************************************/
/*    Error Codes for CLASS_MCHINT                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_MCHSTT                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_MCHTHR                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_MSG                                           */
/**************************************************************************/
#define INVALID_MESSAGE ((CLASS_MSG|ACTION_LOG|TYPE_APPLICATION|NORMAL(1)))
#define INVALID_FIELD ((CLASS_MSG|ACTION_LOG|TYPE_APPLICATION|NORMAL(2)))
#define MESSAGE_LENGTH_ERROR ((CLASS_MSG|ACTION_LOG|TYPE_APPLICATION|NORMAL(3)))

/**************************************************************************/
/*    Error Codes for CLASS_OMISO                                         */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_OMPRT                                         */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_OPRINT                                        */
/**************************************************************************/
#define UNKNOWN_OPERATOR_COMMAND \
			((CLASS_OPRINT|ACTION_LOG|TYPE_APPLICATION|NORMAL(1)))
#define COMMIT_HARIKIRI \
			((CLASS_OPRINT|ACTION_LOG|TYPE_APPLICATION|INFORMATION(2)))

/**************************************************************************/
/*    Error Codes for CLASS_PORT                                          */
/**************************************************************************/
#define LINEDOWN_CONDITION \
			((CLASS_PORT|ACTION_LOG|TYPE_APPLICATION|NORMAL(1)))
#define TIMEOUT_CONDITION \
			((CLASS_PORT|ACTION_LOG|TYPE_APPLICATION|NORMAL(2)))
#define PORT_INIT_ERROR \
			((CLASS_PORT|ACTION_LOG|TYPE_APPLICATION|TERMINATE(3)))
#define END_OF_FILE \
			((CLASS_PORT|ACTION_LOG|TYPE_APPLICATION|NORMAL(4)))
#define READ_ERROR \
			((CLASS_PORT|ACTION_LOG|TYPE_APPLICATION|NORMAL(5)))
#define WRITE_ERROR \
			((CLASS_PORT|ACTION_LOG|TYPE_APPLICATION|NORMAL(6)))
#define POLL_ERROR \
			((CLASS_PORT|ACTION_LOG|TYPE_APPLICATION|CRITICAL(7)))
#define SYSTEM_ERROR \
			((CLASS_PORT|ACTION_LOG|TYPE_APPLICATION|CRITICAL(8)))

/**************************************************************************/
/*    Error Codes for CLASS_STDISO                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_SVRINT                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_SVRSTT                                        */
/**************************************************************************/
#define LOGGER_UNAVAILABLE \
			((CLASS_SVRSTT|ACTION_LOG|TYPE_APPLICATION|TERMINATE(1)))
#define MERCHANT_UNAVAILABLE \
			((CLASS_SVRSTT|ACTION_OPERATOR|TYPE_APPLICATION|NORMAL(2)))

/**************************************************************************/
/*    Error Codes for CLASS_SVRTHR                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_TCPPRT                                        */
/**************************************************************************/
#define GETHOSTENT_ERROR \
			((CLASS_TCPPRT|ACTION_LOG|TYPE_SYSTEM|CRITICAL(1)))
#define BIND_ERROR \
			((CLASS_TCPPRT|ACTION_LOG|TYPE_SYSTEM|CRITICAL(2)))
#define TCPIP_SOCKET_ERROR \
			((CLASS_TCPPRT|ACTION_LOG|TYPE_SYSTEM|CRITICAL(3)))
#define INVALID_POLLFD_ERROR \
			((CLASS_TCPPRT|ACTION_LOG|TYPE_SYSTEM|CRITICAL(4)))
#define TCPIP_LISTEN_ERROR \
			((CLASS_TCPPRT|ACTION_LOG|TYPE_SYSTEM|TERMINATE(5)))
#define TCPIP_LINE_DOWN \
			LINEDOWN_CONDITION


/**************************************************************************/
/*    Error Codes for CLASS_THREAD                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_TRAN                                          */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_TSTMSG                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_UXAPP                                         */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_VRFLD                                         */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_VRLFLD                                        */
/**************************************************************************/

/**************************************************************************/
/*    Error Codes for CLASS_X25PRT                                         */
/**************************************************************************/
#define X25_ADDRESS_ERROR \
				((CLASS_X25PRT|ACTION_LOG|TYPE_SYSTEM|CRITICAL(1)))
#define X25_COMMS_ERROR \
				((CLASS_X25PRT|ACTION_LOG|TYPE_SYSTEM|CRITICAL(2)))
#define UNEXPECTED_NETWORK_EVENT \
				((CLASS_X25PRT|ACTION_LOG|TYPE_SYSTEM|CRITICAL(3)))
#define INVALID_EVENT \
				((CLASS_X25PRT|ACTION_LOG|TYPE_SYSTEM|NORMAL(4)))
#define X25_LINE_DOWN \
				LINEDOWN_CONDITION

#endif // LGERRORS_H
