#ifndef DATATYPES_H
#define DATATYPES_H

#include <string>
#include <vector>

struct Posting {
    int docId;
    int termFrequency;
    
    Posting(int id, int freq) : docId(id), termFrequency(freq) {}
};

struct Document {
    int id;
    std::string title;
    std::string content;
};

#endif 