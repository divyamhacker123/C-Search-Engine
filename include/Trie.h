#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <mutex> 
#include <string>
#include <vector>

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord = false;
    int frequency = 0; 
};

class Trie {
private:
    TrieNode* root;
    mutable std::mutex mtx; 

    void dfs(TrieNode* node, std::string currentWord, std::vector<std::string>& results, int limit) const {
        if (results.size() >= limit) return;
        if (node->isEndOfWord) results.push_back(currentWord);
        
        for (auto const& [ch, childNode] : node->children) {
            dfs(childNode, currentWord + ch, results, limit);
        }
    }

public:
    Trie() { root = new TrieNode(); }

    void insert(const std::string& word) {
        std::lock_guard<std::mutex> lock(mtx);
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
        current->frequency++;
    }

    std::vector<std::string> getSuggestions(const std::string& prefix, int limit = 5) const {
        std::lock_guard<std::mutex> lock(mtx);
        TrieNode* current = root;
        std::vector<std::string> results;
        
        for (char ch : prefix) {
            if (current->children.find(ch) == current->children.end()) {
                return results; 
            }
            current = current->children[ch];
        }
        
        dfs(current, prefix, results, limit);
        return results;
    }
};

#endif 