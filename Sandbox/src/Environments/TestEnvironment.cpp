#include "TestEnvironment.h"
#include "Hazel/Events/Event.h"

TestEnvironment::TestEnvironment()
	: Layer("Test layer"), m_CameraController()
{
	m_Boat.SetShader(m_Library.GetShader("assets/shaders/DefaultPolyShader.glsl"));
	m_Boat.SetModel(m_Library.GetModel("assets/models/BuoyValid.obj", "assets/textures/Palettes.png"));
	m_BoatController.SetBoat(m_Boat);



	m_Ocean.Generate(256, 100, 1, glm::vec2(9.01, 4.01), 6);
	c_Emissive = m_Ocean.GetColorVec4(0);
	c_Ambient = m_Ocean.GetColorVec4(1);
	c_Diffuse = m_Ocean.GetColorVec4(2);
	c_Specular = m_Ocean.GetColorVec4(3);

	m_WPE.SetOcean(&m_Ocean);
	
}

void TestEnvironment::OnUpdate(Hazel::Timestep ts)
{
	Hazel::RenderCommand::SetClearColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	Hazel::RenderCommand::Clear();
	m_Ocean.SetColorVec4(0, c_Emissive);
	m_Ocean.SetColorVec4(1, c_Ambient);
	m_Ocean.SetColorVec4(2, c_Diffuse);
	m_Ocean.SetColorVec4(3, c_Specular);
	m_Ocean.Update(ts);
	m_Boat.OnUpdate(ts, m_WPE);
	//m_CameraController.SetFocus(m_Boat.GetPosition());
	
	/*glm::mat4 boatTransform = m_Boat.GetTranform();
	glm::vec4 boatLightPos = glm::vec4(0.0, 2.0, 0.0, 0.0);
	glm::vec4 SunPos = boatTransform * boatLightPos;
	m_Ocean.SetSunPos(glm::vec3(SunPos));*/
	Hazel::Renderer::BeginScene(m_CameraController.GetCamera(), m_Ocean.GetSunPos());

	m_Ocean.Render(m_CameraController.GetCamera());
	m_CameraController.OnUpdate(ts);
	m_Boat.Render();
	Hazel::Renderer::EndScene();

}

void TestEnvironment::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Emissive", &c_Emissive[0]);
	ImGui::ColorEdit3("Ambient", &c_Ambient[0]);
	ImGui::ColorEdit3("Diffuse", &c_Diffuse[0]);
	ImGui::ColorEdit3("Specular", &c_Specular[0]);
	glm::vec3 temp = m_Ocean.GetSunPos();
	ImGui::ColorEdit3("Sun position", &temp[0]);
	m_Ocean.SetSunPos(temp);
	ImGui::End();
}

void TestEnvironment::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
	m_BoatController.OnEvent(e);
	Hazel::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(TestEnvironment::OnKeyPressed));
}

bool TestEnvironment::OnKeyPressed(Hazel::KeyPressedEvent& e)
{
	glm::vec2 wind = m_Ocean.GetWind();
	if (e.GetKeyCode() == HZ_KEY_I) wind.x = wind.x + 1;
	if (e.GetKeyCode() == HZ_KEY_K) wind.x = wind.x - 1;
	if (e.GetKeyCode() == HZ_KEY_J) wind.y = wind.y - 1;
	if (e.GetKeyCode() == HZ_KEY_L) wind.y = wind.y + 1;
	m_Ocean.SetWind(wind);
	
	//HZ_CORE_INFO("Wind: ({0}, {1})", m_Ocean.GetWind().x, m_Ocean.GetWind().y);

	float amp = m_Ocean.GetAmplitude();
	if (e.GetKeyCode() == HZ_KEY_Q) amp += 0.25;
	if (e.GetKeyCode() == HZ_KEY_E) amp -= 0.25;
	if (e.GetKeyCode() == HZ_KEY_SPACE) m_Ocean.InvDynamic();

	m_Ocean.SetAmplitude(amp);
	//HZ_CORE_INFO("Amplitude = {0}", m_Ocean.GetAmplitude());

	m_Ocean.UpdateSpectrum();
	return true;
}