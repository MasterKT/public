/*
 * iphone-specific implementation of the lsUtilities extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "lsUtilities_internal.h"


#import <Foundation/Foundation.h>
#import <Foundation/NSURL.h>
#import <UIKit/UIKit.h>
#import <UIKit/UIDevice.h>

#include <libkern/OSAtomic.h>
#include <execinfo.h>
#import <sys/xattr.h>

//-------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------
#define SYSTEM_VERSION_EQUAL_TO(v)                  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedSame)
#define SYSTEM_VERSION_GREATER_THAN(v)              ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)
#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)

//-------------------------------------------------------------------------------------
// Forward Declarations
//-------------------------------------------------------------------------------------
void _IsInstallExceptionHandler();
void _IsUninstallExceptionHandler();


//-------------------------------------------------------------------------------------
// Exception Handler
//-------------------------------------------------------------------------------------

@interface IsExceptionHandler : NSObject
   BOOL dismissed;
@end

@implementation IsExceptionHandler

- (id)init
{
    self = [super init];
    if( self ) 
    {
    }
    return self;
}
- (void)alertView:(UIAlertView *)anAlertView clickedButtonAtIndex:(NSInteger)anIndex
{
	if (anIndex == 0)
	{
		dismissed = YES;
	}
}
-(void) HandleException:(NSArray *)pCallStack
{   
   UIAlertView *alert =
   [[[UIAlertView alloc]
     initWithTitle:NSLocalizedString(@"Unhandled exception", nil)
     message:[NSString stringWithFormat:NSLocalizedString(
                                                          @"Boom!\n\n"
                                                          @"Details:\n%@", nil),
              pCallStack]
     delegate:self
     cancelButtonTitle:NSLocalizedString(@"Quit", nil)
     otherButtonTitles:NSLocalizedString(@"Continue", nil), nil]
    autorelease];
	[alert show];
	
	CFRunLoopRef runLoop = CFRunLoopGetCurrent();
	CFArrayRef allModes = CFRunLoopCopyAllModes(runLoop);
	
	while (!dismissed)
	{
		for (NSString *mode in (NSArray *)allModes)
		{
			CFRunLoopRunInMode((CFStringRef)mode, 0.001, false);
		}
	}
	CFRelease(allModes);
   
   _IsUninstallExceptionHandler();
   
   exit(1);
}


@end


NSArray *IsBacktraceCallstackSymbols( int startOffset, int maxFrames )
{
   void* callstack[1024];
   int nrFrames = backtrace( callstack, 1024 );
   
   //ZZZ TODO, change the void pointers according to the magix marmalade stack relocation thingy
   
   char **strs = backtrace_symbols( callstack, nrFrames );
   
   NSMutableArray *pBacktrace = [NSMutableArray arrayWithCapacity:nrFrames];
   for( int i = startOffset; i < startOffset + maxFrames; i++)
   {
      if( i < nrFrames )
      {
         [pBacktrace addObject:[NSString stringWithUTF8String:strs[i]]];
      }
   }
   free(strs);
   return pBacktrace;
}

NSArray *IsBacktraceCallstack( int startOffset, int maxFrames )
{
   void* callstack[1024];
   int nrFrames = backtrace( callstack, 1024 );
   //ZZZ TODO, change the void pointers according to the magix marmalade stack relocation thingy
   
   NSMutableArray *pBacktrace = [NSMutableArray arrayWithCapacity:nrFrames];
   for( int i = startOffset; i < startOffset + maxFrames; i++)
   {
      if( i < nrFrames )
      {
         [pBacktrace addObject:[NSNumber numberWithInteger:(int)callstack[i] ] ];
      }
   }
   return pBacktrace;
}

//-------------------------------------------------------------------------------------
// IsHandleException
//-------------------------------------------------------------------------------------

void IsHandleException( NSException *exception )
{
   
   
   NSArray *pCallStack = IsBacktraceCallstack( 4, 1024 );
   NSArray *pCallStackSymbols = IsBacktraceCallstackSymbols( 4, 1024 );
   
   NSLog( @"CRASH!! Due an exception %@", [exception name] );
   
   for( unsigned int f = 0; f < [pCallStackSymbols count ]; f++ )
   {
      NSLog( @"#%.4d : 0x%.8X - %@", f, [[pCallStack objectAtIndex: f] unsignedIntValue] , [pCallStackSymbols objectAtIndex: f ]);
   }
   
   
   // Show
   [[[[IsExceptionHandler alloc] init] autorelease]
    performSelectorOnMainThread:@selector(HandleException:)
    withObject:pCallStackSymbols
    waitUntilDone:YES];
}


//-------------------------------------------------------------------------------------
// IsSignalHandler
//-------------------------------------------------------------------------------------

void IsSignalHandler( int signal )
{
  
   
   NSArray *pCallStack = IsBacktraceCallstack( 4, 1024 );
   NSArray *pCallStackSymbols = IsBacktraceCallstackSymbols( 4, 1024 );
   
   NSLog( @"CRASH!! Due to signal %d", signal );
   
   for( unsigned int f = 0; f < [pCallStackSymbols count ]; f++ )
   {
      NSLog( @"#%.4d : 0x%.8X - %@", f, [[pCallStack objectAtIndex: f] unsignedIntValue] , [pCallStackSymbols objectAtIndex: f ]);
   }
   

   
   // Show
   [[[[IsExceptionHandler alloc] init] autorelease]
    performSelectorOnMainThread:@selector(HandleException:)
    withObject:pCallStackSymbols
    waitUntilDone:YES];
   
}

//-------------------------------------------------------------------------------------
// lsUtilitiesInit
//-------------------------------------------------------------------------------------

s3eResult lsUtilitiesInit_platform()
{
    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;
}

//-------------------------------------------------------------------------------------
// lsUtilitiesTerminate
//-------------------------------------------------------------------------------------

void lsUtilitiesTerminate_platform()
{
    // Add any platform-specific termination code here
}

//-------------------------------------------------------------------------------------
// lsUtilIOSPreventBackup
//-------------------------------------------------------------------------------------

void lsUtilIOSPreventBackup_platform(const char * file)
{
	if (SYSTEM_VERSION_EQUAL_TO(@"5.0.1") )
	{
		NSString *input = [[[NSString alloc] initWithUTF8String:file] autorelease];
		NSURL *URL = [NSURL fileURLWithPath:input];
		
		const char* filePath = [[URL path] fileSystemRepresentation];
	 
		const char* attrName = "com.apple.MobileBackup";
		u_int8_t attrValue = 1;
	 
		int result = setxattr(filePath, attrName, &attrValue, sizeof(attrValue), 0, 0);
	}
	else if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"5.1")) 
	{
		NSString *input = [[[NSString alloc] initWithUTF8String:file] autorelease];
		NSURL *URL = [NSURL fileURLWithPath:input];
	 
		NSError *error = nil;
		BOOL success = [URL setResourceValue: [NSNumber numberWithBool: YES]
									  forKey: NSURLIsExcludedFromBackupKey error: &error];
		if(!success){
			NSLog(@"Error excluding %@ from backup %@", [URL lastPathComponent], error);
		}
	}
}

//-------------------------------------------------------------------------------------
// lsUtilInstallExceptionHandler
//-------------------------------------------------------------------------------------

void lsUtilInstallExceptionHandler_platform()
{
	_IsInstallExceptionHandler();
}

//-------------------------------------------------------------------------------------
// lsUtilUninstallExceptionHandler
//-------------------------------------------------------------------------------------

void lsUtilUninstallExceptionHandler_platform()
{
	_IsUninstallExceptionHandler();
}

//-------------------------------------------------------------------------------------
// _IsInstallExceptionHandler
//-------------------------------------------------------------------------------------

void _IsInstallExceptionHandler()
{
   NSSetUncaughtExceptionHandler( &IsHandleException );
   signal(SIGABRT, IsSignalHandler);
   signal(SIGILL, IsSignalHandler);
   signal(SIGSEGV, IsSignalHandler);
   signal(SIGFPE, IsSignalHandler);
   signal(SIGBUS, IsSignalHandler);
   signal(SIGPIPE, IsSignalHandler);
}


//-------------------------------------------------------------------------------------
// _IsUninstallExceptionHandler
//-------------------------------------------------------------------------------------
void _IsUninstallExceptionHandler()
{
	NSSetUncaughtExceptionHandler(NULL);
	signal(SIGABRT, SIG_DFL);
	signal(SIGILL, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGFPE, SIG_DFL);
	signal(SIGBUS, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}
   
   
