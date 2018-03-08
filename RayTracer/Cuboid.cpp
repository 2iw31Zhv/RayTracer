#include <algorithm>
#include <cmath>

#include "Cuboid.h"
#include "tools.h"


Cuboid::Cuboid(const Vertice& p, double length, double width, double height) :
	length_(length), width_(width), height_(height)
{
	auto PROCESSV = [&] (int i, int j, int k) {
		vertices_.push_back(Vertice(p.x() + i * length, 
			p.y() + j * height, 
			p.z() + k * width)); };

	PROCESSV(0, 0, 0);
	PROCESSV(1, 0, 0);
	PROCESSV(1, 0, 1);
	PROCESSV(0, 0, 1);
	PROCESSV(0, 1, 0);
	PROCESSV(1, 1, 0);
	PROCESSV(1, 1, 1);
	PROCESSV(0, 1, 1);

	for (const auto& v : vertices_)
	{
		bounding_box_->add(v);
	}
}

bool Cuboid::hit(const Ray& ray, double t0, double t1, double& t)
{
	using std::min;
	using std::max;

	auto PROCESST = [&](float & tmin, float & tmax,
		float rayd, float raye,
		float bmin, float bmax)
	{
		if (std::fabs(rayd) > 1e-4)
		{
			tmin = min((bmin - raye) / rayd, (bmax - raye) / rayd);
			tmax = max((bmin - raye) / rayd, (bmax - raye) / rayd);
		}
		else
		{
			if (raye <= bmin || raye >= bmax)
			{
				tmin = FLT_MAX;
				tmax = FLT_MAX;
			}
			else
			{
				tmin = -FLT_MAX;
				tmax = FLT_MAX;
			}
		}
	};
	
	float tmin[3], tmax[3];
	PROCESST(tmin[0], tmax[0], 
		ray.d().dot(facet_normal(RIGHT)), 
		ray.e().dot(facet_normal(RIGHT)),
		vertices_[0].dot(facet_normal(RIGHT)),
		vertices_[1].dot(facet_normal(RIGHT)));

	PROCESST(tmin[1], tmax[1],
		ray.d().dot(facet_normal(UP)),
		ray.e().dot(facet_normal(UP)),
		vertices_[0].dot(facet_normal(UP)),
		vertices_[4].dot(facet_normal(UP)));

	PROCESST(tmin[2], tmax[2],
		ray.d().dot(facet_normal(BACK)),
		ray.e().dot(facet_normal(BACK)),
		vertices_[0].dot(facet_normal(BACK)),
		vertices_[3].dot(facet_normal(BACK)));
	
	//decide the intersection of the three interval.
	float t_min_intersect = 
		*std::max_element(tmin, tmin + sizeof(tmin) / sizeof(float));
	float t_max_intersect =
		*std::min_element(tmax, tmax + sizeof(tmax) / sizeof(float));

	
	if (t_min_intersect >= t_max_intersect)
	{
		return false;
	}
	else
	{
		if (t_min_intersect >= t0 && t_min_intersect < t1)
		{
			t = t_min_intersect;
		}
		else if (t_max_intersect >= t0 && t_max_intersect < t1)
		{
			t = t_max_intersect;
		}
		else
		{
			return false;
		}
	}

	return true;
}

//left hand coordinate
Point3F Cuboid::facet_normal(Direction dir) const 
{ 
	Point3F result;
	switch (dir)
	{
	case LEFT:
		result = ((vertices_[4] - vertices_[0]).cross(vertices_[7] - vertices_[4])).normalize().reverse();
		break;
	case RIGHT:
		result = ((vertices_[4] - vertices_[0]).cross(vertices_[7] - vertices_[4])).normalize();
		break;
	case UP:
		result = ((vertices_[1] - vertices_[0]).cross(vertices_[2] - vertices_[1])).normalize().reverse();
		break;
	case DOWN:
		result = ((vertices_[1] - vertices_[0]).cross(vertices_[2] - vertices_[1])).normalize();
		break;
	case FRONT:
		result = ((vertices_[1] - vertices_[0]).cross(vertices_[5] - vertices_[1])).normalize().reverse();
		break;
	case BACK:
		result = ((vertices_[1] - vertices_[0]).cross(vertices_[5] - vertices_[1])).normalize();
		break;
	default:
		break;
	}
	return result;
}

Vertice Cuboid::center() const
{
	return 0.5f * (vertices_[0] + vertices_[6]);
}



Point3F Cuboid::hit_normal(const Ray& ray, double t) const
{
	Point3F flow = Point3F(
		(ray.at(t) - center()).dot(facet_normal(RIGHT)),
		(ray.at(t) - center()).dot(facet_normal(UP)),
		(ray.at(t) - center()).dot(facet_normal(BACK)));

	float flown[3] = { std::fabs(flow.x() / length_),
	std::fabs(flow.y() / height_),
	std::fabs(flow.z() / width_) };

	float * maxflown = 
		std::max_element(flown, flown + sizeof(flown) / sizeof(float));
	int maxpos = maxflown - flown;

	Point3F result;
	switch (maxpos)
	{
	case 0:
		result = flow.x() > 0 ? facet_normal(RIGHT): facet_normal(LEFT);
		break;
	case 1:
		result = flow.y() > 0 ? facet_normal(UP) : facet_normal(DOWN);
		break;
	case 2:
		result = flow.z() > 0 ? facet_normal(BACK) : facet_normal(FRONT);
		break;
	default:
		break;
	}

	return result;
}

void Cuboid::givens3(double theta1, double theta2, double theta3)
{
	for (auto& v : vertices_)
	{
		givens(v.x(), v.y(), theta1);
		givens(v.y(), v.z(), theta2);
		givens(v.z(), v.x(), theta3);
	}
}
