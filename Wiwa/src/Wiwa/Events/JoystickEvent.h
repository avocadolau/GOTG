#pragma once

#include "Event.h"
#include <Wiwa/core/KeyCodes.h>

namespace Wiwa {
	class WI_API JoystickEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return m_Axis; }
		inline float GetValue() const { return m_Value; }

		EVENT_CLASS_CATEGORY(EventCategoryJoystickAxis | EventCategoryInput)
	protected:
		JoystickEvent(JoystickAxis axis, float value) : m_Axis(axis), m_Value(value) {}

		JoystickAxis m_Axis;
		float m_Value;
	};
}
