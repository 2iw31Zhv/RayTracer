#pragma once

#include "Surface.h"
#include "Mesh.h"

class MeshSurface :
	public Surface
{
private:
	Mesh mesh_;

	using TimeId = std::pair<float, int>;
	std::vector<TimeId> times_;
public:
	MeshSurface(const Mesh& mesh) :
		mesh_(mesh) {}
	virtual bool hit(const Ray& ray, float t0, float t1, float& t);
	virtual Point3F hit_normal(const Ray& ray, float t) const;
};