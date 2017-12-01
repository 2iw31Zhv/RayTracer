#include "OctreeNode.h"
#include "Polygonal.h"
#include "Geometry.h"

OctreeNode::OctreeNode(OctreeNode * parent,
	OctreeNode * c000,
	OctreeNode * c001,
	OctreeNode * c010,
	OctreeNode * c011,
	OctreeNode * c100,
	OctreeNode * c101,
	OctreeNode * c110,
	OctreeNode * c111) :
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
