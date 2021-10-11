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
 *           Synopsis:  General stuff
 *
 *              Notes:  None.
 *
 ************************************************************************
 *
 *  (C) Copyright 1996, Logica UK Ltd. All rights reserved.
 *
 ************************************************************************
 */

#ifndef LGTYPES_H
#define LGTYPES_H

/////////////////////////////////////////////////////////////////////////////
// Standard header files

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <time.h>
#include <assert.h>

#include <pthread.h>

#include "LGerrors.h"

/////////////////////////////////////////////////////////////////////////////
// Basic types

typedef unsigned char  BYTE;   // 8-bit unsigned entity
typedef unsigned short WORD;   // 16-bit unsigned number
typedef unsigned int   UINT;   // machine sized unsigned number (preferred)
typedef long           LONG;   // 32-bit signed number
typedef unsigned long  DWORD;  // 32-bit unsigned number
typedef unsigned char  BOOL;   // BOOLean (0 or !=0)
// typedef signed short   STATUS; // Return value
typedef unsigned long  STATUS; // Return value

/////////////////////////////////////////////////////////////////////////////
// Thread types

typedef pthread_t THREAD;
typedef pthread_attr_t THREAD_ATTR;

typedef pthread_mutex_t MUTEX;
typedef pthread_mutexattr_t MUTEX_ATTR;

typedef pthread_cond_t CONDVAR;
typedef pthread_condattr_t CONDVAR_ATTR;

/////////////////////////////////////////////////////////////////////////////
// Standard constants

#undef FALSE
#undef TRUE
#undef NULL

#define FALSE   0
#define TRUE    1
#define NULL    0

#define THIS_FILE          __FILE__
#define THIS_LINE          __LINE__

/////////////////////////////////////////////////////////////////////////////
// Helper Macros

#define ASSERT(f) assert(f)
#define TRACE oTheApplication.IsTracingOn()

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif // LGTYPES_H
