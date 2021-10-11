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
#ifndef LGBITFLD_H
#define LGBITFLD_H

#include "LGfield.h"

/************************************************************************
 *
 *   CLASS DECLARATION
 *
 *               Name:  
 *
 *           Synopsis:  .
 *
 *              Notes:  .
 *
 ************************************************************************
 */

class BitMapField : public Field
{

// Attributes

public:

protected:

private:

// Methods

public:
	// Constructors
	BitMapField(WORD nDataLength);

	// Destructors
	~BitMapField();

	void SetBit(WORD nIndex);
	void ClearBit(WORD nIndex);
	BOOL IsBitSet(WORD nIndex);

protected:

private:

};

#endif // LGBITFLD_H
