#pragma once
#include "core/constants.h"
#include "render/point.h"
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <list>
#include <set>

namespace ui
{

	class View;
	class Widget;
	class Event
	{
	public:
		Event(EventType type);
		Event(EventType type, View* sender);
		virtual ~Event();

		EventType type() const { return type_; }

		void StopPropagation();
		bool stopped_propagation() const { return !is_propagation_; }

		void SetSender(View* v);
		View* sender() const;

		template<class T>
		T* To(){
			return dynamic_cast<T*>(this);
		}

	protected:
		EventType type_;
		bool is_propagation_{ true }; //是否继续冒泡
		View* sender_;//消息来源
	};

	typedef std::function<void(View* target, Event* evt)> EventAction;
	typedef std::unordered_set<EventAction> EventActionList;
	typedef std::unordered_map<View*, std::unordered_map<EventType, EventAction>>  EventListenMap;

	Point GetMousePosition(Widget* v);
	int GetMouseKeyFlags();

}