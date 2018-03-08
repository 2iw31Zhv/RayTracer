#pragma once
#include "Geometry.h"
class Mesh;
class OctreeNode;
class Vertice;

class Octree
{
private:
	OctreeNode * root_;

	void remove(OctreeNode * p_node);

	void construct(
		OctreeNode * p_parent, OctreeNode * p_node,
		const Vertice& ref_bmin, const Vertice& ref_bmax,
		int depth);

public:

	Octree(const Mesh& mesh);
	~Octree();

	bool hit(const Ray & ray, double t0, double t1, double & t, int& hit_face_id);
	bool hit(const OctreeNode * p_node, const Ray & ray, double t0, double t1, double & t, int& hit_face_id);
};
