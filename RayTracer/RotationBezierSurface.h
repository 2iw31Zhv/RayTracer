#pragma once
#include "Surface.h"
#include "Bezier.h"
#include "Geometry.h"

class RotationBezierSurface :
	public Surface
{
private:
	BezierCurve bezier_curve_;
	Vertice center_;
	float size_;

	double u_, v_;
public:
	RotationBezierSurface(const BezierCurve& bezier_curve,
		const Vertice& center,
		float size);

	virtual bool hit(const Ray& ray, float t0, float t1, float& t);
	virtual Point3F hit_normal(const Ray& ray, float t) const;

private:
	QPointF pu_(double u) const;
	QPointF dpu_(double u) const;
};