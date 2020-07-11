#include "hzpch.h"
#include "Boat.h"

#include "Hazel/Input.h"
#include "Hazel/Keycodes.h"


namespace Hazel
{
	Boat::Boat()
	{
		SetFloaters();
	}

	void Boat::Render()
	{
		Renderer::Submit(this->GetShader(), this->GetModel()->GetVA(), this->GetModel()->GetTexture(), m_Transform);
	}

	void Boat::Render(glm::mat4& transform)
	{
		Renderer::Submit(this->GetShader(), this->GetModel()->GetVA(), this->GetModel()->GetTexture(), transform);
	}

	void Boat::OnUpdate(Timestep ts, WaterPhysicsEngine& WPE)
	{
		glm::vec4 TF = WPE.FloatingObjectTorque(m_Floaters, m_nFloaters, m_Transform);
		//m_Position.x += m_Velocity * ts * m_Direction.x;
		//m_Position.z += m_Velocity * ts * m_Direction.z;
		m_VelocityVertical = m_VelocityVertical * 0.95 + TF.w * ts / m_Weight;
		m_Position.y += m_VelocityVertical * ts;
		HZ_CORE_INFO("m_VelocityVertical {0}", m_VelocityVertical);
		//m_Normal.x -= (m_Normal.x - TF.x) / 5;
		//m_Normal.z -= (m_Normal.z - TF.z) / 0.5;

		m_Transform = glm::inverse(glm::lookAt(m_Position, m_Position + m_Direction, glm::normalize(m_Normal)));
	}

	void Boat::SetFloaters()
	{
		m_Floaters = new Floater[9];
		for (int x = 0;x < 3;x++)
		{
			for (int y = 0;y < 3;y++)
			{
				m_Floaters[3 * x + y].x = (x - 1) * 1/2;
				m_Floaters[3 * x + y].z = (y - 1) * 1/2;
				m_Floaters[3 * x + y].y = sqrtf(m_Floaters[3 * x + y].x * m_Floaters[3 * x + y].x + m_Floaters[3 * x + y].z * m_Floaters[3 * x + y].z) / 10;
				m_Floaters[3 * x + y].w = 1;
			}
		}
		m_nFloaters = 9;
	}

	void Boat::Steer(bool ccw)
	{
		if (ccw)
		{
			m_DirectionAngle += m_Maneuverability;
		}
		else
		{
			m_DirectionAngle -= m_Maneuverability;
		}
		float cos = glm::cos(glm::radians(m_DirectionAngle));
		float sin = glm::sin(glm::radians(m_DirectionAngle));
		glm::vec3 newDirection;
		m_Direction.x = sin;
		m_Direction.z = cos;
	}


	// BOAT CONTROLLER

	BoatController::BoatController(Boat& boat)
		: m_Boat(boat)
	{

	}

	void BoatController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(BoatController::OnKeyPressed));
	}

	bool BoatController::OnKeyPressed(KeyPressedEvent& e)
	{
		glm::vec2 temp = m_Boat.GetDirection();
		HZ_CORE_INFO("GET: m_Direction.x = {0}, m_Direction.y = {1}", temp.x, temp.y);
		if (e.GetKeyCode() == HZ_KEY_A)
		{
			m_Boat.Steer(true);
			return true;
		}
		else if (e.GetKeyCode() == HZ_KEY_D)
		{
			m_Boat.Steer(false);
			return true;
		}

		if (e.GetKeyCode() == HZ_KEY_W)
		{
			// hard coded max velocity
			m_Boat.SetVelocity(std::min(m_Boat.GetVelocity() + 0.1, 2.0));
		}
		else if (e.GetKeyCode() == HZ_KEY_S)
		{
			// hard coded min velocity
			m_Boat.SetVelocity(std::max(m_Boat.GetVelocity() - 0.1, 0.0));
		}
		return false;
	}
	
	

}