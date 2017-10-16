#pragma once

#include "Surface.h"
#include "Geometry.h"

class Sphere :
	public Surface
{
private:
	Vertice center_;
	float radius_;

public:
	Sphere(const Vertice& center, float radius);

	virtual bool hit(const Ray& ray, float t0, float t1, float& rec);
	virtual Point3F hit_normal(const Ray& ray, float t) const;
	Vertice center() const { return center_; }
	float radius() const { return radius_; }

	virtual Point3F change_normal(const Ray& ray, float t) const;
};