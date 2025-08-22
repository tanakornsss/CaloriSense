#include <jni.h>
#include <string>

// Write C++ code here.
//
// Do not forget to dynamically load the C++ library into your application.
//
// For instance,
//
// In MainActivity.java:
//    static {
//       System.loadLibrary("calorisense");
//    }
//
// Or, in MainActivity.kt:
//    companion object {
//      init {
//         System.loadLibrary("calorisense")
//      }
//    }
extern "C"
JNIEXPORT jstring JNICALL
Java_dev_tanakornsss_calorisense_GemmaTestScreenKt_fetchNameCpp(JNIEnv *env, jclass) {
    std::string name = "Hello from CPP";
    return env -> NewStringUTF(name.c_str());
}