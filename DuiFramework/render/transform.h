#pragma once
#include "render/rect.h"
#include "render/size.h"
#include "render/point.h"

namespace ui
{
	/**
	*	[ a, c, tx ] [x]   [a*x + c*y + tx]
	*  [ b, d, ty ] [y] = [b*x + d*y + ty]
	*  [ 0, 0, 1  ] [1]   [1             ]
	*
	*/
	class Transform
	{
	public:
		Transform();
		Transform(float a, float b, float c, float d, float tx, float ty);
		explicit Transform(const XFORM& xform);
		Transform& operator=(const XFORM& xform);

		void SetTransform(float a, float b, float c, float d, float tx, float ty);
		bool IsEqual(const Transform& t) const;

		Point Apply(const Point& point) const;
		Size Apply(const Size& size) const;
		Rect Apply(const Rect& rect) const;

		void TransformPoint(Point& point) const;
		void TransformSize(Size& size) const;
		void TransformRect(Rect& rect) const;

		void ConcatTransform(const Transform& t);
		/**
		*
		*	    [ 1, 0, tx ]
		*	t *	[ 0, 1, ty ]
		*	    [ 0, 0, 1  ]
		*
		*/
		Transform Translate(float x, float y) const;

		/**
		*	    [ cos(anAngle), -sin(anAngle), 0 ]
		*	t *	[ sin(anAngle),  cos(anAngle), 0 ]
		*	    [            0,             0, 1 ]
		*
		*/
		Transform Rotate(float anAngle) const;

		/**
		*		[ sx,  0, 0 ]
		*	t *	[  0, sy, 0 ]
		*		[  0,  0, 1 ]
		*
		*/
		Transform Scale(float sx, float sy) const;

		Transform Concat(const Transform& t) const;

		Transform Invert() const;

		XFORM ToXFORM() const;
	private:
		float a, b, c, d;
		float tx, ty;
	};
}