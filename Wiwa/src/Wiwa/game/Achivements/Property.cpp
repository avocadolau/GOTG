#include "wipch.h"
#include "Property.h"


namespace  Wiwa {

	Property::Property()
	{
		{
			m_Name = "";
			m_Activation = ActivationRules::NONE;
			m_Value = 0;
			m_ActivationValue = 0;
			m_InitialValue = 0;
		}
	}

	Property::Property(std::string name, int initialValue, ActivationRules activation, int activationvalue)
	{
		m_Name = name;
		m_InitialValue = initialValue;
		m_Activation = activation;
		m_ActivationValue = activationvalue;
	}

	Property::~Property()
	{
	}

	bool Property::IsActive()
	{
		bool ret = false;
		switch (m_Activation)
		{
		case ActivationRules::ACTIVE_IF_GREATER_THAN:
			ret = m_Value > m_ActivationValue;
			break;
		case ActivationRules::ACTIVE_IF_LESS_THAN:
			ret = m_Value < m_ActivationValue;
			break;
		case ActivationRules::ACTIVE_IF_EQUALS_THAN:
			ret = m_Value == m_ActivationValue;
			break;
		default:
			break;
		}
		return ret;
	}

}