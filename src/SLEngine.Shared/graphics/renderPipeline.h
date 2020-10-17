#pragma once

#include "lights.h"
#include "../camera.h"
#include "meshrenderer.h"
#include "texture2D.h"

namespace sle
{
	class Camera;

	namespace graphics
	{
		class MeshRenderer;
		class RenderPipeline
		{
		public:
			SLE_EXPORTS RenderPipeline();
			SLE_EXPORTS ~RenderPipeline();
			SLE_EXPORTS void LoadShader(const BYTE* vertexShader, SIZE_T vertexShaderSize, const BYTE* pixelShader, SIZE_T pixelShaderSize);
			SLE_EXPORTS void StartRender(Camera* camera);
			SLE_EXPORTS void RenderMesh(MeshRenderer const& mesh);
			SLE_EXPORTS void SetSkyboxTexture(Texture2D* skybox) { m_skyboxTexture = skybox; };

		private:
			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_sceneConstBuffer = nullptr;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_objectConstBuffer = nullptr;

			//Skybox
			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_skyboxVertexShader = nullptr;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_skyboxPixelShader = nullptr;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_skyboxInputLayout = nullptr;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_skyboxConstBuffer = nullptr;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_skyboxRasterizerState = nullptr;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_skyboxDepthStencilState = nullptr;

			GameObject* m_skyboxMeshRenerer;
			Texture2D* m_skyboxTexture = nullptr;

			Camera* m_camera;
		private:
			SLE_EXPORTS void CreateSkybox();
			SLE_EXPORTS void RenderSkybox(Camera* camera);
		};
	}
}