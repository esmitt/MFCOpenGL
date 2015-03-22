#include "stdafx.h"
#include "ArcBall.h"


CArcBall::CArcBall()
{
	m_fWidth = 1;
	m_fHeight = 1;
}


CArcBall::~CArcBall()
{
}

void CArcBall::Resize(float NewWidth, float NewHeight)
{
	//Set adjustment factor for width/height
	m_fWidth = 1.0f / ((NewWidth - 1.0f) * 0.5f);
	m_fHeight = 1.0f / ((NewHeight - 1.0f) * 0.5f);
}

void CArcBall::Reset()
{
	m_mLastTranf.Reset();
	m_mThisTranf.Reset();
}

glm::vec3 CArcBall::mapToSphere(glm::ivec2 point)
{
	glm::vec3 vector;
	glm::vec2 tempPoint = point;

	//Adjust point coords and scale down to range of [-1 ... 1]
	tempPoint.x = (tempPoint.x * m_fWidth) - 1.0f;
	tempPoint.y = 1.0f - (tempPoint.y * m_fHeight);

	//Compute square of the length of the vector from this point to the center
	//float length = (tempPoint.x * tempPoint.x) + (tempPoint.y * tempPoint.y);
	float length = glm::dot(tempPoint, tempPoint);
	//tempPoint

	//If the point is mapped outside the sphere... (length > radius squared)
	if (length > 1.0f)
	{
		//Compute a normalizing factor (radius / sqrt(length))
		float norm = (float)(1.0 / glm::sqrt(length));

		//Return the "normalized" vector, a point on the sphere
		vector.x = tempPoint.x * norm;
		vector.y = tempPoint.y * norm;
		vector.z = 0.0f;
	}
	//Else it's inside
	else
	{
		//Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
		vector.x = tempPoint.x;
		vector.y = tempPoint.y;
		vector.z = glm::sqrt(1.0f - length);
	}
	return vector;
}

void CArcBall::OnMouseDown(glm::ivec2 point)
{
	m_mLastTranf = m_mThisTranf;
	m_vClickVector = mapToSphere(point);
	m_MouseStart = point;
}

void CArcBall::OnMouseMove(glm::ivec2 point)
{
	m_vDragVector = mapToSphere(point);
	glm::vec3 perp = glm::cross(m_vClickVector, m_vDragVector);
	glm::vec4 vNewRot(0);
	if (glm::length(perp) > m_fEpsilon)
	{
		vNewRot.x = perp.x;
		vNewRot.y = perp.y;
		vNewRot.z = perp.z;
		vNewRot.w = glm::dot(m_vClickVector, m_vDragVector);
	}
	m_mThisTranf.SetPan(glm::vec3(0));
	m_mThisTranf.SetScale(1.0f);
	m_mThisTranf.SetRotation(vNewRot);
	m_mThisTranf.SetMatrix(m_mThisTranf.GetMatrix() * m_mLastTranf.GetMatrix());
}

void CArcBall::OnMouseUp(glm::ivec2 point)
{

}