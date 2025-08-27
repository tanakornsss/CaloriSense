#include <jni.h>

extern "C"
JNIEXPORT void JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_handleTextTokens(
        JNIEnv *env,
        jclass,
        jstring input_token
        ) {
    // TODO: implement handleTextTokens()
}

extern "C"
JNIEXPORT void JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_handleImageTokens(
        JNIEnv *env
        , jclass clazz
        ) {
    // TODO: implement handleImageTokens()
}

extern "C"
JNIEXPORT jstring JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_returnOutputTokens(JNIEnv *env, jclass clazz) {
    // TODO: implement returnOutputTokens()
}