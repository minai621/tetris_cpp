#ifndef GUARD_grade_h
#define GUARD_grade_h

#include <vector>
#include "score.h"

double grade(double midterm, double final, double homework);
double grade(double midterm, double final, const std::vector<double> &hw);
double grade(const Student_info &s);

#endif