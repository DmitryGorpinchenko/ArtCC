#include <map>
#include <iostream>
#include <string>
#include <vector>

// Serialization

template <typename T> void Serialize(T pod, std::ostream& out);
inline void Serialize(const std::string& str, std::ostream& out);
template <typename T> void Serialize(const std::vector<T>& data, std::ostream& out);
template <typename T1, typename T2> void Serialize(const std::map<T1, T2>& data, std::ostream& out);

template <typename T>
void Serialize(T pod, std::ostream& out) {
    out.write(reinterpret_cast<const char*>(&pod), sizeof(pod));
}

void Serialize(const std::string& str, std::ostream& out) {
    Serialize(str.size(), out);
    out.write(str.c_str(), str.size());
}

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
    Serialize(data.size(), out);
    for (const auto& item : data) {
        Serialize(item, out);
    }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
    Serialize(data.size(), out);
    for (const auto& [k, v] : data) {
        Serialize(k, out);
        Serialize(v, out);
    }
}

// Deserialization

template <typename T> void Deserialize(std::istream& in, T& pod);
inline void Deserialize(std::istream& in, std::string& str);
template <typename T> void Deserialize(std::istream& in, std::vector<T>& data);
template <typename T1, typename T2> void Deserialize(std::istream& in, std::map<T1, T2>& data);

template <typename T>
void Deserialize(std::istream& in, T& pod) {
    in.read(reinterpret_cast<char*>(&pod), sizeof(pod));
}

void Deserialize(std::istream& in, std::string& str) {
    size_t size = 0;
    Deserialize(in, size);
    str.assign(size, '\0');
    in.read(str.data(), size);
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
    size_t size = 0;
    Deserialize(in, size);
    data.assign(size, T());
    for (auto& item : data) {
        Deserialize(in, item);
    }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
    size_t size = 0;
    Deserialize(in, size);
    data.clear();
    for (size_t i = 0; i < size; ++i) {
        T1 k;
        T2 v;
        Deserialize(in, k);
        Deserialize(in, v);
        data.emplace(k, v);
    }
}

