#pragma once
#include <QPoint>

#include "Surface.h"
#include "Geometry.h"

class RotationBezier2 :
	public Surface
{
private:
	QPointF c1_;
	QPointF c2_;
	QPointF c3_;

	Vertice center_;
	double size_;
public:
	RotationBezier2(const QPointF& c1, const QPointF& c2, const QPointF& c3,
		const Vertice& center, double size);

	virtual bool hit(const Ray& ray, double t0, double t1, double& t);
	virtual Point3F hit_normal(const Ray& ray, double t) const;

};