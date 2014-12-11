#include "stdafx.h"
#include "render_path.h"
#include "render_context.h"
#include "render_path_private.h"
#include "render_context_private.h"

namespace ui
{


	RenderPath::RenderPath(RenderContext* context)
		: path_(new RenderPathPrivate(context->context_))
	{

	}

	RenderPath::~RenderPath()
	{
		delete path_;
	}

	void RenderPath::AddArc(const Point& center, uint32 radius, uint32 angleFrom, uint32 angleTo, bool negative /*= false*/)
	{
		path_->AddArc(center, radius, angleFrom, angleTo, negative);
	}

}