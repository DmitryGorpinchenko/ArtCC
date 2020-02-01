#include "json.h"
#include "database.h"

#include <iostream>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    auto dict = Json::Load(cin).GetRoot().AsDict();
    Json::Document req_doc(dict.at("stat_requests"));
    
    dict.erase("stat_requests");
    Json::Document build_doc(dict);
    
    Json::Save(cout, ProcessRequests(Database(build_doc), req_doc));
    cout << '\n';
    return 0;
}

