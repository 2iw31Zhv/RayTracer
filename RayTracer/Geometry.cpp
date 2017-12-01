#include <utility>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include "Geometry.h"

using namespace std;

BoundingBox& BoundingBox::add(const Vertice& v)
{

	pair<Vertice, Vertice> p(
		make_pair(
		Vertice(min(data_.first.x(), v.x()),
			min(data_.first.y(), v.y()),
			min(data_.first.z(), v.z())),
		Vertice(max(data_.second.x(), v.x()),
			max(data_.second.y(), v.y()),
			max(data_.second.z(), v.z()))
		));
	data_.swap(p);

	return *this;
}

BoundingBox & BoundingBox::add(const BoundingBox & b)
{
	add(Vertice(b.min_x(), b.min_y(), b.min_z()));
	add(Vertice(b.max_x(), b.max_y(), b.max_z()));

	return *this;
}
