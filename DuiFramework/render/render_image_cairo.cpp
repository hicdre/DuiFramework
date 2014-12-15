#include "stdafx.h"
#include "render_image_cairo.h"

namespace ui
{


	RenderImageCairo::RenderImageCairo(void* data, size_t len)
	{
		int x, y, n;
		stbi_buffer_ = stbi_load_from_memory((const stbi_uc*)data, len, &x, &y, &n, 4);
		Color* pColorBegin = (Color*)stbi_buffer_;
		for (int i = 0; i < x*y; i++)
		{
			Color* pColor = pColorBegin + i;
			Color origin = *pColor;
			uint8 a = ColorGetA(origin);
			uint8 r = ColorGetB(origin);
			uint8 g = ColorGetG(origin);
			uint8 b = ColorGetR(origin);
			if (a < 255) {
				*pColor = ColorSetARGB(a, r*a/255, g*a/255, b*a/255);
			}
			else {
				*pColor = ColorSetARGB(a, r, g, b);
			}
		}

		size_.SetSize(x, y);

		cairo_format_t format = CAIRO_FORMAT_ARGB32;
		int stride = cairo_format_stride_for_width(format, x);
		image_surface_ = cairo_image_surface_create_for_data(stbi_buffer_, format, x, y, stride);
	}

	RenderImageCairo::~RenderImageCairo()
	{
		cairo_surface_destroy(image_surface_);
		stbi_image_free(stbi_buffer_);
	}

	const Size& RenderImageCairo::GetSize() const
	{
		return size_;
	}

}

