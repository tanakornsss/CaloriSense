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

std::string generate_response(const std::string &prompt, int maxTokens, llama_context *ctx) {
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
        // === STEP 0: GET THE AI MODEL COMPONENTS ===
        // Think of these as getting access to different parts of the AI "brain"

        const llama_model* model = llama_get_model(ctx);         // The actual neural network with billions of parameters
        const llama_vocab* vocab = llama_model_get_vocab(model); // The dictionary that converts words ↔ numbers

        // Safety check: Make sure we have all the pieces we need
        if (!model || !vocab) {
            LOG_E("generate_response: model/vocab missing");
            guard_unset();
            return "";
        }

        // === STEP 1: TOKENIZATION - CONVERTING TEXT TO NUMBERS ===
        /**
         * WHY TOKENIZATION?
         * - Computers can't understand words directly, only numbers
         * - Each word/piece gets converted to a unique number called a "token"
         * - Example: "Hello world" might become [15496, 1917]
         * - Some words are split into pieces: "understanding" → ["under", "stand", "ing"]
         *
         * This is like creating a secret code where each word has a number!
         */

        std::vector<llama_token> tokens;
        tokens.resize(prompt.size() +
                      32); // Reserve extra space (text might become more tokens than characters)

        // Convert the input text into a sequence of token numbers
        int n_input = llama_tokenize(
                vocab,                                          // The AI's dictionary
                prompt.c_str(),                                 // Your input text ("Hello, how are you?")
                static_cast<int32_t>(prompt.size()),           // Length of input text
                tokens.data(),                                  // Where to store the token numbers
                static_cast<int32_t>(tokens.size()),           // Maximum tokens we can store
                true,                                           // add_special: Add special "start of text" token
                false                                           // parse_special: Don't treat special symbols differently
        );

        // Check if tokenization worked (negative means error)
        if (n_input < 0) {
            LOG_E("generate_response: tokenization failed n_input=%d", n_input);
            guard_unset();
            return "";
        }
        tokens.resize(n_input); // Trim to actual number of tokens created

        // === STEP 2: BATCH PREPARATION - ORGANIZING DATA FOR THE AI ===
        /**
         * WHAT IS A BATCH?
         * - AI models process data in organized chunks called "batches"
         * - Think of it like organizing papers on a desk before working on them
         * - Each token needs additional info: position in sentence, which conversation it belongs to
         *
         * It's like preparing ingredients before cooking - everything needs to be measured and organized!
         */

        llama_batch batch = llama_batch_init(std::max(n_input, 1), 0, 1);
        if (!batch.token || !batch.pos || !batch.logits || !batch.n_seq_id) {
            LOG_E("generate_response: llama_batch_init returned incomplete buffers (token=%p pos=%p logits=%p n_seq_id=%p)",
                  (void*)batch.token, (void*)batch.pos, (void*)batch.logits, (void*)batch.n_seq_id);
            llama_batch_free(batch);
            guard_unset();
            return "";
        }

        // Manually fill in all the information for each token
        for (int i = 0; i < n_input; i++) {
            batch.token[i] = tokens[i];                         // The actual token number
            batch.pos[i] = i;                                   // Position in the sequence (0, 1, 2, ...)
            batch.n_seq_id[i] = 1;                              // How many conversations this token belongs to
            batch.seq_id[i] = nullptr;                          // Which specific conversation (we only have one)
            batch.logits[i] = (i == n_input - 1) ? 1
                                                 : 0;       // Only calculate predictions for the last token
        }
        batch.n_tokens = n_input; // Tell the batch how many tokens we actually have
        if (batch.n_tokens <= 0) {
            LOG_E("generate_response: no tokens to decode");
            llama_batch_free(batch);
            guard_unset();
            return "";
        }
        LOG_I("generate_response: about to decode ctx=%p n_tokens=%d first_token=%d last_token=%d",
              (void*)ctx, batch.n_tokens, batch.token[0], batch.token[batch.n_tokens-1]);

        // === STEP 3: FORWARD PASS - RUNNING THE AI NEURAL NETWORK ===
        /**
         * WHAT HAPPENS DURING DECODING?
         * - The AI "reads" all your tokens and builds understanding
         * - Billions of mathematical operations happen (matrix multiplications, etc.)
         * - The model builds a "context" of what you said
         * - At the end, it produces probabilities for what word should come next
         *
         * Think of it like the AI reading your message very carefully and thinking
         * "Based on everything I've learned, what should I say next?"
         */

        LOG_I("batch.n_tokens=%d", batch.n_tokens);
        for (int i = 0; i < batch.n_tokens; i++) {
            LOG_I("token[%d]=%d", i, batch.token[i]);
        }

        int decode_ret = llama_decode(ctx, batch);
        if (decode_ret != 0) {
            LOG_E("generate_response: llama_decode returned error=%d", decode_ret);
            llama_batch_free(batch);
            guard_unset();
            return "";
        }

        // === STEP 4: RESPONSE GENERATION - THE MAIN AI CONVERSATION LOOP ===
        /**
         * NOW THE MAGIC BEGINS!
         * - We'll repeatedly ask the AI "what's the next word?"
         * - Each prediction gets added to our response
         * - The AI considers everything said so far when picking the next word
         *
         * It's like having a conversation where the AI thinks carefully about each word!
         */
        // === SAMPLING SETUP - HOW THE AI PICKS WORDS ===
        /**
         * SAMPLING EXPLAINED:
         * - The AI doesn't just pick the most likely word (that would be boring!)
         * - Instead, we use "smart randomness" to make responses more interesting
         * - Different sampling methods control creativity vs. coherence
         *
         * SAMPLING METHODS WE USE:
         * - Top-K (40): Only consider the 40 most likely next words
         * - Top-P (0.9): Consider words until their combined probability reaches 90%
         * - Temperature (0.8): Add some randomness (lower = more focused, higher = more creative)
         * - Distribution sampling: The final random selection process
         */

        std::string res; // This will store our final AI response
        auto sparams = llama_sampler_chain_default_params();
        llama_sampler* sampler = llama_sampler_chain_init(sparams);

        // Build a "chain" of sampling methods (like filters for creativity)
        llama_sampler_chain_add(sampler, llama_sampler_init_top_k(40));      // Limit to top 40 choices
        llama_sampler_chain_add(sampler,llama_sampler_init_top_p(0.9f, 1)); // Consider 90% probability mass
        llama_sampler_chain_add(sampler,llama_sampler_init_temp(0.8f));     // Add controlled randomness
        llama_sampler_chain_add(sampler, llama_sampler_init_dist(12345));    // Final selection (with seed for consistency)

        int current_pos = n_input; // Keep track of our position in the conversation

        // === THE MAIN GENERATION LOOP ===
        /**
         * THIS IS WHERE THE AI WRITES ITS RESPONSE WORD BY WORD!
         * Each iteration of this loop generates one more piece of text (token)
         */

        for (int32_t i = 0; i < maxTokens; i++) {
            // --- PREDICTION STEP ---
            /**
             * Ask the AI: "Given everything we've discussed so far, what should the next word be?"
             * The sampler looks at all possible next words and their probabilities,
             * then picks one using our sampling strategy (creative but not random)
             */
            llama_token next_token = llama_sampler_sample(sampler, ctx, -1);

            // --- FEEDBACK STEP ---
            /**
             * Tell the sampler what we picked so it can learn for next time
             * This helps maintain consistency in the response
             */
            llama_sampler_accept(sampler, next_token);

            // --- STOPPING CONDITION ---
            /**
             * Check if the AI said "I'm done talking" (end-of-sequence token)
             * This is like the AI saying "period, end of sentence, I'm finished"
             */
            if (next_token == llama_vocab_eos(vocab)) {
                break; // AI decided to stop generating
            }

            // --- CONVERT NUMBER BACK TO TEXT ---
            /**
             * Remember how we converted words to numbers at the start?
             * Now we do the reverse: convert the number back to readable text
             * Example: token 15496 might become "Hello"
             */
            char piece[256]; // Temporary storage for this piece of text
            int n_piece = llama_token_to_piece(vocab, next_token, piece, sizeof(piece), 0, false);
            if (n_piece > 0) {
                res.append(piece, n_piece); // Add this piece to our growing response
            }

            // --- PREPARE FOR NEXT PREDICTION ---
            /**
             * Now we need to "feed" this new word back to the AI so it knows what it just said
             * This is like updating the AI's short-term memory with its latest word
             * The AI needs to remember its own words to continue coherently
             */

            // Reset the batch to contain just our new token
            batch.n_tokens = 1;
            batch.token[0] = next_token;    // The word we just generated
            batch.pos[0] = current_pos;     // Where this word appears in the sequence
            batch.n_seq_id[0] = 1;          // Still the same conversation
            batch.seq_id[0] = nullptr;      // Still the main conversation thread
            batch.logits[0] = 1;            // We need predictions for the next word

            current_pos++; // Move forward in the conversation

            // --- UPDATE AI'S MEMORY ---
            /**
             * Run the neural network again with our new word
             * This updates the AI's internal state to include what it just said
             * Now it "remembers" this word for generating the next one
             */
            if (llama_decode(ctx, batch) != 0) {
                LOG_E("generate_response: llama_decode failed during generation loop");
                break; // Something went wrong, stop generating
            }

            // Loop continues -> AI generates the next word based on everything so far!
        }

        // === CLEANUP AND RETURN ===
        /**
         * Free up memory we used (good programming practice)
         * Return the complete AI response as a string
         */
        llama_sampler_free(sampler);  // Clean up the sampling system
        llama_batch_free(batch);      // Clean up the batch processing
        LOG_I("generate_response: finished, len=%zu", res.size());
        LOG_I("The response is %s", res.c_str());
        guard_unset();
        return res; // Return the AI's complete response!

        /**
        * SUMMARY OF WHAT JUST HAPPENED:
        * 1. We converted your text to numbers the AI understands
        * 2. We fed those numbers through the neural network
        * 3. The AI predicted the most likely next words using learned patterns
        * 4. We used smart sampling to pick interesting (not just predictable) words
        * 5. We converted each predicted number back to readable text
        * 6. We repeated this process until we had a complete response
        *
        * This is essentially how all modern AI chatbots work under the hood!
        * Your phone/computer just did millions of calculations to have a conversation with you.
        */
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
