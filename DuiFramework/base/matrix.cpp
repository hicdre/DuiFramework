#include "stdafx.h"
#include "matrix.h"
#include <cmath>

namespace ui
{
	Matrix::Matrix()
		: a(1.0), b(0.0), c(0.0), d(1.0), tx(0.0), ty(0.0)
	{
	}

	Matrix::Matrix(float a, float b, float c, float d, float tx, float ty)
		: a(a), b(b), c(c), d(d), tx(tx), ty(ty)
	{

	}

	Matrix::Matrix(const XFORM& xform)
		: a(xform.eM11), b(xform.eM12), c(xform.eM21), d(xform.eM22), tx(xform.eDx), ty(xform.eDy)
	{

	}


	void Matrix::SetTransform(float xa, float xb, float xc, float xd, float xtx, float xty)
	{
		a = xa;
		b = xb;
		c = xc;
		d = xd;
		tx = xtx;
		ty = xty;
	}


	Matrix& Matrix::operator=(const XFORM& xform)
	{
		a = xform.eM11;
		b = xform.eM12;
		c = xform.eM21;
		d = xform.eM22;
		tx = xform.eDx;
		ty = xform.eDy;
		return *this;
	}


	bool Matrix::IsEqual(const Matrix& t) const
	{
		return (a == t.a && b == t.b && c == t.c && d == t.d && tx == t.tx && ty == t.ty);
	}

	Point Matrix::Apply(const Point& point) const
	{
		Point p = point;
		TransformPoint(p);
		return p;
	}

	Size Matrix::Apply(const Size& size) const
	{
		Size z = size;
		TransformSize(z);
		return z;
	}

	Rect Matrix::Apply(const Rect& rect) const
	{
		Rect r = rect;
		TransformRect(r);
		return r;
	}

	void Matrix::TransformPoint(Point& point) const
	{
		float x = (float)std::floor((double)a * point.x() + (double)c * point.y() + tx);
		float y = (float)std::floor((double)b * point.x() + (double)d * point.y() + ty);
		point.SetPoint(x, y);
	}

	void Matrix::TransformSize(Size& size) const
	{
		float w = (float)std::floor((double)a * size.width() + (double)c * size.height());
		float h = (float)std::floor((double)b * size.width() + (double)d * size.height());
		size.SetSize(w, h);
	}

	void Matrix::TransformRect(Rect& rect) const
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
	Matrix Matrix::Translate(float x, float y) const
	{
		return Matrix(a, b, c, d, tx + a * x + c * y, y + b * tx + d * ty);
	}

	Matrix Matrix::Rotate(float anAngle) const
	{
		float fSin = std::sin(anAngle);
		float fCos = std::cos(anAngle);

		return Matrix(a * fCos + c * fSin,
			b * fCos + d * fSin,
			c * fCos - a * fSin,
			d * fCos - b * fSin,
			tx,
			ty);
	}

	Matrix Matrix::Scale(float sx, float sy) const
	{
		return Matrix(a * sx, b * sx, c * sy, d * sy, tx, ty);
	}

	Matrix Matrix::Concat(const Matrix& t) const
	{
		return Matrix(a * t.a + b * t.c, a * t.b + b * t.d, //a,b
			c * t.a + d * t.c, c * t.b + d * t.d, //c,d
			tx * t.a + ty * t.c + t.tx,                  //tx
			tx * t.b + ty * t.d + t.ty);                  //ty
	}


	void Matrix::ConcatTransform(const Matrix& t)
	{
		SetTransform(a * t.a + b * t.c, a * t.b + b * t.d, //a,b
			c * t.a + d * t.c, c * t.b + d * t.d, //c,d
			tx * t.a + ty * t.c + t.tx,                  //tx
			tx * t.b + ty * t.d + t.ty);
	}


	Matrix Matrix::Invert() const
	{
		float determinant = 1 / (a * d - b * c);

		return Matrix(determinant * d, -determinant * b, -determinant * c, determinant * a,
			determinant * (c * ty - d * tx), determinant * (b * tx - a * ty));
	}

	XFORM Matrix::ToXFORM() const
	{
		XFORM x = { a, b, c, d, tx, ty };
		return x;
	}

	

}