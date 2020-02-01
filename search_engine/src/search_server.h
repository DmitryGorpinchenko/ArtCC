#pragma once

#include "synchronized.h"

#include <istream>
#include <ostream>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <utility>
#include <mutex>
#include <shared_mutex>
#include <future>

struct DocRank {
    size_t docid;
    size_t count;
    
    bool operator <(const DocRank& rhs) const {
        if (count != rhs.count) {
            return count < rhs.count;
        }
        return docid > rhs.docid;
    }
    bool operator >(const DocRank& rhs) const {
        return rhs < *this;
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;
    explicit InvertedIndex(std::istream& document_input);
    
    InvertedIndex(const InvertedIndex&) = delete; // string_view would point to elements from old deque
    InvertedIndex& operator =(const InvertedIndex&) = delete;
    InvertedIndex(InvertedIndex&&) = default;
    InvertedIndex& operator =(InvertedIndex&&) = default;

    void Add(std::string document);
    const std::vector<DocRank>& Lookup(std::string_view word) const;

    const std::string& GetDocument(size_t id) const {
        return docs[id];
    }
    
    size_t Size() const { return docs.size(); }
    bool Empty() const { return Size() == 0; }

private:
    std::deque<std::string> docs;
    std::map<std::string_view, std::vector<DocRank>> index; // view should not outlive data stored in 'docs'
};

class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(std::istream& document_input);
    void UpdateDocumentBase(std::istream& document_input);
    void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

private:
    void UpdateDocumentBaseImpl(std::istream& document_input);
    void AddQueriesStreamImpl(std::istream& query_input, std::ostream& search_results_output);

    Synchronized<InvertedIndex> index;
    std::vector<std::future<void>> futures; // ensure index is not destroyed when futures destructor runs
};
