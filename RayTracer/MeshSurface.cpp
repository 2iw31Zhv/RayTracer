#include "MeshSurface.h"
#include "Polygonal.h"
#include "Cuboid.h"
#include "Octree.h"
#include <iostream>

using namespace std;
using TimeId = pair<double, int>;

bool MeshSurface::hit(const Ray & ray, double t0, double t1, double & t)
{
	return octree_.hit(ray, t0, t1, t, hit_face_id_);
}

Point3F MeshSurface::hit_normal(const Ray & ray, double t) const
{
	int fid = hit_face_id_;
	const TriFacet& f = mesh_.trifacet_at(fid);
	Point3F hit_point = ray.at(t);
	
	if (mesh_.is_normal_ok())
	{
		const Point3F& n1 = mesh_.normal_at(std::get<0>(f));
		const Point3F& n2 = mesh_.normal_at(std::get<1>(f));
		const Point3F& n3 = mesh_.normal_at(std::get<2>(f));

		const Vertice& v1 = mesh_.vertice_at(std::get<0>(f));
		const Vertice& v2 = mesh_.vertice_at(std::get<1>(f));
		const Vertice& v3 = mesh_.vertice_at(std::get<2>(f));

		double distance_1 = pow((hit_point - v1).norm2(), 2.0);
		double distance_2 = pow((hit_point - v2).norm2(), 2.0);
		double distance_3 = pow((hit_point - v3).norm2(), 2.0);

		Vertice n = (
			distance_2 * distance_3 * n1 +
			distance_1 * distance_3 * n2 +
			distance_1 * distance_2 * n3)
			/ (distance_2 * distance_3 +
				distance_1 * distance_3 +
				distance_1 * distance_2);
		n.normalize();

		return Point3F(n.x(), n.y(), n.z());
	}
	else
	{
		const Vertice& v1 = mesh_.vertice_at(std::get<0>(f));
		const Vertice& v2 = mesh_.vertice_at(std::get<1>(f));
		const Vertice& v3 = mesh_.vertice_at(std::get<2>(f));

		vector<Vertice> pvec;
		pvec.push_back(v1);
		pvec.push_back(v2);
		pvec.push_back(v3);

		Polygonal poly(pvec);
		return poly.normal();
	}
}
