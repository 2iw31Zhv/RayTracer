#pragma once
#include <cmath>
#include <map>
#include <vector>

#include <QColor>

#include "Point3.h"

class Vertice :
	public Point3F
{
public:
	Vertice(float x = 0.0f, float y = 0.0f, float z = 0.0f) :
		Point3F(x, y, z) {}
	Vertice(const Point3F& p) : Point3F(p) {}
};

class Ray
{
private:
	Point3F e_;
	Point3F d_;
public:
	Ray(const Point3F& eye, const Point3F& screen) :
		e_(eye), d_((screen - eye).normalize()) {}

	const Point3F& e() const { return e_; }
	Point3F d() const { return d_; }
	Point3F at(float t)const { return e_ + t * d(); }
};

class BoundingBox
{
private:
	std::pair<Vertice, Vertice> data_;
public:
	BoundingBox(const Vertice& vmin = Vertice(), const Vertice& vmax = Vertice()) :
		data_(std::make_pair(vmin, vmax)) {}
	BoundingBox& add(const Vertice& v);
	BoundingBox& add(const BoundingBox& b);
	const float& min_x() const { return data_.first.x(); }
	const float& max_x() const { return data_.second.x(); }
	const float& min_y() const { return data_.first.y(); }
	const float& max_y() const { return data_.second.y(); }
	const float& min_z() const { return data_.first.z(); }
	const float& max_z() const { return data_.second.z(); }
};

enum Direction
{
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	FRONT = 4,
	BACK = 5
};