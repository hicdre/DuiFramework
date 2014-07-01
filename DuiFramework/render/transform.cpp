#include "stdafx.h"
#include "transform.h"

namespace ui
{
	Transform::Transform()
		: a(1.0), b(0.0), c(0.0), d(1.0), tx(0.0), ty(0.0)
	{
	}

	Transform::Transform(float a, float b, float c, float d, float tx, float ty)
		: a(a), b(b), c(c), d(d), tx(tx), ty(ty)
	{

	}

	Transform::Transform(const XFORM& xform)
		: a(xform.eM11), b(xform.eM12), c(xform.eM21), d(xform.eM22), tx(xform.eDx), ty(xform.eDy)
	{

	}


	Transform& Transform::operator=(const XFORM& xform)
	{
		a = xform.eM11;
		b = xform.eM12;
		c = xform.eM21;
		d = xform.eM22;
		tx = xform.eDx;
		ty = xform.eDy;
		return *this;
	}


	bool Transform::IsEqual(const Transform& t) const
	{
		return (a == t.a && b == t.b && c == t.c && d == t.d && tx == t.tx && ty == t.ty);
	}

	Point Transform::Apply(const Point& point) const
	{
		Point p = point;
		TransformPoint(p);
		return p;
	}

	Size Transform::Apply(const Size& size) const
	{
		Size z = size;
		TransformSize(z);
		return z;
	}

	Rect Transform::Apply(const Rect& rect) const
	{
		Rect r = rect;
		TransformRect(r);
		return r;
	}

	void Transform::TransformPoint(Point& point) const
	{
		float x = (float)((double)a * point.x() + (double)c * point.y() + tx);
		float y = (float)((double)b * point.x() + (double)d * point.y() + ty);
		point.SetPoint(x, y);
	}

	void Transform::TransformSize(Size& size) const
	{
		float w = (float)((double)a * size.width() + (double)c * size.height());
		float h = (float)((double)b * size.width() + (double)d * size.height());
		size.SetSize(w, h);
	}

	void Transform::TransformRect(Rect& rect) const
	{
		float top = rect.y();
		float left = rect.x();
		float right = rect.right();
		float bottom = rect.bottom();

		Point topLeft = Apply(Point(left, top));
		Point topRight = Apply(Point(right, top));
		Point bottomLeft = Apply(Point(left, bottom));
		Point bottomRight = Apply(Point(right, bottom));

		float minX = (std::min)((std::min)(topLeft.x(), topRight.x()), (std::min)(bottomLeft.x(), bottomRight.x()));
		float maxX = (std::max)((std::max)(topLeft.x(), topRight.x()), (std::max)(bottomLeft.x(), bottomRight.x()));
		float minY = (std::min)((std::min)(topLeft.y(), topRight.y()), (std::min)(bottomLeft.y(), bottomRight.y()));
		float maxY = (std::max)((std::max)(topLeft.y(), topRight.y()), (std::max)(bottomLeft.y(), bottomRight.y()));

		rect.SetRect(minX, minY, (maxX - minX), (maxY - minY));
	}															  
	Transform Transform::Translate(float x, float y) const
	{
		return Transform(a, b, c, d, tx + a * x + c * y, y + b * tx + d * ty);
	}

	Transform Transform::Rotate(float anAngle) const
	{
		float fSin = std::sin(anAngle);
		float fCos = std::cos(anAngle);

		return Transform(a * fCos + c * fSin,
			b * fCos + d * fSin,
			c * fCos - a * fSin,
			d * fCos - b * fSin,
			tx,
			ty);
	}

	Transform Transform::Scale(float sx, float sy) const
	{
		return Transform(a * sx, b * sx, c * sy, d * sy, tx, ty);
	}

	Transform Transform::Concat(const Transform& t) const
	{
		return Transform(a * t.a + b * t.c, a * t.b + b * t.d, //a,b
			c * t.a + d * t.c, c * t.b + d * t.d, //c,d
			tx * t.a + ty * t.c + t.tx,                  //tx
			tx * t.b + ty * t.d + t.ty);                  //ty
	}

	Transform Transform::Invert() const
	{
		float determinant = 1 / (a * d - b * c);

		return Transform(determinant * d, -determinant * b, -determinant * c, determinant * a,
			determinant * (c * ty - d * tx), determinant * (b * tx - a * ty));
	}

	XFORM Transform::ToXFORM() const
	{
		XFORM x = { a, b, c, d, tx, ty };
		return x;
	}

	

}