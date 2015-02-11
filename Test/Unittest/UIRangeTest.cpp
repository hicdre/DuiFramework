#include "stdafx.h"

using namespace ui;


std::string UIRangeIntDescripton(const UIRange<int>& u)
{
	return u.rangeDescription([](const int& val)->std::string
	{
		return std::to_string(val);
	});

}

TEST(UIRangeTest, Update) {

	//初始化
	UIRange<int> intRange(Range(0, 100), 20);
	EXPECT_STREQ("[0,100)=>20", UIRangeIntDescripton(intRange).c_str());

	intRange.Update(Range(50, 100), 21);
	EXPECT_STREQ("[0,50)=>20,[50,100)=>21", UIRangeIntDescripton(intRange).c_str());

	intRange.Update(Range(50, 100), 22);
	EXPECT_STREQ("[0,50)=>20,[50,100)=>22", UIRangeIntDescripton(intRange).c_str());

	intRange.Update(Range(26, 60), 23);
	EXPECT_STREQ("[0,26)=>20,[26,60)=>23,[60,100)=>22", UIRangeIntDescripton(intRange).c_str());

	intRange.Update(Range(10, 80), 24);
	EXPECT_STREQ("[0,10)=>20,[10,80)=>24,[80,100)=>22", UIRangeIntDescripton(intRange).c_str());

	intRange.Update(Range(50, 70), 22);
	EXPECT_STREQ("[0,10)=>20,[10,50)=>24,[50,70)=>22,[70,80)=>24,[80,100)=>22", UIRangeIntDescripton(intRange).c_str());

	//多段值相同区间合并
	intRange.Update(Range(70, 80), 22);
	EXPECT_STREQ("[0,10)=>20,[10,50)=>24,[50,100)=>22", UIRangeIntDescripton(intRange).c_str());

	intRange.Update(Range(0, 10), 24);
	EXPECT_STREQ("[0,50)=>24,[50,100)=>22", UIRangeIntDescripton(intRange).c_str());

}
