#include <wipch.h>

#include "SystemScriptClass.h"
#include "ScriptUtils.h"
#include "ScriptEngine.h"

#include <mono/metadata/object.h>

namespace Wiwa {
	SystemScriptClass::~SystemScriptClass()
	{

	}

	SystemScriptClass::SystemScriptClass(MonoAssembly* assembly, const std::string& classNamespace, const std::string& className) :
		scriptClass(assembly, classNamespace, className)
	{
		m_SystemObject = scriptClass.Instantiate();

		m_AwakeMethod = scriptClass.GetMethod("Awake", 0);
		m_InitMethod = scriptClass.GetMethod("Init", 0);
		m_UpdateMethod = scriptClass.GetMethod("Update", 0);
		
		m_OnCollisionEnterMethod = scriptClass.GetMethod("OnCollisionEnter", 2);
		m_OnCollisionMethod = scriptClass.GetMethod("OnCollision", 2);
		m_OnCollisionExitMethod = scriptClass.GetMethod("OnCollisionExit", 2);

		m_EntityIdField = scriptClass.GetField("m_EntityId");
	}

	void SystemScriptClass::OnAwake()
	{
		if (!m_AwakeMethod) return;

		scriptClass.InvokeMethod(m_SystemObject, m_AwakeMethod, NULL);
	}

	void SystemScriptClass::OnInit()
	{
		if (!m_InitMethod) return;

		scriptClass.InvokeMethod(m_SystemObject, m_InitMethod, NULL);
	}

	void SystemScriptClass::OnUpdate()
	{
		if (!m_UpdateMethod) return;

		scriptClass.InvokeMethod(m_SystemObject, m_UpdateMethod, NULL);
	}

	void SystemScriptClass::OnCollisionEnter(Object* obj1, Object* obj2)
	{
		if (!m_OnCollisionEnterMethod) return;

		void* params[] = {
			&obj1->id,
			&obj2->id
		};

		scriptClass.InvokeMethod(m_SystemObject, m_OnCollisionEnterMethod, params);
	}

	void SystemScriptClass::OnCollision(Object* obj1, Object* obj2)
	{
		if (!m_OnCollisionMethod) return;

		void* params[] = {
			&obj1->id,
			&obj2->id
		};

		scriptClass.InvokeMethod(m_SystemObject, m_OnCollisionEnterMethod, params);
	}

	void SystemScriptClass::OnCollisionExit(Object* obj1, Object* obj2)
	{
		if (!m_OnCollisionExitMethod) return;

		void* params[] = {
			&obj1->id,
			&obj2->id
		};

		scriptClass.InvokeMethod(m_SystemObject, m_OnCollisionEnterMethod, params);
	}

	void SystemScriptClass::OnEntitySet()
	{
		scriptClass.SetFieldValue(m_SystemObject, m_EntityIdField, &m_EntityId);
	}
}