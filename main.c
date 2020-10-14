/*
    Ayrton Cavalieri de Almeida, bisection method implementation
    Copyright (C) 2020  Ayrton Cavalieri de Almeida

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <alloca.h>
#include <math.h>
#include <time.h>

#define MAXITERATIONS 10000

double _abs(double x)
{
    return x >= 0 ? x : -x;
}

double errR(double(*func)(double *params), double *lParam)
{
    return _abs(func(lParam));
}

double errX(double x, double xBef) //x -> x(n), xBef = x(n - 1)
{
    return _abs(x - xBef);
}

//Function f allows you to find the square root of a number passed as argument in params[1]
double f(double *params)
{
    return pow(params[0], 2) - params[1];
}

double p(double *params)
{
    return pow(params[0], 5) - pow(params[0], 3) + (3*params[0]) - 5;
}

bool isNeg(double(*func)(double *params), double *lParam, double p1, double p2)
{
    lParam[0] = p1;
    double x1 = func(lParam);
    lParam[0] = p2;
    double x2 = func(lParam);
    return ((x1 * x2) < 0);
}
/*
    *func - Function that will be used for calculations
    *lParam - List of Parameters, the first index, [0], is the "x"
of the function, the other indexes are constants of the parameter function, if any.
    a - lower interval bound
    b - higher interval bound
    epsilon - Error of the result
*/
double bissec(double(*func)(double *params), double *lParam, double a, double b, double epsilon)
{
    double x, xOld;
    double _errX, _errR;
    if(isNeg(func, lParam, a, b) == false){
        return NAN;
    }
    x = (a + b)/2.0;
    lParam[0] = x;
#ifdef DEBUG
    _errR = errR(func, lParam);
    printf("%.15f\t%.15f\t%.15f\n", x, NAN, _errR);
#endif
    if(func(lParam) == 0.0){
        return x;
    }
    unsigned int i;
    for(i = 0; i < MAXITERATIONS; ++i){
        xOld = x;
        if(isNeg(func, lParam, a, x) == true)
            b = x;
        else
            a = x;
        x = (a + b)/2.0;
        lParam[0] = x;
        _errX = errX(x, xOld);
        _errR = errR(func, lParam);
#ifdef DEBUG
        printf("%.15f\t%.15f\t%.15f\n", x, _errX, _errR);
#endif
        if(_errX <= epsilon && _errR <= epsilon){
            break;
        }
    }
#ifdef DEBUG
    printf("Total Iterations: %d\n", i + 1);
#endif
    if(i == MAXITERATIONS){
        return NAN;
    }
    return x;
}

int main(int argc, const char *argv[])
{
    //double *lParam = (double*)alloca(sizeof(double) * 2);
    //lParam[1] = 10;
    //double ret = bissec(f, lParam, 0, 10, pow(10, -10));
    double *lParam = (double*)alloca(sizeof(double));
    clock_t start, end;
    double cpu_time;
    start = clock();
    double ret = bissec(p, lParam, 1, 2, pow(10, -14));
    end = clock();
    cpu_time = ((double) (end - start)) / (double)CLOCKS_PER_SEC;
    printf("Result = %.15f\n", ret);
    printf("Time %.9f\n", cpu_time);
    return 0;
}