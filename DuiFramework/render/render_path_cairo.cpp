#include "stdafx.h"
#include "render_path_cairo.h"
#include "render_target_cairo.h"

namespace ui
{
	static inline double degreeToR(uint32 d){
		return d * (3.14159265358979323846 / 180.0);
	}

	RenderPathCairo::RenderPathCairo(RenderTargetCairo* target)
		: target_(target)
		, cairo_(target->get_context())
		, path_(NULL)
	{
		cairo_reference(cairo_);

		cairo_new_path(cairo_);
	}

	RenderPathCairo::~RenderPathCairo()
	{
		if (path_) {
			cairo_path_destroy(path_);
			path_ = NULL;
		}
		cairo_destroy(cairo_);
	}

	void RenderPathCairo::Arc(const Point& center, uint32 radius, uint32 angleFrom, uint32 angleTo, bool negative /*= false*/)
	{
		if (!negative)
			cairo_arc(cairo_, center.x(), center.y(), radius, degreeToR(angleFrom), degreeToR(angleTo));
		else
			cairo_arc_negative(cairo_, center.x(), center.y(), radius, degreeToR(angleFrom), degreeToR(angleTo));
	}


	void RenderPathCairo::BezierTo(const Point &aCP1, const Point &aCP2, const Point &aCP3)
	{
		cairo_curve_to(cairo_, aCP1.x(), aCP1.y(), aCP2.x(), aCP2.y(), aCP3.x(), aCP3.y());
	}

	void RenderPathCairo::QuadraticBezierTo(const Point &aCP1, const Point &aCP2)
	{
		Point CP0 = CurrentPoint();
		Point CP1((CP0.x() + aCP1.x() * 2.0) / 3.0, (CP0.y() + aCP1.y() * 2.0) / 3.0);
		Point CP2((aCP2.x() + aCP1.x() * 2.0) / 3.0, (aCP2.y() + aCP1.y() * 2.0) / 3.0);
		//Point CP2 = (aCP2 + aCP1 * 2.0) / 3.0;
		Point CP3(aCP2);

		cairo_curve_to(cairo_, CP1.x(), CP1.y(), CP2.x(), CP2.y(), CP3.x(), CP3.y());
	}


	void RenderPathCairo::Rectangle(const Rect& rect, bool negative)
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


	void RenderPathCairo::RoundRectangle(const RoundRect& rect, bool negative /*= false*/)
	{

	}



	void RenderPathCairo::LineTo(const Point &aPoint)
	{
		cairo_line_to(cairo_, aPoint.x(), aPoint.y());
	}

	void RenderPathCairo::MoveTo(const Point &aPoint)
	{
		cairo_move_to(cairo_, aPoint.x(), aPoint.y());
	}

	Point RenderPathCairo::CurrentPoint() const
	{
		double x, y;
		cairo_get_current_point(cairo_, &x, &y);
		return Point((int)x, (int)y);
	}

	cairo_path_t* RenderPathCairo::get_path() const
	{
		return path_;
	}

	void RenderPathCairo::EndPath()
	{
		path_ = cairo_copy_path(cairo_);
		cairo_new_path(cairo_);
	}

}