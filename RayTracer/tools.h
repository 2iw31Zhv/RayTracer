#pragma once
#include <algorithm>
#include <complex>
#include <cmath>
#include <vector>

#include <QPointF>
#include <QColor>

#include "Point3.h"

#define PI 3.14159265
class Vertice;
class Mesh;
class BezierCurve;

void convert(Mesh& mesh, const BezierCurve& curv, int angle_num);
bool load_obj(Mesh& mesh, const std::string filename, Point3F center, float scale);
QPointF orthogonal_project(const Vertice& v);

QColor operator+(const QColor& lhs, const QColor& rhs);

QColor operator*(float val, const QColor& rhs);

QColor operator*(const QColor& lhs, float val);

QColor operator*(const Point3F& lhs, const QColor& rhs);

QColor operator*(const QColor& lhs, const Point3F& rhs);

void givens(float& x1, float& x2, float theta);

double determinant2(double a11, double a12, double a21, double a22);

double determinant3(double a11, double a12, double a13,
	double a21, double a22, double a23,
	double a31, double a32, double a33);

double nrmi(int n, double * x);

void copy_arr(int n, const double * a, double * b);

//void quartic_equation(double a, double b, double c, double d, double e,
//	std::complex<double> &x1, std::complex<double> &x2,
//	std::complex<double> &x3, std::complex<double> &x4
//);

double pow2(double x);
double pow4(double x);

int solve_quadratic(double * x, double a, double b, double c);
int solve_quartic(double * x, double a, double b, double c, double d, double e);