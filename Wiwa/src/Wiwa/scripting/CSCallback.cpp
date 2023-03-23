#include <wipch.h>
#include "CSCallback.h"

#include <mono/metadata/object.h>
#include <Wiwa/utilities/Hashing.h>

namespace Wiwa {
	CSCallback::CSCallback(MonoAssembly* assembly, const std::string& classNamespace, const std::string& className) :
		m_ScriptClass(assembly, classNamespace, className)
	{
		m_Execute = m_ScriptClass.GetMethodByName("OnExecute");

		MonoMethodSignature* signature = m_ScriptClass.GetMethodSignature(m_Execute);

		uint32_t pcount = m_ScriptClass.GetMethodParamCount(signature);

		WI_INFO("Callback [{}]", className.c_str());

		for (size_t i = 0; i < pcount; i++) {
			m_Params.push_back(m_ScriptClass.GetMethodParamType(signature, (int)i));

			WI_INFO("Param {} of type {}", i + 1, m_Params[i]->name.c_str());
		}
		
		m_Name = className;
		m_Hash = FNV1A_HASH(m_Name.c_str());
	}

	CSCallback::~CSCallback()
	{

	}

	void CSCallback::Execute(void** params)
	{
		mono_runtime_invoke(m_Execute, NULL, params, NULL);
	}
}