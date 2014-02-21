#include "s3e.h"

#include "lsUtilities.h"

#include <iostream>

// Main entry point for the application
int main()
{

	lsDeviceClass intClass = lsUtilGetDeviceClassInt();
	const char * strClass = lsUtilGetDeviceClassString();

	lsDeviceSetTabletThreshold(150);
	lsUtilGetDeviceStorageInformation();



    // Wait for a quit request from the host OS
    while (!s3eDeviceCheckQuitRequest())
    {
        // Fill background blue
        s3eSurfaceClear(0, 0, 255);
		 
		char temp[255];

		sprintf(temp, "Device Class String: %s", strClass);
		s3eDebugPrint(20, 20, temp, 0);

		sprintf(temp, "Device Class Int: %d", intClass);
		s3eDebugPrint(20, 40, temp, 0);

		sprintf(temp, "Device DPI");
		s3eDebugPrint(20, 60, temp, 0);
		sprintf(temp, "Device Free Memory");
		s3eDebugPrint(20, 80, temp , 0);

        // Flip the surface buffer to screen
        s3eSurfaceShow();

        // Sleep for 0ms to allow the OS to process events etc.
        s3eDeviceYield(0);
    }
    return 0;
}
