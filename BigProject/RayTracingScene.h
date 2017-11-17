#pragma once
#include <vector>

#include <QWidget>

#include "Geometry.h"

class Surface;

class RayTracingScene :
	public QWidget
{
	Q_OBJECT
public:
	RayTracingScene();
	~RayTracingScene();

	void precompute();
	void set_scene();
protected:
	virtual void paintEvent(QPaintEvent * e);

private:
	std::vector<Surface *> surface_vec_;
	QColor background_color_;
	QColor environment_color_;
	Point3F light_color_;

	bool is_finish_trace_;

	QColor * screenbuffer_;

	Ray cast_ray(const Point3F& source, int i, int j);
	bool check_hit(const Ray& ray, float& t, Surface *& hit_object);

	void ray_tracer(const Ray& ray, 
		Point3F weight, 
		int depth, 
		QColor& color, 
		const Point3F& light_source,
		int i,
		int j);
	
	void evaluate_depth(const Ray& ray,
		double& depth);

	bool isInShadow(const Point3F& light_source,
		const Point3F& hit_point,
		int i,
		int j);
};