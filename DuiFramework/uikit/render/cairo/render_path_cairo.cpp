#include "stdafx.h"
#include "render_path_cairo.h"
#include "render_target_cairo.h"

namespace ui
{
	static inline double degreeToR(uint32 d){
		return d * (3.14159265358979323846 / 180.0);
	}

	UIRenderPathCairo::UIRenderPathCairo(UIRenderTargetCairo* target)
		: target_(target)
		, cairo_(target->get_context())
		, path_(NULL)
	{
		cairo_reference(cairo_);

		cairo_new_path(cairo_);
	}

	UIRenderPathCairo::~UIRenderPathCairo()
	{
		if (path_) {
			cairo_path_destroy(path_);
			path_ = NULL;
		}
		cairo_destroy(cairo_);
	}

	void UIRenderPathCairo::Arc(const Point& center, uint32 radius, uint32 angleFrom, uint32 angleTo, bool negative /*= false*/)
	{
		if (!negative)
			cairo_arc(cairo_, center.x(), center.y(), radius, degreeToR(angleFrom), degreeToR(angleTo));
		else
			cairo_arc_negative(cairo_, center.x(), center.y(), radius, degreeToR(angleFrom), degreeToR(angleTo));
	}


	void UIRenderPathCairo::BezierTo(const Point &aCP1, const Point &aCP2, const Point &aCP3)
	{
		cairo_curve_to(cairo_, aCP1.x(), aCP1.y(), aCP2.x(), aCP2.y(), aCP3.x(), aCP3.y());
	}

	void UIRenderPathCairo::QuadraticBezierTo(const Point &aCP1, const Point &aCP2)
	{
		Point CP0 = CurrentPoint();
		Point CP1((CP0.x() + aCP1.x() * 2.0) / 3.0, (CP0.y() + aCP1.y() * 2.0) / 3.0);
		Point CP2((aCP2.x() + aCP1.x() * 2.0) / 3.0, (aCP2.y() + aCP1.y() * 2.0) / 3.0);
		//Point CP2 = (aCP2 + aCP1 * 2.0) / 3.0;
		Point CP3(aCP2);

		cairo_curve_to(cairo_, CP1.x(), CP1.y(), CP2.x(), CP2.y(), CP3.x(), CP3.y());
	}


	void UIRenderPathCairo::Rectangle(const Rect& rect, bool negative)
	{
		if (!negative) {
			cairo_rectangle(cairo_, rect.x(), rect.y(), rect.width(), rect.height());
		}
		else {
			cairo_move_to(cairo_, rect.x(), rect.y());
			cairo_rel_line_to(cairo_, 0, rect.height());
			cairo_rel_line_to(cairo_, rect.width(), 0);
			cairo_rel_line_to(cairo_, 0, -rect.height());
			cairo_close_path(cairo_);
		}
	}


	void UIRenderPathCairo::RoundRectangle(const RoundRect& rect, bool negative /*= false*/)
	{
		const double alpha = 0.55191497064665766025;

		typedef struct { double a, b; } twoFloats;

		twoFloats cwCornerMults[4] = { 
			{ -1, 0 },
			{ 0, -1 },
			{ +1, 0 },
			{ 0, +1 } 
		};
		twoFloats ccwCornerMults[4] = { 
			{ +1, 0 },
			{ 0, -1 },
			{ -1, 0 },
			{ 0, +1 } 
		};

		twoFloats *cornerMults = negative ? ccwCornerMults : cwCornerMults;

		Point pc, p0, p1, p2, p3;

		if (!negative)
			cairo_move_to(cairo_, rect.x() + rect.GetCornerSize(RoundRect::TopLeft).width(), rect.y());
		else
			cairo_move_to(cairo_, rect.x() + rect.width() - rect.GetCornerSize(RoundRect::TopRight).width(), rect.y());

		for(int i=0; i<4; ++i)
		{
			// the corner index -- either 1 2 3 0 (cw) or 0 3 2 1 (ccw)
			RoundRect::Corner c = (RoundRect::Corner)(!negative ? ((i + 1) % 4) : ((4 - i) % 4));

			// i+2 and i+3 respectively.  These are used to index into the corner
			// multiplier table, and were deduced by calculating out the long form
			// of each corner and finding a pattern in the signs and values.
			int i2 = (i + 2) % 4;
			int i3 = (i + 3) % 4;

			pc = rect.AtCorner(c);

			Size cz = rect.GetCornerSize(c);

			if (cz.width() > 0.0 && cz.height() > 0.0) {
				p0.set_x(pc.x() + cornerMults[i].a * cz.width());
				p0.set_y(pc.y() + cornerMults[i].b * cz.height());

				p3.set_x(pc.x() + cornerMults[i3].a * cz.width());
				p3.set_y(pc.y() + cornerMults[i3].b * cz.height());

				p1.set_x(p0.x() + alpha * cornerMults[i2].a * cz.width());
				p1.set_y(p0.y() + alpha * cornerMults[i2].b * cz.height());
				   			 
				p2.set_x(p3.x() - alpha * cornerMults[i3].a * cz.width());
				p2.set_y(p3.y() - alpha * cornerMults[i3].b * cz.height());

				cairo_line_to(cairo_, p0.x(), p0.y());
				cairo_curve_to(cairo_,
					p2.x(), p2.y(),
					p1.x(), p1.y(),
					p3.x(), p3.y());
			}
			else {
				cairo_line_to(cairo_, pc.x(), pc.y());
			}
		}

		cairo_close_path(cairo_);
	}



	void UIRenderPathCairo::LineTo(const Point &aPoint)
	{
		cairo_line_to(cairo_, aPoint.x(), aPoint.y());
	}

	void UIRenderPathCairo::MoveTo(const Point &aPoint)
	{
		cairo_move_to(cairo_, aPoint.x(), aPoint.y());
	}

	Point UIRenderPathCairo::CurrentPoint() const
	{
		double x, y;
		cairo_get_current_point(cairo_, &x, &y);
		return Point((int)x, (int)y);
	}

	cairo_path_t* UIRenderPathCairo::get_path() const
	{
		return path_;
	}

	void UIRenderPathCairo::EndPath()
	{
		path_ = cairo_copy_path(cairo_);
		//cairo_new_path(cairo_);
	}

}