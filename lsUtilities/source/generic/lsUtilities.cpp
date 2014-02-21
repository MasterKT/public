/*
Generic implementation of the lsUtilities extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "lsUtilities_internal.h"
s3eResult lsUtilitiesInit()
{
    //Add any generic initialisation code here
    return lsUtilitiesInit_platform();
}

void lsUtilitiesTerminate()
{
    //Add any generic termination code here
    lsUtilitiesTerminate_platform();
}

void lsUtilIOSPreventBackup(const char * file)
{
	lsUtilIOSPreventBackup_platform(file);
}

void lsUtilInstallExceptionHandler()
{
	lsUtilInstallExceptionHandler_platform();
}

void lsUtilUninstallExceptionHandler()
{
	lsUtilUninstallExceptionHandler_platform();
}

lsDeviceClass lsUtilGetDeviceClassInt()
{
	return lsUtilGetDeviceClassInt_platform();
}

const char * lsUtilGetDeviceClassString()
{
	return lsUtilGetDeviceClassString_platform();
}

void lsDeviceSetTabletThreshold(int threshold)
{
	lsDeviceSetTabletThreshold_platform(threshold);
}

lsDeviceStorage* lsUtilGetDeviceStorageInformation()
{
	return lsUtilGetDeviceStorageInformation_platform();
}
