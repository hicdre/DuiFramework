#pragma once

namespace ui
{
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

		//static Length FromString(const std::string& str);
	private:
		LengthType type_;
		union {
			int int_value_;
			float float_value_;
		};
		bool is_float_;
	};
}