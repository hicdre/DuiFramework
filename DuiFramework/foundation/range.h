#pragma once
#include "foundation/basictypes.h"
namespace ui
{
	class Range {
	public:
		// Creates an empty range {0,0}.
		Range();

		// Initializes the range with a start and end.
		Range(size_t start, size_t end);

		// Initializes the range with the same start and end positions.
		explicit Range(size_t position);

		// Returns a range that is invalid, which is {size_t_max,size_t_max}.
		static const Range InvalidRange();

		// Checks if the range is valid through comparision to InvalidRange().
		bool IsValid() const;

		// Getters and setters.
		size_t start() const { return start_; }
		void set_start(size_t start) { start_ = start; }

		size_t end() const { return end_; }
		void set_end(size_t end) { end_ = end; }

		void setRange(size_t start, size_t end) {
			start_ = start;
			end_ = end;
		}

		// Returns the absolute value of the length.
		size_t length() const {
			int32 length = end() - start();
			return length >= 0 ? length : -length;
		}

		bool is_reversed() const { return start() > end(); }
		bool is_empty() const { return start() == end(); }

		// Returns the minimum and maximum values.
		size_t GetMin() const;
		size_t GetMax() const;

		bool operator==(const Range& other) const;
		bool operator!=(const Range& other) const;
		bool operator< (const Range& other) const;
		bool operator<=(const Range& other) const;
		bool operator> (const Range& other) const;
		bool operator>=(const Range& other) const;
		bool EqualsIgnoringDirection(const Range& other) const;

		// Returns true if this range intersects the specified |range|.
		bool Intersects(const Range& range) const;

		// Returns true if this range contains the specified |range|.
		bool Contains(const Range& range) const;

		// Computes the intersection of this range with the given |range|.
		// If they don't intersect, it returns an InvalidRange().
		// The returned range is always empty or forward (never reversed).
		Range Intersect(const Range& range) const;
	private:
		size_t start_;
		size_t end_;
	};
}