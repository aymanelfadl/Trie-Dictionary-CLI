#include "Trie.hpp"

#define END_FLAG isWord

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


void Trie::exportNodeDot(const Node* node,
                         std::ostream& out,
                         std::unordered_map<const Node*, int>& ids,
                         int& nextId) const
{
    if (!node) return;

    auto it = ids.find(node);
    if (it == ids.end()) {
        ids[node] = nextId++;
    }
    int myId = ids[node];

    out << "  n" << myId
        << " [shape=" << (node->END_FLAG ? "doublecircle" : "circle")
        << ",label=\"\"];\n";

    for (int i = 0; i < 26; i++) {
        if (node->child[i]) {
            const Node* ch = node->child[i].get();

            if (!ids.count(ch)) ids[ch] = nextId++;
            int chId = ids[ch];

            char letter = char('a' + i);

            out << "  n" << myId << " -> n" << chId
                << " [label=\"" << letter << "\"];\n";

            exportNodeDot(ch, out, ids, nextId);
        }
    }
}

void Trie::exportDot(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) return;

    out << "digraph Trie {\n";
    out << "  rankdir=LR;\n";
    out << "  splines=true;\n";
    out << "  bgcolor=\"white\";\n";
    out << "  nodesep=0.35;\n";
    out << "  ranksep=0.60;\n\n";

    out << "  node [\n";
    out << "    shape=circle,\n";
    out << "    style=filled,\n";
    out << "    fillcolor=\"#E3F2FD\",\n";
    out << "    color=\"#1565C0\",\n";
    out << "    fontname=\"Arial\",\n";
    out << "    fontsize=11,\n";
    out << "    margin=\"0.08\"\n";
    out << "  ];\n\n";
    out << "  edge [\n";
    out << "    color=\"#555555\",\n";
    out << "    fontname=\"Arial\",\n";
    out << "    fontsize=11,\n";
    out << "    arrowsize=0.7\n";
    out << "  ];\n\n";
    out << "  n0 [shape=box, fillcolor=\"#FFF9C4\", color=\"#F9A825\", label=\"ROOT\"];\n\n";

    std::unordered_map<const Node*, int> ids;
    int nextId = 0;

    exportNodeDot(root.get(), out, ids, nextId);

    out << "}\n";
}