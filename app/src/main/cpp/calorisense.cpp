#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_dev_tanakornsss_calorisense_GemmaTestScreenKt_fetchNameCpp(JNIEnv *env, jclass) {
    std::string name = "Hello from CPP";
    return env -> NewStringUTF(name.c_str());
}