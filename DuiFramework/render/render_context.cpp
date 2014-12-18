#include "stdafx.h"
#include "render_context.h"

#include "framework/widget.h"
#include "ui/ui_constants.h"
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

	void RenderContext::DrawImage(const RenderImage *image, const Rect& dest)
	{
		target_->DrawImage(image, dest);
	}

	void RenderContext::DrawImage(const RenderImage *image, const Rect& src, const Rect& dest)
	{
		target_->DrawImage(image, src, dest);
	}



	void RenderContext::ClipRect(const Rect& rect)
	{
		target_->ClipRect(rect);
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

	void RenderContext::ClipPath(const RenderPath* path)
	{
		target_->ClipPath(path);
	}

	void RenderContext::DrawText(const std::wstring& text, const Font& font, Color color)
	{
		if (text.empty())
			return;
// 
// 		Glyph* glyps = new Glyph[text.size()];
// 		for (size_t i = 0; i < text.size(); i++)
// 		{
// 			glyps[i] = ;
// 		}

		target_->DrawText(text, font, color);
//
	}

	

	
}