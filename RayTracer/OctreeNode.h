#pragma once
#include <memory>
#include <vector>
#include "Geometry.h"

class BoundingBox;
class Polygonal;

class OctreeNode
{
private:
	std::shared_ptr<BoundingBox> bounding_box_;
	std::vector<Polygonal *> facets_;

	Vertice ref_bmin_;
	Vertice ref_bmax_;
public:
	OctreeNode * parent;

	OctreeNode * c000;
	OctreeNode * c001;
	OctreeNode * c010;
	OctreeNode * c011;
	OctreeNode * c100;
	OctreeNode * c101;
	OctreeNode * c110;
	OctreeNode * c111;
public:

	OctreeNode(const Vertice& ref_bmin,
		const Vertice& ref_bmax,
		OctreeNode * parent = nullptr,
		OctreeNode * c000 = nullptr,
		OctreeNode * c001 = nullptr,
		OctreeNode * c010 = nullptr,
		OctreeNode * c011 = nullptr,
		OctreeNode * c100 = nullptr,
		OctreeNode * c101 = nullptr,
		OctreeNode * c110 = nullptr,
		OctreeNode * c111 = nullptr);

	~OctreeNode();

	void add_face(const Polygonal& poly);

	const std::vector<Polygonal *> & get_faces() { return facets_; }
	bool soft_has(const Polygonal& poly);
};