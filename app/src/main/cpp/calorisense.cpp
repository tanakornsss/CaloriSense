#include <jni.h>
#include <string>
#include "llamacpp/llama_jni_bridge.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_dev_tanakornsss_calorisense_GemmaTestScreenKt_fetchNameCpp(JNIEnv *env, jclass) {
    std::string name = "Hello from CaloriSense\n" + showStringLLaMA();
    return env -> NewStringUTF(name.c_str());
}