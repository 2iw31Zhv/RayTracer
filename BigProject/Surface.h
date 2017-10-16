#pragma once
#include <memory>
#include <vector>

#include <QColor>

#include "Point3.h"
#include "Geometry.h"

class Material;

class Surface
{
protected:
	std::shared_ptr<Material> material_;
	std::shared_ptr<BoundingBox> bounding_box_;

public:
	Surface();
	virtual ~Surface() {}
	virtual bool hit(const Ray& ray, float t0, float t1, float& t) = 0;
	virtual Point3F hit_normal(const Ray& ray, float t) const = 0;
	virtual const BoundingBox& bounding_box() const { return *bounding_box_; }

	Material& get_material() { return *material_; }

	virtual QColor read_texture(const Ray& ray, float t) const;
	virtual Point3F change_normal(const Ray& ray, float t) const;
};