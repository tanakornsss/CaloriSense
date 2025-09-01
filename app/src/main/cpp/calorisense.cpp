#include <jni.h>
#include "util/util_log.h"

extern "C"
JNIEXPORT void JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_handleTextTokens(
        JNIEnv *env,
        jclass,
        jstring input_token
        ) {
    // TODO: implement handleTextTokens()
    const char* inputChar = env -> GetStringUTFChars(input_token, nullptr); // Parse input to const char*
//    __android_log_print(ANDROID_LOG_DEBUG, "JNI", "You said \"%s\"", inputChar);
    LOG_I("You said \"%s\"", inputChar);
    env -> ReleaseStringUTFChars(input_token, inputChar); // Release the now used const char*
}

extern "C"
JNIEXPORT void JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_handleImageTokens(
        JNIEnv *env,
        jclass clazz
        ) {
    // TODO: implement handleImageTokens()
}

extern "C"
JNIEXPORT jstring JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_returnOutputTokens(JNIEnv *env, jclass clazz) {
    // TODO: implement returnOutputTokens()
}