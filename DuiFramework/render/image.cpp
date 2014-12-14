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
			float x0 = dest.width() * pfrom_.x() / 100.0;
			float y0 = dest.height() * pfrom_.y() / 100.0;
			float x1 = dest.width() * pto_.x() / 100.0;
			float y1 = dest.height() * pto_.y() / 100.0;
			float dx = x1 - x0;
			float dy = y1 - y0;
			float angle = atan2(dy, dx);
			float length = dx * cos(angle) + dy * sin(angle);
			Matrix m;
			m = m.Translate(dest.x(), dest.y()).Translate(x0, y0).Rotate(angle);

			if (length == 0)
			{
				painter->FillRect(dest, cfrom_);
				return;
			}

			//进行4点变换
			Matrix mi = m.Invert();
			Point lt(dest.x(), dest.y());
			Point lb(dest.x(), dest.bottom());
			Point rt(dest.right(), dest.y());
			Point rb(dest.right(), dest.bottom());
			mi.TransformPoint(lt);
			mi.TransformPoint(lb);
			mi.TransformPoint(rt);
			mi.TransformPoint(rb);
			int minx = (std::min)({ lt.x(), lb.x(), rt.x(), rb.x() });
			int maxx = (std::max)({ lt.x(), lb.x(), rt.x(), rb.x() });
			int miny = (std::min)({ lt.y(), lb.y(), rt.y(), rb.y() });
			int maxy = (std::max)({ lt.y(), lb.y(), rt.y(), rb.y() });

			ScopedPainter sp(painter, m);

			int vx0 = 0;
			int vx1 = length;
			Color cf = cfrom_;
			Color ct = cto_;
			if (length < 0) {
				vx0 = length;
				vx1 = 0;
				cf = cto_;
				ct = cfrom_;
			}

			TRIVERTEX vertex[6];
			SetVertexPos(&vertex[0], minx, miny);
			SetVertexColor(&vertex[0], cf);

			SetVertexPos(&vertex[1], vx0, maxy);
			SetVertexColor(&vertex[1], cf);

			SetVertexPos(&vertex[2], vx0, miny);
			SetVertexColor(&vertex[2], cf);

			SetVertexPos(&vertex[3], vx1, maxy);
			SetVertexColor(&vertex[3], ct);

			SetVertexPos(&vertex[4], vx1, miny);
			SetVertexColor(&vertex[4], ct);

			SetVertexPos(&vertex[5], maxx, maxy);
			SetVertexColor(&vertex[5], ct);

			// Create a GRADIENT_RECT structure that 
			// references the TRIVERTEX vertices. 
			GRADIENT_RECT gRect[3];
			gRect[0].UpperLeft  = 0;
			gRect[0].LowerRight = 1;
			gRect[1].UpperLeft  = 2;
			gRect[1].LowerRight = 3;
			gRect[2].UpperLeft  = 4;
			gRect[2].LowerRight = 5;

			// Draw a shaded rectangle. 
			GradientFill(painter->GetHDC(), vertex, 6, &gRect, 3, GRADIENT_FILL_RECT_H);

			//painter->FillRect(Rect(0, 0, length, 5), Color_Black);
		}

		void SetVertexPos(TRIVERTEX* v, int x, int y)
		{
			v->x = x;
			v->y = y;
		}

		void SetVertexColor(TRIVERTEX* v, Color c)
		{
			v->Red   = ColorGetR(c) << 8;
			v->Green = ColorGetG(c) << 8;
			v->Blue  = ColorGetB(c) << 8;
			v->Alpha = 0;
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