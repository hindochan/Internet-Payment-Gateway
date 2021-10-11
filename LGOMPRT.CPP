/***********************************************************************
 *  LAG - Source file header
 *
 *              Layer:  
 *             Module: 
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

#include "LGomprt.h"

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OMTCPIPPort::OMTCPIPPort
 *
 *           Synopsis:  Constructor for class object
 *
 *   Input parameters:  Comm type & config string
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
OMTCPIPPort::OMTCPIPPort(WORD nType, RWCString strCommsConfig) :
	TCPIPPort(nType, strCommsConfig),
	m_oBitMapField(8),
	m_o3DigVarField(3)
{
	m_nMsgHdrInLen = 0;
	m_nMsgTrlInLen = 0;
	m_nMsgHdrOutLen = 0;
	m_nMsgTrlOutLen = 0;
	// m_cMsgTrlOut[0] = 0x0d;
	m_nInitialReadLength = 18;
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OMTCPIPPort::~OMTCPIPPort
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
OMTCPIPPort::~OMTCPIPPort()
{
}

/****************************************************************************
 *
 *   MEMBER FUNCTION DEFINITION
 *
 *               Name:  OMTCPIPPort::ValidateMessageFromLength
 *
 *           Synopsis:  Validates if all the data has been read
 *
 *   Input parameters:  None
 *
 *  Output parameters:  None
 *
 *       Return value:	BOOL 
 *
 *        Description:
 *
 *              Notes:
 *
 ****************************************************************************
 */
BOOL OMTCPIPPort::ValidateMessageFromLength ()
{
	BOOL bReturnValue = TRUE;
	BOOL bBreakCalled = FALSE;
	BOOL bSecondBMProcessed = FALSE;
	WORD nIndex = 0;
	WORD nExpectedLen = 0;
	char *pcPos = NULL;

	if (m_stCommsBuffer.nMessageLen < m_nInitialReadLength)
		bReturnValue =  FALSE;
	else
	{
		nExpectedLen = m_nMsgHdrInLen + 18;
		pcPos = &m_stCommsBuffer.cMessage[m_nMsgHdrInLen + 10];
		m_oBitMapField.Set(&pcPos);

		for (nIndex = 0;
			nIndex < (8 * m_oBitMapField.Length()) && !bBreakCalled;
			nIndex++)
		{
			if (m_oBitMapField.IsBitSet(nIndex + 1))
			{
				switch (nIndex + 1)
				{
				case 1:
					nExpectedLen += 8;
					if (nExpectedLen <= m_stCommsBuffer.nMessageLen)
						pcPos += 8;
					else
						bBreakCalled = TRUE;
					break;

				case 3:
				case 11:
				case 12:
				case 38:
					nExpectedLen += 6;
					if (nExpectedLen <= m_stCommsBuffer.nMessageLen)
						pcPos += 6;
					else
						bBreakCalled = TRUE;
					break;

				case 4:
					nExpectedLen += 12;
					if (nExpectedLen <= m_stCommsBuffer.nMessageLen)
						pcPos += 12;
					else
						bBreakCalled = TRUE;
					break;

				case 7:
					nExpectedLen += 10;
					if (nExpectedLen <= m_stCommsBuffer.nMessageLen)
						pcPos += 10;
					else
						bBreakCalled = TRUE;
					break;

				case 13:
				case 17:
					nExpectedLen += 4;
					if (nExpectedLen <= m_stCommsBuffer.nMessageLen)
						pcPos += 4;
					else
						bBreakCalled = TRUE;
					break;

				case 35:
				case 44:
					nExpectedLen +=2;
					if (nExpectedLen <= m_stCommsBuffer.nMessageLen)
					{
						m_oStdVarField.Set(&pcPos);
						nExpectedLen += m_oStdVarField.Length();
					}
					else
						bBreakCalled = TRUE;
					break;

				case 37:
					nExpectedLen += 9;
					if (nExpectedLen <= m_stCommsBuffer.nMessageLen)
						pcPos += 9;
					else
						bBreakCalled = TRUE;
					break;

				case 39:
					nExpectedLen += 2;
					if (nExpectedLen <= m_stCommsBuffer.nMessageLen)
						pcPos += 2;
					else
						bBreakCalled = TRUE;
					break;

				case 48:
				case 60:
				case 61:
					nExpectedLen +=3;
					if (nExpectedLen <= m_stCommsBuffer.nMessageLen)
					{
						m_o3DigVarField.Set(&pcPos);
						nExpectedLen += m_o3DigVarField.Length();
					}
					else
						bBreakCalled = TRUE;
					break;

				default:
					bBreakCalled = TRUE;
				}
			}
		}

		if (!bBreakCalled)
		{
			if (m_oBitMapField.IsBitSet(1))
			{
				pcPos = &m_stCommsBuffer.cMessage[m_nMsgHdrInLen + 18];
				m_oBitMapField.Set(&pcPos);
				pcPos = &m_stCommsBuffer.cMessage[nExpectedLen];
				for (nIndex = 0;
					nIndex < (8 * m_oBitMapField.Length()) && !bBreakCalled;
					nIndex++)
				{
					if (m_oBitMapField.IsBitSet(nIndex + 1))
					{
						switch (nIndex + 1)
						{
							case 58:
							case 63:
							nExpectedLen +=3;
							if (nExpectedLen <= m_stCommsBuffer.nMessageLen)
							{
								m_o3DigVarField.Set(&pcPos);
								nExpectedLen += m_o3DigVarField.Length();
							}
							else
								bBreakCalled = TRUE;
							break;

						default:
							bBreakCalled = TRUE;
						}
					}
				}
			}
		}
	}

	if (m_stCommsBuffer.nMessageLen  < nExpectedLen || bBreakCalled)
		bReturnValue =  FALSE;

	return bReturnValue;
}
