#pragma once
#include "base/basictypes.h"

namespace ui
{
	class RenderContext;
	class RenderObject;
	class RenderBox;
	class RenderContent;
	class RenderWidget;
	class RenderStyles;
	typedef scoped_refptr<RenderObject> RenderObjectPtr;
	typedef scoped_refptr<RenderBox> RenderBoxPtr;
	typedef scoped_refptr<RenderContent> RenderChildPtr;
}