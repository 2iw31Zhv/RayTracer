#pragma once
#include <memory>
#include <vector>

class BoundingBox;
class Polygonal;

class OctreeNode
{
private:
	std::shared_ptr<BoundingBox> bounding_box_;
	std::vector<Polygonal *> facets_;
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

	OctreeNode(OctreeNode * parent = nullptr,
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


};