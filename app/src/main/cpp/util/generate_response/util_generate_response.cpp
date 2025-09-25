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
        int n_input = llama_tokenize(
                vocab,
                prompt.c_str(),
                static_cast<int32_t>(prompt.size()),
                tokens.data(),
                static_cast<int32_t>(tokens.size()),
                true,
                false
        );
        if (n_input < 0) {
            LOG_E("n_input is below zero");
            guard_unset();
            return "";
        }
        LOG_I("n_input is %d", n_input);

        LOG_I("tokens is initially %lu", tokens.size());
        tokens.resize(n_input);
        LOG_I("tokens is now %lu", tokens.size());

        // Initialize batch
        llama_batch batch = llama_batch_init(std::max(n_input, 1), 0, 1);
        for (int i = 0; i < n_input; i++) {
            batch.token[i] = tokens[i];
            batch.pos[i] = i;
            batch.n_seq_id[i] = 1;
            batch.seq_id[i] = nullptr;
            batch.logits[i] = (i == n_input - 1) ? 1 : 0;
        }
        batch.n_tokens = n_input;
        LOG_I("batch.n_tokens is %d", batch.n_tokens);

//        // Decode initial prompt ; crashed here
//        if (llama_decode(ctx, batch) != 0) {
//            LOG_E("Failed to decode initial prompt");
//            llama_batch_free(batch);
//            guard_unset();
//            return "";
//        }

        // Sampling loop & generation
        std::string res;
        // Setup sampler chain (top-k, top-p, temp, etc.)
        auto sparams = llama_sampler_chain_default_params();
        llama_sampler *sampler = llama_sampler_chain_init(sparams);
        llama_sampler_chain_add(sampler, llama_sampler_init_top_k(40));
        llama_sampler_chain_add(sampler, llama_sampler_init_top_p(0.9f, 1));
        llama_sampler_chain_add(sampler, llama_sampler_init_temp(0.8f));
        llama_sampler_chain_add(sampler, llama_sampler_init_dist(12345));

        guard_unset();
        return "completed";
    }
    catch (const std::exception &e) {
        LOG_E("generate_response: exception: %s", e.what());
        g_generating.store(false);
        return "";
    }
    catch (...) {
        LOG_E("generate_response: unknown exception");
        g_generating.store(false);
        return "";
    }
}
