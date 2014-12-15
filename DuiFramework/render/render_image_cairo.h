#pragma  once
#include "render/render_image.h"
#include "render/render_cairo_forward.h"
#include "third_party/stb_image.h"

namespace ui
{
	class RenderImageCairo : public RenderImage
	{
	public:
		RenderImageCairo(void* data, size_t len);
		virtual ~RenderImageCairo() override;

		virtual const Size& GetSize() const override;

		cairo_surface_t* GetSurface() const { return image_surface_; }


	private:
		cairo_surface_t *image_surface_;
		stbi_uc* stbi_buffer_;
		Size size_;
	};
}