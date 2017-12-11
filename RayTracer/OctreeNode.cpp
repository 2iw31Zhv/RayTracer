#include "OctreeNode.h"
#include "Polygonal.h"
#include "Geometry.h"

using namespace std;

OctreeNode::OctreeNode(
	const Vertice& ref_bmin,
	const Vertice& ref_bmax,
	OctreeNode * parent,
	OctreeNode * c000,
	OctreeNode * c001,
	OctreeNode * c010,
	OctreeNode * c011,
	OctreeNode * c100,
	OctreeNode * c101,
	OctreeNode * c110,
	OctreeNode * c111) :
	ref_bmin_(ref_bmin),
	ref_bmax_(ref_bmax),
	parent(parent),
	c000(c000),
	c001(c001),
	c010(c010),
	c011(c011),
	c100(c100),
	c101(c101),
	c110(c110),
	c111(c111),
	bounding_box_(new BoundingBox)
{

}

OctreeNode::~OctreeNode()
{
	for (auto & pf : facets_)
	{
		delete pf;
	}
}

void OctreeNode::add_face(const Polygonal & poly)
{
	facets_.push_back(new Polygonal(poly));
	bounding_box_->add(poly.bounding_box());
}

bool OctreeNode::soft_has(const Polygonal & poly)
{
	bool is_not_have = true;

	const vector<Vertice> & vertices = poly.get_vertices();

	for (const auto& v : vertices)
	{
		if (v.x() >= ref_bmin_.x() &&
			v.y() >= ref_bmin_.y() &&
			v.z() >= ref_bmin_.z() &&
			v.x() <= ref_bmax_.x() &&
			v.y() <= ref_bmax_.y() &&
			v.z() <= ref_bmax_.z())
		{
			is_not_have = false;
		}
	}

	return !is_not_have;
}