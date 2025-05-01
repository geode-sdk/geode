#pragma once

#include "fmod_common.h"
#include <jni.h>

extern "C" FMOD_RESULT F_API FMOD_Android_JNI_Init(JavaVM *vm, jobject javaActivity);
extern "C" FMOD_RESULT F_API FMOD_Android_JNI_Close();