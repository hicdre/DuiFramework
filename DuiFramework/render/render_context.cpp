#include "stdafx.h"
#include "render_context.h"

#include "base/scoped_ptr.h"
#include "core/widget.h"
#include "core/constants.h"
#include "utils/utils.h"



#include <vector>

namespace ui
{

	RenderContext::RenderContext(Widget* widget)
		: target_(RenderEngine::CreateRenderTarget(widget))
	{
	}

	RenderContext::~RenderContext()
	{
	}

	void RenderContext::FillRect(const Rect& rect, Color color)
	{
		target_->FillRect(rect, color);
	}


	void RenderContext::DrawArc(const Rect& rect, int from, int angles, Color color, int width)
	{
		//target_->DrawArc(rect, from, angles, color, width);
	}


	void RenderContext::Trans(const Matrix& m)
	{
		target_->Trans(m);
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


	void RenderContext::PushState()
	{
		target_->PushState();
	}

	void RenderContext::PopState()
	{
		target_->PopState();
	}

	scoped_refptr<RenderPath> RenderContext::CreatePath()
	{
		return target_->OpenRenderPath();
	}

	void RenderContext::FillPath(const RenderPath* path, Color color)
	{
		target_->FillPath(path, color);
	}

	

	
}