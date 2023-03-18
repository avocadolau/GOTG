#include <wipch.h>
#include "ScriptClass.h"

#include "ScriptEngine.h"
#include "ScriptUtils.h"

#include <Wiwa/utilities/Hashing.h>

#include <mono/metadata/object.h>

#include "MonoWiwaTranslations.h"

namespace Wiwa {
	ScriptClass::ScriptClass(MonoAssembly* assembly, const std::string& classNamespace, const std::string& className)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = Utils::GetClassInAssembly(assembly, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	int ScriptClass::GetMethodCount()
	{
		return mono_class_num_methods(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethodByName(const char* name)
	{
		void* iter = NULL;
		MonoMethod* method = nullptr;
		while (method = mono_class_get_methods(m_MonoClass, &iter))
		{
			const char* method_name = mono_method_get_name(method);

			if (strcmp(name, method_name) == 0) {
				break;
			}
		}

		return method;
	}

	MonoMethodSignature* ScriptClass::GetMethodSignature(MonoMethod* method)
	{
		return mono_method_signature(method);
	}

	uint32_t ScriptClass::GetMethodParamCount(MonoMethodSignature* signature)
	{
		return mono_signature_get_param_count(signature);;
	}

	const Type* ScriptClass::GetMethodParamType(MonoMethodSignature* signature, int idx)
	{
		void* iter = NULL;
		MonoType* param = nullptr;

		int index = 0;

		const Type* paramType = NULL;

		while (param = mono_signature_get_params(signature, &iter))
		{
			if (index == idx) {
				paramType = ConvertType(param);

				break;
			}

			index++;
		}

		return paramType;
	}

	MonoClassField* ScriptClass::GetField(const std::string& name)
	{
		return mono_class_get_field_from_name(m_MonoClass, name.c_str());
	}

	void ScriptClass::SetFieldValue(MonoObject* instance, MonoClassField* field, void* value)
	{
		mono_field_set_value(instance, field, value);
	}
}