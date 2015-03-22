#pragma once
#include "glm\glm.hpp"
#include "ArcBallMatrix.h"

class CArcBall
{
private:
	const float m_fEpsilon = 1.0e-5f;
	glm::vec3 m_vClickVector;
	glm::vec3 m_vDragVector;
	float m_fWidth;
	float m_fHeight;
	glm::vec3 mapToSphere(glm::ivec2 point);
protected:
	CArcBallMatrix m_mLastTranf;
	CArcBallMatrix m_mThisTranf;
	glm::ivec2 m_MouseStart;
public:
	CArcBall();
	~CArcBall();
	void Resize(float NewWidth, float NewHeight);
	void Reset();
	const glm::mat4 & GetTransformation(){ return m_mThisTranf.GetMatrix(); }
	void OnMouseDown(glm::ivec2 point);
	void OnMouseMove(glm::ivec2 point);
	void OnMouseUp(glm::ivec2 point);
};

