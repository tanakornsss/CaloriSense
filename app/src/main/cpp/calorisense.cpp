#include <jni.h>
#include <string>
#include "llamacpp/llama_jni_bridge.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_dev_tanakornsss_calorisense_GemmaTestScreenKt_fetchNameCpp(
        JNIEnv *env,
        jclass,
        jstring testInput
        ) {
    // Retrieve input from Java side
    const char* inputCStr = env -> GetStringUTFChars(testInput, nullptr);
    // Parse char* to std::string
    std::string inputString(inputCStr);
    // Release inputCStr to free up memory
    env -> ReleaseStringUTFChars(testInput, inputCStr);

    std::string name = "Hello from CaloriSense\n" + showStringLLaMA() + "\n" + inputString;
    return env -> NewStringUTF(name.c_str());
}