#include "MeshSurface.h"
#include "Polygonal.h"
#include "Cuboid.h"
#include "Octree.h"
#include <iostream>

using namespace std;
using TimeId = pair<float, int>;

bool MeshSurface::hit(const Ray & ray, float t0, float t1, float & t)
{
	//vector<TimeId> times_;
	//// bounding box test
	//Point3F bmin(mesh_.bounding_box().min_x(),
	//	mesh_.bounding_box().min_y(),
	//	mesh_.bounding_box().min_z());
	//Point3F bmax(mesh_.bounding_box().max_x(),
	//	mesh_.bounding_box().max_y(),
	//	mesh_.bounding_box().max_z());
	//Point3F diag = bmax - bmin;
	//Cuboid box(bmin, diag.x(), diag.y(), diag.z());
	//if (!box.hit(ray, t0, t1, t))
	//{
	//	return false;
	//}

	//for (int i = 1; i <= mesh_.trifacets_num(); ++i)
	//{
	//	const TriFacet& f = mesh_.trifacet_at(i);
	//	const Vertice& v1 = mesh_.vertice_at(std::get<0>(f));
	//	const Vertice& v2 = mesh_.vertice_at(std::get<1>(f));
	//	const Vertice& v3 = mesh_.vertice_at(std::get<2>(f));

	//	vector<Vertice> pvec;
	//	pvec.push_back(v1);
	//	pvec.push_back(v2);
	//	pvec.push_back(v3);

	//	Polygonal poly(pvec);

	//	float temp_t;
	//	if (poly.hit(ray, t0, t1, temp_t))
	//	{
	//		times_.push_back(make_pair(temp_t, i));
	//	}
	//}

	//if (times_.size() == 0)
	//{
	//	return false;
	//}

	//sort(times_.begin(), times_.end(), [](const TimeId& t1, const TimeId& t2)
	//{
	//	return t1.first < t2.first;
	//	; });

	//t = times_[0].first;
	//
	//hit_face_id_ = times_[0].second;

	//return true;
	return octree_.hit(ray, t0, t1, t, hit_face_id_);
}

Point3F MeshSurface::hit_normal(const Ray & ray, float t) const
{
	int fid = hit_face_id_;
	const TriFacet& f = mesh_.trifacet_at(fid);
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
