#include "Surface.h"

#include "Geometry.h"
#include "Material.h"

Surface::Surface() :
	material_(new Material),
	bounding_box_(new BoundingBox)
{

}

QColor Surface::read_texture(const Ray& ray, double t) const
{
	return material_->lambert_color;
}

Point3F Surface::change_normal(const Ray & ray, double t)const 
{
	return hit_normal(ray, t);
}
