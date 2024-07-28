#ifndef MOD_H
#define MOD_H

#include <cmath>

/*
Actual modulo formula. IDK why c++ uses remainder and not modulo
*/
inline double mod(double a, double b) { return a - b * std::floor(a / b); }

#endif