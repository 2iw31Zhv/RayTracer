#include <fstream>
#include <iomanip>
#include <utility>
#include <algorithm>

#include "Mesh.h"
#include "Geometry.h"

Mesh::Mesh() :
	bounding_box_(new BoundingBox)
{

}

Mesh::Mesh(const Mesh& mesh)
{
	for (const auto& v : mesh.vertices_)
	{
		vertices_.insert(v);
	}

	for (const auto& q : mesh.quadfacets_)
	{
		quadfacets_.insert(q);
	}

	for (const auto& t : mesh.trifacets_)
	{
		trifacets_.insert(t);
	}

	bounding_box_.reset(new BoundingBox(mesh.bounding_box()));
}

void Mesh::reset()
{
	vertices_.clear();
	quadfacets_.clear();
	trifacets_.clear();
}

void Mesh::insert(const Vertice& p)
{
	size_t vid = vertices_.size() + 1;
	vertices_.insert(std::make_pair(vid, p));
	bounding_box_->add(p);
}

void Mesh::insert(const QuadFacet& f)
{
	size_t fid = quadfacets_.size() + 1;
	quadfacets_.insert(std::make_pair(fid, f));
}

void Mesh::insert(const TriFacet& f)
{
	size_t fid = trifacets_.size() + 1;
	trifacets_.insert(std::make_pair(fid, f));
}

const Vertice& Mesh::vertice_at(size_t vid) const
{
	return vertices_.at(vid);
}

const QuadFacet& Mesh::quadfacet_at(size_t fid) const
{
	return quadfacets_.at(fid);
}

const TriFacet& Mesh::trifacet_at(size_t fid) const
{
	return trifacets_.at(fid);
}

void Mesh::save(const char * filename) const
{
	std::ofstream fout(filename, std::ios::out);
	fout << std::setprecision(4);
	fout << std::fixed;

	fout << "# " << filename << std::endl;
	fout << "# " << "number of vertices: " << vertices_.size() << std::endl;
	fout << "# " << "number of quadfacets: " << quadfacets_.size() << std::endl;
	fout << "# " << "number of trifacets: " << trifacets_.size() << std::endl;
	for (const auto& v : vertices_)
	{
		const Vertice& vert = v.second;
		fout << "v ";
		fout << vert.x() << " ";
		fout << vert.y() << " ";
		fout << vert.z() << "\n";
	}

	for (const auto& f : quadfacets_)
	{
		const QuadFacet& qf = f.second;
		fout << "f ";
		fout << std::get<0>(qf) << " ";
		fout << std::get<1>(qf) << " ";
		fout << std::get<2>(qf) << " ";
		fout << std::get<3>(qf) << "\n";
	}

	for (const auto& f : trifacets_)
	{
		const TriFacet& tf = f.second;
		fout << "f ";
		fout << std::get<0>(tf) << " ";
		fout << std::get<1>(tf) << " ";
		fout << std::get<2>(tf) << "\n";
	}
	fout.close();
}