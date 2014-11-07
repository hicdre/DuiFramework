#pragma once
#include "base/basictypes.h"

namespace ui
{
	enum StyleValueType
	{
		StyleValue_Null,
		StyleValue_None,
		StyleValue_Auto,

		StyleValue_Integer, //int
		StyleValue_Percent, //float
		StyleValue_Number, //float
		StyleValue_Pixel, //int

		StyleValue_Color, //unsigned

		StyleValue_Degree, //float

		StyleValue_Array, // array
		StyleValue_Pair, // pair
		StyleValue_Triplet, //triplet
		StyleValue_Rect, // rect

		StyleValue_Function, // array
		StyleValue_String, //string

		StyleValue_ResourceImage, // string id
		StyleValue_LineGradient,

	};
	class StyleValueString;
	class StyleValueArray;
	class StyleValuePair;
	class StyleValueTriplet;
	class StyleValue : public RefCounted<StyleValue>
	{
	public:
		StyleValue(StyleValueType t = StyleValue_Null);
		StyleValue(int32 val, StyleValueType t);
		StyleValue(float val, StyleValueType t);
		StyleValue(const std::string& val, StyleValueType t);
		

		StyleValueType GetType() const;

		bool IsNull() const;
		bool IsIntValue() const;
		bool IsStringValue() const;
		bool IsArrayValue() const;
		bool IsColorValue() const;
		
		bool IsAutoValue() const;
		int32 GetIntValue() const;
		int32 GetPixel() const;
		float GetPercentValue() const;
		float GetFloatValue() const;
		const std::string& GetStringValue() const;
		Color GetColorValue() const;

		void SetAutoValue();
		void SetIntValue(int32 value, StyleValueType type);
		void SetPercentValue(float value);
		void SetFloatValue(float value, StyleValueType type);
		void SetStringValue(const std::string& val, StyleValueType type);
		void SetColorValue(Color value);

		bool IsEqual(StyleValue* val) const;
	private:
		friend class RefCounted < StyleValue > ;
		~StyleValue();
		void Reset();
		StyleValueType type_;
		union {
			int32 int_value_;
			uint32 uint_value_;
			float float_value_;
			Color color_value_;
			StyleValueString* string_value_;
			StyleValueArray* array_value_;
			//StyleValuePair* pair_value_;
			//StyleValueTriplet* triplet_value_;
		};

		DISALLOW_COPY_AND_ASSIGN(StyleValue);
	};

	class StyleValueString
	{
	public:
		StyleValueString(const std::string& str);
		~StyleValueString();

		bool IsEqual(StyleValueString* val);
		const std::string& Get() const;
		void Set(const std::string& val);
	private:
		std::string str_;
		DISALLOW_COPY_AND_ASSIGN(StyleValueString);
	};

	class StyleValueArray
	{
	public:
		StyleValueArray();
		StyleValueArray(StyleValue* val);
		~StyleValueArray();

		int GetCount() const;
		StyleValue* Get(int index) const;

		void Reset();

		void Add(StyleValue* val);

		void Remove(StyleValue* val);

		bool IsEqual(StyleValueArray* val) const;

	private:
		std::vector<StyleValue*> array_;
		DISALLOW_COPY_AND_ASSIGN(StyleValueArray);
	};
}