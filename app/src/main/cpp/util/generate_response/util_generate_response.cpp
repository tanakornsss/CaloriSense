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
        std::vector<llama_seq_id> seq_ids(n_input, 0);

        llama_batch batch = llama_batch_init(std::max(n_input, 1), 0, 1);
        for (int i = 0; i < n_input; i++) {
            batch.token[i] = tokens[i];
            batch.pos[i] = i;
            batch.n_seq_id[i] = 1;
            batch.seq_id[i] = &seq_ids[i];
            batch.logits[i] = (i == n_input - 1) ? 1 : 0;
        }
        batch.n_tokens = n_input;
        LOG_I("batch.n_tokens is %d", batch.n_tokens);

        if (!batch.token || !batch.pos || !batch.seq_id || !batch.logits || !batch.n_seq_id) {
            LOG_E("batch not initialized properly");
            llama_batch_free(batch);
            guard_unset();
            return "";
        }

        for (int i = 0; i < batch.n_tokens; i++) {
            if (batch.token[i] < 0 || batch.token[i] >= llama_vocab_n_tokens(vocab)) {
                LOG_E("Invalid token %d (max=%d)", batch.token[i], llama_vocab_n_tokens(vocab));
                llama_batch_free(batch);
                guard_unset();
                return "";
            }
        }

        // Decode initial prompt
        if (llama_decode(ctx, batch) != 0) {
            LOG_E("Failed to decode initial prompt");
            llama_batch_free(batch);
            guard_unset();
            return "";
        }

        // Sampling loop & generation
        std::string res;
        // Setup sampler chain (top-k, top-p, temp, etc.)
        auto sparams = llama_sampler_chain_default_params();
        llama_sampler *sampler = llama_sampler_chain_init(sparams);
        llama_sampler_chain_add(sampler, llama_sampler_init_top_k(40));
        llama_sampler_chain_add(sampler, llama_sampler_init_top_p(0.9f, 1));
        llama_sampler_chain_add(sampler, llama_sampler_init_temp(0.8f));
        llama_sampler_chain_add(sampler, llama_sampler_init_dist(12345));

        int current_pos = n_input;
        for (int i = 0; i < maxTokens; i++) {
            llama_token next_token = llama_sampler_sample(sampler, ctx, -1);
            llama_sampler_accept(sampler, next_token);

            if (next_token == llama_vocab_eos(vocab)) {
                guard_unset();
                break;
            }

            char buf[256];
            int len = llama_token_to_piece(vocab, next_token, buf, sizeof(buf), 0, false);
            if (len > 0) {
                res.append(buf, len);
            }
            LOG_I("%s", res.c_str());

            // Prepare next batch
            batch.n_tokens = 1;
            batch.token[0] = next_token;
            batch.pos[0] = current_pos++;
            batch.n_seq_id[0] = 1;
            batch.seq_id[0] = &seq_ids[0];
            batch.logits[0] = 1;

            if (llama_decode(ctx, batch) != 0) {
                guard_unset();
                break;
            }
        }

        guard_unset();
        return res;
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
