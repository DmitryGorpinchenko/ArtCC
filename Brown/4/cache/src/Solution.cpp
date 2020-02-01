#include "Common.h"

#include <list>
#include <unordered_map>
#include <mutex>

using namespace std;

class LruCache : public ICache {
public:
    LruCache(shared_ptr<IBooksUnpacker> books_unpacker, const Settings& settings)
        : unpacker(move(books_unpacker))
        , setts(settings)
    {}

    BookPtr GetBook(const string& book_name) override {
        unique_lock lock(m);
        
        auto it = index.find(book_name);
        if (it != end(index)) {
            auto data_it = (*it).second;
            if (data_it != begin(data)) {
                data.splice(begin(data), data, data_it);
            }
            return *data_it;
        }
        
        BookPtr book = unpacker->UnpackBook(book_name);
        size_t bytes = book->GetContent().size();
        if (MakeRoomFor(bytes)) {
            data.push_front(book);
            index.emplace(book->GetName(), begin(data));
            cached_bytes += bytes;
        }
        return book;
    }
private:
    bool MakeRoomFor(size_t bytes) {
        while ((!data.empty()) && (cached_bytes + bytes > setts.max_memory)) {
            auto& b = data.back();
            size_t sz = b->GetContent().size();
            index.erase(b->GetName());
            data.pop_back();
            cached_bytes -= sz;
        }
        return !(cached_bytes + bytes > setts.max_memory);
    }

    shared_ptr<IBooksUnpacker> unpacker;
    Settings setts;
    
    mutex m;
    list<BookPtr> data;
    unordered_map<string_view, list<BookPtr>::iterator> index;
    size_t cached_bytes = 0;
};

unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker, const ICache::Settings& settings) {
    return make_unique<LruCache>(move(books_unpacker), settings);
}
