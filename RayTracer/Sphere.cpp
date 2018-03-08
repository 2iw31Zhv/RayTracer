#include <cmath>

#include "Sphere.h"
#include "Material.h"
#include "tools.h"

Sphere::Sphere(const Vertice& center, double radius) :
	center_(center),
	radius_(radius)
{
	bounding_box_->add(Vertice(center.x() - radius,
		center.y() - radius,
		center.z() - radius));
	bounding_box_->add(Vertice(center.x() + radius,
		center.y() + radius,
		center.z() + radius));
}

bool Sphere::hit(const Ray& ray, double t0, double t1, double& t)
{
	const Point3F& d = ray.d();
	const Point3F& e = ray.e();

	double A = d.dot(d);
	double B = d.dot(e - center_);
	double C = (e - center_).dot(e - center_) - radius_ * radius_;

	double delta = B * B - A * C;
	if (delta < 0)
	{
		return false;
	}

	if (fabs(delta) < 1e-4)
	{
		t = - B / A;
		return false;
	}
	else
	{
		float ta = (-B - std::sqrt(delta)) / A;
		float tb = (-B + std::sqrt(delta)) / A;
		if (ta >= t0 && ta < t1)
		{
			t = ta;
		}
		else if (tb >= t0 && tb < t1)
		{
			t = tb;
		}
		else
		{
			return false;
		}
	}

	return true;
}

Point3F Sphere::hit_normal(const Ray & ray, double t) const
{
	return (ray.at(t) - center()).normalize();
}

Point3F Sphere::change_normal(const Ray & ray, double t) const
{
	Point3F old_normal = hit_normal(ray, t);

	if (!material_->has_texture() || !(material_->is_normal_texture))
	{
		return hit_normal(ray, t);
	}

	double u = asin(old_normal.y());
	
	double v;

	if (fabs(old_normal.x()) < 1e-4f)
	{
		v = (old_normal.z() > 0.0f) ? 0.5 * PI: 1.5 * PI;
	}
	else if (old_normal.x() > 0.0f)
	{
		double temp = atan(old_normal.z() / old_normal.x());
		if (temp < 0)
		{
			v = temp + 2 * PI;
		}
		else
		{
			v = temp;
		}
	}
	else if (old_normal.x() < 0.0f)
	{
		double temp = atan(old_normal.z() / old_normal.x());
		if (temp < 0)
		{
			v = temp + PI;
		}
		else
		{
			v = temp + PI;
		}
	}
	// compute tangent B T
	Point3F T(-sin(u) * cos(v), cos(u), -sin(u) * sin(v));
	Point3F B = old_normal.cross(T).normalize();

	u = (u + 0.5 * PI) / PI;
	v /= 2 * PI;

	// compute x_gradient and y_gradient
	ImageMatrix& image = *(material_->texture);
	int height = image.size();
	int width = image[0].size();

	int hit_i = int(height * v);
	int hit_j = int(width * u);
	
	double x_gradient = (hit_j == 0 || hit_j == width - 1) ? 0.0:
		(pow2((image[hit_i][hit_j - 1].red - image[hit_i][hit_j + 1].red) / 255.0)
		+ pow2((image[hit_i][hit_j - 1].green - image[hit_i][hit_j + 1].green) / 255.0)
		+ pow2((image[hit_i][hit_j - 1].blue - image[hit_i][hit_j + 1].blue) / 255.0));

	double y_gradient = (hit_i == 0 || hit_i == height - 1) ? 0.0 :
		(pow2((image[hit_i - 1][hit_j].red - image[hit_i + 1][hit_j].red) / 255.0)
			+ pow2((image[hit_i - 1][hit_j].green - image[hit_i + 1][hit_j].green) / 255.0)
			+ pow2((image[hit_i - 1][hit_j].blue - image[hit_i + 1][hit_j].blue) / 255.0));

	
	Point3F new_normal = old_normal + x_gradient * B + y_gradient * T;

	// change normal
	return new_normal.normalize();
}

