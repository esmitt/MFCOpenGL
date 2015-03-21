#pragma once
class COGLBasic
{
public:
	static COGLBasic& getInstance()
	{
		static COGLBasic    instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
	const float fNCP = 0.01;
	const float fFCP = 500.f;
	const float fAngle = 45.f;
private:
	COGLBasic() {};                   // Constructor? (the {} brackets) are needed here.

	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.
	COGLBasic(COGLBasic const&) = delete;
	void operator=(COGLBasic const&) = delete;
};
