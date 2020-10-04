#include "pch.h"
#include "enemy.h"
#include "colors.h"
#include "graphics/lights.h"
#include "audio/audioCore.h"
#include "math/random.h"
#include "physics/rigidBody.h"

std::set<Enemy*> g_activeEnemies;

constexpr int ColorCount = 10;

void Enemy::SetDetected()
{
	auto r = math::g_RNG.NextFloat();
	auto g = math::g_RNG.NextFloat();
	auto b = math::g_RNG.NextFloat();

	//auto enemyColor = enemyColors[index];
	Color enemyColor = Color(r, g, b, 1.0f);

	m_isDetected = true;
	//m_meshRenderer->SetAlbedoTexture(m_detectedTexture);
	m_gameObject->GetMeshRenderer()->SetEmission(enemyColor);

	//Create a new game object for light.
	GameObject* enemyLight = new GameObject();
	enemyLight->SetParent(m_gameObject);
	enemyLight->SetPosition(math::Vector3(0, 0.5f, 0));
	enemyLight->AddComponent<LightComponent>(enemyColor, LightType::Point, 7.0f, 10.0f);

	audio::PlayAudioFile(L"audioFiles/enemy.wav");
}

void Enemy::OnCollision(physics::RigidBody* other)
{
	if (!m_isDetected)
		SetDetected();
}

void Enemy::FixedUpdate(float detalT)
{
}