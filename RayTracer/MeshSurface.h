#pragma once

#include "Surface.h"
#include "Mesh.h"
#include "Octree.h"

class MeshSurface :
	public Surface
{
private:
	Mesh mesh_;

	//using TimeId = std::pair<float, int>;
	//std::vector<TimeId> times_;
	int hit_face_id_;

	Octree octree_;
public:
	MeshSurface(const Mesh& mesh) :
		mesh_(mesh),
		octree_(mesh) {}

	virtual bool hit(const Ray& ray, double t0, double t1, double& t);
	virtual Point3F hit_normal(const Ray& ray, double t) const;
};