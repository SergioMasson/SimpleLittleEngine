#pragma once

#include "graphics/meshRenderer.h"
#include "gameObject.h"

class Enemy;
extern std::set<Enemy*> g_activeEnemies;

class Enemy : public BehaviourComponent
{
private:
	Enemy(GameObject* gameObject, graphics::Texture2D* texture) :
		BehaviourComponent{ gameObject },
		m_detectedTexture{ texture },
		m_isDetected{ false }
	{
		g_activeEnemies.insert(this);
	}

	~Enemy()
	{
		g_activeEnemies.erase(this);
	}

public:
	void SetDetected();
	bool IsDetected() { return m_isDetected; }
	void OnCollision(physics::RigidBody* other) override;
	void FixedUpdate(float detalT) override;
	math::Vector3 GetPosition() { return m_gameObject->GetPosition(); }
	math::BoudingBox WBoudingBox() const { return m_gameObject->GetMeshRenderer()->WBoudingBox(); }

private:
	graphics::Texture2D* m_detectedTexture;
	bool m_isDetected;

	friend GameObject;
};
