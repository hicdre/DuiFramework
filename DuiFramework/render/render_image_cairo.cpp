#include "stdafx.h"
#include "render_image_cairo.h"

namespace ui
{


	RenderImageCairo::RenderImageCairo(void* data, size_t len)
	{
		int x, y, n;
		stbi_buffer_ = stbi_load_from_memory((const stbi_uc*)data, len, &x, &y, &n, 4);

		cairo_format_t format = CAIRO_FORMAT_ARGB32;
		int stride = cairo_format_stride_for_width(format, y);
		image_surface_ = cairo_image_surface_create_for_data(stbi_buffer_, format, x, y, n);
	}

	RenderImageCairo::~RenderImageCairo()
	{
		cairo_surface_destroy(image_surface_);
		stbi_image_free(stbi_buffer_);
	}

}

