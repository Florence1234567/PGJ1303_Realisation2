#include "MeshScene.h"
#include <glm/ext/matrix_clip_space.hpp>
#include "EnvironmentObject.h"
#include "BillboardObject.h"
#include "OpenGlUtils.h"

CMeshScene::CMeshScene()
{
	std::vector<std::string> fbxPaths = {
	    "./models/hills01.fbx",           // hill
	    "./models/tent_red.fbx",          // tent
	    "./models/tree01.fbx",            // tree
	    "./models/flower02_orange.fbx",   // flower orange
	    "./models/mushrooom01_brown.fbx", // mushroom
	    "./models/rock_big01.fbx",        // rock
	    "./models/bush_berries_blue.fbx", // bush blue
	    "./models/bush_berries_red.fbx",  // bush red
	    "./models/flower02_pink.fbx",     // flower pink
	    "./models/flower02_yellow.fbx",   // flower yellow
	    "./models/grass01.fbx",           // grass 1
	    "./models/grass03.fbx",           // grass 2
	    "./models/grass01.fbx",           // extra grass 3
	    "./models/grass03.fbx",           // extra grass 4
	    "./models/flower02_pink.fbx",     // extra flower pink 2
	    "./models/flower02_yellow.fbx"    // extra flower yellow 2
	};

	std::vector<glm::vec3> positions = {
	    {0.0f, 0.0f, 0.0f},   // hill
	    {6.5f, 4.6f, -5.0f},  // tent
	    {9.0f, 3.8f, 6.0f},   // tree
	    {-4.0f, 4.4f, 8.0f},  // flower orange
	    {2.0f, 4.3f, -3.5f},  // mushroom
	    {-6.0f, 4.4f, -5.5f}, // rock
	    {-8.0f, 4.6f, 9.0f},  // bush blue
	    {7.0f, 4.5f, 9.5f},   // bush red
	    {-3.0f, 4.3f, 10.0f}, // flower pink
	    {8.5f, 4.3f, -9.0f},  // flower yellow
	    {2.5f, 4.4f, 6.5f},   // grass 1
	    {5.5f, 4.3f, 2.0f},   // grass 2
	    {-5.0f, 4.2f, 3.5f},  // grass 3 (new)
	    {4.0f, 4.2f, 8.0f},   // grass 4 (new)
	    {-7.0f, 4.3f, 5.0f},  // flower pink 2 (new)
	    {6.5f, 4.3f, -7.5f}   // flower yellow 2 (new)
	};

	std::vector<glm::vec3> scales = {
	    {0.02f, 0.02f, 0.02f},    // hill
	    {0.006f, 0.006f, 0.006f}, // tent
	    {0.016f, 0.022f, 0.016f}, // tree (taller)
	    {0.013f, 0.013f, 0.013f}, // flower orange
	    {0.010f, 0.010f, 0.010f}, // mushroom
	    {0.006f, 0.006f, 0.006f}, // rock (small)
	    {0.007f, 0.007f, 0.007f}, // bush blue
	    {0.007f, 0.007f, 0.007f}, // bush red
	    {0.013f, 0.013f, 0.013f}, // flower pink
	    {0.013f, 0.013f, 0.013f}, // flower yellow
	    {0.012f, 0.012f, 0.012f}, // grass 1
	    {0.012f, 0.012f, 0.012f}, // grass 2
	    {0.012f, 0.012f, 0.012f}, // grass 3
	    {0.012f, 0.012f, 0.012f}, // grass 4
	    {0.012f, 0.012f, 0.012f}, // flower pink 2
	    {0.012f, 0.012f, 0.012f}  // flower yellow 2
	};

	for(size_t i = 0; i < fbxPaths.size(); ++i)
	{
		auto object = std::make_shared<CEnvironmentObject>();
		object->Init(fbxPaths[i], "./textures/colormap.png");
		object->SetScale(scales[i]);
		object->SetPosition(positions[i]);
		object->SetRotation(glm::vec3(glm::radians(270.0f), 0.0f, 0.0f));
		m_drawer.AddObject(object);
	}


	for(size_t i = 0; i < fbxPaths.size(); ++i)
	{
		auto object = std::make_shared<CEnvironmentObject>();
		object->Init(fbxPaths[i], "./textures/colormap.png");
		object->SetScale(scales[i]);
		object->SetPosition(positions[i]);
		object->SetRotation(glm::vec3(glm::radians(270.0f), 0.0f, 0.0f));

		m_drawer.AddObject(object);
	}

	{
		auto object = std::make_shared<CBillboardObject>();
		object->SetIsTransparent(true);
		object->SetScale(glm::vec3(0.10f));
		object->SetPosition(glm::vec3(0.00, 0.25, 0.08f));
		object->SetRotation(glm::quat(glm::vec3(0, glm::quarter_pi<float>(), 0)));
		object->SetTexture(OpenGl::LoadTextureFromFile("./textures/badger.png"));
		m_drawer.AddObject(object);
	}

	{
		auto object = std::make_shared<CBillboardObject>();
		object->SetIsTransparent(true);
		object->SetScale(glm::vec3(0.10f, 0.30f, 0.10f));
		object->SetTexture(OpenGl::LoadTextureFromFile("./textures/palmier.png"));
		object->SetPosition(glm::vec3(0.10, 0.45, 0.10));
		m_drawer.AddObject(object);
	}
}

void CMeshScene::Update(double dt)
{
	CScene::Update(dt);

	auto mousePosition = m_inputProvider->GetMousePosition();
	if(m_lastMousePosition.x != DBL_MAX)
	{
		glm::dvec2 delta = mousePosition - m_lastMousePosition;

		m_cameraAngle += glm::vec2(delta) * 0.0015f;
		m_cameraAngle.y = glm::clamp(m_cameraAngle.y, -glm::half_pi<float>(), glm::half_pi<float>());
	}
	m_lastMousePosition = mousePosition;

	float cosPitch = cos(-m_cameraAngle.y);
	float sinPitch = sin(-m_cameraAngle.y);
	float cosYaw = cos(-m_cameraAngle.x);
	float sinYaw = sin(-m_cameraAngle.x);

	glm::vec3 xaxis = {cosYaw, 0, -sinYaw};
	glm::vec3 yaxis = {sinYaw * sinPitch, cosPitch, cosYaw * sinPitch};
	glm::vec3 zaxis = {sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw};

	if(m_inputProvider->IsKeyPressed(IInputProvider::KEY_FRONT))
	{
		m_cameraPosition -= zaxis * 0.01f;
	}
	if(m_inputProvider->IsKeyPressed(IInputProvider::KEY_BACK))
	{
		m_cameraPosition += zaxis * 0.01f;
	}
	if(m_inputProvider->IsKeyPressed(IInputProvider::KEY_LEFT))
	{
		m_cameraPosition -= xaxis * 0.01f;
	}
	if(m_inputProvider->IsKeyPressed(IInputProvider::KEY_RIGHT))
	{
		m_cameraPosition += xaxis * 0.01f;
	}

	m_viewParams.viewMatrix = {
	    glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0),
	    glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0),
	    glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0),
	    glm::vec4(-glm::dot(xaxis, m_cameraPosition), -glm::dot(yaxis, m_cameraPosition), -glm::dot(zaxis, m_cameraPosition), 1)};

	float aspectRatio = static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight);
	m_viewParams.projMatrix = glm::perspective(glm::pi<float>() * 0.25f, aspectRatio, 0.1f, 100000.f);

	m_drawer.Update(dt);
}

void CMeshScene::Draw()
{
	glViewport(0, 0, m_windowWidth, m_windowHeight);

	glDepthMask(GL_TRUE);
	glClearDepthf(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_drawer.Draw(m_viewParams);
}
