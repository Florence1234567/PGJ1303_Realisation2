#pragma once

#include "SceneObject.h"
#include "OpenGlDef.h"
#include "OpenGlResource.h"
#include "OpenGlProgram.h"
#include <string>
#include <vector>

class CSkyboxObject : public CSceneObject
{
public:
	CSkyboxObject();
	void Init(const std::string& fbx, const char* texture);
	void Update(double) override;
	void Draw(const VIEW_PARAMS&) override;

private:
	struct Matrices
	{
		glm::mat4 worldMatrix;
		glm::mat4 viewProjMatrix;
	};

	OpenGl::CBuffer m_vertexBuffer;
	OpenGl::CBuffer m_indexBuffer;
	OpenGl::CTexture m_texture;
	OpenGl::CBuffer m_matricesUniformBuffer;
	OpenGl::CVertexArray m_vertexArray;
	OpenGl::CProgram m_program;

	Matrices m_matrices = {};
	uint32_t m_numIndices = 0;
};