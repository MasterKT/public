/*
 * Internal header for the lsUtilities extension.
 *
 * This file should be used for any common function definitions etc that need to
 * be shared between the platform-dependent and platform-indepdendent parts of
 * this extension.
 */

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#ifndef LSUTILITIES_INTERNAL_H
#define LSUTILITIES_INTERNAL_H

#include "s3eTypes.h"
#include "lsUtilities.h"
#include "lsUtilities_autodefs.h"


/**
 * Initialise the extension.  This is called once then the extension is first
 * accessed by s3eregister.  If this function returns S3E_RESULT_ERROR the
 * extension will be reported as not-existing on the device.
 */
s3eResult lsUtilitiesInit();

/**
 * Platform-specific initialisation, implemented on each platform
 */
s3eResult lsUtilitiesInit_platform();

/**
 * Terminate the extension.  This is called once on shutdown, but only if the
 * extension was loader and Init() was successful.
 */
void lsUtilitiesTerminate();

/**
 * Platform-specific termination, implemented on each platform
 */
void lsUtilitiesTerminate_platform();
void lsUtilIOSPreventBackup_platform(const char * file);

void lsUtilInstallExceptionHandler_platform();

void lsUtilUninstallExceptionHandler_platform();

lsDeviceClass lsUtilGetDeviceClassInt_platform();

const char * lsUtilGetDeviceClassString_platform();

void lsDeviceSetTabletThreshold_platform(int threshold);

lsDeviceStorage* lsUtilGetDeviceStorageInformation_platform();


#endif /* !LSUTILITIES_INTERNAL_H */