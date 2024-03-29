#pragma once
#include <memory>
#include <tuple>
#include <map>

typedef std::tuple<int, int, int> TriFacet;
typedef	std::tuple<int, int, int, int> QuadFacet;

class Vertice;
class BoundingBox;

class Mesh
{
private:
	std::map<size_t, Vertice> vertices_;
	std::map<size_t, Vertice> normals_;
	std::map<size_t, QuadFacet> quadfacets_;
	std::map<size_t, TriFacet> trifacets_;

	std::shared_ptr<BoundingBox> bounding_box_;

	bool isNormalOk;
public:
	Mesh();
	Mesh(const Mesh& mesh);

	void reset();
	void insert(const Vertice& p);
	void insert_normal(const Vertice& n);
	void insert(const QuadFacet& f);
	void insert(const TriFacet& f);
	void set_normal_ok(bool ok) { isNormalOk = ok; }
	bool is_normal_ok() const { return isNormalOk; }
	//all IDs start from 1, not zero
	const Vertice& vertice_at(size_t vid) const;
	const QuadFacet& quadfacet_at(size_t fid) const ;
	const TriFacet& trifacet_at(size_t fid) const;
	const Vertice& normal_at(size_t nid) const;
	const BoundingBox& bounding_box() const { return *bounding_box_; }

	size_t vertices_num() const { return vertices_.size(); }
	size_t normals_num() const { return normals_.size(); }
	size_t trifacets_num() const { return trifacets_.size(); }
	size_t quadfacets_num() const { return quadfacets_.size(); }
	void save(const char* filename) const;
};
