#include "stdafx.h"
#include "render_context.h"
#include "render_context_private.h"

#include "base/scoped_ptr.h"
#include "core/widget.h"
#include "core/constants.h"
#include "utils/utils.h"



#include <vector>

namespace ui
{

	RenderContext::RenderContext(Widget* widget)
		: context_(new RenderContextPrivate(widget))
	{
	}

	RenderContext::~RenderContext()
	{
		delete context_;
	}

	void RenderContext::FillRect(const Rect& rect, Color color)
	{
		context_->FillRect(rect, color);
	}


	void RenderContext::DrawArc(const Rect& rect, int from, int angles, Color color, int width)
	{
		context_->DrawArc(rect, from, angles, color, width);
	}


	void RenderContext::Trans(const Matrix& m)
	{
		context_->Trans(m);
	}

	void RenderContext::DrawImage(ImagePart* clip, const Rect& dest_rect)
	{
		//context_->DrawImage(clip, dest_rect);
	}

	void RenderContext::DrawImage(ImageFile* image, const Rect& src_rect, const Rect& dest_rect)
	{
		//context_->DrawImage(image, src_rect, dest_rect);
	}



	void RenderContext::SetClip(const Rect& rect)
	{
	}

	void RenderContext::Init()
	{
		InitCario();
	}

	void RenderContext::Uninit()
	{
		UnintCario();
	}

	void RenderContext::PushState()
	{
		context_->PushState();
	}

	void RenderContext::PopState()
	{
		context_->PopState();
	}

	

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
	}


	ScopedClipper::~ScopedClipper()
	{
	}
}