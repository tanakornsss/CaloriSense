//
// Created by Tanakorn on 9/1/2025.
//

#ifndef CALORISENSE_UTIL_LOG_H
#define CALORISENSE_UTIL_LOG_H
#include <android/log.h>

#define TAG "JNI"
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOG_W(...) __android_log_print(ANDROID_LOG_WARN,  TAG, __VA_ARGS__)
#define LOG_I(...) __android_log_print(ANDROID_LOG_INFO,  TAG, __VA_ARGS__)
#define LOG_D(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

#endif //CALORISENSE_UTIL_LOG_H
