#include "ssph.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Sas {
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearSpace, float farSpace)
	{

		m_OrthographicSize = size;
		m_OrthographicNear = -nearSpace;
		m_OrthographicFar = farSpace;
		RecalculateProjection();
	}
	void SceneCamera::SetViewPortSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}
	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = m_OrthographicSize * 0.5f;
		m_Projection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
	}
}