#pragma once

#include <Wiwa/utilities/functions/Callback.h>
#include "ScriptClass.h"

namespace Wiwa {
	class CSCallback : public Callback {
	private:
		ScriptClass m_ScriptClass;

		MonoMethod* m_Execute;
	public:
		CSCallback() = default;
		CSCallback(MonoAssembly* assembly, const std::string& classNamespace, const std::string& className);
		~CSCallback();

		void Execute(void** params=NULL) override;
	};
}