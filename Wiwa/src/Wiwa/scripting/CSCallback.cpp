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

		m_HasParam = m_ScriptClass.GetMethodParamCount(signature) > 0;
		m_ParamType = m_ScriptClass.GetMethodParamType(signature, 0);

		m_Name = className;
		m_Hash = FNV1A_HASH(m_Name.c_str());
	}

	CSCallback::~CSCallback()
	{

	}

	void CSCallback::Execute(void* param)
	{
		mono_runtime_invoke(m_Execute, NULL, &param, NULL);
	}
}