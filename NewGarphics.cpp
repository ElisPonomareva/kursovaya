#include "NewGarphics.h"

void NewGarphics::OnUserCreate()
{
	shapes.resize(2);

	shapes[0].tris =
	{
		// Основание
		{ 0.0f, 0.0f, 0.0f,    2.0f, 0.0f, 0.0f,    1.0f, 0.0f, 2.0f },

		// Перед                                                     
		{ 0.0f, 0.0f, 0.0f,    1.0f, 2.0f, 1.0f,    2.0f, 0.0f, 0.0f },

		// Право                                                       
		{ 2.0f, 0.0f, 0.0f,    1.0f, 2.0f, 1.0f,    1.0f, 0.0f, 2.0f },

		// Лево                                                    
		{ 1.0f, 0.0f, 2.0f,    1.0f, 2.0f, 1.0f,    0.0f, 0.0f, 0.0f }

	};

	shapes[1].tris =
	{
		// Перед
		{ 0.0f, 0.0f, 0.0f,    0.0f, 2.0f, 0.0f,    1.0f, 2.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 2.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// Право                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 2.0f, 0.0f,    1.0f, 2.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 2.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// Лево                                                      
		{ 0.0f, 0.0f, 0.0f,    0.0f, 2.0f, 0.0f,    1.0f, 2.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 1.0f,    1.0f, 2.0f, 1.0f },

		// Вверх                                                       
		{ 0.0f, 2.0f, 0.0f,    1.0f, 2.0f, 1.0f,    1.0f, 2.0f, 0.0f },

		// Низ                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f }
	};

	/* Инициализация матрицы проекции, где:
			1) FOV - field of view (угол обзора) - это то, какой угол обзора у наблюдателя (сколько фигур он видет и тд)
			2) Соотношение сторон (Необходимо для нормализации нашего окна (Чтобы было от -1 до 1 и по высоте, и по ширине))
			3) Znear - самое маленькое значение Z, также является самым ближним значением Z до наблюдателя
			4) Zfar - самое дальнее значение Z
			Вся фигура будет рисоваться внутри этих значений Z. 
			
			Если необходимо увеличить разницу между передней и задней гранями, то нужно изменить 3-ии значения во время инициализации, например, 
			
			Было: { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f }
			Стало: { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 3.0f }

			Но в таком случае необходимо менять все 3-ии параметры на данное число.

	*/
	matProj = Matrix_MakeProjection(90.0f, static_cast<float>(GetConsoleHeight()) / static_cast<float>(GetConsoleWidth()), 0.1f, 1000.0f);

	light.x = 1.0f;
	light.y = -100.0f;
	light.z = 1.0f;
	
	scale = 1.0f;							
	_x = 0.5f; _y = 0.75f; _z = 4.0f;
	fThetaX = fThetaY = fThetaZ = 0.0f;
}

void NewGarphics::OnUserUpdate(float fElapsedTime)
{
	// Clear our console
	Fill(0, 0, GetConsoleWidth(), GetConsoleHeight());

	// Surface
	Fill(0, iConsoleHeight / 2, iConsoleWidth, iConsoleHeight, PIXEL_SOLID, FG_PINK);

	//// Move around axies
	if (GetKey(L'W').bHeld)
		fThetaX += 8.0f * fElapsedTime;
	if (GetKey(L'S').bHeld)
		fThetaX -= 8.0f * fElapsedTime;
	if (GetKey(L'A').bHeld)
		fThetaY += 8.0f * fElapsedTime;
	if (GetKey(L'D').bHeld)
		fThetaY -= 8.0f * fElapsedTime;
	if (GetKey(L'Q').bHeld)
		fThetaZ += 8.0f * fElapsedTime;
	if (GetKey(L'E').bHeld)
		fThetaZ -= 8.0f * fElapsedTime;

	// Scaling
	if (GetKey(L'Z').bHeld)		// Increace
		scale = (scale <= 1.2f) ? scale + 0.01f : scale;
	if (GetKey(L'X').bHeld)		// Decreace
		scale = (scale >= 0.5f) ? scale - 0.01f : scale;

	// Shifts
	//if (GetKey(L'R').bHeld)		// Move to right
	//	_x += 0.01f;
	//if (GetKey(L'F').bHeld)		// Move to left
	//	_x -= 0.01f;
	//if (GetKey(L'T').bHeld)		// Move to up
	//	_y -= 0.01f;
	//if (GetKey(L'G').bHeld)		// Move to down
	//	_y += 0.01f;
	//if (GetKey(L'Y').bHeld)		// Move to up
	//	_z = (_z > 1.0f) ? _z - 0.1f : _z;
	//if (GetKey(L'H').bHeld)		// Move to down
	//	_z += 0.1f;

	if (IsFocused())
	{
		if (GetKey(VK_LBUTTON).bHeld)
		{
			if (GetMouseX() >= 0.9f * iConsoleWidth)		// Move to right
				_x += 0.005f;
			if (GetMouseX() <= 0.2f * iConsoleWidth)		// Move to left
				_x -= 0.005f;
			if (GetMouseY() <= 0.2f * iConsoleHeight)		// Move to up
				_y -= 0.005f;
			if (GetMouseY() >= 0.9f * iConsoleHeight)		// Move to down
				_y += 0.005f;
			if ((GetMouseX() >= 0.40f * iConsoleWidth) && (GetMouseX() <= 0.60f * iConsoleWidth)
				&& (GetMouseY() >= 0.35f * iConsoleHeight) && (GetMouseY() <= 0.49f * iConsoleHeight))		// Move to forward
				_z = (_z < 7.0f) ? _z + 0.1f : _z;
			if ((GetMouseX() >= 0.40f * iConsoleWidth) && (GetMouseX() <= 0.60f * iConsoleWidth)
				&& (GetMouseY() >= 0.51f * iConsoleHeight) && (GetMouseY() <= 0.65f * iConsoleHeight))		// Move to back
				_z = (_z > 4.5f) ? _z - 0.1f : _z;
		}
	}


	mat4x4 matRotX, matRotY, matRotZ;
	matRotX = Matrix_MakeRotationX(fThetaX * 0.5f);
	matRotY = Matrix_MakeRotationY(fThetaY * 0.5f);
	matRotZ = Matrix_MakeRotationZ(fThetaZ * 0.5f);

	mat4x4 ScalingMatrix;
	ScalingMatrix = Matrix_MakeScale(scale, scale, scale);

	mat4x4 TranslationMatrix;
	TranslationMatrix = Matrix_MakeTranslation(0.0f, 0.0f, _z);

	mat4x4 WorldMatrix;
	WorldMatrix = Matrix_MakeIdentity();
	WorldMatrix = matRotY * matRotX * matRotZ * ScalingMatrix * TranslationMatrix;

	std::vector<triangle> vecTrianglesToRaster;

	float  t = 0.0f;					// move X coor for another figure
	int16_t tri_color = FG_DARK_GREEN;
	int16_t count_tris = 0;
	for (auto& sh: shapes) 
	{
		// Take all triangles
		for (auto tri : sh.tris)
		{
			triangle triProjected, triTransformed;

			for (int16_t i = 0; i < 3; i++)
			{
				triTransformed.points[i] = MultiplyMatrixVector(WorldMatrix, tri.points[i]);

				// Преобразуем из 3D -> 2D
				triProjected.points[i] = MultiplyMatrixVector(matProj, triTransformed.points[i]);

				// Делим на Z для того, чтобы наши грани, которые находятся дальше, были меньше.
				triProjected.points[i] = triProjected.points[i] / triProjected.points[i].w;

				// X/Y are inverted so put them back
				triProjected.points[i].x *= -1.0f;
				triProjected.points[i].y *= -1.0f;
			}

			// Scaling to the size of the console
			for (int16_t i = 0; i < 3; i++)
			{
				triProjected.points[i].x += _x + t;
				triProjected.points[i].y += _y;

				triProjected.points[i].x *= 0.5f * static_cast<float>(GetConsoleWidth());
				triProjected.points[i].y *= 0.5f * static_cast<float>(GetConsoleHeight());

				// Counting barycenter
				barycenter += triProjected.points[i];
			}
			count_tris++;

			// Change color
			tri_color++;
			if (tri_color == FG_GREY) tri_color++;
			triProjected.col = tri_color;

			vecTrianglesToRaster.push_back(triProjected);
		}

		// Get barycenter of figure
		barycenter /= count_tris * 3;

		// Sort triangles from back to front
		std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
			{
				float z1 = (t1.points[0].z + t1.points[1].z + t1.points[2].z) / 3.0f;
				float z2 = (t2.points[0].z + t2.points[1].z + t2.points[2].z) / 3.0f;
				return z1 > z2; // was >
			});

		// Round all coord of points
		for (auto& tri : vecTrianglesToRaster)
		{
			for (int16_t i = 0; i < 3; i++)
			{
				tri.points[i].x = roundf(tri.points[i].x);
				tri.points[i].y = roundf(tri.points[i].y);
			}
		}

		// Draw
		DrawShadow(vecTrianglesToRaster, light);
		std::vector<triangle> vecVisibleSurfaces;
		fPoint3D view_point = { static_cast<float>(iConsoleWidth) / 2.0f, static_cast<float>(iConsoleHeight) / 2.0f, -100.0f };

		vecVisibleSurfaces = RobertsAlgorithm(vecTrianglesToRaster, view_point, barycenter, PIXEL_SOLID, FG_BLUE);
		
		t += 1.0f;
		count_tris = 0;
		barycenter = 0.0f;
		vecTrianglesToRaster.clear();
	}
}
