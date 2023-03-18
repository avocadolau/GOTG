#pragma once

#include <string>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class Callback {
	protected:
		std::string m_Name;
		size_t m_Hash;

		bool m_HasParam;
		const Type* m_ParamType;
	public:
		std::string getName() { return m_Name; }
		size_t getHash() { return m_Hash; }

		bool HasParam() { return m_HasParam; }
		size_t ParamHash() { if (m_ParamType) return m_ParamType->hash; return WI_INVALID_INDEX; }

		Callback();
		
		virtual ~Callback() = default;

		virtual void Execute(void* param=NULL){}
	};
}