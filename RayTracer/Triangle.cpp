#include "Triangle.h"
#include <cassert>

Triangle::Triangle(const Vertice & v1, const Vertice & v2, const Vertice & v3):
	v1_(v1), v2_(v2), v3_(v3)
{
	bounding_box_->add(v1);
	bounding_box_->add(v2);
	bounding_box_->add(v3);

	//check bad triangle
	Point3F b1 = (v2_ - v1_).normalize();
	Point3F b2 = (v3_ - v1_).normalize();
	assert(fabs(b1.dot(b2) - 1.0f) >= 1e-3f);
}

bool Triangle::hit(const Ray & ray, float t0, float t1, float & t)
{
	float dn = ray.d().dot(normal());
	if (fabs(dn) < 1e-3f)
	{
		return false;
	}

	//hit with plane
	float tempt = (v1_ - ray.e()).dot(normal()) / dn;
	if (tempt < t0 || tempt >= t1)
	{
		return false;
	}

	//check the cross point
	Point3F crossPoint = ray.at(tempt) - v1_;
	Point3F base1 = v2_ - v1_;
	Point3F base2 = v3_ - v1_;

	float denominator = base2.x() * base1.y() - base1.x() * base2.y();
	assert(fabs(denominator) > 1e-3f);

	float alpha = (crossPoint.y() * base2.x()
		- crossPoint.x() * base2.y()) / denominator;
	float beta = (crossPoint.x() * base1.y()
		- crossPoint.y() * base1.x()) / denominator;

	if (alpha <= 0.0f || alpha >= 1.0f)
	{
		return false;
	}

	if (beta <= 0.0f || beta >= 1.0f)
	{
		return false;
	}

	if (alpha + beta >= 1.0f)
	{
		return false;
	}

	t = tempt;

	return true;
}

Point3F Triangle::normal() const
{
	return ((v2_ - v1_).cross(v3_ - v1_)).normalize().reverse();
}
Point3F Triangle::hit_normal(const Ray & ray, float t) const
{
	return normal();
}
