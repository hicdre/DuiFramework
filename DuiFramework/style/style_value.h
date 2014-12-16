#pragma once
#include "base/basictypes.h"
#include "ui/ui_constants.h"

namespace ui
{
	enum StyleValueType
	{
		StyleValue_Null,
		StyleValue_Auto,
		StyleValue_Inherit,
		StyleValue_None,
		
		//基础类型
		StyleValue_Integer, //int
		StyleValue_Float, //int
		StyleValue_Color, //Color
		StyleValue_String, //string
		StyleValue_URL,//url

		//扩展类型
		StyleValue_Pixel, //float
		StyleValue_Cursor, //unsigned

		StyleValue_Percent, //float
		StyleValue_Number, //float
		StyleValue_Degree, //float

		//组合类型
		StyleValue_Array, // array
		StyleValue_Pair, // pair
		StyleValue_Triplet, //triplet
		StyleValue_Rect, // rect

		//扩展类型
		StyleValue_Function, // array
		StyleValue_ResourceId, // string id
		StyleValue_LineGradient,

	};
	class StyleValueString;
	class StyleValueArray;
	class StyleValuePair;
	class StyleValueTriplet;
	class StyleValueFunction;
	class StyleValue : public RefCounted<StyleValue>
	{
	public:
		StyleValue(StyleValueType t = StyleValue_Null);
		StyleValue(int32 val, StyleValueType t);
		StyleValue(float val, StyleValueType t);
		StyleValue(const std::string& val, StyleValueType t);

		StyleValueType GetType() const;

		//boolean
		bool IsNull() const;
		bool IsAutoValue() const;

		bool IsIntValue() const;
		bool IsColorValue() const;
		bool IsFloatValue() const;
		bool IsStringValue() const;
		bool IsArrayValue() const;
		bool IsUrlValue() const;

		bool IsPercentValue() const;
		bool IsPixelValue() const;
		bool IsCursorValue() const;
		bool IsFunctionValue() const;
		
		//getter
		int32 GetIntValue() const;
		float GetFloatValue() const;
		Color GetColorValue() const;
		const std::string& GetStringValue() const;
		StyleValueArray* GetArrayValue() const;
		const URL& GetUrlValue() const;
		StyleValueFunction* GetFunctionValue() const;
		
		CursorId GetCursorValue() const;
		int32 GetPixel() const;
		float GetPercentValue() const;
		
		//setter
		void SetAutoValue();
		
		void SetIntValue(int32 value);
		void SetColorValue(Color value);
		void SetFloatValue(float value);
		void SetFloatValue(float value, StyleValueType type);
		void SetNumberValue(float value);
		void SetStringValue(const std::string& value);
		void SetStringValue(const std::string& val, StyleValueType type);
		void SetArrayValue(StyleValueArray* value);
		void SetUrlValue(const std::string& str);
		void SetUrlValue(const std::wstring& str);
		void SetUrlValue(const URL& url);
		void SetFunctionValue(const std::string& name);
		void SetFunctionValue(StyleValueFunction* value);

		void SetPercentValue(float value);
		void SetCursorValue(CursorId id);

		bool IsEqual(StyleValue* val) const;
	private:
		friend class RefCounted < StyleValue > ;
		~StyleValue();
		void Reset();
		void SetIntValue(int32 value, StyleValueType type);
		void SetUIntValue(uint32 value, StyleValueType type);
		void SetFloatValueInternal(float value, StyleValueType type);
		void SetStringValueInternal(const std::string& val, StyleValueType type);
		static bool IsIntType(StyleValueType t);
		static bool IsFloatType(StyleValueType t);
		static bool IsStringType(StyleValueType t);
		StyleValueType type_;
		union {
			int32 int_value_;
			uint32 uint_value_;
			float float_value_;
			Color color_value_;
			StyleValueString* string_value_;
			StyleValueArray* array_value_;
			URL* url_value_;
			StyleValueFunction* function_value_;
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

	class StyleValueFunction
	{
	public:
		StyleValueFunction(const std::string& name);
		~StyleValueFunction();

		const std::string& GetFunctionName() const { return name_; }

		size_t GetParamsCount() const { return params_.size(); }

		StyleValue* GetParam(size_t index) const;
		void AddParam(StyleValue* val);
		
		bool IsEqual(StyleValueFunction* val) const;
	private:
		std::string name_;
		std::vector<StyleValue*> params_;
		DISALLOW_COPY_AND_ASSIGN(StyleValueFunction);
	};

}