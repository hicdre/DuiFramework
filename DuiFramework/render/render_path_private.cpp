#include "stdafx.h"
#include "render_path_private.h"
#include "render_context_private.h"

namespace ui
{


	RenderPathPrivate::RenderPathPrivate(RenderContextPrivate* context)
		: context_(context)
	{
		context_->NewPath();
	}

	RenderPathPrivate::~RenderPathPrivate()
	{
		context_->ClosePath();
	}

	void RenderPathPrivate::AddArc(const Point& center, uint32 radius, uint32 angleFrom, uint32 angleTo, bool negative /*= false*/)
	{
		//context_
	}

}