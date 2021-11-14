#ifndef _NEW_GRAPHICS_H_
#define _NEW_GRAPHICS_H_

#include "Graphics.h"

class NewGarphics : public Graphics
{
	// Variables
private:

	std::vector<mesh> shapes;			// figures;

	float scale;						// For scaling
	float _x, _y, _z;					// For Moving
	float fThetaX, fThetaY, fThetaZ;

	mat4x4 matProj;						// Matrix that converts from view space to screen space

	fPoint3D light;
	fPoint3D barycenter;

	// Overrided methods
private:
	virtual void OnUserCreate() override;
	virtual void OnUserUpdate(float fElapsedTime) override;
};

#endif // !_NEW_GRAPHICS_H_
