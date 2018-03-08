#pragma once
#include <vector>

#include "Geometry.h"
#include "Surface.h"

class Cuboid:
	public Surface
{
private:
	std::vector<Vertice> vertices_;
	double length_;
	double width_;
	double height_;

public:
	Cuboid(const Vertice& p, double length, double width, double height);

	virtual bool hit(const Ray& ray, double t0, double t1, double& t);
	virtual Point3F hit_normal(const Ray& ray, double t) const;

	void givens3(double theta1, double theta2, double theta3);
	Point3F facet_normal(Direction dir) const;
	Vertice center() const;
};