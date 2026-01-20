#pragma once

#include "Trie.hpp"

class TrieCLI {
public:
    TrieCLI(Trie& trie);
    void run();

private:
    Trie& trie;

    static void printHelp();
    static bool readToken(std::istringstream& iss, std::string& out);
    static size_t readLimitOrDefault(std::istringstream& iss, size_t def = 20);

    void handleLoad(std::istringstream& iss);
    void handleInsert(std::istringstream& iss);
    void handleSearch(std::istringstream& iss);
    void handleSuggest(std::istringstream& iss);
    void handleCount(std::istringstream& iss);
};