#include "MeshScene.h"
#include <glm/ext/matrix_clip_space.hpp>
#include "EnvironmentObject.h"
#include "BillboardObject.h"
#include "OpenGlUtils.h"

CMeshScene::CMeshScene()
{
	std::vector<std::string> fbxPaths = {
	    "./models/hills01.fbx",
	    "./models/tent_red.fbx",
	    "./models/tree01.fbx",
	    "./models/tree01.fbx", 
	    "./models/tree01.fbx",
	    "./models/flower02_orange.fbx",
	    "./models/mushrooom01_brown.fbx",
	    "./models/rock_big01.fbx",
	    "./models/mushrooom01_red.fbx", 
	    "./models/bush_berries_blue.fbx",
	    "./models/bush_berries_red.fbx",
	    "./models/bush_berries_blue.fbx", 
	    "./models/flower02_pink.fbx",
	    "./models/flower02_yellow.fbx",
	    "./models/flower02_pink.fbx",
	    "./models/flower02_yellow.fbx",
	    "./models/grass01.fbx",
	    "./models/grass03.fbx",
	    "./models/grass01.fbx",
	    "./models/grass03.fbx",
	    "./models/grass01.fbx", 
	    "./models/grass03.fbx" 
	};

	std::vector<glm::vec3> positions = {
	    {0.0f, 0.0f, 0.0f},   // hill
	    {0.5f, 5.5f, 0.0f},  // tent
	    {9.0f, 0.8f, 14.0f},   // tree 1
	    {-11.0f, 3.0f, 7.5f}, // tree 2
	    {12.5f, 0.4f, -15.5f}, // tree 3
	    {-16.0f, 1.3f, 8.0f},  // flower orange
	    {-6.0f, 4.8f, -3.5f},  // mushroom
	    {-6.2f, 1.1f, -15.5f}, // rock 1
	    {10.5f, 3.2f, -9.0f}, // rock 2
	    {-8.0f, 3.9f, 9.0f},  // bush blue 1
	    {7.0f, 3.8f, -3.5f},   // bush red 1
	    {9.0f, 2.7f, 4.5f}, // bush blue 2
	    {-3.0f, 2.5f, -10.0f}, // flower pink 1
	    {-10.5f, 2.8f, -8.0f},  // flower yellow 1
	    {-7.0f, 1.1f, 15.0f},  // flower pink 2
	    {16.5f, 0.8f, -2.5f},  // flower yellow 2
	    {13.5f, 1.7f, 6.5f},   // grass 1
	    {-5.5f, 3.0f, 12.0f},   // grass 2
	    {-5.0f, 4.2f, 3.5f},  // grass 3
	    {4.0f, 3.8f, 8.0f},   // grass 4
	    {-9.5f, 2.8f, -4.0f}, // grass 5 (extra)
	    {8.0f, 2.1f, 10.5f}   // grass 6 (extra)
	};

	std::vector<glm::vec3> scales = {
	    {0.02f, 0.02f, 0.02f},    // hill
	    {0.012f, 0.012f, 0.012f}, // tent
	    {0.016f, 0.022f, 0.016f}, // tree 1 (taller)
	    {0.017f, 0.024f, 0.017f}, // tree 2
	    {0.018f, 0.025f, 0.018f}, // tree 3
	    {0.013f, 0.013f, 0.013f}, // flower orange
	    {0.010f, 0.010f, 0.010f}, // mushroom
	    {0.006f, 0.006f, 0.006f}, // rock 1
	    {0.010f, 0.010f, 0.010f}, // rock 2
	    {0.007f, 0.007f, 0.007f}, // bush blue 1
	    {0.007f, 0.007f, 0.007f}, // bush red 1
	    {0.007f, 0.007f, 0.007f}, // bush blue 2
	    {0.013f, 0.013f, 0.013f}, // flower pink 1
	    {0.013f, 0.013f, 0.013f}, // flower yellow 1
	    {0.012f, 0.012f, 0.012f}, // flower pink 2
	    {0.012f, 0.012f, 0.012f}, // flower yellow 2
	    {0.012f, 0.012f, 0.012f}, // grass 1
	    {0.012f, 0.012f, 0.012f}, // grass 2
	    {0.012f, 0.012f, 0.012f}, // grass 3
	    {0.012f, 0.012f, 0.012f}, // grass 4
	    {0.012f, 0.012f, 0.012f}, // grass 5
	    {0.012f, 0.012f, 0.012f}  // grass 6
	};

	for(size_t i = 0; i < fbxPaths.size(); ++i)
	{
		auto object = std::make_shared<CEnvironmentObject>();
		object->Init(fbxPaths[i], "./textures/colormap.png");
		object->SetScale(scales[i]);
		object->SetPosition(positions[i]);

		glm::vec3 rotation(glm::radians(270.0f), 0.0f, 0.0f);

		if(i != 0)
		{
			float randomY = glm::radians(static_cast<float>(rand() % 360));
			rotation.y = randomY;
		}

		object->SetRotation(rotation);
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

	m_cameraPosition.z += 10.0f;
	m_cameraPosition.y += 10.0f;
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
