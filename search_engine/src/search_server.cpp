#include "search_server.h"

#include <algorithm>
#include <iterator>
#include <queue>
#include <functional>

vector<string_view> SplitIntoWords(string_view line) {
    vector<string_view> res;
    do {
        size_t lpos = line.find_first_not_of(' ');
        if (lpos == line.npos) {
            break;
        }
        size_t rpos = line.find(' ', lpos + 1);
        res.push_back(line.substr(lpos, rpos - lpos));
        if (rpos == line.npos) {
            break;
        }
        line.remove_prefix(rpos + 1);
    } while (true);
    return res;
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document); ) {
        new_index.Add(move(current_document));
    }

    index = move(new_index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
    vector<size_t> docid_count(index.Size(), 0);
    
    for (string current_query; getline(query_input, current_query); ) {
        docid_count.assign(index.Size(), 0);

        for (const auto w_view : SplitIntoWords(current_query)) {
            for (const auto& [docid, count] : index.Lookup(w_view)) {
                docid_count[docid] += count;
            }
        }

        priority_queue<DocRank, vector<DocRank>, greater<DocRank>> q;
        for (size_t i = 0; i < docid_count.size(); ++i) {
            if (docid_count[i] > 0) {
                DocRank rank = { i, docid_count[i] };
                if (q.size() < 5) {
                    q.push(rank);
                } else if (rank > q.top()) {
                    q.pop();
                    q.push(rank);
                }
            }
        }

        deque<DocRank> res;
        while (!q.empty()) {
            res.push_front(q.top());
            q.pop();
        }
        
        current_query += ':';
        for (const auto& [docid, hitcount] : res) {
            current_query += " {docid: ";
            current_query += to_string(docid) + ", hitcount: ";
            current_query += to_string(hitcount) + '}';
        }
        current_query += '\n';
        search_results_output << current_query;
    }
}

void InvertedIndex::Add(string document) {
    docs.push_back(move(document));
    const size_t docid = docs.size() - 1;
    for (const auto w_view : SplitIntoWords(docs.back())) {
        auto& doc_counts = index[w_view];
        if (doc_counts.empty() || (doc_counts.back().docid != docid)) {
            doc_counts.push_back({ docid, 1 });
        } else {
            ++doc_counts.back().count;
        }
    }
}

const vector<DocRank>& InvertedIndex::Lookup(string_view word) const {
    if (auto it = index.find(word); it != index.end()) {
        return (*it).second;
    }
    static const vector<DocRank> null;
    return null;
}
