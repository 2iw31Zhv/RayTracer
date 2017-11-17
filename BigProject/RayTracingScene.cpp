#include <cassert>

#include <QPainter>

#include "RayTracingScene.h"
#include "Cuboid.h"
#include "Material.h"
#include "Polygonal.h"
#include "RotationBezier2.h"
#include "Surface.h"
#include "Sphere.h"
#include "Triangle.h"
#include "tools.h"

#include "Bezier.h"
#include "Mesh.h"

#include "easylogging++\easylogging++.h"
#include <fstream>
#include <iostream>

RayTracingScene::RayTracingScene():
	background_color_(QColor(20, 20, 20)),
	environment_color_(QColor(50, 50, 50)),
	light_color_(Point3F(0.7, 0.7, 0.7)),
	is_finish_trace_(false)
{
	
}

RayTracingScene::~RayTracingScene()
{
	for (auto& s : surface_vec_)
	{
		delete s;
	}

	delete[] screenbuffer_;
}

void RayTracingScene::ray_tracer(const Ray & ray, 
	Point3F weight, 
	int depth, 
	QColor & color, 
	const Point3F& light_source,
	int i,
	int j)
{
	color = QColor(0, 0, 0);

	if (weight.normi() < 0.001)
	{
		return;
	}

	color = weight * background_color_;

	float time;
	Surface * hit_object = nullptr;
	if (!check_hit(ray, time, hit_object))
	{
		return;
	}

	using std::max;

	Material& hit_material = hit_object->get_material();
	
	Point3F normal = hit_object->hit_normal(ray, time);
	Point3F hit_point = ray.at(time);

	if (hit_material.is_normal_texture)
	{
		normal = hit_object->change_normal(ray, time);
	}

	Ray light_ray{ light_source, ray.at(time) };
	Point3F l = light_ray.d().reverse();
	Point3F d = ray.d();
	Point3F h = (l - d).normalize();
	
	if (!isInShadow(light_source, hit_point, i, j))
	{
		if (!hit_material.is_dielectric())
		{
			QColor lambert;

			// texture
			if (hit_material.has_texture() && !(hit_material.is_normal_texture))
			{
				lambert = hit_object->read_texture(ray, time);
				color = weight * lambert;
			}
			else
			{
				lambert = hit_material.lambert_color;
				color = weight * (environment_color_
					+ light_color_ *
					// two facets lighting
					(lambert * fabs(normal.dot(l))));
			}
		}

		//phong
		color = color + weight * hit_material.phong_color
			* pow(h.dot(normal), hit_material.phong_ratio);
	}

	if (depth > 1)
	{
		if (hit_material.is_dielectric())
		{
			float nt = hit_material.dielectric;
			
			// handle reflect
			QColor reflectColor;
			Ray reflectRay(ray.at(time - 0.008f), 
				ray.at(time - 0.008f) + ray.d() - 2 * (d.dot(normal)) * normal);
			float c = -d.dot(normal);
			
			Point3F tvec;
			Point3F k(1.0f, 1.0f, 1.0f);

			bool isRefract = false;
			

			// outer refract
			if (d.dot(normal) < 0)
			{
				float delta = 1.0f
					- (1.0f - pow(d.dot(normal), 2)) / (nt * nt);

				if (delta > 0.0f)
				{
					tvec = ((d - (d.dot(normal)) * normal)/nt
						- normal * sqrt(delta)).normalize();
					assert(tvec.dot(normal) < 0);
					isRefract = true;
				}
			}
			// inner refract
			else
			{
				const Point3F & alpha = hit_material.alpha;

				//Beer's Law
				k.x() = exp(-alpha.x() * time);
				k.y() = exp(-alpha.y() * time);
				k.z() = exp(-alpha.z() * time);

				float delta = 1.0f
					- nt * nt * (1.0f - pow(d.dot(normal), 2));

				if (delta > 0.0f)
				{
					tvec = (nt * (d - (d.dot(normal)) * normal)
						+ normal * sqrt(delta)).normalize();
					c = tvec.dot(normal);
					assert(c > 0);
					isRefract = true;
				}
			}

			if (!isRefract)
			{
				ray_tracer(reflectRay,
					weight,
					depth - 1,
					reflectColor,
					light_source,
					i,
					j);
				color = color + k * reflectColor;
			}
			else
			{
				float R0 = (nt - 1.0f) * (nt - 1.0f)
					/ ((nt + 1.0f) * (nt + 1.0f));

				float R = R0 + (1.0f - R0) * pow(1.0f - c, 5.0f);

				Ray refractRay(ray.at(time + 0.008f),
					ray.at(time + 0.008f) + tvec);

				QColor refractColor;

				ray_tracer(reflectRay,
					weight * R,
					depth - 1,
					reflectColor,
					light_source,
					i,
					j);

				
				ray_tracer(refractRay,
					weight * (1 - R),
					depth - 1,
					refractColor,
					light_source,
					i,
					j);

				color = color + k * (reflectColor + refractColor);
			}
		}
	}
}

void RayTracingScene::evaluate_depth(const Ray & ray, double & depth)
{
	float time;
	Surface * hit_object = nullptr;
	depth = FLT_MAX;
	if (!check_hit(ray, time, hit_object))
	{
		return;
	}

	Point3F hit_point = ray.at(time);
	Point3F ray_source = ray.e();

	depth = sqrt(pow2(hit_point.x() - ray_source.x())
		+ pow2(hit_point.y() - ray_source.y())
		+ pow2(hit_point.z() - ray_source.z()));

}

bool RayTracingScene::isInShadow(const Point3F& light_source,
	const Point3F & hit_point, 
	int i, int j)
{
	double hit_distance = sqrt(pow2(light_source.x() - hit_point.x())
		+ pow2(light_source.y() - hit_point.y())
		+ pow2(light_source.z() - hit_point.z()));

	double depth;
	evaluate_depth(Ray(light_source, hit_point),
		depth);
	return hit_distance > depth + 0.008;
}

void RayTracingScene::precompute()
{
	Point3F light_source(0.0f, height() * 0.495f, height() * 0.2f);
	set_scene();

	screenbuffer_ = new QColor[width() * height()];

	Point3F eye_source(0.0f, 0.0f, -height() * 0.5f);
	for (int j = 0; j < height(); ++j)
	{
		for (int i = 0; i < width(); ++i)
		{
			ray_tracer(cast_ray(eye_source, i, j), 
				Point3F(1.0f, 1.0f, 1.0f), 
				20, 
				screenbuffer_[j * width() + i],
				light_source,
				i, j);
		}
	}
	is_finish_trace_ = true;
}

void RayTracingScene::set_scene()
{
	Sphere * sph1 = new Sphere(Vertice(width() * 0.1f, 0.0f, height() * 0.2f), 50);
	sph1->get_material().phong_color = Qt::white;
	sph1->get_material().phong_ratio = 256;
	sph1->get_material().dielectric = 3.5;
	sph1->get_material().alpha = Point3F(0.005f, 0.001f, 0.001f);

	surface_vec_.push_back(sph1);
/*	Cuboid * cub1 = new Cuboid(Vertice(0.0f - width() * 0.5f, 0.0f - height() * 0.5f, 0.0f - height() * 1.0f), width(), height() * 2.0f, height());
	cub1->get_material().lambert_color = Qt::lightGray;
	cub1->get_material().dielectric = 1.0*/;

	//surface_vec_.push_back(cub1);

	std::vector<Vertice> pv1;
	pv1.push_back(Vertice(0.0f - width() * 0.5f, 0.0f - height() * 0.5f, 0.0f - height() * 1.0f));
	pv1.push_back(Vertice(0.0f + width() * 0.5f, 0.0f - height() * 0.5f, 0.0f - height() * 1.0f));
	pv1.push_back(Vertice(0.0f + width() * 0.5f, 0.0f - height() * 0.5f, 0.0f + height() * 1.0f));
	pv1.push_back(Vertice(0.0f - width() * 0.5f, 0.0f - height() * 0.5f, 0.0f + height() * 1.0f));
	Polygonal * ply1 = new Polygonal(pv1);
	ply1->get_material().lambert_color = Qt::red;
	//ply1->get_material().set_texture_from_file("texture/texture2.png");
	surface_vec_.push_back(ply1);

	std::vector<Vertice> pv2;
	pv2.push_back(Vertice(0.0f - width() * 0.5f, 0.0f - height() * 0.5f, 0.0f + height() * 1.0f));
	pv2.push_back(Vertice(0.0f + width() * 0.5f, 0.0f - height() * 0.5f, 0.0f + height() * 1.0f));
	pv2.push_back(Vertice(0.0f + width() * 0.5f, 0.0f + height() * 0.5f, 0.0f + height() * 1.0f));
	pv2.push_back(Vertice(0.0f - width() * 0.5f, 0.0f + height() * 0.5f, 0.0f + height() * 1.0f));
	Polygonal * ply2 = new Polygonal(pv2);
	ply2->get_material().dielectric = 20.0f;
	surface_vec_.push_back(ply2);

	std::vector<Vertice> pv3;
	pv3.push_back(Vertice(0.0f + width() * 0.5f, 0.0f - height() * 0.5f, 0.0f + height() * 1.0f));
	pv3.push_back(Vertice(0.0f + width() * 0.5f, 0.0f - height() * 0.5f, 0.0f - height() * 1.0f));
	pv3.push_back(Vertice(0.0f + width() * 0.5f, 0.0f + height() * 0.5f, 0.0f - height() * 1.0f));
	pv3.push_back(Vertice(0.0f + width() * 0.5f, 0.0f + height() * 0.5f, 0.0f + height() * 1.0f));
	Polygonal * ply3 = new Polygonal(pv3);
	ply3->get_material().lambert_color = Qt::blue;
	//ply3->get_material().dielectric = 20.0f;
	surface_vec_.push_back(ply3);

	std::vector<Vertice> pv4;
	pv4.push_back(Vertice(0.0f - width() * 0.5f, 0.0f - height() * 0.5f, 0.0f + height() * 1.0f));
	pv4.push_back(Vertice(0.0f - width() * 0.5f, 0.0f - height() * 0.5f, 0.0f - height() * 1.0f));
	pv4.push_back(Vertice(0.0f - width() * 0.5f, 0.0f + height() * 0.5f, 0.0f - height() * 1.0f));
	pv4.push_back(Vertice(0.0f - width() * 0.5f, 0.0f + height() * 0.5f, 0.0f + height() * 1.0f));
	Polygonal * ply4 = new Polygonal(pv4);
	ply4->get_material().lambert_color = Qt::green;
	//ply4->get_material().dielectric = 20.0f;
	surface_vec_.push_back(ply4);

	std::vector<Vertice> pv5;
	pv5.push_back(Vertice(0.0f - width() * 0.5f, 0.0f + height() * 0.5f, 0.0f - height() * 1.0f));
	pv5.push_back(Vertice(0.0f + width() * 0.5f, 0.0f + height() * 0.5f, 0.0f - height() * 1.0f));
	pv5.push_back(Vertice(0.0f + width() * 0.5f, 0.0f + height() * 0.5f, 0.0f + height() * 1.0f));
	pv5.push_back(Vertice(0.0f - width() * 0.5f, 0.0f + height() * 0.5f, 0.0f + height() * 1.0f));
	Polygonal * ply5 = new Polygonal(pv5);
	ply5->get_material().lambert_color = Qt::darkYellow;
	//ply5->get_material().dielectric = 20.0f;
	surface_vec_.push_back(ply5);

	std::vector<Vertice> pv6;
	pv6.push_back(Vertice(0.0f - width() * 0.5f, 0.0f - height() * 0.5f, -height() * 1.0f));
	pv6.push_back(Vertice(0.0f + width() * 0.5f, 0.0f - height() * 0.5f, -height() * 1.0f));
	pv6.push_back(Vertice(0.0f + width() * 0.5f, 0.0f + height() * 0.5f, -height() * 1.0f));
	pv6.push_back(Vertice(0.0f - width() * 0.5f, 0.0f + height() * 0.5f, -height() * 1.0f));
	Polygonal * ply6 = new Polygonal(pv6);
	ply6->get_material().set_texture_from_file("texture/texture2.png");
	surface_vec_.push_back(ply6);


	Sphere * sph2 = new Sphere(Vertice(-200, -70, height() * 0.3f), 30);
	sph2->get_material().lambert_color = Qt::red;
	sph2->get_material().phong_color = Qt::yellow;
	sph2->get_material().phong_ratio = 16;

	surface_vec_.push_back(sph2);

	//Sphere * sph3 = new Sphere(Vertice(200, -70, height() * 0.001f), 60);
	//sph3->get_material().lambert_color = Qt::blue;
	//sph3->get_material().set_texture_from_file("texture/normaltexture3.jpg");
	//sph3->get_material().set_normal_texture();
	//sph3->get_material().phong_color = Qt::green;
	//sph3->get_material().phong_ratio = 4.0;
	//surface_vec_.push_back(sph3);

	//Cuboid * cub2 = new Cuboid(Vertice(0.0f, -height() * 0.5f, height() * 0.5f), 100, 100, height() * 0.7f);
	//cub2->get_material().lambert_color = Qt::green;
	//cub2->givens3(0.0, 0.0, PI / 6);
	////surface_vec_.push_back(cub2);

	//std::vector<Vertice> plvec;
	//plvec.push_back(Vertice(-200, -270.0, height() * 0.40f));
	//plvec.push_back(Vertice(-200 - height() * 0.10f, -270.0, height() * 0.30f));
	//plvec.push_back(Vertice(-200, -270.0, height() * 0.20f));
	//plvec.push_back(Vertice(-200 + height() * 0.10f, -270.0, height() * 0.30f));
	//
	//Polygonal * poy = new Polygonal(plvec);
	//poy->get_material().lambert_color = Qt::red;
	//surface_vec_.push_back(poy);

	//BezierCurve bc;
	//bc.push_ctrl_points(QPointF(0.0, height() * 0.4));
	//bc.push_ctrl_points(QPointF(100.0, 0.0));
	//bc.push_ctrl_points(QPointF(0.0, -height() * -0.4));

	RotationBezier2 * rbs = new RotationBezier2(
		QPointF(0.0, -height() * 0.4),
		QPointF(200.0, -height() * 0.3),
		QPointF(0.0, height() * 0.4),
		Vertice(-200, -70, height() * 0.3f),
		0.7);
	rbs->get_material().lambert_color = Qt::yellow;
	//rbs->get_material().dielectric = 1.05f;
	rbs->get_material().alpha = Point3F(1e-5, 0.001, 0.001);
	rbs->get_material().phong_color = QColor(100, 100, 100);
	rbs->get_material().phong_ratio = 32;
	surface_vec_.push_back(rbs);

}

void RayTracingScene::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);
	painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0));

	if (is_finish_trace_)
	{
		for (int j = 0; j < height(); ++j)
		{
			for (int i = 0; i < width(); ++i)
			{
				painter.setPen(screenbuffer_[j * width() + i]);
				painter.drawRect(i, j, 1, 1);
			}
		}
	}
	painter.end();
}

Ray RayTracingScene::cast_ray(const Point3F& source, int i, int j)
{
	return Ray(source,
		Point3F(i - width() * 0.5f, height() * 0.5f - j, 0.0f));
}

bool RayTracingScene::check_hit(const Ray& ray, float& t, Surface *& hit_object)
{
	bool is_hit = false;
	float t0 = 0;
	float t1 = FLT_MAX;

	for (int i = 0; i < surface_vec_.size(); ++i)
	{
		float tempt;
		if (surface_vec_[i]->hit(ray, t0, t1, tempt))
		{
			t = tempt;
			is_hit = true;
			hit_object = surface_vec_[i];
			t1 = t;
		}
	}

	return is_hit;
}