#pragma once

#include <string>

namespace Wiwa {
	class Callback {
	protected:
		std::string m_Name;
		size_t m_Hash;
	public:
		std::string getName() { return m_Name; }
		size_t getHash() { return m_Hash; }

		Callback() = default;
		
		virtual ~Callback() = default;

		virtual void Execute(){}
	};
}