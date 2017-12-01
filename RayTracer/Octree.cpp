#include "Octree.h"
#include "OctreeNode.h"

void Octree::remove(OctreeNode * p_node)
{
	if (p_node == nullptr)
	{
		return;
	}

	remove(p_node->c000);
	remove(p_node->c001);
	remove(p_node->c010);
	remove(p_node->c011);
	remove(p_node->c100);
	remove(p_node->c101);
	remove(p_node->c110);
	remove(p_node->c111);

	delete p_node;
}

Octree::Octree(const Mesh& mesh)
{
	// TODO

}

Octree::~Octree()
{
	remove(root_);
}
