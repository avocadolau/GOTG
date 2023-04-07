#pragma once
#include <string>
namespace Wiwa {
	enum class ActivationRules {
		ACTIVE_IF_GREATER_THAN,
		ACTIVE_IF_LESS_THAN,
		ACTIVE_IF_EQUALS_THAN,
		NONE
	};

	class Property {
	public:
		Property();
		Property(std::string name,std::string description, int initialValue, ActivationRules activation, int activationvalue);
		~Property();

		bool IsActive();

		std::string m_Name;
		std::string m_Description;
		ActivationRules m_Activation;
		int m_Value;
		int m_ActivationValue;
		int m_InitialValue;
	};
}
