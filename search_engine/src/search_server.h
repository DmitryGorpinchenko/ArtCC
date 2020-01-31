#pragma once

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
using namespace std;

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
    void Add(string document);
    const vector<DocRank>& Lookup(string_view word) const;

    const string& GetDocument(size_t id) const {
        return docs[id];
    }
    
    size_t Size() const { return docs.size(); }
    bool Empty() const { return Size() == 0; }

private:
    map<string_view, vector<DocRank>> index;
    deque<string> docs;
};

class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(istream& document_input);
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    void UpdateDocumentBaseImpl(istream& document_input);
    void AddQueriesStreamImpl(istream& query_input, ostream& search_results_output);

    shared_mutex m;
    InvertedIndex index;
    bool first = true;
    
    vector<future<void>> futures; // ensure index is not destroyed when futures destructor runs 
};
