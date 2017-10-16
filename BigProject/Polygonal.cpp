#include <cassert>
#include <cmath>

#include <QPoint>
#include "Material.h"
#include "Polygonal.h"

#include "easylogging++\easylogging++.h"

Polygonal::Polygonal(const std::vector<Vertice>& vertices)
{
	
	for (const auto& v : vertices)
	{
		vertices_.push_back(v);
		bounding_box_->add(v);
	}

	assert(vertices_.size() >= 3);

	normal_ = ((vertices_[2] - vertices_[1]).cross(
		vertices_[1] - vertices_[0])).normalize();

	assert(is_coplanar_());
	assert(is_convex_());
}

bool Polygonal::hit(const Ray & ray, float t0, float t1, float & t)
{
	float dn = ray.d().dot(normal());
	if (fabs(dn) < 1e-3f)
	{
		return false;
	}

	//hit with plane
	float tempt = (vertices_[0] - ray.e()).dot(normal()) / dn;
	if (tempt < t0 || tempt >= t1)
	{
		return false;
	}

	Vertice crossPoint = ray.at(tempt);
	
	// choose the maximum direction of normal
	// set the plane _|_ max normal_ direction
	// project all points on the plane
	std::vector<Point3F> project_vertices;
	Point3F project_crs;

	//LOG(DEBUG) << fabs(normal_.x()) << " " << fabs(normal_.y())<< " " << fabs(normal_.z());
	if (fabs(normal_.z()) > fabs(normal_.x())
		&& fabs(normal_.z()) > fabs(normal_.y()))
	{
		for (const auto& v : vertices_)
		{
			project_vertices.push_back(Point3F(v.x(), v.y()));
		}
		project_crs.x() = (crossPoint.x());
		project_crs.y() = (crossPoint.y());
	}
	else if (fabs(normal_.x()) > fabs(normal_.y()))
	{
		for (const auto& v : vertices_)
		{
			project_vertices.push_back(Point3F(v.y(), v.z()));
		}
		project_crs.x() = (crossPoint.y());
		project_crs.y() = (crossPoint.z());
	}
	else
	{
		for (const auto& v : vertices_)
		{
			project_vertices.push_back(Point3F(v.z(), v.x()));
		}
		project_crs.x() = (crossPoint.z());
		project_crs.y() = (crossPoint.x());
	}

	using std::min;
	using std::max;

	// check the 2D ray cross all boundaries
	int count = 0;
	int n = project_vertices.size();

	auto HIT_PROJ = [&](int i, int j)
	{
		float p1x = project_vertices[i].x();
		float p1y = project_vertices[i].y();
		float p2x = project_vertices[j].x();
		float p2y = project_vertices[j].y();
		float px = project_crs.x();
		float py = project_crs.y();

		float proj_dnmy = p1y - p2y;
		if (fabs(proj_dnmy) < 1e-3f) return;

		float proj_t = ((p1x - p2x) * (py - p1y) 
			- (px - p1x) * (p1y - p2y)) / proj_dnmy;
		if (proj_t > 0.0f)
		{
			float proj_crx = px + proj_t;
			if ((proj_crx - p1x) * (proj_crx - p2x)
				+ (py - p1y) * (py - p2y) < 0.0f)
			{
				count = (count + 1) % 2;
			}
		}
	};

	for (int i = 0; i < n - 1; ++i)
	{
		HIT_PROJ(i, i + 1);
	}

	HIT_PROJ(n - 1, 0);
	if (count == 0)
	{
		return false;
	}
	
	t = tempt;
	return true;
}

Point3F Polygonal::hit_normal(const Ray & ray, float t) const
{
	return normal();
}

Point3F Polygonal::normal() const
{
	return normal_;
}

QColor Polygonal::read_texture(const Ray & ray, float t) const
{
	if (!is_rectangle() || !material_->has_texture())
	{
		return material_->lambert_color;
	}

	Point3F base1 = (vertices_[1] - vertices_[0]).normalize();
	Point3F base2 = (vertices_[3] - vertices_[0]).normalize();

	Point3F hit_point = ray.at(t) - vertices_[0];

	double u = hit_point.dot(base1) / (vertices_[1] - vertices_[0]).norm2();
	double v = hit_point.dot(base2) / (vertices_[3] - vertices_[0]).norm2();

	
	ImageMatrix& image = *(material_->texture);
	int height = image.size();
	int width = image[0].size();

	int hit_i = int(height * v);
	int hit_j = int(width * u);

	FIRGB16 color = image[hit_i][hit_j];
	LOG(DEBUG) << color.red << " " << color.green << " " << color.blue;
	return QColor(color.red, color.green, color.blue);
}

bool Polygonal::is_coplanar_() const
{
	for (int i = 1; i < vertices_.size(); ++i)
	{
		if (fabs((vertices_[i] - vertices_[0])
			.normalize().dot(normal_)) > 1e-3f)
		{
			return false;
		}
	}

	return true;
}

bool Polygonal::is_convex_() const
{
	int n = vertices_.size();

	auto CHECKFALSE = [&](int i, int j, int k)
	{
		Point3F crs = ((vertices_[k] - vertices_[j])
			.cross(vertices_[j] - vertices_[i]));
		return (crs.dot(normal_) < 0.0f);
	};

	for (int i = 0; i < n - 2; ++i)
	{
		if (CHECKFALSE(i, i + 1, i + 2))
			return false;
	}

	return !(CHECKFALSE(n-2, n-1, 0) || CHECKFALSE(n-1, 0, 1));
}

bool Polygonal::is_rectangle() const
{
	if (vertices_.size() != 4)
	{
		return false;
	}
	
	auto CHECKRECT = [&](int i, int j, int k)
	{
		return fabs((vertices_[j] - vertices_[i]).dot(vertices_[k] - vertices_[j])) < 1e-4;
	};

	return CHECKRECT(0, 1, 2)
		&& CHECKRECT(1, 2, 3)
		&& CHECKRECT(2, 3, 0)
		&& CHECKRECT(3, 0, 1);
}
