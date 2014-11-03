#pragma once
#include "base\basictypes.h"

namespace ui
{
	/*
		x: absolute
		y: absolute
		margin-left: hgroup, vgroup
		margin-top: hgroup, vgroup
		margin-right: hgroup, vgroup
		margin-bottom: hgroup, vgroup

		width : 
		height : 
		width-percent:
		height_percent:
	*/
	enum LengthType {
	Auto, Percent, Fixed,
	Intrinsic, MinIntrinsic,
	MinContent, MaxContent, FillAvailable, FitContent,
	Calculated,
	ExtendToZoom, DeviceWidth, DeviceHeight,
	MaxSizeNone
	};

	class Length
	{
	public:
		Length();
		Length(LengthType type);
		Length(int v, LengthType t);
		Length(float v, LengthType t);

		LengthType type() const;
		bool IsAuto() const;
		bool IsPercent() const;
		bool IsFixed() const;

		float percent() const;
		int intValue() const;
		float value() const;

		void setValue(float value);

		void setValue(LengthType t, float value);

		void setValue(LengthType t, int value);

		static Length FromString(const std::string& str);
	private:
		LengthType type_;
		union {
			int int_value_;
			float float_value_;
		};
		bool is_float_;
	};

	class LayoutBox
	{
	public:
		LayoutBox();
		~LayoutBox();

		const Length& x();
		const Length& y();
		const Length& width();
		const Length& height();

		const Length& marginLeft();
		const Length& marginTop();
		const Length& marginRight();
		const Length& marginBottom();

		void setX(const Length& l);
		void setY(const Length& l);
		void setWidth(const Length& l);
		void setHeight(const Length& l);

		void setMarginLeft(const Length& l);
		void setMarginTop(const Length& l);
		void setMarginRight(const Length& l);
		void setMarginBottom(const Length& l);
	private:
		Length width_;
		Length height_;

		//absoulte时相对与parent
		//hbox，vbox时相对于上一个
		Length margin_left_;
		Length margin_top_;

		Length margin_right_;
		Length margin_bottom_;
		
	};
}