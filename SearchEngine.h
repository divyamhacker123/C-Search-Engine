#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "InvertedIndex.h"
#include "Trie.h"
#include "ThreadPool.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

class SearchEngine {
private:
    InvertedIndex index;
    Trie autocompleteTrie;
    ThreadPool threadPool;
    std::unordered_map<int, Document> documentStore; 
    std::mutex docStoreMutex; 
    std::mutex printMutex; 

    std::vector<std::string> tokenize(const std::string& text) {
        std::vector<std::string> tokens;
        std::string word;
        // make a stream of word
        std::istringstream stream(text);
        // take all words in the string text to word
        while (stream >> word) {
            std::string cleanWord = "";
            for (char c : word) {
                if (std::isalnum(c)) cleanWord += std::tolower(c);
            }
            if (!cleanWord.empty()) tokens.push_back(cleanWord);
        }
        return tokens;
    }

public:
    SearchEngine(size_t threads = 4) : threadPool(threads) {}

    void indexDocumentAsync(int docId, const std::string& title, const std::string& content) {
        threadPool.enqueue([this, docId, title, content] {
            
            {
                std::lock_guard<std::mutex> lock(docStoreMutex);
                documentStore[docId] = {docId, title, content};
            }

            std::vector<std::string> tokens = tokenize(content);
            
            std::unordered_map<std::string, int> localFreqs;
            for (const std::string& token : tokens) {
                localFreqs[token]++;
            }

            for (const auto& [term, freq] : localFreqs) {
                index.addTerm(term, docId, freq);
                autocompleteTrie.insert(term);
            }
            
            {
                std::lock_guard<std::mutex> printLock(printMutex);
                std::cout << "Indexed Document ID: " << docId << " on background thread.\n";
            }
        });
    }

    std::vector<Document> search(const std::string& query) {
        std::vector<std::string> tokens = tokenize(query);
        if (tokens.empty()) return {};

        std::vector<Posting> postings = index.search(tokens[0]);
        
        std::sort(postings.begin(), postings.end(), [](const Posting& a, const Posting& b) {
            return a.termFrequency > b.termFrequency;
        });

        std::vector<Document> results;
        std::lock_guard<std::mutex> lock(docStoreMutex);
        for (const auto& p : postings) {
            results.push_back(documentStore[p.docId]);
        }
        return results;
    }

    std::vector<std::string> getAutocomplete(const std::string& prefix) {
        std::string cleanPrefix = "";
        for (char c : prefix) {
            if (std::isalnum(c)) cleanPrefix += std::tolower(c);
        }
        return autocompleteTrie.getSuggestions(cleanPrefix);
    }
};

#endif 