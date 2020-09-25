#include "pch.h"
#include "enemy.h"
#include "colors.h"
#include "graphics/lights.h"

std::set<Enemy*> g_activeEnemies;

void Enemy::SetDetected()
{
	static const Color enemyColor = Color::White;

	m_isDetected = true;
	//m_meshRenderer->SetAlbedoTexture(m_detectedTexture);
	m_gameObject->GetMeshRenderer()->SetEmission(enemyColor);

	//Create a new game object for light.
	GameObject* enemyLight = new GameObject();
	enemyLight->SetParent(m_gameObject);
	enemyLight->SetPosition(math::Vector3(0, 0.5f, 0));
	enemyLight->AddComponent<LightComponent>(enemyColor, LightType::Point, 7.0f, 10.0f);
}