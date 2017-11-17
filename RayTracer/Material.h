#pragma once
#include <memory>
#include <vector>

#include <QColor>
#include "Point3.h"

#include "FreeImage\FreeImage.h"

typedef std::vector<std::vector<FIRGB16>> ImageMatrix;

class Material
{
public:
	bool is_normal_texture;
	QColor lambert_color;
	QColor phong_color;

	float dielectric;
	Point3F alpha;

	float phong_ratio;
	
	std::shared_ptr<ImageMatrix> texture;

	Material(const QColor& l = Qt::white, const QColor& p = Qt::black) :
		lambert_color(l),
		phong_color(p),
		dielectric(1.0f),
		alpha(0.0f),
		phong_ratio(0.0f),
		is_normal_texture(false){}

	bool is_dielectric() const;
	bool has_texture() const;
	void set_normal_texture();
	void set_texture_from_file(std::string filename);
};