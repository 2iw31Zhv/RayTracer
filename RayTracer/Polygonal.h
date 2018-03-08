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

	virtual bool hit(const Ray& ray, double t0, double t1, double& t);
	virtual Point3F hit_normal(const Ray& ray, double t) const;

	Point3F normal() const;
	const std::vector<Vertice>& get_vertices() const { return vertices_; }
	virtual QColor read_texture(const Ray& ray, double t) const;
private:
	bool is_coplanar_() const;
	bool is_convex_() const;
	bool is_rectangle() const;
};