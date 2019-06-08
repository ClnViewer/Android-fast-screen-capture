
#include "libfake.h"
#include <jni.h>

/// see README-libart.md for details..

# define FUNWEAK __attribute__ ((weak))
# include "libsigchain.h"

extern "C" JNIEXPORT jint JNI_GetDefaultJavaVMInitArgs(void *v)
{
	LOG("JNI_GetDefaultJavaVMInitArgs(*)\n");
	v = nullptr;
	return 0;
}
extern "C" JNIEXPORT jint JNI_CreateJavaVM(JavaVM **jv, JNIEnv **ev, void *v)
{
	LOG("JNI_CreateJavaVM(*)\n");
	jv = nullptr;
	ev = nullptr;
	jint *ji = static_cast<jint*>(v);
	*ji = 0;
	return 0;
}
extern "C" JNIEXPORT jint JNI_GetCreatedJavaVMs(JavaVM **jv, jsize jsz, jsize *pjsz)
{
	LOG("JNI_GetCreatedJavaVMs(*)\n");
	(void) jsz;
	jv = nullptr;
	*pjsz = 0;
	return 0;
}
