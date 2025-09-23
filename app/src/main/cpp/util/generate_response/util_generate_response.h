//
// Created by Tanakorn on 9/20/2025.
//

#ifndef CALORISENSE_UTIL_GENERATE_RESPONSE_H
#define CALORISENSE_UTIL_GENERATE_RESPONSE_H

#include <string>
#include <mutex>
#include <atomic>
#include "../log/util_log.h"
#include "../../llamacpp/src/llama-context.h"

std::string generate_response(
        const std::string& prompt,
        int maxTokens = 128,
        llama_context* ctx = nullptr
);

#endif //CALORISENSE_UTIL_GENERATE_RESPONSE_H
