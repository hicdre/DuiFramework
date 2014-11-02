#pragma once
#include "base/rect.h"
#include "base/size.h"
#include "base/point.h"

namespace ui
{
	/**
	*	[ a, c, tx ] [x]   [a*x + c*y + tx]
	*  [ b, d, ty ] [y] = [b*x + d*y + ty]
	*  [ 0, 0, 1  ] [1]   [1             ]
	*
	*/
	class Matrix
	{
	public:
		Matrix();
		Matrix(float a, float b, float c, float d, float tx, float ty);
		explicit Matrix(const XFORM& xform);
		Matrix& operator=(const XFORM& xform);

		void SetTransform(float a, float b, float c, float d, float tx, float ty);
		bool IsEqual(const Matrix& t) const;

		Point Apply(const Point& point) const;
		Size Apply(const Size& size) const;
		Rect Apply(const Rect& rect) const;

		void TransformPoint(Point& point) const;
		void TransformSize(Size& size) const;
		void TransformRect(Rect& rect) const;

		void ConcatTransform(const Matrix& t);
		/**
		*
		*	    [ 1, 0, tx ]
		*	t *	[ 0, 1, ty ]
		*	    [ 0, 0, 1  ]
		*
		*/
		Matrix Translate(float x, float y) const;

		/**
		*  弧度
		*	    [ cos(anAngle), -sin(anAngle), 0 ]
		*	t *	[ sin(anAngle),  cos(anAngle), 0 ]
		*	    [            0,             0, 1 ]
		*
		*/
		Matrix Rotate(float anAngle) const;

		/**
		*		[ sx,  0, 0 ]
		*	t *	[  0, sy, 0 ]
		*		[  0,  0, 1 ]
		*
		*/
		Matrix Scale(float sx, float sy) const;

		Matrix Concat(const Matrix& t) const;

		Matrix Invert() const;

		XFORM ToXFORM() const;
	private:
		float a, b, c, d;
		float tx, ty;
	};
}