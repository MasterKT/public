/*
 * windows-specific implementation of the lsUtilities extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "lsUtilities_internal.h"

#include <signal.h>

//------------------------------------------------------------------------------
// Signal Handler
//------------------------------------------------------------------------------
void lsExceptionHandler(int mHandler)
{

}

//------------------------------------------------------------------------------
// Statics
//------------------------------------------------------------------------------
lsDeviceStorage* g_Storage = NULL;

s3eResult lsUtilitiesInit_platform()
{
	g_Storage = new lsDeviceStorage();
    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;
}

void lsUtilitiesTerminate_platform()
{
    if(g_Storage)
		delete g_Storage;
}

void lsUtilIOSPreventBackup_platform(const char * file)
{
}

void lsUtilInstallExceptionHandler_platform()
{
   signal(SIGABRT,	lsExceptionHandler);
   signal(SIGILL,	lsExceptionHandler);
   signal(SIGSEGV,	lsExceptionHandler);
   signal(SIGFPE,	lsExceptionHandler);
}

void lsUtilUninstallExceptionHandler_platform()
{
   signal(SIGABRT,	SIG_DFL);
   signal(SIGILL,	SIG_DFL);
   signal(SIGSEGV,	SIG_DFL);
   signal(SIGFPE,	SIG_DFL);
}

lsDeviceClass lsUtilGetDeviceClassInt_platform()
{
	return eGeneric;
}

const char * lsUtilGetDeviceClassString_platform()
{
	return "generic";
}


void lsDeviceSetTabletThreshold_platform(int threshold)
{
	// does nothing, currently android only
}

lsDeviceStorage* lsUtilGetDeviceStorageInformation_platform()
{
	return g_Storage;
}


