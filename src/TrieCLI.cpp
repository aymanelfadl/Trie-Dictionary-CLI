#include "TrieCLI.hpp"

TrieCLI::TrieCLI(Trie& t) : trie(t) {}

void TrieCLI::run() {
    printHelp();

    std::string line;
    while (true) {
        std::cout << "\ntrie> ";
        if (!std::getline(std::cin, line)) break;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd.empty()) continue;
        if (cmd == "quit" || cmd == "exit") break;
        else if (cmd == "help") printHelp();
        else if (cmd == "load") handleLoad(iss);
        else if (cmd == "insert") handleInsert(iss);
        else if (cmd == "search") handleSearch(iss);
        else if (cmd == "suggest") handleSuggest(iss);
        else if (cmd == "count") handleCount(iss);
        else if (cmd == "export")
        {
                trie.exportDot("trie.dot");
                std::cout << "Generated trie.dot\n";
        }
        else std::cout << "Unknown command. Type 'help'.\n";
    }

    std::cout << "Bye.\n";
}

void TrieCLI::printHelp() {
    std::cout
        << "Commands:\n"
        << "  load <file>            Load dictionary file (1 word per line)\n"
        << "  insert <word>          Insert a word\n"
        << "  search <word>          Search exact word\n"
        << "  suggest <prefix> [n]   Suggest words for prefix (optional limit n)\n"
        << "  count <prefix>         Count words sharing prefix\n"
        << "  help                   Show this help\n"
        << "  quit                   Exit\n";
}

bool TrieCLI::readToken(std::istringstream& iss, std::string& out) {
    return static_cast<bool>(iss >> out);
}

size_t TrieCLI::readLimitOrDefault(std::istringstream& iss, size_t def) {
    size_t n;
    return (iss >> n) ? n : def;
}

void TrieCLI::handleLoad(std::istringstream& iss) {
    std::string file;
    if (!readToken(iss, file)) {
        std::cout << "Usage: load <file>\n";
        return;
    }
    if (trie.loadFromFile(file))
        std::cout << "Loaded dictionary: " << file << "\n";
    else
        std::cout << "Failed to open: " << file << "\n";
}

void TrieCLI::handleInsert(std::istringstream& iss) {
    std::string word;
    if (!readToken(iss, word)) {
        std::cout << "Usage: insert <word>\n";
        return;
    }
    trie.insert(word);
    std::cout << "Inserted: " << word << "\n";
}

void TrieCLI::handleSearch(std::istringstream& iss) {
    std::string word;
    if (!readToken(iss, word)) {
        std::cout << "Usage: search <word>\n";
        return;
    }
    std::cout << (trie.search(word) ? "FOUND" : "NOT FOUND") << "\n";
}

void TrieCLI::handleSuggest(std::istringstream& iss) {
    std::string prefix;
    if (!readToken(iss, prefix)) {
        std::cout << "Usage: suggest <prefix> [limit]\n";
        return;
    }

    size_t limit = readLimitOrDefault(iss, 20);
    auto words = trie.suggest(prefix, limit);

    if (words.empty()) {
        std::cout << "(no suggestions)\n";
        return;
    }
    for (const auto& w : words)
        std::cout << "  - " << w << "\n";
}

void TrieCLI::handleCount(std::istringstream& iss) {
    std::string prefix;
    if (!readToken(iss, prefix)) {
        std::cout << "Usage: count <prefix>\n";
        return;
    }
    std::cout << trie.countPrefix(prefix) << "\n";
}
