#include "stdafx.h"
#include "relative_layout.h"
#include "utils/utils.h"

#include <regex>

namespace ui
{
	const char kRelativeLayout[] = "relative_layout_data";
	const char kLayout[] = "layout";
	const char kRelativeToLeft[] = "relative.left";
	const char kRelativeToRight[] = "relative.right";
	const char kRelativeToTop[] = "relative.top";
	const char kRelativeToBottom[] = "relative.bottom";
	const char kRelativeToWidth[] = "relative.width";
	const char kRelativeToHeight[] = "relative.height";

	class StringParser
	{
	public:
		StringParser(const char* begin, size_t len)
			: begin_(begin), end_(begin + len), pos_(begin), is_end_(false){}

		bool IsValid() const { return !is_end_ && pos_ < end_; }

		char NextChar() const {
			if (is_end_)
				return 0;
			return *pos_;
		}

		bool Advance(int offset) {
			if (pos_ + offset <= end_) {
				pos_ += offset;
				return true;
			}
			is_end_ = true;
			return false;
		}

		void EatSpace() 
		{
			while (char ch = NextChar())
			{
				if (ch == ' ' || ch == '\t')
					Advance(1);
				else
					break;
			}
		}

		bool MatchWord(std::string& str)
		{
			EatSpace();

			const char* begin = pos_;
			size_t len = 0;
			while (char ch = NextChar())
			{
				if (IsAlphaChar(ch)) {
					len++;
					Advance(1);
				}
				else
					break;
			}
			if (len == 0)
				return false;
			str.assign(begin, len);
			return true;
		}

		bool MatchNumber(int &num)
		{
			bool has_matched = false;
			EatSpace();

			num = 0;
			while (char ch = NextChar())
			{
				if (IsNumberChar(ch)) {
					has_matched = true;
					num = num * 10 + ch - '0';
					Advance(1);
				}
				else
					break;
			}
			return has_matched;
		}

		bool MatchChar(char ch)
		{
			EatSpace();
			if (NextChar() != ch)
				return false;
			Advance(1);
			return true;
		}

		bool TryMatchSignedNumber(int &num)
		{
			EatSpace();
			char ch = NextChar();
			if (!ch)
				return false;

			bool negative = false;
			const char * point = pos_;
			if (ch == '+' || ch == '-')
			{
				negative = (ch == '-');
				Advance(1);				
			}

			if (!MatchNumber(num)){
				pos_ = point;
				return false;
			}

			if (negative)
				num = num * (-1);
			return true;
		}

		bool TryMatchPercent(int &num)
		{
			//»¹Ô­µã
			const char* point = pos_;
			if (!MatchNumber(num)){
				pos_ = point;
				return false;
			}

			if (!MatchChar('%')) {
				pos_ = point;
				return false;
			}
			return true;
		}

		bool MatchRelativePostion(RelativePosition& p, bool axis_x)
		{
			EatSpace();

			std::string relative_to;
			if (!MatchWord(relative_to))
				return false;

			if (relative_to == "left" && axis_x)
				p = RELATIVE_LEFT;
			else if (relative_to == "right" && axis_x)
				p = RELATIVE_RIGHT;
			else if (relative_to == "top" && !axis_x)
				p = RELATIVE_TOP;
			else if (relative_to == "bottom" && !axis_x)
				p = RELATIVE_BOTTOM;
			else if (relative_to == "center")
				p = RELATIVE_CENTER;
			else
				return false;

			return true;
		}

		bool MatchRelativeType(RelativeToType& p)
		{
			EatSpace();

			std::string relative_type;
			if (!MatchWord(relative_type))
				return false;

			if (relative_type == "parent")
				p = RELATIVE_TO_PARENT;
			else if (relative_type == "prev")
				p = RELATIVE_TO_PREV;
			else if (relative_type == "next")
				p = RELATIVE_TO_NEXT;
			else
				return false;

			return true;
		}

		bool MatchPositionType(RLayoutData::PositionType& t, bool axis_x)
		{
			EatSpace();

			int num = 0;
			if (TryMatchSignedNumber(num))
			{
				t.Set(RELATIVE_LEFT, RELATIVE_TO_PARENT, num);
				return true;
			}

			RelativeToType type;
			if (!MatchRelativeType(type))
				return false;

			if (!MatchChar(':'))
				return false;

			RelativePosition position;
			if (!MatchRelativePostion(position, axis_x))
				return false;

			if (!TryMatchSignedNumber(num))
				return false;

			t.Set(position, type, num);
			return true;
		}

		bool MatchSizeType(RLayoutData::SizeType& t)
		{
			EatSpace();

			int num = 0;

			char ch = NextChar();
			if (IsNumberChar(ch))
			{
				if (!MatchNumber(num))
					return false;

				if (MatchChar('%')) 
				{
					t.Set(RLAYOUT_SIZE_RELATIVE_PERCENT, RELATIVE_TO_PARENT, num);
				}
				else
				{
					t.Set(RLAYOUT_SIZE_FIXED, RELATIVE_TO_PARENT, num);
				}
				return true;
			}

			RelativeToType type;
			if (!MatchRelativeType(type))
				return false;

			EatSpace();
			ch = NextChar();
			if (ch == ':')
			{
				Advance(1);
				if (!TryMatchPercent(num))
					return false;

				t.Set(RLAYOUT_SIZE_RELATIVE_PERCENT, type, num);
				return true;
			}
			else if (ch == '+' || ch == '-')
			{
				if (!TryMatchSignedNumber(num))
					return false;

				t.Set(RLAYOUT_SIZE_RELATIVE_OFFSET, type, num);
				return true;
			}

			return true;
		}
	private:
		bool IsAlphaChar(char ch) {
			return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
		}
		bool IsNumberChar(char ch) {
			return '0' <= ch && ch <= '9';
		}
		const char* begin_;
		const char* end_;
		const char* pos_;
		bool is_end_;

	};



	RelativeLayoutManager::RelativeLayoutManager()
	{

	}

	RelativeLayoutManager::~RelativeLayoutManager()
	{

	}

	void RelativeLayoutManager::Layout(View* host)
	{
		Views layot_views;
		for (View* v = host->first_child(); v; v = v->next_sibling())
		{
			if (!v->visible() || !IsRelativeLayout(v))
				continue;
			layot_views.push_back(v);
		}
	}

	bool RelativeLayoutManager::IsRelativeLayout(View* v)
	{
		RLayoutData* data = GetLayoutData(v);
		if (data)
			return true;

		return ParseRelativeData(v);
	}

	RLayoutData* RelativeLayoutManager::GetLayoutData(View* v)
	{
		return static_cast<RLayoutData*>(v->GetData(kRelativeLayout));
	}

	bool RelativeLayoutManager::ParseRelativeData(View* v)
	{
		scoped_ptr<RLayoutData> data(new RLayoutData);
		
		{// left = "prev:right + 10"
			std::string str;
			if (v->GetPropertyString(kRelativeToLeft, &str))
			{
				StringParser p(str.c_str(), str.size());
				p.MatchPositionType(data->left, true);
			}
		}

		{// top
			std::string str;
			if (v->GetPropertyString(kRelativeToTop, &str))
			{
				StringParser p(str.c_str(), str.size());
				p.MatchPositionType(data->top, false);
			}
		}

		{// right
			std::string str;
			if (v->GetPropertyString(kRelativeToRight, &str))
			{
				StringParser p(str.c_str(), str.size());
				p.MatchPositionType(data->right, true);
			}
		}

		{// bottom
			std::string str;
			if (v->GetPropertyString(kRelativeToBottom, &str))
			{
				StringParser p(str.c_str(), str.size());
				p.MatchPositionType(data->bottom, true);
			}
		}

		{// width
			std::string str;
			if (v->GetPropertyString(kRelativeToWidth, &str))
			{
				StringParser p(str.c_str(), str.size());
				p.MatchSizeType(data->width);
			}
		}

		{// height
			std::string str;
			if (v->GetPropertyString(kRelativeToWidth, &str))
			{
				StringParser p(str.c_str(), str.size());
				p.MatchSizeType(data->height);
			}
		}

		v->SetData(kRelativeLayout, data.release());
		return true;
	}

	void RelativeLayoutTest()
	{
		
		{
			RLayoutData::PositionType type;
			std::string str = "68";
			StringParser p(str.c_str(), str.size());
			p.MatchPositionType(type, true);
			assert(type.to == RELATIVE_TO_PARENT);
			assert(type.position == RELATIVE_LEFT);
			assert(type.offset == 68);
		}

		{
			RLayoutData::PositionType type;
			std::string str = "prev:top + 32";
			StringParser p(str.c_str(), str.size());
			p.MatchPositionType(type, false);
			assert(type.to == RELATIVE_TO_PREV);
			assert(type.position == RELATIVE_TOP);
			assert(type.offset == 32);
		}

		{
			RLayoutData::PositionType type;
			std::string str = "next:left - 44";
			StringParser p(str.c_str(), str.size());
			p.MatchPositionType(type, true);
			assert(type.to == RELATIVE_TO_NEXT);
			assert(type.position == RELATIVE_LEFT);
			assert(type.offset == -44);
		}

		{
			RLayoutData::SizeType type;
			std::string str = "544";
			StringParser p(str.c_str(), str.size());
			p.MatchSizeType(type);
			assert(type.type == RLAYOUT_SIZE_FIXED);
			//assert(type.to == RELATIVE_LEFT);
			assert(type.data == 544);
		}

		{
			RLayoutData::SizeType type;
			std::string str = "544%";
			StringParser p(str.c_str(), str.size());
			p.MatchSizeType(type);
			assert(type.type == RLAYOUT_SIZE_RELATIVE_PERCENT);
			assert(type.to == RELATIVE_TO_PARENT);
			assert(type.data == 544);
		}

		{
			RLayoutData::SizeType type;
			std::string str = "prev : 144%";
			StringParser p(str.c_str(), str.size());
			p.MatchSizeType(type);
			assert(type.type == RLAYOUT_SIZE_RELATIVE_PERCENT);
			assert(type.to == RELATIVE_TO_PREV);
			assert(type.data == 144);
		}

		{
			RLayoutData::SizeType type;
			std::string str = "next - 30";
			StringParser p(str.c_str(), str.size());
			p.MatchSizeType(type);
			assert(type.type == RLAYOUT_SIZE_RELATIVE_OFFSET);
			assert(type.to == RELATIVE_TO_NEXT);
			assert(type.data == -30);
		}
	}



}