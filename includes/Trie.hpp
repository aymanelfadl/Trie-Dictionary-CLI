#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cctype>
#include <fstream>

class Trie {
private:
    struct Node {
        std::array<std::unique_ptr<Node>, 26> child{};
        bool isWord = false;
    };

    std::unique_ptr<Node> root;

    static bool isLowerLetter(char c);
    static int idx(char c);
    static std::string normalize(const std::string& s);

    const Node* walk(const std::string& s) const;
    Node* walkCreate(const std::string& s);

    void dfsCollect(const Node* node, std::string& current, std::vector<std::string>& out, size_t limit) const;

    void dfsCount(const Node* node, int& count) const;

public:
    Trie();

    void insert(const std::string& word);
    bool search(const std::string& word) const;

    std::vector<std::string> suggest(const std::string& prefix, size_t limit = 20) const;
    int countPrefix(const std::string& prefix) const;

    bool loadFromFile(const std::string& filepath);
};
