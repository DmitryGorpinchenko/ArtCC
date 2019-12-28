struct Incognizable {
    int a = 0;
    int b = 0;
};

/*class Incognizable {
public:
    Incognizable(int _a = 0, int _b = 0) : a(_a), b(_b) {}
private:
    int a;
    int b;
};*/

int main() {
    Incognizable a;
    Incognizable b = {};
    Incognizable c = {0};
    Incognizable d = {0, 1};
    return 0;
}
