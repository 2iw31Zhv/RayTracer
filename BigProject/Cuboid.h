#pragma once
#include <vector>

#include "Geometry.h"
#include "Surface.h"

class Cuboid:
	public Surface
{
private:
	std::vector<Vertice> vertices_;
	float length_;
	float width_;
	float height_;

public:
	Cuboid(const Vertice& p, float length, float width, float height);

	virtual bool hit(const Ray& ray, float t0, float t1, float& t);
	virtual Point3F hit_normal(const Ray& ray, float t) const;

	void givens3(float theta1, float theta2, float theta3);
	Point3F facet_normal(Direction dir) const;
	Vertice center() const;
};