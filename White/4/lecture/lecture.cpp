#include <string>

struct Specialization {
    std::string val;
    explicit Specialization(const std::string& _val) : val(_val) {}
};
struct Course {
    std::string val;
    explicit Course(const std::string& _val) : val(_val) {}
};
struct Week {
    std::string val;
    explicit Week(const std::string& _val) : val(_val) {}
};

struct LectureTitle {
    std::string specialization;
    std::string course;
    std::string week;

    LectureTitle(const Specialization& s, const Course& c, const Week& w)
        : specialization(s.val)
        , course(c.val)
        , week(w.val)
    {}
};

int main() {
    LectureTitle title(
        Specialization("C++"),
        Course("White belt"),
        Week("4th")
    );

    //LectureTitle title1("C++", "White belt", "4th");
    //LectureTitle title2(std::string("C++"), std::string("White belt"), std::string("4th"));
    //LectureTitle title3 = { "C++", "White belt", "4th" };
    //LectureTitle title4 = { { "C++" }, { "White belt" }, { "4th" } };
    /*LectureTitle title(
        Course("White belt"),
        Specialization("C++"),
        Week("4th")
    );*/
    /*LectureTitle title(
        Specialization("C++"),
        Week("4th"),
        Course("White belt")
    );*/

    return 0;
}
