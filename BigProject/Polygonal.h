#pragma once

#include <vector>

#include "Surface.h"
#include "Geometry.h"

class Polygonal :
	public Surface
{
private:
	std::vector<Vertice> vertices_;
	Point3F normal_;

public:
	Polygonal(const std::vector<Vertice>& vertices);

	virtual bool hit(const Ray& ray, float t0, float t1, float& t);
	virtual Point3F hit_normal(const Ray& ray, float t) const;

	Point3F normal() const;
	virtual QColor read_texture(const Ray& ray, float t) const;
private:
	bool is_coplanar_() const;
	bool is_convex_() const;
	bool is_rectangle() const;
};