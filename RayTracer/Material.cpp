#include "Material.h"
#include <cmath>

#include "easylogging++\easylogging++.h"

bool Material::is_dielectric() const
{
	return fabs(dielectric - 1.0) > 0.001f;
}

bool Material::has_texture() const
{
	return (texture != nullptr);
}

void Material::set_normal_texture()
{
	if (has_texture())
	{
		is_normal_texture = true;
	}
}

void Material::set_texture_from_file(std::string filename)
{
	const char * file = filename.data();
	FreeImage_GetFileType(file);

	FIBITMAP * bitmap;
	bitmap = FreeImage_Load(FreeImage_GetFileType(file), file);
	if (bitmap == nullptr)
	{
		exit(1);
	}

	size_t width = FreeImage_GetWidth(bitmap);
	size_t height = FreeImage_GetHeight(bitmap);
	

	size_t pixelbyte = FreeImage_GetLine(bitmap) / width;

	ImageMatrix * imag_ptr = new ImageMatrix;
	texture.reset(imag_ptr);

	imag_ptr->resize(height);

	for (int i = 0; i < height; ++i)
	{
		imag_ptr->at(i).resize(width);
	}

	for (size_t i = 0; i < height; ++i)
	{
		BYTE *bitsLine = FreeImage_GetScanLine(bitmap, i);
		for (size_t j = 0; j < width; ++j)
		{
			imag_ptr->at(i).at(j).red = bitsLine[FI_RGBA_RED];
			imag_ptr->at(i).at(j).green = bitsLine[FI_RGBA_GREEN];
			imag_ptr->at(i).at(j).blue = bitsLine[FI_RGBA_BLUE];
			bitsLine += pixelbyte;
		}
	}

	
}
