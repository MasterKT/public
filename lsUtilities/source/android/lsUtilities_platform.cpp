/*
 * android-specific implementation of the lsUtilities extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "lsUtilities_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"
#include <string>


static char* g_RetStr;                      // Handles Java Strings to C
static int g_RetStrLen = 16384;             // Handles Java Strings to C


//----------------------------------------------------------------------------------------
// Helpers for strings, grrr
//----------------------------------------------------------------------------------------
struct JavaString
{
    jstring _Ref;

    JavaString ( const char *Buffer )
    {
        JNIEnv *env = s3eEdkJNIGetEnv();
        _Ref = env->NewStringUTF ( Buffer );
    }

    JavaString ( jstring Ref )
    {
        _Ref = Ref;
    }

    ~JavaString()
    {
        JNIEnv *env = s3eEdkJNIGetEnv();
        env->DeleteLocalRef ( _Ref );
    }

    operator jstring()
    {
        return _Ref;
    }
};
//----------------------------------------------------------------------------------------
const char* getCString(jstring str)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    if (!str)
        return NULL;
    jboolean free;
    const char* res = env->GetStringUTFChars(str, &free);

    g_RetStrLen = strlen(res);
    s3eEdkReallocOS(g_RetStr, g_RetStrLen);
    strncpy(g_RetStr, res, g_RetStrLen);
    env->ReleaseStringUTFChars(str, res);
    return g_RetStr;
}


static jobject g_Obj;
static jmethodID g_lsUtilIOSPreventBackup;
static jmethodID g_lsUtilInstallExceptionHandler;
static jmethodID g_lsUtilUninstallExceptionHandler;
static jmethodID g_lsUtilGetDeviceClassInt;
static jmethodID g_lsUtilGetDeviceClassString;
static jmethodID g_lsDeviceSetTabletThreshold;
static jmethodID g_lsUtilGetDeviceStorageInformation;

s3eResult lsUtilitiesInit_platform()
{
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;


	
    //Alloc buffer for returning strings
    g_RetStr = (char*)s3eEdkMallocOS(g_RetStrLen);

    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("lsUtilities");
    if (!cls)
        goto fail;

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_lsUtilIOSPreventBackup = env->GetMethodID(cls, "lsUtilIOSPreventBackup", "(Ljava/lang/String;)V");
    if (!g_lsUtilIOSPreventBackup)
        goto fail;

    g_lsUtilInstallExceptionHandler = env->GetMethodID(cls, "lsUtilInstallExceptionHandler", "()V");
    if (!g_lsUtilInstallExceptionHandler)
        goto fail;

    g_lsUtilUninstallExceptionHandler = env->GetMethodID(cls, "lsUtilUninstallExceptionHandler", "()V");
    if (!g_lsUtilUninstallExceptionHandler)
        goto fail;

    g_lsUtilGetDeviceClassInt = env->GetMethodID(cls, "lsUtilGetDeviceClassInt", "()I");
    if (!g_lsUtilGetDeviceClassInt)
        goto fail;

    g_lsUtilGetDeviceClassString = env->GetMethodID(cls, "lsUtilGetDeviceClassString", "()Ljava/lang/String;");
    if (!g_lsUtilGetDeviceClassString)
        goto fail;

    g_lsDeviceSetTabletThreshold = env->GetMethodID(cls, "lsDeviceSetTabletThreshold", "(I)V");
    if (!g_lsDeviceSetTabletThreshold)
        goto fail;

    g_lsUtilGetDeviceStorageInformation = env->GetMethodID(cls, "lsUtilGetDeviceStorageInformation", "()V");
    if (!g_lsUtilGetDeviceStorageInformation)
        goto fail;



    IwTrace(LSUTILITIES, ("LSUTILITIES init success"));
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(lsUtilities, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;

}

void lsUtilitiesTerminate_platform()
{
    // Add any platform-specific termination code here
}

void lsUtilIOSPreventBackup_platform(const char * file)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring file_jstr = env->NewStringUTF(file);
    env->CallVoidMethod(g_Obj, g_lsUtilIOSPreventBackup, file_jstr);
}

void lsUtilInstallExceptionHandler_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_lsUtilInstallExceptionHandler);
}

void lsUtilUninstallExceptionHandler_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_lsUtilUninstallExceptionHandler);
}

lsDeviceClass lsUtilGetDeviceClassInt_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    int rtn = (int)env->CallIntMethod(g_Obj, g_lsUtilGetDeviceClassInt);

	
    return (lsDeviceClass)rtn;
}

const char * lsUtilGetDeviceClassString_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();

    jstring jString = (jstring)env->CallObjectMethod(g_Obj, g_lsUtilGetDeviceClassString);


	std::string rtn =  getCString( jString ) ;
	// convert jstring to cstring easiest method

	IwTrace(LSUTILITIES, ("========================"));
	IwTrace(LSUTILITIES, ( rtn.c_str()));
    return rtn.c_str();
}

void lsDeviceSetTabletThreshold_platform(int threshold)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_lsDeviceSetTabletThreshold, threshold);
}

lsDeviceStorage* lsUtilGetDeviceStorageInformation_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_lsUtilGetDeviceStorageInformation);
    return NULL;
}
