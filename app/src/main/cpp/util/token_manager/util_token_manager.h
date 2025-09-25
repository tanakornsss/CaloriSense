//
// Created by Tanakorn on 9/4/2025.
//

#ifndef CALORISENSE_UTIL_TOKEN_MANAGER_H
#define CALORISENSE_UTIL_TOKEN_MANAGER_H

#include <string>
#include <mutex>


// TokenManager is a singleton class that manages a global text token in a thread-safe manner.
class TokenManager {
public:
    // Returns the singleton instance of TokenManager.
    static TokenManager& getInstance();

    // Sets the text token in a thread-safe way.
    static void setTextToken(const std::string& token);

    // Gets the text token in a thread-safe way.
    static std::string getTextToken();

    // Delete copy constructor to prevent copying.
    TokenManager(const TokenManager&) = delete;

    // Delete assignment operator to prevent assignment.
    TokenManager& operator = (const TokenManager&) = delete;

private:
    // Private constructor to prevent direct instantiation (singleton pattern).
    TokenManager() = default;

    // Private destructor.
    ~TokenManager() = default;

    // Mutex to protect access to textToken for thread safety.
    static std::mutex textTokenMutex;

    // The actual text token being managed.
    static std::string textToken;
};


#endif //CALORISENSE_UTIL_TOKEN_MANAGER_H
