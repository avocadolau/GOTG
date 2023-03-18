#include "wipch.h"
#include "Callback.h"

namespace Wiwa {
	Callback::Callback()
	{
		m_Name = "None";
		m_Hash = WI_INVALID_INDEX;

		m_HasParam = false;
		m_ParamType = NULL;
	}
}
