//
// Created by Tanakorn on 9/20/2025.
//

#include "util_generate_response.h"
#include "../log/util_log.h"

std::string generate_response(const std::string& prompt, int maxTokens, llama_context* ctx) {
    const llama_model* model = llama_get_model(ctx);
    const llama_vocab* vocab = llama_model_get_vocab(model);

    if (!ctx || !model || !vocab) return "";

    // --- Step 1: Tokenize input ---
    std::vector<llama_token> tokens;
    tokens.reserve(prompt.size() + 32);

    int n_input = llama_tokenize(
            vocab,
            prompt.c_str(),
            static_cast<int32_t>(prompt.size()), // Cast unsigned int to int32_t
            tokens.data(),
            static_cast<int32_t>(tokens.size()),
            true,
            false
    );

    if (n_input < 0) return "";
    tokens.resize(n_input);

    // --- Step 2: Create batch from input tokens ---
    llama_batch batch = llama_batch_init(n_input, 0, 1);
    for (int i = 0; i < n_input; i++) {
        batch.token[i] = tokens[i];
        batch.pos[i] = i;
        batch.seq_id[i] = nullptr; // assuming single sequence
    }
    batch.n_tokens = n_input;

    // --- Step 3: Decode initial prompt ---
    if (llama_decode(ctx, batch) != 0) {
        llama_batch_free(batch);
        return "";
    }

    // --- Step 4: Generate response tokens ---
    std::string res;

    auto sparams = llama_sampler_chain_default_params();
    llama_sampler* sampler = llama_sampler_chain_init(sparams);

    for (int32_t i = 0; i < maxTokens; i++) {
        llama_token next_token = llama_sampler_sample(sampler, ctx, -1);
    }

    return "";
}
