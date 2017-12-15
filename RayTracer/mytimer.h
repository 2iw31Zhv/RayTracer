#pragma once
#include <ctime>
#include <ratio>
#include <chrono>
#include <cstdio>

using namespace std::chrono;

steady_clock::time_point t1, t2;
duration<double> time_span;
#define tic t1 = steady_clock::now();

#define toc t2 = steady_clock::now();\
time_span = duration_cast<duration<double>>(t2 - t1);\
printf("time: %f\n", time_span.count());