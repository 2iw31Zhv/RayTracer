#include <cassert>
#include <algorithm>
#include "RotationBezier2.h"
#include "tools.h"

#include <iostream>
#include <cstdio>

RotationBezier2::RotationBezier2(const QPointF & c1, 
	const QPointF & c2, 
	const QPointF & c3,
	const Vertice& center,
	double size):
	c1_(c1), c2_(c2), c3_(c3),
	center_(center), size_(size)
{
	//assert(c2_.y() < c1_.y());
	//assert(c3_.y() < c2_.y());
}


// e + t d = center + ( rx cos(theta), ry, rx sin(theta))
// 3 equations, t, u, theta
// (e + t d - center.y) = ry
// (e + t d - center)^2_x + (e + t d - center)^2_z = rx^2
// 2 equations, t, u


// r = c1 (1 - u)^2 + 2 c2 u (1 - u) + c3 u^2
// r = c1 (1 - 2 u + u^2) + c2 ( 2u - 2u^2) + c3 u^2
// rx = Ax u^2 + Bx u + Cx
// ry = Ay u^2 + By u + Cy
void bezier_parameters_(double c1, double c2, double c3,
	double& A, double& B, double& C)
{
	A = c1 -2 * c2 + c3;
	B = 2 * c2 - 2 * c1;
	C = c1;
}

// local_eye.y + t d.y = Ay u^2 + By u + Cy
void t2u(double Ay, double By, double Cy, double local_eyey, double dy,
	double& Au, double& Bu, double& Cu)
{
	Au = Ay / dy;
	Bu = By / dy;
	Cu = (Cy - local_eyey) / dy;
}

// local_eye.x + d.x (Au u^2 + Bu u + Cu)
void fillqf(double Au, double Bu, double Cu, double local_eyei, double di,
	double &Aq, double &Bq, double &Cq)
{
	Aq = Au * di;
	Bq = Bu * di;
	Cq = Cu * di + local_eyei;
}

// (Aq u^2 + Bq u + Cq) * (Aq u^2 + Bq u + Cq)
// = Aq * Aq u^4 + 2 Aq Bq u^3 + (2 Aq Cq + Bq^2) u^2 + 2 Bq Cq u + Cq^2
void qf2pl(double Aq, double Bq, double Cq,
	double &A, double &B, double &C, double &D, double &E,
	double lambda)
{
	A += lambda * (Aq * Aq);
	B += lambda * (2 * Aq * Bq);
	C += lambda * (2 * Aq * Cq + Bq * Bq);
	D += lambda * (2 * Bq * Cq);
	E += lambda * (Cq * Cq);
}

// local_eye.y + t d.y = Ay u^2 + By u + Cy
// (local_eye.x + t d.x)^2 + (local_eye.z + t d.z)^2 = (Ax u^2 + Bx u + Cx)^2
void solve_hit_u(const Point3F& local_eye, const Point3F& direction, 
	double Ax, double Bx, double Cx,
	double Ay, double By, double Cy,
	int * n, double * u)
{
	if (fabs(direction.y()) < 1e-3)
	{
		// A u^2 + B u + C = 0
		*n = solve_quadratic(u, Ay, By, Cy - local_eye.y());
	}
	else
	{
		// get the parameters
		// local_eye.y + t d.y = Ay u^2 + By u + Cy
		// t = (Ay u^2 + By u + Cy - local_eye.y) / d.y
		double Au, Bu, Cu;
		t2u(Ay, By, Cy, local_eye.y(), direction.y(), Au, Bu, Cu);
		
		// (A1 u^2 + B1 u + C1)^2
		double A1, B1, C1;
		fillqf(Au, Bu, Cu, local_eye.x(), direction.x(), A1, B1, C1);

		// (A2 u^2 + B2 u + C2)^2
		double A2, B2, C2;
		fillqf(Au, Bu, Cu, local_eye.z(), direction.z(), A2, B2, C2);

		double A(.0f), B(.0f), C(.0f), D(.0f), E(.0f);
		// expand to A, B, C, D, E

		qf2pl(Ax, Bx, Cx,
			A, B, C, D, E, -1.0);
		
		qf2pl(A1, B1, C1,
			A, B, C, D, E, 1.0);

		qf2pl(A2, B2, C2,
			A, B, C, D, E, 1.0);

		// A u^4 + B u^3 + C u^2 + D u + E = 0
		*n = solve_quartic(u, A, B, C, D, E);
	}
}

bool RotationBezier2::hit(const Ray & ray, double t0, double t1, double & t)
{
	// solve the respective t and u
	Point3F local_eye = ray.e() - center_;
	double Ax, Bx, Cx;
	double Ay, By, Cy;
	bezier_parameters_(c1_.x(), c2_.x(), c3_.x(), Ax, Bx, Cx);
	bezier_parameters_(c1_.y(), c2_.y(), c3_.y(), Ay, By, Cy);

	int n;
	double u[4];
	solve_hit_u(local_eye, ray.d(), Ax, Bx, Cx,
		Ay, By, Cy, &n, u);

	std::sort(u, u + n);
	std::vector<double> uvec;
	for (int i = 0; i < n; ++i)
	{
		if (.0 <= u[i] && u[i] <= 1.0)
		{
			uvec.push_back(u[i]);
		}
	}
	
	auto new_end = std::unique(uvec.begin(), uvec.end(), [](double a, double b) {
		return fabs(a - b) < 1e-7; });
	uvec.erase(new_end, uvec.end());

	
	if (uvec.size() > 2)
	{

		std::cerr << "BAD THING! " << __LINE__ << std::endl;
		while (1);
	}
	// solve t from u
	
	std::vector<double> tvec;
	if (fabs(ray.d().y()) < 1e-3)
	{
		if (uvec.size() > 1)
		{
			std::cerr << "BAD THING! " << __LINE__ << std::endl;
			while (1);
		}
		// (local_eye.x + t d.x)^2 + (local_eye.z + t d.z)^2 = (Ax u^2 + Bx u + Cx)^2
		for (auto u : uvec)
		{
			
			double t_buff[2];
			int m = solve_quadratic(t_buff,
				pow2(ray.d().x()) + pow2(ray.d().z()),
				2 * (ray.d().x() * local_eye.x() + ray.d().z() * local_eye.z()),
				pow2(local_eye.x()) + pow2(local_eye.z()) - pow2(Ax * u * u + Bx * u + Cx));

			while (m--)
			{
				tvec.push_back(t_buff[m]);
			}
		}
	}
	else
	{
		// local_eye.y + t d.y = Ay u^2 + By u + Cy
		for (auto u : uvec)
		{
			double t_temp = (Ay * u * u + By * u + Cy - local_eye.y()) / ray.d().y();
			tvec.push_back(t_temp);
		}
	}
	std::sort(tvec.begin(), tvec.end());

	if (tvec.size() == 0)
	{
		return false;
	}

	// find the minimum t
	if (tvec.size() >= 1 && t0 <= tvec[0] && tvec[0] < t1)
	{
		t = tvec[0];
		return true;
	}
	else if (tvec.size() >= 2 && t0 <= tvec[1] && tvec[1] < t1)
	{
		t = tvec[1];
		return true;
	}
	else
	{
		return false;
	}

}

// local_eye.y + t d.y = Ay u^2 + By u + Cy
// (local_eye.x + t d.x)^2 + (local_eye.z + t d.z)^2 = (Ax u^2 + Bx u + Cx)^2
Point3F RotationBezier2::hit_normal(const Ray & ray, double t) const
{
	Point3F local_hit = ray.at(t) - center_;
	double ay, by, cy;
	bezier_parameters_(c1_.y(), c2_.y(), c3_.y(), ay, by, cy);

	double u_buff[2];
	int m = solve_quadratic(u_buff, ay, by, cy - local_hit.y());

	double u = 0.0;
	bool exist = false;

	if (m == 1)
	{
		if (.0 <= u_buff[0] && u_buff[0] <= 1.0)
		{
			u = u_buff[0];
			exist = true;
		}
	}
	else if (m == 2)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (.0 <= u_buff[i] && u_buff[i] <= 1.0)
			{
				u = u_buff[0];
				exist = true;
			}
		}
	}

	double ax, bx, cx;
	bezier_parameters_(c1_.x(), c2_.x(), c3_.x(), ax, bx, cx);

	double dxu = 2.0 * ax * u + bx;
	double dyu = 2.0 * ay * u + by;

	if (dyu > 0.0)
	{
		dyu = -dyu;
		dxu = -dxu;
	}

	QPointF n(-dyu, dxu);

	QPointF st(local_hit.x(), local_hit.z());
	double nrmst = sqrt(st.x() * st.x() + st.y() * st.y());
	st /= nrmst;
	st *= -dyu;

	return Point3F(st.x(), dxu, st.y()).normalize();

}

