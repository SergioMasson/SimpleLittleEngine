#pragma once

#include "application.h"
#include "mathHelpers.h"
#include "camera.h"
#include "enemy.h"
#include "playerController.h"
#include "graphics/lights.h"
#include "graphics/meshRenderer.h"
#include "graphics/renderPipeline.h"
#include "graphics/UI/guiText.h"
#include "graphics/UI/guiPanel.h"
#include "graphics/UI/guiSprite.h"

namespace sle
{
	namespace graphics
	{
		class RenderPipeline;
		struct MeshData;
		class MeshRenderer;
		class Material;
	}
}

class RobotHunterApp : public sle::IGameApp
{
public:
	virtual void Startup(void) override;
	virtual void Cleanup(void) override;
	virtual bool IsDone(void) override;
	virtual void Update(float deltaT, float totalTime) override;
	virtual void FixedUpdate(float deltaT, float totalTime) override;
	virtual void RenderScene(void) override;
	virtual void RenderUI(void) override;
	virtual void Resize(uint32_t width, uint32_t height) override;

private:

	std::unique_ptr<sle::graphics::RenderPipeline> m_renderPipeline;
	std::vector<sle::graphics::MeshRenderer*> m_sceneMeshRenderer;
	std::vector<sle::graphics::UI::DrawableElement*> m_sceneGuiElements;
	std::vector<sle::GameObject*> m_enemiesLeft;
	std::vector<sle::graphics::MeshRenderer*> m_pilars;

	sle::graphics::UI::GuiText* m_informationText;
	sle::graphics::UI::GuiText* m_counterText;
	sle::graphics::UI::GuiText* m_enemiesLeftText;

	float m_time{ 0 };
	uint32_t enemiesLeft;

	sle::GameObject m_player;
	sle::GameObject m_floor;
	sle::GameObject m_trophy;
	sle::GameObject m_scenePointLight;
	sle::GameObject m_sceneSpotLight;

	sle::graphics::Texture2D m_skybox;
	sle::math::Vector3 mouse_position;
	bool m_isDone = false;
	bool m_firstFrame = true;

private:
	void CreateCamera();
	void CreateLights();
	void CreateObjects();
	void CreateEnemy(sle::graphics::MeshData* enemyData, sle::graphics::Texture2D& enemyTexture, sle::graphics::Texture2D& enemyNormal, sle::graphics::Texture2D& detectedTexture, sle::graphics::Texture2D& emissionMap);
	void CreatePilars(sle::graphics::MeshData* pilarData, sle::graphics::Texture2D& pilarTexture, sle::graphics::Texture2D& pilarNormal, sle::graphics::Texture2D& emissionMap);
	void CreateGUI();
	void CheckForEnemyCollision();
	bool CheckIfInsideScene();
};