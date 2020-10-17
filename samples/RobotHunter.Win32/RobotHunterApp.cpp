#include "pch.h"
#include "RobotHunterApp.h"
#include "application.h"
#include "graphics/meshRenderer.h"
#include "graphics/renderPipeline.h"
#include "graphics/coreGraphics.h"
#include "graphics/texture2D.h"
#include "graphics/PostProcessing/Bloom.h"
#include "graphics/PostProcessing/FXAA.h"
#include "physics/rigidBody.h"
#include "audio/audioCore.h"
#include "math/random.h"
#include "input.h"
#include "colors.h"
#include "playerControllerPhysics.h"

#include "pbrPS.h"
#include "pbrVS.h"

#include <string>

#define ENEMY_COUNT 10
#define PILAR_COUNT 100
#define WORLD_X  100.0f
#define WORLD_Y  100.0f

using namespace sle;

graphics::MeshData quad;
graphics::MeshData playerCharacter;
graphics::MeshData enemyMesh;
graphics::MeshData pilarMesh;
graphics::MeshData trophyMesh;
graphics::MeshData worldMesh;

LightData dirLight;

float currentTime;

struct FilterGroup
{
	enum Enum
	{
		ePLAYER = (1 << 0),
		eENEMY = (1 << 1),
		ePILAR = (1 << 2),
		eFLOOR = (1 << 3)
	};
};

void RobotHunterApp::Startup(void)
{
	enemiesLeft = ENEMY_COUNT;

	//Create the render pipeline
	m_renderPipeline = std::make_unique<graphics::RenderPipeline>();
	m_renderPipeline->LoadShader(g_ppbrVS, sizeof(g_ppbrVS), g_ppbrPS, sizeof(g_ppbrPS));

	CreateObjects();
	CreateLights();
	CreateGUI();
	CreateCamera();

	m_player.AddComponent<PlayerControllerPhysics>(math::Vector3(0, 1, 0));
	audio::PlayAudioFile(L"audioFiles/test.wav", true);
}

bool RobotHunterApp::IsDone()
{
	return m_isDone;
}

void RobotHunterApp::Update(float deltaT, float totalTime)
{
	currentTime = totalTime;

	if (Input::IsPressed(KeyCode::Key_p))
		graphics::bloom::Enable = true;
	else if (Input::IsPressed(KeyCode::Key_c))
		graphics::bloom::Enable = false;

	if (Input::IsPressed(KeyCode::Key_f))
		graphics::fxaa::FXAAEnable = true;
	else if (Input::IsPressed(KeyCode::Key_g))
		graphics::fxaa::FXAAEnable = false;

	if (Input::IsPressed(KeyCode::Key_add))
		graphics::bloom::Exposure += 0.1f;
	else if (Input::IsPressed(KeyCode::Key_minus))
		graphics::bloom::Exposure -= 0.1f;

	if (m_firstFrame)
	{
		m_firstFrame = false;
		return;
	}

	m_isDone = Input::IsPressed(KeyCode::Key_escape) || (Input::IsPressed(KeyCode::Key_return) && enemiesLeft == 0);

	auto oldPlayerPosition = m_player.GetPosition();

	for (auto behaviours : g_activeBehaviours)
		behaviours->Update(deltaT);

	CheckForEnemyCollision();

	if (m_enemiesLeft.size() != 0)
		m_time += deltaT;
	else
	{
		m_trophy.SetRotation(m_trophy.GetLocalRotation() * math::Quaternion(math::Vector3(0, 1, 0), deltaT * 2));
		m_trophy.SetPosition(m_trophy.GetLocalPosition() + math::Vector3(0.0f, 0.8f * sin(1.5f * totalTime) * 0.005f, 0));
	}

	m_counterText->SetText(L"TOTAL TIME: " + std::to_wstring(m_time));
	m_enemiesLeftText->SetText(L" X " + std::to_wstring(enemiesLeft));
}

void RobotHunterApp::FixedUpdate(float deltaT, float totalTime)
{
	for (auto behaviours : g_activeBehaviours)
		behaviours->FixedUpdate(deltaT);
}

void RobotHunterApp::RenderScene(void)
{
	m_renderPipeline->StartRender(Camera::MainCamera());

	for (auto renderer : m_sceneMeshRenderer)
		m_renderPipeline->RenderMesh(*renderer);
}

//TODO(Sergio): Implement those things later.
void RobotHunterApp::RenderUI(void)
{
	Microsoft::WRL::ComPtr<ID2D1Image> renderTarget;

	graphics::g_d2dDeviceContext->GetTarget(renderTarget.GetAddressOf());

	if (renderTarget != nullptr)
	{
		graphics::g_d2dDeviceContext->BeginDraw();

		//Draw all GUI elements.
		for (auto guiElement : m_sceneGuiElements)
			guiElement->Draw();

		ASSERT_SUCCEEDED(graphics::g_d2dDeviceContext->EndDraw());
	}
}

void RobotHunterApp::Resize(uint32_t width, uint32_t height)
{
	auto aspectRation = static_cast<float>(width) / static_cast<float>(height);
	Camera::MainCamera()->SetPerspectiveMatrix(0.25f * math::Pi, aspectRation, 1.0f, 1000.0f);
	Camera::MainCamera()->Update();
}

void RobotHunterApp::CreateCamera()
{
	auto mainCamera = new Camera();
	math::Vector3 cameraPosition{ 0.0f, 4.0f, 10.0f };
	math::Vector3 target{ 0, 0, 0 };
	math::Vector3 up{ 0.0f, 1.0f, 0.0f };
	mainCamera->SetEyeAtUp(cameraPosition, target, up);

	auto aspectRation = static_cast<float>(graphics::g_windowWidth) / static_cast<float>(graphics::g_windowHeight);
	mainCamera->SetPerspectiveMatrix(0.25f * math::Pi, aspectRation, 1.0f, 30.0f);
	mainCamera->Update();
}

void RobotHunterApp::CreateLights()
{
	auto lightComponent = m_scenePointLight.AddComponent<LightComponent>(Color::Aqua, LightType::Point, 2.0f, 7);

	m_scenePointLight.SetParent(&m_player);
	m_scenePointLight.SetPosition(math::Vector3{ 0, 1.5f, 1.5f });

	m_sceneSpotLight.SetPosition(math::Vector3{ 0, 5.5f, -1.0f });
	lightComponent = m_sceneSpotLight.AddComponent<LightComponent>(Color::White, LightType::Point, 0.005f, 15.0f, 96.0f);
	// Spot light--position and direction changed every frame to animate in UpdateScene function.

	m_sceneSpotLight.SetParent(&m_player);
}

void RobotHunterApp::CreateObjects()
{
	graphics::MeshData::CreateGrid(100, 100, 20, 20, quad);
	graphics::MeshData::LoadFromOBJFile(L"models/littleRobot.model", playerCharacter);
	graphics::MeshData::LoadFromOBJFile(L"models/enemy.model", enemyMesh);
	graphics::MeshData::LoadFromOBJFile(L"models/trophy.model", trophyMesh);
	graphics::MeshData::CreateCylinder(1, 1, 10, 20, 20, pilarMesh);

	//TODO(Sergio): Load this on a separate thread. Asset loading will take a time...
	graphics::Texture2D normalMap = graphics::Texture2D(L"textures/neutralNormal.dds");
	graphics::Texture2D playerTexture = graphics::Texture2D(L"textures/littleRobot.dds");
	graphics::Texture2D floorTexture = graphics::Texture2D(L"textures/checkboard_mips.dds");
	graphics::Texture2D floorNormalMap = graphics::Texture2D(L"textures/tile_nmap.dds");
	graphics::Texture2D enemyTexture = graphics::Texture2D(L"textures/enemy.dds");
	graphics::Texture2D enemyDetectedTexture = graphics::Texture2D(L"textures/enemy_inv.dds");
	graphics::Texture2D pilarTexture = graphics::Texture2D(L"textures/checkboard_mips.dds");
	graphics::Texture2D pilarNormal = graphics::Texture2D(L"textures/tile_nmap.dds");
	graphics::Texture2D defaultEmissionMap = graphics::Texture2D(L"textures/defaultEmissionMap.dds");
	graphics::Texture2D playerEmissionMap = graphics::Texture2D(L"textures/newEmissionMap.dds");

	graphics::Material playerMaterial{};
	playerMaterial.Metalness = 0.2f;
	playerMaterial.Roughness = 0.2f;
	playerMaterial.Color = Color::White;

	m_player.SetPosition(math::Vector3(0, 1, 0));
	m_player.SetName(L"Player");
	auto playerRenderer = m_player.AddMeshRenderer(&playerCharacter, playerMaterial);
	auto playerRigidBody = m_player.AddComponent<physics::RigidBody>(10.0f, false);
	playerRigidBody->AddBoxCollider(2, 1, 1, math::Vector3(0, 1.0f, 0));
	playerRigidBody->IsKinematic(false);
	playerRigidBody->SetCollisionFilters(FilterGroup::Enum::ePLAYER, FilterGroup::Enum::eENEMY);
	playerRenderer->SetAlbedoTexture(playerTexture);
	playerRenderer->SetNormalMap(normalMap);
	playerRenderer->SetEmissionMap(playerEmissionMap);
	m_sceneMeshRenderer.push_back(playerRenderer);

	graphics::Material trophyMaterial{};
	trophyMaterial.Metalness = 0.9f;
	trophyMaterial.Roughness = 0.7f;
	trophyMaterial.Color = Color::White;

	m_trophy.SetName(L"Trophy");
	m_trophy.SetParent(&m_player);
	m_trophy.SetPosition(math::Vector3(0, 1.2f, 0));
	m_trophy.SetScale(math::Vector3(0.5f, 0.5f, 0.5f));

	auto trophyRenderer = m_trophy.AddMeshRenderer(&trophyMesh, trophyMaterial);
	trophyRenderer->SetAlbedoTexture(playerTexture);
	trophyRenderer->SetNormalMap(normalMap);
	trophyRenderer->SetNormalMap(normalMap);
	trophyRenderer->SetEmissionMap(defaultEmissionMap);

	CreateEnemy(&enemyMesh, enemyTexture, normalMap, enemyDetectedTexture, defaultEmissionMap);
	CreatePilars(&pilarMesh, pilarTexture, pilarNormal, defaultEmissionMap);

	graphics::Material floorMaterial{};
	floorMaterial.Color = Color::White;
	floorMaterial.Metalness = 0.3f;
	floorMaterial.Roughness = 0.1f;

	m_floor.SetName(L"Floor");
	auto floorRenderer = m_floor.AddMeshRenderer(&quad, floorMaterial);
	floorRenderer->SetAlbedoTexture(floorTexture);
	floorRenderer->SetNormalMap(floorNormalMap);
	floorRenderer->SetEmissionMap(defaultEmissionMap);
	floorRenderer->SetTextureScale(20, 20);

	auto floorRigidBodyComponent = m_floor.AddComponent<physics::RigidBody>(10, true);
	floorRigidBodyComponent->AddPlaneCollider(math::Vector3(0, 0, 0), math::Vector3(0, 1, 0));
	//floorRigidBodyComponent->SetCollisionFilters(FilterGroup::Enum::eFLOOR, FilterGroup::Enum::ePLAYER);

	//Invisible walls.
	auto wall1 = new GameObject();
	auto wall1RigidBodyComponent = wall1->AddComponent<physics::RigidBody>(10, true);
	wall1RigidBodyComponent->AddPlaneCollider(math::Vector3(50, 0, 0), math::Vector3(-1, 0, 0));

	auto wall2 = new GameObject();
	auto wall2RigidBodyComponent = wall2->AddComponent<physics::RigidBody>(10, true);
	wall2RigidBodyComponent->AddPlaneCollider(math::Vector3(-50, 0, 0), math::Vector3(1, 0, 0));

	auto wall3 = new GameObject();
	auto wall3RigidBodyComponent = wall3->AddComponent<physics::RigidBody>(10, true);
	wall3RigidBodyComponent->AddPlaneCollider(math::Vector3(0, 0, 50), math::Vector3(0, 0, -1));

	auto wall4 = new GameObject();
	auto wall4RigidBodyComponent = wall4->AddComponent<physics::RigidBody>(10, true);
	wall4RigidBodyComponent->AddPlaneCollider(math::Vector3(0, 0, -50), math::Vector3(0, 0, 1));

	m_sceneMeshRenderer.push_back(floorRenderer);
}

void RobotHunterApp::CreateEnemy(graphics::MeshData* enemyData, graphics::Texture2D& enemyTexture, graphics::Texture2D& enemyNormal, graphics::Texture2D& detectedTexture, graphics::Texture2D& emissionMap)
{
	graphics::Material material1{};
	material1.Color = Color::White;
	material1.Metalness = 0.2f;
	material1.Roughness = 0.1f;

	math::RandomNumberGenerator random{};

	for (size_t i = 0; i < ENEMY_COUNT; i++)
	{
		float randomX = random.NextFloat();
		float randomY = random.NextFloat();

		float enemyX = (randomX * WORLD_X) - ((WORLD_X) / 2.0f);
		float enemyY = (randomY * WORLD_Y) - ((WORLD_Y) / 2.0f);
		auto enemyPosition = math::Vector3(enemyX, 0, enemyY);

		auto enemyGO = new GameObject(enemyPosition, math::Quaternion());
		enemyGO->SetName(L"Enemy_" + std::to_wstring(i));

		auto enemyRigidBody = enemyGO->AddComponent<physics::RigidBody>(1.0f, false);
		enemyRigidBody->AddBoxCollider(2, 0.7f, 0.7f, math::Vector3(0, 1.0f, 0));
		enemyRigidBody->SetCollisionFilters(FilterGroup::Enum::eENEMY, FilterGroup::Enum::ePLAYER);

		auto enemyRenderer = enemyGO->AddMeshRenderer(enemyData, material1);
		enemyRenderer->SetAlbedoTexture(enemyTexture);
		enemyRenderer->SetNormalMap(enemyNormal);
		enemyRenderer->SetEmissionMap(emissionMap);
		m_sceneMeshRenderer.push_back(enemyRenderer);
		enemyGO->AddComponent<Enemy>(detectedTexture);

		m_enemiesLeft.push_back(std::move(enemyGO));
	}
}

void RobotHunterApp::CreatePilars(graphics::MeshData* pilarData, graphics::Texture2D& pilarTexture, graphics::Texture2D& pilarNormal, graphics::Texture2D& emissionMap)
{
	graphics::Material material1{};
	material1.Color = Color::White;
	material1.Metalness = 0.3f;
	material1.Roughness = 0.1f;

	math::RandomNumberGenerator random{};

	for (size_t i = 0; i < PILAR_COUNT; i++)
	{
		float pilarX = 0.0f;
		float pilarY = 0.0f;

		math::Vector3 distance;

		auto pilar = new GameObject(math::Vector3(0, 5, 0), math::Quaternion());
		pilar->SetName(L"Pilar");

		auto pilarRenderer = pilar->AddMeshRenderer(pilarData, material1);
		pilarRenderer->SetAlbedoTexture(pilarTexture);
		pilarRenderer->SetNormalMap(pilarNormal);
		pilarRenderer->SetTextureScale(3, 5);
		pilarRenderer->SetEmissionMap(emissionMap);

		bool overlapingEnemy = false;

		do
		{
			overlapingEnemy = false;

			pilarX = (random.NextFloat() * (WORLD_X - 3.0f)) - ((WORLD_X - 3.0f) / 2.0f);
			pilarY = (random.NextFloat() * (WORLD_Y - 3.0f)) - ((WORLD_Y - 3.0f) / 2.0f);

			pilar->SetPosition(math::Vector3(pilarX, 5, pilarY));
			auto pilarBound = pilarRenderer->WBoudingBox();

			overlapingEnemy = pilarBound.IsOverlaping(m_player.GetMeshRenderer()->WBoudingBox());

			for (GameObject* enemy : m_enemiesLeft)
				overlapingEnemy = overlapingEnemy || enemy->GetMeshRenderer()->WBoudingBox().IsOverlaping(pilarBound);

			for (auto otherPilar : m_pilars)
				overlapingEnemy = overlapingEnemy || otherPilar->WBoudingBox().IsOverlaping(pilarBound);
		} while (math::Length(distance) <= 10.0f && overlapingEnemy);

		auto pilarRigidBody = pilar->AddComponent<physics::RigidBody>(10.0f, true);
		pilarRigidBody->AddCapsuleCollider(1, 10);
		pilarRigidBody->SetCollisionFilters(FilterGroup::Enum::ePILAR, FilterGroup::Enum::ePLAYER);

		m_sceneMeshRenderer.push_back(pilarRenderer);
		m_pilars.push_back(pilarRenderer);
	}
}

void RobotHunterApp::CreateGUI()
{
	m_informationText = new graphics::UI::GuiText(nullptr, 10, 10, 500, 100, 30);

	m_informationText->SetText(std::to_wstring(enemiesLeft) + L" evil robots are lost in the room, find all of them! How fast can you do it?");
	m_informationText->SetColor(D2D1::ColorF::White);

	m_sceneGuiElements.push_back(m_informationText);

	m_counterText = new graphics::UI::GuiText(nullptr, -500, 0, 500, 100, 20);
	m_counterText->SetColor(D2D1::ColorF::Yellow);
	m_counterText->SetAnchorType(graphics::UI::ParentAnchorType::TopRight);
	m_counterText->SetText(L"Time left: 30");

	m_sceneGuiElements.push_back(m_counterText);

	auto scorePanel = new graphics::UI::GuiPanel(nullptr, -200, 30, 170, 100);
	scorePanel->SetColor(Color{ Color::White, 0.5f });
	scorePanel->SetAnchorType(graphics::UI::ParentAnchorType::TopRight);
	m_sceneGuiElements.push_back(scorePanel);

	m_enemiesLeftText = new graphics::UI::GuiText(scorePanel, 80, 0, 300, 100, 30);
	m_enemiesLeftText->SetColor(D2D1::ColorF::Purple);
	m_enemiesLeftText->SetAnchorType(graphics::UI::ParentAnchorType::TopLeft);
	m_enemiesLeftText->SetText(L" X " + std::to_wstring(enemiesLeft));

	m_sceneGuiElements.push_back(m_enemiesLeftText);

	auto enemy_icon = new graphics::UI::GuiSprite(m_enemiesLeftText, -60, 20, 60, 60);
	enemy_icon->LoadBitmapFromFile(L"icons/enemy_icon.png", false);
	m_sceneGuiElements.push_back(enemy_icon);
}

void RobotHunterApp::CheckForEnemyCollision()
{
	auto it = m_enemiesLeft.begin();

	auto playerRigidBody = m_player.GetComponent<physics::RigidBody>();

	while (it != m_enemiesLeft.end())
	{
		GameObject* gameObject = *it;
		Enemy* enemy = gameObject->GetComponent<Enemy>();

		if (enemy->IsDetected())
		{
			it = m_enemiesLeft.erase(it);
			enemiesLeft--;

			if (enemiesLeft == 0)
			{
				m_sceneGuiElements.clear();

				auto congratulationsSprite = new graphics::UI::GuiSprite(nullptr, 0, 0, 0, 0);
				congratulationsSprite->LoadBitmapFromFile(L"icons/trophy.png", true);

				float imageYPosition = graphics::g_windowHeight - congratulationsSprite->GetHeight();

				m_counterText->SetFontSize(100);

				m_sceneGuiElements.push_back(m_counterText);

				congratulationsSprite->SetLocalPosition(0, imageYPosition / 2.0f);

				m_sceneGuiElements.push_back(congratulationsSprite);

				auto meshRenderer = m_trophy.GetMeshRenderer();
				m_sceneMeshRenderer.push_back(meshRenderer);

				auto pressEnterText = new graphics::UI::GuiText(nullptr, 0.0f, 0.0f, static_cast<float>(graphics::g_windowWidth), -200.0f, 40.0f);
				pressEnterText->SetColor(Color::White);
				pressEnterText->SetAnchorType(graphics::UI::ParentAnchorType::BottomLeft);
				pressEnterText->SetText(L"Press enter to EXIT");
				pressEnterText->SetTextAlignment(TextAlignment::Center);
				m_sceneGuiElements.push_back(pressEnterText);
			}
		}
		else
		{
			++it;
		}
	}
}

bool RobotHunterApp::CheckIfInsideScene()
{
	return m_floor.GetMeshRenderer()->WBoudingBox().IsPointInside(m_player.GetPosition());
}

void RobotHunterApp::Cleanup(void)
{
}