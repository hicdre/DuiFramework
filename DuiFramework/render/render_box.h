#pragma once
#include "render/render_object.h"

namespace ui
{

	class RenderBox : public RenderObject
	{
	public:
		RenderBox(UIElement* elem);
		~RenderBox();

		int marginLeft() const;
		int marginTop() const;
		int marginRight() const;
		int marginBottom() const;
		int marginWidth() const { return marginLeft() + marginRight(); }
		int marginHeight() const { return marginTop() + marginBottom(); }

		virtual void DoPaint(RenderContext* painter, const Rect& r) override;
		virtual void Layout() override;
		virtual void LayoutIfNeeded() override;

		virtual void OnBoundsChanged() override;
		virtual void OnChildBoundsChanged(RenderBox* box);
		Rect GetContentBounds();
	protected:
		void PaintBackground(RenderContext* painter);
		void PaintBorder(RenderContext* painter);
		void PaintContents(RenderContext* painter);

		RenderBox* parentBox();

		bool needs_layout_;//self needs layout
		bool child_needs_layout_;
	};

	class RenderHBox : public RenderBox
	{
	public:
		RenderHBox(UIElement* elem);
		~RenderHBox();

		virtual void Layout() override;
	};

	class RenderVBox : public RenderBox
	{
	public:
		RenderVBox(UIElement* elem);
		~RenderVBox();

		virtual void Layout() override;
	};
}