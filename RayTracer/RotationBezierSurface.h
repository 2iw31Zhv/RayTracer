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
	double size_;

	double u_, v_;
public:
	RotationBezierSurface(const BezierCurve& bezier_curve,
		const Vertice& center,
		double size);

	virtual bool hit(const Ray& ray, double t0, double t1, double& t);
	virtual Point3F hit_normal(const Ray& ray, double t) const;

private:
	QPointF pu_(double u) const;
	QPointF dpu_(double u) const;
};