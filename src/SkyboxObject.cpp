#include "SkyboxObject.h"
#include "OpenGlShader.h"
#include "OpenGlUtils.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cassert>

    namespace
{
	struct VERTEX
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	enum VERTEX_ATTRIBUTES
	{
		POSITION,
		NORMAL,
		TEXCOORD,
	};

	enum UNIFORM_BINDINGS
	{
		MATRICES,
	};
}

CSkyboxObject::CSkyboxObject()
{
}

void CSkyboxObject::Init(const std::string& fbx, const char* texture)
{
	Assimp::Importer importer;
	importer.ReadFile(fbx, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	auto scene = importer.GetScene();
	assert(scene && scene->HasMeshes());
	auto mesh = scene->mMeshes[0];

	std::vector<VERTEX> vertices;
	vertices.reserve(mesh->mNumVertices);
	for(int i = 0; i < mesh->mNumVertices; i++)
	{
		auto position = mesh->mVertices[i];
		aiVector3D normal = mesh->mNormals ? mesh->mNormals[i] : aiVector3D{0, 0, 0};
		aiVector3D texCoord = (mesh->mTextureCoords[0]) ? mesh->mTextureCoords[0][i] : aiVector3D{0, 0, 0};

		vertices.push_back(
		    {{position.x, -position.z, position.y},
		     {normal.x, -normal.z, normal.y},
		     {texCoord.x, 1.0f - texCoord.y}});
	}

	std::vector<uint16_t> indices;
	indices.reserve(mesh->mNumFaces * 3);
	for(int i = 0; i < mesh->mNumFaces; i++)
	{
		auto face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		for(int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(static_cast<uint16_t>(face.mIndices[j]));
		}
	}

	m_numIndices = static_cast<uint32_t>(indices.size());

	{
		m_vertexBuffer = OpenGl::CBuffer::Create();
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}

	{
		m_indexBuffer = OpenGl::CBuffer::Create();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
	}

	m_texture = OpenGl::LoadTextureFromFile(texture);

	m_matricesUniformBuffer = OpenGl::CBuffer::Create();

	{
		auto vertShader = OpenGl::CShader::CreateFromFile(GL_VERTEX_SHADER, "./shaders/skybox_v.glsl");
		auto fragShader = OpenGl::CShader::CreateFromFile(GL_FRAGMENT_SHADER, "./shaders/skybox_f.glsl");

		vertShader.Compile();
		fragShader.Compile();

		m_program = OpenGl::CProgram::Create();
		m_program.AttachShader(vertShader);
		m_program.AttachShader(fragShader);

		glBindAttribLocation(m_program, static_cast<GLuint>(VERTEX_ATTRIBUTES::POSITION), "a_position");
		glBindAttribLocation(m_program, static_cast<GLuint>(VERTEX_ATTRIBUTES::TEXCOORD), "a_texCoord");

		m_program.Link();

		{
			GLint uniformBinding = glGetUniformBlockIndex(m_program, "Matrices");
			if(uniformBinding != GL_INVALID_INDEX)
			{
				glUniformBlockBinding(m_program, uniformBinding, UNIFORM_BINDINGS::MATRICES);
			}
		}
	}

	m_vertexArray = OpenGl::CVertexArray::Create();

	{
		glBindVertexArray(m_vertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

		glEnableVertexAttribArray(VERTEX_ATTRIBUTES::POSITION);
		glVertexAttribPointer(VERTEX_ATTRIBUTES::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), reinterpret_cast<GLvoid*>(offsetof(VERTEX, position)));

		glEnableVertexAttribArray(VERTEX_ATTRIBUTES::NORMAL);
		glVertexAttribPointer(VERTEX_ATTRIBUTES::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), reinterpret_cast<GLvoid*>(offsetof(VERTEX, normal)));

		glEnableVertexAttribArray(VERTEX_ATTRIBUTES::TEXCOORD);
		glVertexAttribPointer(VERTEX_ATTRIBUTES::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), reinterpret_cast<GLvoid*>(offsetof(VERTEX, texCoord)));

		glBindVertexArray(0);
	}
}

void CSkyboxObject::Update(double dt)
{
	CSceneObject::Update(dt);
}

void CSkyboxObject::Draw(const VIEW_PARAMS& viewParams)
{
	glm::mat4 viewNoTranslation = viewParams.viewMatrix;
	viewNoTranslation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 world = m_worldMatrix;
	world[3] = glm::vec4(0.0f, 0.0f, 0.0f, world[3].w);

	m_matrices.worldMatrix = world;
	m_matrices.viewProjMatrix = viewParams.projMatrix * viewNoTranslation;

	glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(m_matrices), &m_matrices, GL_DYNAMIC_DRAW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	glDisable(GL_CULL_FACE);

	glDisable(GL_BLEND);

	glUseProgram(m_program);
	glBindBufferBase(GL_UNIFORM_BUFFER, UNIFORM_BINDINGS::MATRICES, m_matricesUniformBuffer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glBindVertexArray(m_vertexArray);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_numIndices), GL_UNSIGNED_SHORT, nullptr);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
	glUseProgram(0);
}