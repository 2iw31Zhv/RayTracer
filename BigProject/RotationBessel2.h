#pragma once
#include <QPoint>

#include "Surface.h"
#include "Geometry.h"

class RotationBessel2 :
	public Surface
{
private:
	QPointF c1_;
	QPointF c2_;
	QPointF c3_;

	Vertice center_;
	float size_;
public:
	RotationBessel2(const QPointF& c1, const QPointF& c2, const QPointF& c3,
		const Vertice& center, float size);

	virtual bool hit(const Ray& ray, float t0, float t1, float& t);
	virtual Point3F hit_normal(const Ray& ray, float t) const;

};