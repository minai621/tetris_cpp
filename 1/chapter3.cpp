#include <iostream>
using namespace std;

int main()
{
    cout << "Please enter your first name: ";
    string name;
    cin >> name;
    cout << "Hello, " << name << "!" << endl;

    cout << "Please enter your midterm and final exam grades: ";
    double midterm, final;
    cin >> midterm >> final;

    cout << "Enter all your homework grades, "
            "followed by end-of-file: ";

    int count = 0;
    double sum = 0;

    double x;
    vector<double> homework;

    while (cin >> x)
    {
        homework.push_back(x);
    }

    typedef vector<double>::size_type vec_sz;
    vec_sz size = homework.size();

    if (size == 0)
    {
        cout << endl
             << "You must enter your grades. "
                "Please try again."
             << endl;
        return 1;
    }

    sort(homework.begin(), homework.end());

    streamsize prec = cout.precision();
    cout << "Your final grade is " << 0.2 * midterm + 0.4 * final + 0.4 * sum / count << endl;
    cout.precision(prec);

    system("pause");
    return 0;
}
