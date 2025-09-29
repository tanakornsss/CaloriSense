#include <jni.h>
#include <string>
#include "llama.h"
#include "util/log/util_log.h"
#include "util/token_manager/util_token_manager.h"
#include "util/generate_response/util_generate_response.h"

static llama_context* ctx = nullptr;
static std::string out_prompt;

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
    if (!model) {
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
    const char* in_text_c = env -> GetStringUTFChars(input_token, nullptr);
    std::string in_text_str(in_text_c);
    out_prompt = generate_response(in_text_str, 128, ctx);
    env -> ReleaseStringUTFChars(input_token, in_text_c);
    LOG_I("%s", out_prompt.c_str());
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
    return env -> NewStringUTF(out_prompt.c_str());
}
