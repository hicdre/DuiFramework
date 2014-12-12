#pragma once
#include "base/basictypes.h"

namespace ui
{

	class RenderPath : public RefCounted < RenderPath >
	{
	public:
		virtual ~RenderPath() {}

		virtual void Arc(const Point& center, uint32 radius,
			uint32 angleFrom, uint32 angleTo,
			bool negative = false) = 0;

		virtual void BezierTo(const Point &aCP1,
			const Point &aCP2,
			const Point &aCP3) = 0;

		virtual void QuadraticBezierTo(const Point &aCP1,
			const Point &aCP2) = 0;

		virtual void Rectangle(const Rect& rect, bool negative = false) = 0;

		virtual void RoundRectangle(const RoundRect& rect, bool negative = false) = 0;

		virtual void LineTo(const Point &aPoint) = 0;

		virtual void MoveTo(const Point &aPoint) = 0;

		virtual Point CurrentPoint() const = 0;

		virtual void EndPath() = 0;
	};

	class RenderTarget : public RefCounted < RenderTarget >
	{
	public:
		virtual ~RenderTarget() {}

		virtual scoped_refptr<RenderPath> OpenRenderPath() = 0;

		virtual void StorkeRect(const Rect& rect, Color color, uint32 size) = 0;

		virtual void StorkePath(const RenderPath* path, Color color, uint32 size) = 0;

		virtual void FillRect(const Rect& rect, Color color) = 0;

		virtual void FillPath(const RenderPath* path, Color color) = 0;

		virtual void Trans(const Matrix& m) = 0;

		virtual void PushState() = 0;

		virtual void PopState() = 0;
	};

	class Widget;
	class RenderEngine
	{
	public:
		static void InitEngine();
		static void UnintEngine();

		static scoped_refptr<RenderTarget> CreateRenderTarget(Widget* w);
	};
}