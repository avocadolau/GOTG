#pragma once
#include <string>

#include <Wiwa/utilities/Reflection.h>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoArray MonoArray;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoMethodSignature MonoMethodSignature;
}

namespace Wiwa {
	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(MonoAssembly* assembly,const std::string& classNamespace, const std::string& className);

		MonoObject* Instantiate();

		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		int GetMethodCount();
		MonoMethod* GetMethodByName(const char* name);
		MonoMethodSignature* GetMethodSignature(MonoMethod* method);
		uint32_t GetMethodParamCount(MonoMethodSignature* signature);
		const Type* GetMethodParamType(MonoMethodSignature* signature, int idx);

		MonoClassField* GetField(const std::string& name);
		void SetFieldValue(MonoObject* instance, MonoClassField* field, void* value);

		MonoClass* m_MonoClass = nullptr;
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;
	};
}