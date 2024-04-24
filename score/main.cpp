#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "grade.h"
#include "score.h"

using std::cin;
using std::cout;
using std::domain_error;
using std::endl;
using std::istream;
using std::max;
using std::setprecision;
using std::sort;
using std::streamsize;
using std::string;
using std::vector;

// 학생의 과락 여부를 결정하는 서술 함수
bool fgrade(const Student_info &s)
{
    return grade(s) < 60;
}

vector<Student_info> extract_fails(vector<Student_info> &students)
{
    vector<Student_info> pass, fail;

    for (vector<Student_info>::size_type i = 0; i != students.size(); ++i)
    {
        if (fgrade(students[i]))
            fail.push_back(students[i]);
        else
            pass.push_back(students[i]);
    }
    students = pass;
    return fail;
}

int main()
{
    vector<Student_info> students;
    Student_info record;
    string::size_type maxlen = 0;

    while (read(cin, record))
    {
        maxlen = max(maxlen, record.name.size());
        students.push_back(record);
    }

    sort(students.begin(), students.end(), compare);

    for (vector<Student_info>::size_type i = 0; i != students.size(); ++i)
    {
        cout << students[i].name << string(maxlen + 1 - students[i].name.size(), ' ');

        try
        {
            double final_grade = grade(students[i]);
            streamsize prec = cout.precision();
            cout << setprecision(3) << final_grade << setprecision(prec);
        }
        catch (domain_error e)
        {
            cout << e.what();
        }

        cout << endl;
    }

    return 0;
}