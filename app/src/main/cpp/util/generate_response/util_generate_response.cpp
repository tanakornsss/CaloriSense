//
// Created by Tanakorn on 9/20/2025.
//

#include "util_generate_response.h"

/**
 * COMPREHENSIVE GUIDE TO LOW-LEVEL AI TEXT GENERATION
 *
 * This function implements the core process of how AI language models (like ChatGPT, LLaMA)
 * generate text. Think of it like teaching a computer to predict the next word in a sentence,
 * but using mathematical patterns learned from billions of text examples.
 *
 * THE BIG PICTURE:
 * 1. Convert text to numbers (tokens) that the AI can understand
 * 2. Feed those numbers through the neural network to get predictions
 * 3. Use smart sampling to pick the next word/token
 * 4. Convert back to text and repeat until we have a complete response
 *
 * It's like having a very sophisticated autocomplete that can write entire paragraphs!
 */

static std::mutex g_gen_mutex;
static std::atomic<bool> g_generating{false};

std::string generate_response(const std::string& prompt, int maxTokens, llama_context* ctx) {
    if (!ctx) {
        LOG_E("generate response: ctx is null");
        return "";
    }

    std::unique_lock<std::mutex> lock(g_gen_mutex, std::try_to_lock);
    if (!lock.owns_lock()) {
        LOG_I("generate_response: generation already in progress, skipping");
        return "";
    }
    g_generating.store(true);

    auto guard_unset = [&]() {
        g_generating.store(false);
    };

    try {
        const llama_model* model = llama_get_model(ctx);
        const llama_vocab* vocab = llama_model_get_vocab(model);
        if (!model || !vocab) {
            LOG_E("Model and vocab missing");
            guard_unset();
            return "";
        }
        LOG_I("Model is %p vocab is %p", (void*) model, (void*) vocab);

        // Tokenize
        std::vector<llama_token> tokens(prompt.size() + 32);

        return "completed";
    }
    catch(const std::exception& e) {
        LOG_E("generate_response: exception: %s", e.what());
        g_generating.store(false);
        return "";
    }
    catch(...) {
        LOG_E("generate_response: unknown exception");
        g_generating.store(false);
        return "";
    }
}
