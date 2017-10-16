#pragma once
#include "Surface.h"
#include "Bessel.h"
#include "Geometry.h"

class RotationBesselSurface :
	public Surface
{
private:
	BesselCurve bessel_curve_;
	Vertice center_;
	float size_;

	double u_, v_;
public:
	RotationBesselSurface(const BesselCurve& bessel_curve,
		const Vertice& center,
		float size);

	virtual bool hit(const Ray& ray, float t0, float t1, float& t);
	virtual Point3F hit_normal(const Ray& ray, float t) const;

private:
	QPointF pu_(double u) const;
	QPointF dpu_(double u) const;
};