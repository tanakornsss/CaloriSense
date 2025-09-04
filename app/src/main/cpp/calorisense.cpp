#include <jni.h>
#include <string>
#include "util/util_log.h"

std::string gTextToken; // TODO : Implement mutex

extern "C"
JNIEXPORT void JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_handleTextTokens(
        JNIEnv *env,
        jclass,
        jstring input_token
        ) {
    // TODO: implement handleTextTokens()
    const char* cStr = env -> GetStringUTFChars(input_token, nullptr); // Parse input to const char*
    std::string cppStr(cStr);
    gTextToken = cppStr;
    env -> ReleaseStringUTFChars(input_token, cStr); // Release the now used const char*
}

extern "C"
JNIEXPORT void JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_handleImageTokens(
        JNIEnv *env,
        jclass
        ) {
    // TODO: implement handleImageTokens()
}

extern "C"
JNIEXPORT jstring JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_returnOutputTokens(
        JNIEnv *env,
        jclass
        ) {
    // TODO: implement returnOutputTokens()
    const char* test = "asdf";
    return env -> NewStringUTF(test);
}