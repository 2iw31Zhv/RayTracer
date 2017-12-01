#pragma once

class Mesh;
class OctreeNode;

class Octree
{
private:
	OctreeNode * root_;

	void remove(OctreeNode * p_node);
public:
	Octree(const Mesh& mesh);
	~Octree();
};
