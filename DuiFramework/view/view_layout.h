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
		bool IsAuto();
		bool IsPercent();
		bool IsFixed();

		float percent() const;
		int intValue() const;
		float value() const;

		void setValue(float value);

		void setValue(LengthType t, float value);

		void setValue(LengthType t, int value);
	private:
		LengthType type_;
		union {
			int int_value_;
			int float_value_;
		};
		bool is_float_;
	};

	class LayoutData
	{
	public:
		LayoutData();

		bool IsFloat();
		int x();
		int y();
		int marginLeft();
		int marginTop();
		int marginRight();
		int marginBottom();

		const Length& width();
		const Length& height();

	private:
		bool float_;//浮动，不服从容器布局
		int x_;
		int y_;
		int margin_left_{ 0 }; //float 和在 absolute中时忽略
		int margin_top_{ 0 };
		int margin_right_{ 0 };
		int margin_bottom_{ 0 };
		Length width_;
		Length height_;
	};
}