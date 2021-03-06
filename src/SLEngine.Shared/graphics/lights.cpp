#include "pch.h"
#include "lights.h"

using namespace sle;

std::set<LightComponent*> sle::g_activeLights;

LightComponent::LightComponent(GameObject& gameObject, Color color,
	LightType type,
	float intensity,
	float range,
	float spot) :
	Component(gameObject),
	m_color{ color },
	m_type{ type },
	m_intensity{ intensity },
	m_range{ range },
	m_spot{ m_spot }
{
	g_activeLights.insert(this);
}

LightComponent::~LightComponent()
{
	g_activeLights.erase(this);
}

LightData LightComponent::GetData()
{
	DirectX::XMFLOAT3 position = m_gameObject.GetPosition();
	DirectX::XMFLOAT3 direction = -math::Matrix3{ m_gameObject.GetRotation() }.GetZ();

	switch (m_type)
	{
	case Directional:
		return CreateDirectionalLight(m_color, direction, m_intensity, 0.0f);
	case Spot:
		return CreateSpotLight(m_color, direction, position, m_intensity, 0.0f, m_range, m_spot);
	case Point:
		return CreatePointLight(m_color, position, m_intensity, 0.0f, m_range);
	default:
		return CreatePointLight(m_color, position, m_intensity, 0.0f, m_range);;
	}
}