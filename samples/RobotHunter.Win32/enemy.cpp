#include "pch.h"
#include "enemy.h"
#include "colors.h"
#include "graphics/lights.h"
#include "audio/audioCore.h"
#include "math/random.h"
#include "physics/rigidBody.h"

using namespace sle;

std::set<Enemy*> g_activeEnemies;

constexpr int ColorCount = 10;

void Enemy::SetDetected()
{
	sle::Color enemyColor = sle::Color(0xE500FF);

	m_isDetected = true;
	m_gameObject.GetMeshRenderer()->SetEmission(enemyColor);

	//Create a new game object for light.
	sle::GameObject* enemyLight = new sle::GameObject();
	enemyLight->SetParent(&m_gameObject);
	enemyLight->SetPosition(sle::math::Vector3(0, 0.5f, 0));
	enemyLight->AddComponent<sle::LightComponent>(enemyColor, sle::LightType::Point, 7.0f, 10.0f);
	sle::audio::PlayAudioFile(L"audioFiles/enemy.wav");
}

void Enemy::OnCollision(sle::physics::RigidBody* other)
{
	if (!m_isDetected)
		SetDetected();
}

void Enemy::FixedUpdate(float detalT)
{
}