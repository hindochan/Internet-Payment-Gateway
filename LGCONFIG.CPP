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

#include "LGconfig.h"
#include "LGglobls.h"
#include "LGtypes.h"

static void TraceWrite (char *cText)
{
	if (TRACE)
	{
		cout << cText;
		cout.flush();
	}
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::LGConfiguration
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
LGConfiguration::LGConfiguration()
{
	// Initialise variables
	m_nNbrServerInterfaces = 0;
	m_nNbrMerchantInterfaces = 0;
	m_nNbrLoggerInterfaces = 0;
	m_nNbrOperatorInterfaces = 1;
}

LGConfiguration::LGConfiguration(RWTPtrDlist<ServerThread> *pllSrvrThrds,
								RWTPtrDlist<MerchantThread> *pllMrchThrds,
								RWTPtrDlist<LoggerThread> *pllLggrThrds,
								RWTPtrDlist<OperatorThread> *pllOprThrds)
{
	// Initialise variables
	LGConfiguration();
	m_pllServerThrdLL = pllSrvrThrds;
	m_pllMerchantThrdLL = pllMrchThrds;
	m_pllLoggerThrdLL = pllLggrThrds;
	m_pllOperatorThrdLL = pllOprThrds;
}


/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::~LGConfiguration
 *
 *           Synopsis:  Destructor for class object
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
LGConfiguration::~LGConfiguration()
{

}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::LoadConfiguration
 *
 *           Synopsis:  Loads the configuration
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description: Format of file is:
 *			#
 *			#
 *			# LAG CONFIGURATION FILE
 *			#
 *			#
 *			LAGTracingFlag=Y
 *			LAGASFPrefix=asf
 *			LAGResponseTimeout=120
 *			LAGOperatorName=hawleya@logica.com
 *			#LAGServerInterface=OMISS:2:3:1:billy,8044,LISTEN
 *			#LAGServerInterface=OMISS:1:21:8:1065,LISTEN
 *			#LAGMerchantInterface=BMS:0:0:1:/LAG/mrch.0
 *			LAGMerchantInterface=BMS:3:2:1:0.234261601124,CALL,SVC,/dev/x25
 *			LAGLoggerInterface=TLF:1:0:1:/LAG/LGtlf.date
 *			LAGOperatorInterface=OPERATOR:4:1:1:billy,8001,LISTEN
 *			CMDOperatorInterface=OPERATOR:4:1:1:billy,8001,CALL
 *
 *
 *              Notes:
 *
 ****************************************************************************
 */
STATUS LGConfiguration::LoadConfiguration ()
{
	char *pcEnvVar;
	RWCString strLine;
	RWCString strToken;
	RWCString strKey, strValue;
	STATUS nStatus = 0;
	BOOL bSrvrIntPresent = FALSE;
	BOOL bMrchIntPresent = FALSE;
	BOOL bLggrIntPresent = FALSE;
	BOOL bOperIntPresent = FALSE;

	pcEnvVar = getenv( "LAGCONFIG" );
	if (pcEnvVar == NULL)
	{
		cout << "LAGCONFIG variable not defined\n";
		cout << "Exiting..\n";
		cout.flush();
		exit(0);
	}

	ifstream ConfigFile (pcEnvVar);

	// Read Config file in here
	while (strLine.readLine (ConfigFile, TRUE))
	{
	RWCTokenizer next (strLine);

		if (strLine (0) == '#')
        {
			continue;
        }

		strKey = next("=");
		strValue = next ("=");

		if (strKey == "LAGASFPrefix")
		{
			m_strAsfPrefix = strValue;
			continue;
		}
		if (strKey == "LAGTracingFlag")
		{
			m_strTracingFlag = strValue;
			continue;
		}
		if (strKey == "LAGResponseTimeout")
		{
			m_nResponseTimeout = atoi (strValue.data());
			continue;
		}
		if (strKey == "LAGOperatorName")
		{
			m_strOperatorName = strValue;
			continue;
		}
		if (strKey == "LAGMBDFFileName")
		{
			m_strMBDFFileName = strValue;
			continue;
		}
		if (strKey == "LAGServerInterface")
		{
		RWCTokenizer next (strValue);
		RWCString strToken;
		int iCount = 0;
		bSrvrIntPresent = TRUE;
		ServerInterface *NewServer = new ServerInterface (m_pllServerThrdLL);

			if (NewServer == (ServerInterface*)NULL)
			{
			// Error recovery required ??
			}
			NewServer->InterfaceNbr() = m_nNbrServerInterfaces;

			NewServer->Key () = strKey;
			while (!(strToken = next(":")).isNull())
			{
				switch (iCount)
				{
				case 0: // Server Name
					NewServer->Name () = strToken;
					iCount++;
					break;
				case 1: // Msg Type
					NewServer->MsgType () = atoi (strToken.data());
					iCount++;
					break;
				case 2: // Comm Type
					NewServer->CommType () = atoi (strToken.data());
					iCount++;
					break;
				case 3: // No of Ports
					NewServer->NbrPorts () = atoi (strToken.data());
					iCount++;
					break;
				case 4: // Comm Specific Data
					NewServer->CommsConfig () = strToken;
					iCount++;
					break;
				default:
					break;
				}
			}
			m_llServerInterface.insert ((Interface*)NewServer);
			m_nNbrServerInterfaces++;
			continue;
		}
		if (strKey == "LAGMerchantInterface")
		{
		RWCTokenizer next (strValue);
		RWCString strToken;
		int iCount = 0;
		bMrchIntPresent = TRUE;
		MerchantInterface *NewMerchant =
			new MerchantInterface (m_pllMerchantThrdLL);

			if (NewMerchant == (MerchantInterface*)NULL)
			{
			}
			NewMerchant->InterfaceNbr() = m_nNbrMerchantInterfaces;

			NewMerchant->Key () = strKey;
			while (!(strToken = next(":")).isNull())
			{
				switch (iCount)
				{
				case 0: // Merchant Name
					NewMerchant->Name () = strToken;
					iCount++;
					break;
				case 1: // Data Type
					NewMerchant->MsgType () = atoi (strToken.data());
					iCount++;
					break;
				case 2: // Comm Type
					NewMerchant->CommType () = atoi (strToken.data());
					iCount++;
					break;
				case 3: // No of Ports
					NewMerchant->NbrPorts () = atoi (strToken.data());
					iCount++;
					break;
				case 4: // Comm Specific Data
					NewMerchant->CommsConfig () = strToken;
					iCount++;
					break;
				default:
					break;
				}
			}
			m_llMerchantInterface.insert ((Interface*)NewMerchant);
			m_nNbrMerchantInterfaces++;
			continue;
		}
		if (strKey == "LAGLoggerInterface")
		{
		RWCTokenizer next (strValue);
		RWCString strToken;
		int iCount = 0;
		bLggrIntPresent = TRUE;
		LoggerInterface *NewLogger = new LoggerInterface (m_pllLoggerThrdLL);

			if (NewLogger == (LoggerInterface*)NULL)
			{
			}
			NewLogger->InterfaceNbr() = m_nNbrLoggerInterfaces;

			NewLogger->Key () = strKey;
			while (!(strToken = next(":")).isNull())
			{
				switch (iCount)
				{
				case 0: // Logger Name
					NewLogger->Name () = strToken;
					iCount++;
					break;
				case 1: // Data Type
					NewLogger->MsgType () = atoi (strToken.data());
					iCount++;
					break;
				case 2: // Comm Type
					NewLogger->CommType () = atoi (strToken.data());
					iCount++;
					break;
				case 3: // No of Ports
					NewLogger->NbrPorts () = atoi (strToken.data());
					iCount++;
					break;
				case 4: // Comm Specific Data
					NewLogger->CommsConfig () = strToken;
					iCount++;
					break;
				default:
					break;
				}
			}
			m_llLoggerInterface.insert ((Interface*)NewLogger);
			m_nNbrLoggerInterfaces++;
			continue;
		}
		if (strKey == "LAGOperatorInterface")
		{
		RWCTokenizer next (strValue);
		RWCString strToken;
		int iCount = 0;
		bOperIntPresent = TRUE;
		OperatorInterface *Operator
							= new OperatorInterface (m_pllOperatorThrdLL);

			if (Operator == (OperatorInterface*)NULL)
			{
			// Error recovery required
			}
			Operator->InterfaceNbr() = m_nNbrOperatorInterfaces;

			Operator->Key () = strKey;
			while (!(strToken = next(":")).isNull())
			{
				switch (iCount)
				{
				case 0: // Server Name
					Operator->Name () = strToken;
					iCount++;
					break;
				case 1: // Msg Type
					Operator->MsgType () = atoi (strToken.data());
					iCount++;
					break;
				case 2: // Comm Type
					Operator->CommType () = atoi (strToken.data());
					iCount++;
					break;
				case 3: // No of Ports
					Operator->NbrPorts () = atoi (strToken.data());
					iCount++;
					break;
				case 4: // Comm Specific Data
					Operator->CommsConfig () = strToken;
					iCount++;
					break;
				default:
					break;
				}
			}
			m_llOperatorInterface.insert ((Interface*)Operator);
			m_nNbrOperatorInterfaces++;
			continue;
		}
		if (strKey == "CMDOperatorInterface")
		{
		RWCTokenizer next (strValue);
		RWCString strToken;
		int iCount = 0;
		OperatorInterface *Operator
						= new OperatorInterface (m_pllOperatorThrdLL);

			if (Operator == (OperatorInterface*)NULL)
			{
			// Error recovery required
			}
			Operator->InterfaceNbr() = m_nNbrOperatorInterfaces;

			Operator->Key () = strKey;
			while (!(strToken = next(":")).isNull())
			{
				switch (iCount)
				{
				case 0: // Server Name
					Operator->Name () = strToken;
					iCount++;
					break;
				case 1: // Msg Type
					Operator->MsgType () = atoi (strToken.data());
					iCount++;
					break;
				case 2: // Comm Type
					Operator->CommType () = atoi (strToken.data());
					iCount++;
					break;
				case 3: // No of Ports
					Operator->NbrPorts () = atoi (strToken.data());
					iCount++;
					break;
				case 4: // Comm Specific Data
					Operator->CommsConfig () = strToken;
					iCount++;
					break;
				default:
					break;
				}
			}
			m_llOperatorInterface.insert ((Interface*)Operator);
			m_nNbrOperatorInterfaces++;
			continue;
		}
		else
		{
		char	buffer[128];
			// Unknown Line in the file, Log it and continue.
			sprintf (buffer,"Unknown line: %s\n", strLine.data());
			TraceWrite (buffer);
			continue;
		}
	}

	if (!nStatus)
	{
		if (!bSrvrIntPresent)
			nStatus = SRVRINT_MISSING;
		else
		{
			if (!bMrchIntPresent)
				nStatus = MRCHINT_MISSING;
			else
			{
				if (!bLggrIntPresent)
					nStatus = LGGRINT_MISSING;
				else
				{
					if (!bOperIntPresent)
						nStatus = OPERINT_MISSING;
				}
			}
		}
	}

	return nStatus;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetNbrServerInterfaces
 *
 *           Synopsis:  Returns the number of Server Interfaces in the config
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
WORD LGConfiguration::GetNbrServerInterfaces ()
{
	return m_nNbrServerInterfaces;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetNbrMerchantInterfaces
 *
 *           Synopsis:  Returns the number of Merchant Interfaces in the config
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
WORD LGConfiguration::GetNbrMerchantInterfaces ()
{
	return m_nNbrMerchantInterfaces;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetNbrLoggerInterfaces
 *
 *           Synopsis:  Returns the number of Logger Interfaces
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
WORD LGConfiguration::GetNbrLoggerInterfaces ()
{
	return m_nNbrLoggerInterfaces;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetNbrOperatorInterfaces
 *
 *           Synopsis:  Returns the number of Operator Interfaces
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
WORD LGConfiguration::GetNbrOperatorInterfaces ()
{
	return m_nNbrOperatorInterfaces;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetASFPrefix
 *
 *           Synopsis:  Returns the ASF Prefix from the config.
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
RWCString	LGConfiguration::GetASFPrefix ()
{
	return m_strAsfPrefix;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetTracingFlag
 *
 *           Synopsis:  Returns the TracingFlag from the config file.
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
RWCString	LGConfiguration::GetTracingFlag ()
{
	return m_strTracingFlag;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetResponseTimeout
 *
 *           Synopsis:  Returns the Response Timeout from the config
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
WORD		LGConfiguration::GetResponseTimeout ()
{
	return m_nResponseTimeout;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetMBDFFileName
 *
 *           Synopsis:  Returns the MBDFFileName from the Config
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
RWCString	LGConfiguration::GetMBDFFileName ()
{
	return m_strMBDFFileName;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetOperatorName
 *
 *           Synopsis:  Returns the OperatorName from the Config
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
RWCString	LGConfiguration::GetOperatorName ()
{
	return m_strOperatorName;
}

RWBoolean CheckInterfaceNbr (Interface *interface, void *id)
{
WORD	*i = (WORD*)id;

	if (interface->InterfaceNbr() == *i) return TRUE;
	else return FALSE;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetServerInterface
 *
 *           Synopsis:  Returns the specified ServerInterface in the config
 *
 *   Input parameters:  index into the Server Interface table
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
ServerInterface	*LGConfiguration::GetServerInterface (WORD i)
{
ServerInterface *m_ptrServerInterface = (ServerInterface*)
						m_llServerInterface.find(CheckInterfaceNbr, (void*)&i);
	return m_ptrServerInterface;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetLoggerInterface
 *
 *           Synopsis:  Returns the Specified LoggerInterface in the config
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
LoggerInterface	*LGConfiguration::GetLoggerInterface (WORD i)
{
LoggerInterface *m_ptrLoggerInterface = (LoggerInterface*)
					m_llLoggerInterface.find(CheckInterfaceNbr, (void*)&i);
	return m_ptrLoggerInterface;
}

RWBoolean CheckKeyNamePrefix (Interface *interface, void *str)
{
char	*pcPrefix = (char*)str;

	if (interface->Key()(0,3) == pcPrefix) return TRUE;
	else return FALSE;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetOperatorInterface
 *
 *           Synopsis:  Returns the OperatorInterface in the config
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
OperatorInterface	*LGConfiguration::GetOperatorInterface ()
{
OperatorInterface *m_ptrOperatorInterface = (OperatorInterface*)
			m_llOperatorInterface.find(CheckKeyNamePrefix, (void*)"LAG");
	return m_ptrOperatorInterface;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetCommandInterface
 *
 *           Synopsis:  Returns the specified CommandInterface in the config
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
OperatorInterface	*LGConfiguration::GetCommandInterface ()
{
OperatorInterface *m_ptrOperatorInterface = (OperatorInterface*)
			m_llOperatorInterface.find(CheckKeyNamePrefix, (void*)"CMD");
	return m_ptrOperatorInterface;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  LGConfiguration::GetMerchantInterface
 *
 *           Synopsis:  Returns the Specified MerchantInterface in the config
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
MerchantInterface	*LGConfiguration::GetMerchantInterface (WORD i)
{
MerchantInterface *m_ptrMerchantInterface = (MerchantInterface*)
					m_llMerchantInterface.find(CheckInterfaceNbr, (void*)&i);
	return m_ptrMerchantInterface;
}
