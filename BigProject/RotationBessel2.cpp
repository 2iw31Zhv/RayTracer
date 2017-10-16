#include <cassert>
#include <algorithm>
#include "RotationBessel2.h"
#include "tools.h"

#include "easylogging++\easylogging++.h"

RotationBessel2::RotationBessel2(const QPointF & c1, 
	const QPointF & c2, 
	const QPointF & c3,
	const Vertice& center,
	float size):
	c1_(c1), c2_(c2), c3_(c3),
	center_(center), size_(size)
{
	//assert(c2_.y() < c1_.y());
	//assert(c3_.y() < c2_.y());
}

bool RotationBessel2::hit(const Ray & ray, float t0, float t1, float & t)
{
	double x1 = c1_.x();
	double y1 = c1_.y();
	double x2 = c2_.x();
	double y2 = c2_.y();
	double x3 = c3_.x();
	double y3 = c3_.y();

	double xc = center_.x();
	double yc = center_.y();
	double zc = center_.z();

	double xe = ray.e().x();
	double ye = ray.e().y();
	double ze = ray.e().z();
	double xd = ray.d().x();
	double yd = ray.d().y();
	double zd = ray.d().z();

	double s = size_;

	double dnmy = y1 - 2 * y2 + y3;
	double dnmx = x1 - 2 * x2 + x3;

	double a, b, c, d, e;

	if (fabs(dnmy) < 1e-3)
	{
		//if (fabs(yd) < 1e-3)
		//{
		//	double u = (s * y1 + yc - ye) / (2 * s *(y1 - y2));
		//	double xu = x1 * pow2(1 - u) + 2 * x2 * (1 - u) * u
		//		+ x3 * pow2(u);

		//	a = -pow2(xd) - pow2(zd);
		//	b = 2 * (xc - xe) * xd + 2 * (zc - ze) * zd;
		//	c = -pow2(xc - xe) + pow2(s) * pow2(xu) - pow2(zc - ze);
		//	assert(a < 0.0);

		//	double delta = b * b - 4 * a * c;
		//	if (delta < 0.0)
		//	{
		//		return false;
		//	}

		//	double tempt1 = (-b - sqrt(delta)) / 2 / a;
		//	double tempt2 = (-b + sqrt(delta)) / 2 / a;

		//	if (t0 <= tempt1 && tempt1 < t1)
		//	{
		//		t = tempt1;
		//		u_ = u;
		//		return true;
		//	}
		//	else if (t0 <= tempt2 && tempt2 < t1)
		//	{
		//		t = tempt2;
		//		u_ = u;
		//		return true;
		//	}
		//	else
		//	{
		//		return false;
		//	}
		//}

		double Au = (s * y1 + yc - ye) / (2 * s *(y1 - y2));
		double Bu = -yd / (2 * s *(y1 - y2));

		double A = pow2(Bu) * dnmx;
		double B = 2 * Bu * (x2 - x1) + 2 * Au * Bu * dnmx;
		double C = x1 - 2 * Au * x1 + pow2(Au) * x1 
			+ 2 * Au * x2 - 2 * pow2(Au) * x2 + pow2(Au) * x3;

		a = pow2(A) * pow2(s);
		b = 2 * A * B * pow2(s);
		c = pow2(B) * pow2(s) + 2 * A * C * pow2(s) - pow2(xd) - pow2(zd);
		d = 2 * B * C * pow2(s) + 2 * xc * xd
			- 2 * xd * xe + 2 * zc * zd - 2 * zd * ze;
		e = pow2(C) * pow2(s) - pow2(xc - xe) - pow2(zc - ze);
	}
	else
	{
		double Au = (y1 - y2) / dnmy;
		double Bu = s * dnmy;

		double Cu = (y2 * y2 - y1 * y3) * s * s
			- dnmy * s * (yc - ye);
		double Du = s * yd * dnmy;


		double Eu = x1 * ((Au - 1) * (Au - 1) + Cu / Bu / Bu)
			+ x2 * (2 * Au * (1 - Au) - 2 * Cu / Bu / Bu)
			+ x3 * (Au * Au + Cu / Bu / Bu);


		double Fu = Du * dnmx / Bu / Bu;

		double Gu = (2 * (1 - Au) * x1 - 2 * (1 - 2 * Au) * x2
			- 2 * Au * x3) / Bu;

		double Hu = (Eu * Eu + Cu * Gu * Gu) * s * s;
		double Iu = (2 * Eu * Fu + Du * Gu * Gu) * s * s;

		double A = -Fu * s * s + xd * xd
			+ zd * zd;
		double B = -Iu - 2 * (xc - xe) * xd
			- 2 * zd * (zc - ze);
		double C = -Hu + (xc - xe) * (xc - xe)
			+ (zc - ze) * (zc - ze);


		a = -A * A;
		b = -2 * A * B + 4 * Du * pow2(Fu * Gu * s * s);
		c = -B * B - 2 * A * C + 8 * Du * Eu * Fu * Gu* Gu * pow4(s)
			+ 4 * Cu * pow2(Fu) * pow2(Gu) * pow4(s);
		d = -2 * B * C + 4 * Du * pow2(Eu) * pow2(Gu) * pow4(s)
			+ 8 * Cu * Eu * Fu * pow2(Gu) * pow4(s);
		e = -C * C + 4 * Cu * pow2(Eu) * pow2(Gu) * pow4(s);

		if (fabs(yd) < 1e-3)
		{
			LOG(DEBUG) << a << " " << b << " " << c << " " << d << " " << e;
		}
	}

	double buffer[4];
	int root_num = solve_quartic(buffer, a, b, c, d, e);

	if (root_num > 1)
	{
		std::sort(buffer, buffer + root_num);
	}
	bool is_hit = false;
	double u;
	double candt;

	for (int i = 0; i < root_num; ++i)
	{
		candt = buffer[i];
		
		if (candt < t0 || candt >= t1)
		{
			continue;
		}

		Point3F hit_point = (ray.at(candt) - center_) / size_;

		double a = y1 - 2 * y2 + y3;
		double b = 2 * (y2 - y1);
		double c = y1 - hit_point.y();

		if (fabs(a) < 1e-3)
		{
			u = - c / b;
			if (0 <= u && u <= 1)
			{
				is_hit = true;
				break;
			}
		}
		else
		{
			double delta = b * b - 4 * a * c;

			if (delta < 0.0)
			{
				continue;
			}

			double u1 = (-b - sqrt(delta)) / 2 * a;
			double u2 = (-b + sqrt(delta)) / 2 * a;


			if (0 <= u1 && u1 <= 1)
			{
				u = u1;
				is_hit = true;
				break;
			}
			else if (0 <= u2 && u2 <= 1)
			{
				u = u2;
				is_hit = true;
				break;
			}
			else
			{
				continue;
			}
		}
	}
	
	if (!is_hit)
	{
		return false;
	}

	t = candt;
	return true;
}


Point3F RotationBessel2::hit_normal(const Ray & ray, float t) const
{
	Point3F hit_point = (ray.at(t) - center_) / size_;

	double a = c1_.y() - 2 * c2_.y() + c3_.y();
	double b = 2 * (c2_.y() - c1_.y());
	double c = c1_.y() - hit_point.y();

	double u;
	if (fabs(a) < 1e-4)
	{
		u = - c / b;
	}
	else
	{
		double delta = b * b - 4 * a * c;
		assert(delta >= 0.0);
		double u1 = (-b - sqrt(delta)) / 2 * a;
		double u2 = (-b + sqrt(delta)) / 2 * a;

		if (0 <= u1 && u1 <= 1)
		{
			u = u1;
		}
		else
		{
			assert(0 <= u2 && u2 <= 1);
			u = u2;
		}
	}

	double dxu = (c1_.x() - 2 * c2_.x() + c3_.x()) * u + (c2_.x() - c1_.x());
	double dyu = (c1_.y() - 2 * c2_.y() + c3_.y()) * u + (c2_.y() - c1_.y());
	
	if (dyu > 0.0)
	{
		dyu = -dyu;
		dxu = -dxu;
	}

	QPointF n(-dyu, dxu);

	QPointF st(hit_point.x(), hit_point.z());
	double nrmst = sqrt(st.x() * st.x() + st.y() * st.y());
	st /= nrmst;
	st *= dyu;

	return Point3F(st.x(), -dxu, st.y()).normalize();
}

