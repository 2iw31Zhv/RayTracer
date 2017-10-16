#include <algorithm>
#include <complex>
#include "tools.h"

#include "Bessel.h"
#include "Mesh.h"
#include "Geometry.h"
#include "poly34.h"


void convert(Mesh& mesh, const BesselCurve& curv, const int angle_num)
{
	mesh.reset();

	size_t floors = curv.samples_size();

	//insert points
	for (int i = 0; i < floors; ++i)
	{
		const QPointF& samplePoint = curv.at(i);
		float r = samplePoint.x();
		float z = samplePoint.y();

		if (std::fabs(r) < 1e-5)
		{
			mesh.insert(Vertice(0.0f, 0.0f, z));
			continue;
		}

		for (int t = 0; t < angle_num; ++t)
		{
			float x = r * cos(t * 2.0 * PI / angle_num);
			float y = r * sin(t * 2.0 * PI / angle_num);
			mesh.insert(Vertice(x, y, z));
		}
	}

	for (int t = 1; t < angle_num; ++t)
	{
		mesh.insert(TriFacet(t + 2, t + 1, 1));
	}
	mesh.insert(TriFacet(2, angle_num + 1, 1));
	
	for (int i = 1; i < floors - 2; ++i)
	{
		int start_id = (i - 1) * angle_num + 1;
		for (int t = 0; t < angle_num - 1; ++t)
		{
			mesh.insert(QuadFacet(start_id + t + 1, 
				start_id + t + 2, 
				start_id + t + angle_num + 2,
				start_id + t + angle_num + 1));
		}
		mesh.insert(QuadFacet(start_id + angle_num,
			start_id + 1,
			start_id + angle_num + 1,
			start_id + 2 * angle_num));
	}

	int tri_start_id = (floors - 3) * angle_num + 1;
	for (int t = 0; t < angle_num - 1; ++t)
	{
		mesh.insert(TriFacet(tri_start_id + t + 1,
			tri_start_id + t + 2,
			(floors - 2) * angle_num + 2));
	}
	
	mesh.insert(TriFacet(tri_start_id + angle_num,
		tri_start_id + 1,
		(floors - 2) * angle_num + 2));
}

QPointF orthogonal_project(const Vertice& v)
{
	return QPointF(v.x(), v.z());
}

QColor operator+(const QColor& lhs, const QColor& rhs)
{
	using namespace std;
	return QColor(min(255, lhs.red() + rhs.red()),
		min(255, lhs.green() + rhs.green()),
		min(255, lhs.blue() + rhs.blue()));
}

QColor operator*(float val, const QColor& rhs)
{
	return QColor(val * rhs.red(),
		val * rhs.green(),
		val * rhs.blue());
}

QColor operator*(const QColor& lhs, float val)
{
	return QColor(lhs.red() * val,
		lhs.green() * val,
		lhs.blue() * val);
}

QColor operator*(const Point3F & lhs, const QColor & rhs)
{
	return QColor(lhs.x() * rhs.red(),
		lhs.y() * rhs.green(),
		lhs.z() * rhs.blue());
}

QColor operator*(const QColor & lhs, const Point3F & rhs)
{
	return QColor(lhs.red() * rhs.x(),
		lhs.green() * rhs.y(),
		lhs.blue() * rhs.z());
}

void givens(float& x1, float& x2, float theta)
{
	float x1_temp = x1;
	float x2_temp = x2;
	x1 = std::cos(theta) * x1_temp - std::sin(theta) * x2_temp;
	x2 = std::sin(theta) * x1_temp + std::cos(theta) * x2_temp;
}

double determinant2(double a11, double a12, double a21, double a22)
{
	return (a11 * a22 - a12 * a21);
}

double determinant3(double a11, double a12, double a13,
	double a21, double a22, double a23,
	double a31, double a32, double a33)
{
	return a11 * determinant2(a22, a23, a32, a33)
		- a12 * determinant2(a21, a23, a31, a33)
		+ a13 * determinant2(a21, a22, a31, a32);
}

double nrmi(int n, double * x)
{
	using std::max;
	double r = 0.0;
	for (int i = 0; i < n; ++i)
	{
		r = max(r, fabs(x[i]));
	}
	return r;
}

void copy_arr(int n, const double * a, double * b)
{
	for (int i = 0; i < n; ++i)
	{
		b[i] = a[i];
	}
}

double pow2(double x)
{
	return x * x;
}

double pow4(double x)
{
	return x * x * x * x;
}

int solve_quartic(double * x, double a, double b, double c, double d, double e)
{
	double tolerance = DBL_MIN;
	if (fabs(a) < tolerance && fabs(b) < tolerance && fabs(c) < tolerance && fabs(d) < tolerance)
	{
		return 0;
	}
	else if (fabs(a) < tolerance && fabs(b) < tolerance && fabs(c) < tolerance)
	{
		x[0] = - e / d;
		return 1;
	}
	else if (fabs(a) < tolerance && fabs(b) < tolerance)
	{
		return SolveP2(x, d / c, e / c);
	}
	else if (fabs(a) < tolerance)
	{
		return SolveP3(x, c / b, d / b, e / b);
	}
	else
	{
		return SolveP4(x, b / a, c / a, d / a, e / a);
	}
}
