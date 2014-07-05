#pragma once
#include "core/constants.h"
#include "render/point.h"
#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace ui
{
	class View;
	class Event
	{
	public:
		Event(EventType type, View* v);
		virtual ~Event();

		bool IsMouseEvent() const {
			return type_ == EVENT_MOUSE_PRESSED ||
				type_ == EVENT_MOUSE_DRAGGED ||
				type_ == EVENT_MOUSE_RELEASED ||
				type_ == EVENT_MOUSE_MOVE ||
				type_ == EVENT_MOUSE_ENTER ||
				type_ == EVENT_MOUSE_LEAVE ||
				type_ == EVENT_MOUSEWHEEL;
		}

		EventType type() const { return type_; }

		void StopPropagation();
		bool stopped_propagation() const { return is_propagation_; }

		void SetOwner(View* v);
		View* owner() const;

		void SetSender(View* v);
		View* sender() const;

	protected:
		EventType type_;
		bool is_propagation_{ true }; //是否继续冒泡
		View* sender_;//消息上一个传递者
		View* owner_;//消息发出者
	};

	class MouseEvent : public Event
	{
	public:
		MouseEvent(EventType type, const Point& pt, View* owner, int flags);

		int flags() const;

		bool IsOnlyLeftMouseButton() const;

		bool IsLeftMouseButton() const;

		bool IsOnlyMiddleMouseButton() const;

		bool IsMiddleMouseButton() const;

		bool IsOnlyRightMouseButton() const;

		bool IsRightMouseButton() const;

		bool IsAnyButton() const;

		Point GetPosition() const;
		void DispathTo(View* v);
	protected:
		Point pt_; //在WidgetView坐标系中
		int flags_{ 0 };
	};

	class EventListener;
	typedef std::function<void(Event* evt)> EventAction;
	typedef std::unordered_set<EventAction> EventActionList;
	typedef std::unordered_map<View*, std::unordered_map<EventType, EventAction>>  EventListenMap;

	class EventDispatcher
	{
	public:
		EventDispatcher();
		~EventDispatcher();

		void AddListener(EventListener* listener);
		void RemoveListener(EventListener* listener); 

		void RemoveView(View* v);

		void DispatchEvent(View* v, Event* e);

		static EventDispatcher* Default();
	private:
		std::vector<EventListener*> listener_list_;
	};


	class EventListener
	{
	public:
		EventListener();
		virtual ~EventListener();

		void Listen(View* from, EventType type, EventAction action);
		void UnListen(View* from, EventType type);
		void UnListen(View* from);

		void DispatchEvent(View* v, Event* e);
	private:
		EventListenMap event_map_;
	};

	Point GetMouseLocation(const MSG& msg);
	int GetEventFlags(const MSG& msg);
}