#include "Trie.hpp"
#include "TrieCLI.hpp"

int main() {
    Trie trie;
    TrieCLI cli(trie);
    cli.run();
    return 0;
}
