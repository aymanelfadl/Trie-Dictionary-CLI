#include "Trie.hpp"


Trie::Trie() : root(std::make_unique<Node>()) {}

bool Trie::isLowerLetter(char c)
{
    return (c >= 'a' && c <= 'z');
}

int Trie::idx(char c) {
    return c - 'a';
}

std::string Trie::normalize(const std::string& s)
{
    std::string out;

    for (unsigned char uc : s)
    {
        char c = static_cast<char>(std::tolower(uc));
        if (isLowerLetter(c))
            out.push_back(c);
    }
    return out;
}

Trie::Node* Trie::walkCreate(const std::string& s)
{
    Node* cur = root.get();
    for (char c : s)
    {
        int i = idx(c);
        if (!cur->child[i])
            cur->child[i] = std::make_unique<Node>();
        cur = cur->child[i].get();
    }
    return cur;
}

const Trie::Node* Trie::walk(const std::string& s) const
{
    const Node* cur = root.get();
    for (char c : s)
    {
        int i = idx(c);
        if (!cur->child[i])
            return nullptr;
        cur = cur->child[i].get();
    }
    return cur;
}

void Trie::insert(const std::string& word)
{
    std::string w = normalize(word);
    if (w.empty()) return;

    Node* end = walkCreate(w);
    end->isWord = true;
}

bool Trie::search(const std::string& word) const
{
    std::string w = normalize(word);
    if (w.empty())
        return false;

    const Node* node = walk(w);

    return node != nullptr && node->isWord;
}

void Trie::dfsCollect(const Node* node, std::string& current, std::vector<std::string>& out, size_t limit) const
{
    if (!node && limit != 0 && out.size() >= limit)
        return;

    if (node->isWord)
    {
        out.push_back(current);
        if (limit != 0 && out.size() >= limit)
            return;
    }

    for (int i = 0; i < 26; ++i)
    {
        if (node->child[i])
        {
            current.push_back(static_cast<char>('a' + i));
            dfsCollect(node->child[i].get(), current, out, limit);
            current.pop_back();
            if (limit != 0 && out.size() >= limit) 
                return;
        }
    }
}

std::vector<std::string> Trie::suggest(const std::string& prefix, size_t limit) const {
    std::string p = normalize(prefix);
    if (p.empty())
        return {};

    const Node* start = walk(p);
    if (!start)
        return {};

    std::vector<std::string> results;
    std::string current = p;
    dfsCollect(start, current, results, limit);
    return results;
}

void Trie::dfsCount(const Node* node, int& count) const {
    
    if (!node)
        return;
    
    if (node->isWord)
        count++;

    for (int i = 0; i < 26; ++i)
        if (node->child[i]) 
            dfsCount(node->child[i].get(), count);
}

int Trie::countPrefix(const std::string& prefix) const {
    std::string p = normalize(prefix);
    if (p.empty())
        return 0;

    const Node* start = walk(p);
    if (!start)
        return 0;

    int count = 0;
    dfsCount(start, count);
    return count;
}

bool Trie::loadFromFile(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in.is_open()) return false;

    std::string line;
    std::string word;

    while (std::getline(in, line))
    {
        word.clear();

        for (char c : line)
        {
            if (std::isalpha(static_cast<unsigned char>(c)))
                word.push_back(static_cast<char>(std::tolower(c))); 
            else
            {
                if (!word.empty())
                {
                    insert(word);
                    word.clear();
                }
            }
        }
        if (!word.empty())
            insert(word);
    }
    return true;
}

// // debuging 

// void Trie::display() const {
//     displayNode(root.get(), 0);
// }

// void Trie::displayNode(const Node* node, int depth) const {
//     for (int i = 0; i < 26; ++i) {
//         if (node->child[i]) {
//             for (int d = 0; d < depth; ++d) std::cout << "  ";
//             char letter = static_cast<char>('a' + i);
//             std::cout << letter;
//             if (node->child[i]->isWord) std::cout << "*";
//             std::cout << "\n";
//             displayNode(node->child[i].get(), depth + 1);
//         }
//     }
// }
