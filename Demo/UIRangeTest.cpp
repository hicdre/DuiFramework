#include "stdafx.h"
#include "uikit/uikit.h"

using namespace ui;


void RunUikitRangeTest()
{
	UIRange<int> intRange(Range(0,100), 20);
	intRange.Update(Range(50, 100), 21);
	intRange.Update(Range(50, 100), 22);
	intRange.Update(Range(26, 60), 23);
	intRange.Update(Range(10, 80), 24);

}
