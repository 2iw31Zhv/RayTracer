#include "Octree.h"
#include "OctreeNode.h"
#include "Mesh.h"
#include "Polygonal.h"
#include "Cuboid.h"
#include <utility>

#define MAX_DEPTH 20

using namespace std;
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

void Octree::construct(
	OctreeNode * p_parent,
	OctreeNode * p_node,
	const Vertice& ref_bmin,
	const Vertice& ref_bmax,
	int depth)
{
	p_node = new OctreeNode(ref_bmin, ref_bmax, p_parent);
	
	if (p_parent->get_faces().size() < 8)
	{
		return;
	}

	for (const auto& poly : p_parent->get_faces())
	{
		if (p_node->soft_has(*(poly.first)))
		{
			p_node->add_face(*(poly.first), poly.second);
		}
	}

	if (depth > MAX_DEPTH)
	{
		return;
	}

	Point3F x(ref_bmax.x() - ref_bmin.x(), 0.0, 0.0);
	Point3F y(0.0, ref_bmax.y() - ref_bmin.y(), 0.0);
	Point3F z(0.0, 0.0, ref_bmax.z() - ref_bmin.z());

	auto PROCESS = [&](int i, int j, int k, OctreeNode * node)
	{
		Vertice b0 = ref_bmin + i * 0.5 * x + j * 0.5 * y + k * 0.5 * z;
		Vertice b1 = b0 + 0.5 * x + 0.5 * y + 0.5 * z;
		construct(p_node, node, b0, b1, depth + 1);
	};

	PROCESS(0, 0, 0, p_node->c000);
	PROCESS(0, 0, 1, p_node->c001);
	PROCESS(0, 1, 0, p_node->c010);
	PROCESS(0, 1, 1, p_node->c011);
	PROCESS(1, 0, 0, p_node->c100);
	PROCESS(1, 0, 1, p_node->c101);
	PROCESS(1, 1, 0, p_node->c110);
	PROCESS(1, 1, 1, p_node->c111);
}

Octree::Octree(const Mesh& mesh)
{
	const BoundingBox& bbox = mesh.bounding_box();
	Vertice bmin(bbox.min_x(), bbox.min_y(), bbox.min_z());
	Vertice bmax(bbox.max_x(), bbox.max_y(), bbox.max_z());
	
	root_ = new OctreeNode(bmin, bmax, nullptr);

	for (int i = 1; i <= mesh.quadfacets_num(); ++i)
	{
		const QuadFacet& f = mesh.quadfacet_at(i);

		vector<Vertice> pvec;
		pvec.push_back(mesh.vertice_at(get<0>(f)));
		pvec.push_back(mesh.vertice_at(get<1>(f)));
		pvec.push_back(mesh.vertice_at(get<2>(f)));
		pvec.push_back(mesh.vertice_at(get<3>(f)));

		root_->add_face(Polygonal(pvec), i);
	}

	for (int i = 1; i <= mesh.trifacets_num(); ++i)
	{
		const TriFacet& f = mesh.trifacet_at(i);
		vector<Vertice> pvec;
		pvec.push_back(mesh.vertice_at(get<0>(f)));
		pvec.push_back(mesh.vertice_at(get<1>(f)));
		pvec.push_back(mesh.vertice_at(get<2>(f)));

		root_->add_face(Polygonal(pvec), i);
	}

	Point3F x(bmax.x() - bmin.x(), 0.0, 0.0);
	Point3F y(0.0, bmax.y() - bmin.y(), 0.0);
	Point3F z(0.0, 0.0, bmax.z() - bmin.z());

	auto PROCESS = [&](int i, int j, int k, OctreeNode * node)
	{
		Vertice b0 = bmin + i * 0.5 * x + j * 0.5 * y + k * 0.5 * z;
		Vertice b1 = b0 + 0.5 * x + 0.5 * y + 0.5 * z;
		construct(root_, node, b0, b1, 1);
	};

	PROCESS(0, 0, 0, root_->c000);
	PROCESS(0, 0, 1, root_->c001);
	PROCESS(0, 1, 0, root_->c010);
	PROCESS(0, 1, 1, root_->c011);
	PROCESS(1, 0, 0, root_->c100);
	PROCESS(1, 0, 1, root_->c101);
	PROCESS(1, 1, 0, root_->c110);
	PROCESS(1, 1, 1, root_->c111);
}

Octree::~Octree()
{
	remove(root_);
}

bool Octree::hit(const Ray & ray, float t0, float t1, float & t, int & hit_face_id)
{
	return hit(root_, ray, t0, t1, t, hit_face_id);
}

bool Octree::hit(const OctreeNode * p_node, const Ray & ray, float t0, float t1, float & t, int& hit_face_id)
{
	Point3F bmin(p_node->bounding_box()->min_x(),
		p_node->bounding_box()->min_y(),
		p_node->bounding_box()->min_z());
	Point3F bmax(p_node->bounding_box()->max_x(),
		p_node->bounding_box()->max_y(),
		p_node->bounding_box()->max_z());

	Point3F diag = bmax - bmin;
	Cuboid box(bmin, diag.x(), diag.y(), diag.z());
	if (!box.hit(ray, t0, t1, t))
	{
		return false;
	}
	using TimeId = pair<float, int>;

	if (p_node->c000 == nullptr ||
		p_node->c001 == nullptr ||
		p_node->c010 == nullptr ||
		p_node->c011 == nullptr ||
		p_node->c100 == nullptr ||
		p_node->c101 == nullptr ||
		p_node->c110 == nullptr ||
		p_node->c111 == nullptr) // leaf
	{
		const vector < pair<Polygonal*, int> >& faces = p_node->get_faces();

		float temp_t;

		vector< pair<float, int> > timesId;

		for (const auto& pPoly : faces)
		{
			if (pPoly.first->hit(ray, t0, t1, temp_t))
			{
				timesId.push_back(make_pair(temp_t, pPoly.second));
			}
		}

		if (timesId.size() == 0)
		{
			return false;
		}

		sort(timesId.begin(), timesId.end(), [](const TimeId& t1, const TimeId& t2)
		{
			return t1.first < t2.first;
		});

		t = timesId[0].first;
		hit_face_id = timesId[0].second;
		return true;
	}
	else
	{
		float temp_t;
		int temp_id;
		vector<TimeId> timesId;

		auto PROCESS_NODE = [&](const OctreeNode * p_cnode)
		{
			if (hit(p_cnode, ray, t0, t1, temp_t, temp_id))
			{
				timesId.push_back(make_pair(temp_t, temp_id));
			}
		};

		PROCESS_NODE(p_node->c000);
		PROCESS_NODE(p_node->c001);
		PROCESS_NODE(p_node->c010);
		PROCESS_NODE(p_node->c011);
		PROCESS_NODE(p_node->c100);
		PROCESS_NODE(p_node->c101);
		PROCESS_NODE(p_node->c110);
		PROCESS_NODE(p_node->c111);

		if (timesId.size() == 0)
		{
			return false;
		}
		
		sort(timesId.begin(), timesId.end(), [](const TimeId& t1, const TimeId& t2)
		{
			return t1.first < t2.first;
		});

		t = timesId[0].first;
		hit_face_id = timesId[0].second;
		return true;
	}
}


