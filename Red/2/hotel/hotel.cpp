#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <cstdint>
#include <limits>

using namespace std;

static const int64_t TIME_WINDOW = 86400;
static int64_t curr_time = numeric_limits<int64_t>::min();

class Hotel {
public:
    void Book(int64_t time, int client, int rooms) {
        data.q.push({ time, client, rooms });
        data.rooms_count += rooms;
        ++data.clients[client];
    }
    
    int Clients() const {
        LazyUpdateData();
        return data.clients.size();
    }
    int Rooms() const {
        LazyUpdateData();
        return data.rooms_count;
    }
private:
    void LazyUpdateData() const {
        auto& q = data.q;
        while (!q.empty()) {
            const auto& front = q.front();
            if (front.time + TIME_WINDOW > curr_time) {
                break;
            }
            data.rooms_count -= front.rooms;
            auto& cnt = data.clients[front.client];
            --cnt;
            if (cnt == 0) {
                data.clients.erase(front.client);
            }
            q.pop();
        }
    }

    struct {
        struct Record {
            int64_t time;
            int client;
            int rooms;
        };
        queue<Record> q;
        map<int, int> clients;
        int rooms_count = 0;
    } mutable data;
};

class HotelsManager {
public:
    void Book(const string& name, int64_t time, int client, int rooms) {
        curr_time = time;
        hotels[name].Book(time, client, rooms);
    }
    
    int Clients(const string& name) const {
        auto it = hotels.find(name);
        if (it != end(hotels)) {
            return (*it).second.Clients();
        }
        return 0;
    }
    int Rooms(const string& name) const {
        auto it = hotels.find(name);
        if (it != end(hotels)) {
            return (*it).second.Rooms();
        }
        return 0;
    }
    
private:
    map<string, Hotel> hotels;
};

//

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    HotelsManager hotels;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type, hotel_name;
        cin >> query_type;
        if (query_type == "BOOK") {
            int64_t time;
            int client, rooms;
            cin >> time >> hotel_name >> client >> rooms;
            hotels.Book(hotel_name, time, client, rooms);
        } else if (query_type == "CLIENTS") {
            cin >> hotel_name;
            cout << hotels.Clients(hotel_name) << '\n';
        } else if (query_type == "ROOMS") {
            cin >> hotel_name;
            cout << hotels.Rooms(hotel_name) << '\n';
        }
    }

    return 0;
}
