# Mini Search Engine (C++)

A multi-threaded, in-memory search engine built entirely from scratch in C++. 

I built this project to get hands-on experience with how large-scale text retrieval systems work under the hood. Instead of relying on off-the-shelf solutions like Elasticsearch or Lucene, I implemented the core data structures, tokenization, and concurrency management manually.

## Core Features

* **Custom Inverted Index:** Maps string tokens to document IDs using `std::unordered_map` for fast, average O(1) lookups.
* **Real-time Autocomplete:** Implemented a custom Trie data structure to support fast prefix matching (O(L) time complexity, where L is the prefix length).
* **Concurrency & Multithreading:** Built a custom Thread Pool from scratch. Instead of spawning new threads per query, worker threads handle document indexing and searching asynchronously to maximize CPU utilization.
* **Thread Safety:** Implemented `std::mutex` locks to prevent race conditions and memory corruption during concurrent document ingestion.
* **Custom Tokenizer:** Streams, sanitizes, and normalizes raw text input without relying on heavy external regex libraries.

## Architecture

The system is designed around a producer-consumer pattern. When a batch of documents is loaded:
1. The text is passed to the **Tokenizer** to strip punctuation and normalize case.
2. The **Thread Pool** assigns the parsing tasks to available worker threads.
3. Threads safely acquire a lock (`docStoreMutex`) to update the global **Inverted Index** and the **Document Store**.
4. Simultaneously, words are inserted into the **Trie** to keep the autocomplete suggestions updated.

## Tech Stack
* **Language:** C++ (C++17 standard)
* **Build System:** CMake
* **Libraries:** Standard Template Library (STL) only. No external dependencies.

## Getting Started

### Prerequisites
* A C++17 compatible compiler (GCC, Clang, or MSVC)
* CMake (v3.10+)

### Build Instructions

Clone the repository and build using CMake:
```bash
git clone [https://github.com/YOUR_USERNAME/mini-search-engine.git](https://github.com/YOUR_USERNAME/mini-search-engine.git)
cd mini-search-engine
mkdir build && cd build
cmake ..
make