#include "stdafx.h"
#include "render_helper.h"

namespace ui
{
	ScopedPainter::ScopedPainter(RenderContext* painter, const Matrix& m)
		: p_(painter), m_(m.Invert())
	{
		p_->PushState();
		p_->Trans(m);
	}

	ScopedPainter::~ScopedPainter()
	{
		p_->PopState();
	}


	ScopedClipper::ScopedClipper(RenderContext* painter, const Rect& r)
		: p_(painter)
	{
		p_->PushState();
		p_->ClipRect(r);
	}

	ScopedClipper::ScopedClipper(RenderContext* painter, const RenderPath* r)
		: p_(painter)
	{
		p_->PushState();
		p_->ClipPath(r);
	}


	ScopedClipper::~ScopedClipper()
	{
		p_->PopState();
	}
}