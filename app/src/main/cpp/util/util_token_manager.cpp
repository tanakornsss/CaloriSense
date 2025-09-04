//
// Created by Tanakorn on 9/4/2025.
//

#include <mutex>
#include <string>
#include "util_token_manager.h"

std::mutex TokenManager::textTokenMutex;
std::string TokenManager::textToken;

TokenManager& TokenManager::getInstance() {
    static TokenManager instance;
    return instance;
}

void TokenManager::setTextToken(const std::string& token) {
    std::lock_guard<std::mutex> lock(textTokenMutex);
    textToken = token;
}

std::string TokenManager::getTextToken() {
    std::lock_guard<std::mutex> lock(textTokenMutex);
    return textToken;
}
