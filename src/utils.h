/*----------------------------------------------------------------------------

NAME
Utils.h

PURPOSE
Defines for the general-purpose functions for the WinTab demos.

COPYRIGHT
Copyright (c) Wacom Company, Ltd. 2014 All Rights Reserved
All rights reserved.

The text and information contained in this file may be freely used,
copied, or distributed without compensation or licensing restrictions.

---------------------------------------------------------------------------- */
#pragma once

#include	<windows.h>
#include	<stdio.h>
#include	<assert.h>
#include	<stdarg.h>

#include	"wintab.h"		// NOTE: get from wactab header package


//////////////////////////////////////////////////////////////////////////////
#ifdef WACOM_DEBUG

void WacomTrace(char *lpszFormat, ...);

#define WACOM_ASSERT( x ) assert( x )
#define WACOM_TRACE(...)  WacomTrace(__VA_ARGS__)
#else
#define WACOM_TRACE(...)
#define WACOM_ASSERT( x )

#endif // WACOM_DEBUG


#define GETPROCADDRESS(type, func) \
	gp##func = (type)GetProcAddress(ghWintab, #func); \
	if (!gp##func){ WACOM_ASSERT(FALSE);  return FALSE; }


// Ignore warnings about using unsafe string functions.
#pragma warning( disable : 4996 )

//////////////////////////////////////////////////////////////////////////////
// Function pointers to Wintab functions exported from wintab32.dll. 
typedef UINT(API * WTINFOA) (UINT, UINT, LPVOID);
typedef HCTX(API * WTOPENA)(HWND, LPLOGCONTEXTA, BOOL);
typedef BOOL(API * WTGETA) (HCTX, LPLOGCONTEXT);
typedef BOOL(API * WTSETA) (HCTX, LPLOGCONTEXT);
typedef BOOL(API * WTCLOSE) (HCTX);
typedef BOOL(API * WTENABLE) (HCTX, BOOL);
typedef BOOL(API * WTPACKET) (HCTX, UINT, LPVOID);
typedef BOOL(API * WTOVERLAP) (HCTX, BOOL);
typedef BOOL(API * WTSAVE) (HCTX, LPVOID);
typedef BOOL(API * WTCONFIG) (HCTX, HWND);
typedef HCTX(API * WTRESTORE) (HWND, LPVOID, BOOL);
typedef BOOL(API * WTEXTSET) (HCTX, UINT, LPVOID);
typedef BOOL(API * WTEXTGET) (HCTX, UINT, LPVOID);
typedef BOOL(API * WTQUEUESIZESET) (HCTX, int);
typedef int  (API * WTDATAPEEK) (HCTX, UINT, UINT, int, LPVOID, LPINT);
typedef int  (API * WTPACKETSGET) (HCTX, int, LPVOID);


//////////////////////////////////////////////////////////////////////////////

HINSTANCE ghWintab = NULL;
WTINFOA gpWTInfoA = NULL;
WTOPENA gpWTOpenA = NULL;
WTGETA gpWTGetA = NULL;
WTSETA gpWTSetA = NULL;
WTCLOSE gpWTClose = NULL;
WTPACKET gpWTPacket = NULL;
WTENABLE gpWTEnable = NULL;
WTOVERLAP gpWTOverlap = NULL;
WTSAVE gpWTSave = NULL;
WTCONFIG gpWTConfig = NULL;
WTRESTORE gpWTRestore = NULL;
WTEXTSET gpWTExtSet = NULL;
WTEXTGET gpWTExtGet = NULL;
WTQUEUESIZESET gpWTQueueSizeSet = NULL;
WTDATAPEEK gpWTDataPeek = NULL;
WTPACKETSGET gpWTPacketsGet = NULL;

//////////////////////////////////////////////////////////////////////////////
void ShowError(char *pszErrorMessage)
{
	WACOM_TRACE("ShowError()\n");

	WACOM_ASSERT(pszErrorMessage);

	MessageBoxA(NULL, pszErrorMessage, "WINTAB", MB_OK | MB_ICONHAND);
}

BOOL LoadWintab(void)
{
	ghWintab = LoadLibraryA("Wintab32.dll");
	if (!ghWintab)
	{
		DWORD err = GetLastError();
		WACOM_TRACE("LoadLibrary error: %i\n", err);
		ShowError("Could not load Wintab32.dll");
		return FALSE;
	}

	// Explicitly find the exported Wintab functions in which we are interested.
	// We are using the ASCII, not unicode versions (where applicable).
	GETPROCADDRESS(WTOPENA, WTOpenA);
	GETPROCADDRESS(WTINFOA, WTInfoA);
	GETPROCADDRESS(WTGETA, WTGetA);
	GETPROCADDRESS(WTSETA, WTSetA);
	GETPROCADDRESS(WTPACKET, WTPacket);
	GETPROCADDRESS(WTCLOSE, WTClose);
	GETPROCADDRESS(WTENABLE, WTEnable);
	GETPROCADDRESS(WTOVERLAP, WTOverlap);
	GETPROCADDRESS(WTSAVE, WTSave);
	GETPROCADDRESS(WTCONFIG, WTConfig);
	GETPROCADDRESS(WTRESTORE, WTRestore);
	GETPROCADDRESS(WTEXTSET, WTExtSet);
	GETPROCADDRESS(WTEXTGET, WTExtGet);
	GETPROCADDRESS(WTQUEUESIZESET, WTQueueSizeSet);
	GETPROCADDRESS(WTDATAPEEK, WTDataPeek);
	GETPROCADDRESS(WTPACKETSGET, WTPacketsGet);


	// TODO - don't forget to NULL out pointers in UnloadWintab().
	return TRUE;
	}

void UnloadWintab()
{
	WACOM_TRACE("UnloadWintab()\n");

	if (ghWintab)
	{
		FreeLibrary( ghWintab );
		ghWintab = NULL;
	}

	gpWTOpenA			= NULL;
	gpWTClose = NULL;
	gpWTInfoA = NULL;
	gpWTPacket = NULL;
	gpWTEnable = NULL;
	gpWTOverlap = NULL;
	gpWTSave = NULL;
	gpWTConfig = NULL;
	gpWTGetA = NULL;
	gpWTSetA = NULL;
	gpWTRestore = NULL;
	gpWTExtSet = NULL;
	gpWTExtGet = NULL;
	gpWTQueueSizeSet = NULL;
	gpWTDataPeek = NULL;
	gpWTPacketsGet = NULL;
}

//////////////////////////////////////////////////////////////////////////////
#ifdef WACOM_DEBUG

void WacomTrace( char *lpszFormat, ...)
{
	char szTraceMessage[ 128 ];

	int nBytesWritten;

	va_list args;

	WACOM_ASSERT(lpszFormat);

	va_start(args, lpszFormat);

	nBytesWritten = _vsnprintf(szTraceMessage, sizeof(szTraceMessage) - 1,
		lpszFormat, args);

	if (nBytesWritten > 0)
	{
		OutputDebugString(szTraceMessage);
	}

	va_end(args);
}
#define WACOM_ASSERT( x ) assert( x )
#define WACOM_TRACE(...)  WacomTrace(__VA_ARGS__)
#else
#define WACOM_TRACE(...)
#define WACOM_ASSERT( x )

#endif // WACOM_DEBUG

