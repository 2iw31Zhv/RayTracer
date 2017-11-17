#pragma once

#include "Surface.h"
#include "Geometry.h"

class Triangle:
	public Surface
{
private:
	Vertice v1_;
	Vertice v2_;
	Vertice v3_;

public:

	Triangle(const Vertice& v1, const Vertice& v2, const Vertice& v3);
	virtual bool hit(const Ray& ray, float t0, float t1, float& t);
	Point3F normal() const;
	virtual Point3F hit_normal(const Ray& ray, float t) const;
};