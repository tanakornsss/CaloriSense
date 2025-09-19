#include <jni.h>
#include <string>
#include "util/util_log.h"
#include "util/util_token_manager.h"
#include "llama.h"

static llama_context* ctx = nullptr;

extern "C"
JNIEXPORT jboolean JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_loadModel(
        JNIEnv *env,
        jclass,
        jstring jPath
) {
    const char* path = env -> GetStringUTFChars(jPath, nullptr);

    llama_model_params model_params = llama_model_default_params();
    llama_context_params ctx_params = llama_context_default_params();

    llama_model* model = llama_model_load_from_file(path, model_params);
    if (model == nullptr) {
        env -> ReleaseStringUTFChars(jPath, path);
        return JNI_FALSE;
    }

    ctx = llama_init_from_model(model, ctx_params);
    env -> ReleaseStringUTFChars(jPath, path);

    return ctx != nullptr ? JNI_TRUE : JNI_FALSE;
}

extern "C"
JNIEXPORT void JNICALL
Java_dev_tanakornsss_calorisense_JNIBridgeKt_handleTextTokens(
        JNIEnv *env,
        jclass,
        jstring input_token
        ) {
    // TODO: implement handleTextTokens()
    TokenManager::getInstance();
    const char* cStr = env -> GetStringUTFChars(input_token, nullptr); // Parse input to const char*
    std::string cppStr(cStr);
    TokenManager::setTextToken(cppStr);
    env -> ReleaseStringUTFChars(input_token, cStr); // Release the now used const char*
    LOG_I("You typed \"%s\"", TokenManager::getTextToken().c_str());
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
    const char* test = "The output returns here";
    return env -> NewStringUTF(test);
}
