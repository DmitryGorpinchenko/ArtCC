#include "json.h"
#include "database.h"

#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, const char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    /*if (argc != 2) {
        cerr << "Usage: transport_catalog_part_o [make_base|process_requests]\n";
        return 5;
    }

    const string_view mode(argv[1]);*/

    auto dict = Json::Load(cin).GetRoot().AsDict();
    Json::Document req_doc(dict.at("stat_requests"));
    dict.erase("stat_requests");
    Json::Document build_doc(dict);
    Json::Save(cout, ProcessRequests(Database(build_doc), req_doc));
    cout << '\n';
    
    /*auto dict = Json::Load(cin).GetRoot().AsDict();
    auto file = dict.at("serialization_settings").AsDict().at("file").AsString();
    if (mode == "make_base") {
        ofstream os(file);
        dict.erase("serialization_settings");
        Database(Json::Document(dict)).Serialize(os);
    } else if (mode == "process_requests") {
        ifstream is(file);
        Json::Save(cout, ProcessRequests(Database(is), Json::Document(dict.at("stat_requests"))));
        cout << '\n';
    }*/
    return 0;
}

