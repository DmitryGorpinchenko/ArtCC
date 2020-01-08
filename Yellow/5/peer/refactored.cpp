#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person(const string& name, const string& type)
        : Name(name)
        , Type(type)
    {}

    const string& GetType() const { return Type; }
    const string& GetName() const { return Name; }
    string GetInfo() const { return Type + ": " + Name; }

    virtual void Walk(const string& destination) {
        cout << GetInfo() << " walks to: " << destination << endl;
    }

private:
    string Name;
    const string Type;
};

class Student : public Person {
public:
    Student(const string& name, const string& favouriteSong)
        : Person(name, "Student")
        , FavouriteSong(favouriteSong)
    {}
    
    void Walk(const string& destination) override {
        Person::Walk(destination);
        SingSong();
    }

    void Learn() {
        cout << GetInfo() << " learns" << endl;
    }

    void SingSong() {
        cout << GetInfo() << " sings a song: " << FavouriteSong << endl;
    }

private:
    string FavouriteSong;
};

class Teacher : public Person {
public:
    Teacher(const string& name, const string& subject)
        : Person(name, "Teacher")
        , Subject(subject)
    {}

    void Teach() {
        cout << GetInfo() << " teaches: " << Subject << endl;
    }

private:
    string Subject;
};

class Policeman : public Person {
public:
    Policeman(const string& name) : Person(name, "Policeman") {}

    void Check(const Person& p) {
        cout << GetInfo() << " checks " << p.GetType() << ". " << p.GetType() << "'s name is: " << p.GetName() << endl;
    }
};

void VisitPlaces(Person& person, const vector<string>& places) {
    for (const auto& p : places) {
        person.Walk(p);
    }
}

int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}

