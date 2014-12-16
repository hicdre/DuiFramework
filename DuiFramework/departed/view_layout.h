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