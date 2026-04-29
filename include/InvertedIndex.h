#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include "DataTypes.h"
#include <unordered_map>
#include <mutex> 
#include <string>
#include <vector>

class InvertedIndex {
private:
    // particular word freq in every document
    // index[word] = vector<{documentId, freq}>
    std::unordered_map<std::string, std::vector<Posting>> index;
    mutable std::mutex mtx; // Stable standard mutex

public:
    void addTerm(const std::string& term, int docId, int frequency) {
        std::lock_guard<std::mutex> lock(mtx);
        
        auto& postings = index[term];
        bool found = false;
        for (auto& p : postings) {
            if (p.docId == docId) {
                p.termFrequency += frequency;
                found = true;
                break;
            }
        }
        if (!found) {
            postings.emplace_back(docId, frequency);
        }
    }

    std::vector<Posting> search(const std::string& term) const {
        std::lock_guard<std::mutex> lock(mtx);
        
        auto it = index.find(term);
        if (it != index.end()) {
            return it->second;
        }
        return {}; 
    }
};

#endif 