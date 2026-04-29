#include "./include/SearchEngine.h"
#include <iostream>
#include <chrono>

int main() {
    std::cout << "Initializing Multi-threaded Search Engine...\n";
    SearchEngine engine(4); 

    engine.indexDocumentAsync(1, "C++ Intro", "C++ is a highly optimized programming language for systems.");
    engine.indexDocumentAsync(2, "Multithreading Guide", "Thread safety in C++ requires mutexes. Concurrency is hard but optimized.");
    engine.indexDocumentAsync(3, "Data Structures", "A Trie is used for autocomplete. An inverted index handles search queries.");
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "\n--- Autocomplete Suggestions for 'opt' ---\n";
    auto suggestions = engine.getAutocomplete("opt");
    for (const auto& s : suggestions) {
        std::cout << "- " << s << "\n";
    }

    std::cout << "\n--- Search Results for 'C++' ---\n";
    auto results = engine.search("C++");
    for (const auto& doc : results) {
        std::cout << "Doc " << doc.id << ": " << doc.title << "\n";
    }

    return 0;
}