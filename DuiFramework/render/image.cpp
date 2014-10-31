#include "stdafx.h"
#include "image.h"

#include "render/render_context.h"
#include "utils/image_store.h"
#include "utils/image_file.h"

namespace ui
{
	class ResourceImage : public Image
	{
	public:
		ResourceImage(const std::string& id)
			: image_id_(id)
		{
			LoadImage();
		}

		void SetImageId(const std::string& id)
		{
			if (image_id_ == id)
				return;
			LoadImage();
		}

		std::string image_id() const
		{
			return image_id_;
		}

		virtual void DoPaint(RenderContext* painter, const Rect& dest) override
		{
			if (!image_)
				return;
			painter->DrawImage(image_, dest);
		}

	private:
		void LoadImage()
		{
			image_ = ImageStore::Default()->GetImageById(image_id_);
		}
		ImagePart* image_{ NULL };
		std::string image_id_;
	};

	class LocalPathImage : public Image
	{
	public:
		LocalPathImage(const std::wstring& path)
			: image_path_(path)
		{
			LoadImage();
		}
		~LocalPathImage()
		{
			if (image_)
			{
				delete image_;
				image_ = NULL;
			}
		}

		std::wstring GetImagePath() const
		{
			return image_path_;
		}
		virtual void DoPaint(RenderContext* painter, const Rect& dest) override
		{
			if (!image_)
				return;
			painter->DrawImage(image_, dest);
		}
		
	private:
		void LoadImage()
		{
			image_ = ImageStore::LoadImageByPath(image_path_);
		}
		ImagePart* image_{ NULL };
		std::wstring image_path_;
	};


	class LinearGradientImage : public Image
	{
	public:
		LinearGradientImage(const Point& pos_from, const Point& pos_to,
			Color color_from, Color color_to)
			: pfrom_(pos_from), pto_(pos_to), cfrom_(color_from), cto_(color_to)
		{

		}

		virtual void DoPaint(RenderContext* painter, const Rect& dest) override
		{
			
			TRIVERTEX vertex[2];
			vertex[0].x = dest.x() + dest.width() * pfrom_.x() / 100;
			vertex[0].y = dest.y() + dest.height() * pfrom_.y() / 100;
			vertex[0].Red   = ColorGetR(cfrom_);
			vertex[0].Green = ColorGetG(cfrom_);
			vertex[0].Blue  = ColorGetB(cfrom_);
			vertex[0].Alpha = 0;

			vertex[1].x = dest.x() + dest.width() * pto_.x() / 100;
			vertex[1].y = dest.y() + dest.height() * pto_.y() / 100;
			vertex[1].Red = ColorGetR(cto_);
			vertex[1].Green = ColorGetG(cto_);
			vertex[1].Blue = ColorGetB(cto_);
			vertex[1].Alpha = 0;

			// Create a GRADIENT_RECT structure that 
			// references the TRIVERTEX vertices. 
			GRADIENT_RECT gRect;
			gRect.UpperLeft = 0;
			gRect.LowerRight = 1;

			// Draw a shaded rectangle. 
			BOOL suc = GradientFill(painter->GetHDC(), vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
			int i;
			i = 0;
		}

	private:
		Point pfrom_;
		Point pto_;
		Color cfrom_;
		Color cto_;
	};

	Image* Image::CreateFromResourceId(const std::string& id)
	{
		return new ResourceImage(id);
	}

	Image* Image::CreateFromLocalPath(const std::wstring& path)
	{
		return new LocalPathImage(path);
	}

	Image* Image::CreateLinearGradient(const Point& pos_from, const Point& pos_to, Color color_from, Color color_to)
	{
		return new LinearGradientImage(pos_from, pos_to, color_from, color_to);
	}

}