#ifndef UNICODE
#error Please enable UNICODE for your compiler! VS: Project Properties -> General -> \
Character Set -> Use Unicode.
#endif

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <Windows.h>
#include <cstdint>
#include <iostream>

#include <chrono>
//#include <thread>

#include <queue>
#include <vector>

#include <cmath>
#include <algorithm>

constexpr float PI = 3.14159f;

	// Enum of colors for drawing
enum COLOUR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	FG_PINK = 0xffc0cb,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,

};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};

//###################//
	// Main Class
//###################//

class Graphics
{
	// Main variables for project
protected:
	int16_t iConsoleWidth, iConsoleHeight;				// Size of console
	HANDLE hConsole;									// Current output handle
	HANDLE m_hConsoleIn;
	HANDLE hOriginalConsole;							// Original handle (need when we got some error)
	CHAR_INFO* console;									// Array of characters
	SMALL_RECT rectWindow;								
	std::wstring wsApp_name;

		// Keyboard using
	struct sKeyState
	{
		bool bPressed;
		bool bReleased;
		bool bHeld;
	};

	int16_t m_keyOldState[256];
	int16_t m_keyNewState[256];
	sKeyState m_keys[256];

	sKeyState m_mouse[5];
	bool m_mouseOldState[5];
	bool m_mouseNewState[5];
	bool m_bConsoleInFocus;

	int16_t m_mousePosX;
	int16_t m_mousePosY;

	// Main methods
public:
	Graphics();
	~Graphics();

	int16_t ConstructConsole(int16_t width, int16_t height, int16_t font_w, int16_t font_h, std::wstring Console_name = L"Light\'s");

	int16_t GetConsoleWidth();
	int16_t GetConsoleHeight();
	sKeyState& GetKey(int16_t key_id);
	int GetMouseX() { return m_mousePosX; }
	int GetMouseY() { return m_mousePosY; }
	sKeyState GetMouse(int nMouseButtonID) { return m_mouse[nMouseButtonID]; }
	bool IsFocused() { return m_bConsoleInFocus; }

protected:
	int16_t Error(const wchar_t* msg);

	virtual void OnUserCreate() = 0;
	virtual void OnUserUpdate(float fElapsedTime) = 0;

private:
	void SetConsoleDefault();

public:
	void Loop();

//---Draw---//
	// Drawing variables & structures
protected:
		// Structs for 2D graphics
	struct fPoint3D;

	struct mat3x3
	{
		float m[3][3] = { 0 };
	};
	struct fPoint2D										// Point struct, which have X,Y coords
	{
		float x;
		float y;
		float w;

	public:
		fPoint2D(): x(0.0f), y(0.0f), w(0.0f) {}
		fPoint2D(float x, float y, float w = 1.0f): x(x), y(y), w(w) {}

		fPoint2D& operator=(const fPoint2D& obj)
		{
			x = obj.x;
			y = obj.y;
			w = obj.w;
			return *this;
		}
		fPoint2D& operator=(const fPoint3D& obj)
		{
			x = obj.x;
			y = obj.y;
			w = obj.w;
			return *this;
		}
		fPoint2D& operator+=(const fPoint2D& obj)
		{
			x += obj.x;
			y += obj.y;
			return *this;
		}
		fPoint2D& operator-=(const fPoint2D& obj)
		{
			x -= obj.x;
			y -= obj.y;
			return *this;
		}
		fPoint2D& operator*=(float value)
		{
			x *= value;
			y *= value;
			return *this;
		}
		fPoint2D& operator/=(float value)
		{
			x /= value;
			y /= value;
			return *this;
		}

		fPoint2D operator+(const fPoint2D& obj)
		{
			return fPoint2D(x + obj.x, y + obj.y);
		}
		fPoint2D operator-(const fPoint2D& obj)
		{
			return fPoint2D(x - obj.x, y - obj.y);
		}
		fPoint2D operator*(float value)
		{
			return fPoint2D(x * value, y * value);
		}
		fPoint2D operator/(float value)
		{
			return fPoint2D(x / value, y / value);
		}

		void MultiplyMatrixVector(mat3x3& m)
		{
			x = x * m.m[0][0] + x * m.m[1][0] + x * m.m[2][0];
			y = y * m.m[0][1] + y * m.m[1][1] + y * m.m[2][1];
		}
	};
		// Struct for ScanLine algorithm
	struct iEdgeScanLine
	{
		int16_t x1, x2, y1, y2;
		float del_x, del_y, del_xy, del_yx;

		iEdgeScanLine()
		{
			x1 = x2 = y1 = y2 = 0;
			del_x = del_y = del_xy = del_yx = 0.0f;
		}

		iEdgeScanLine& operator=(const iEdgeScanLine& obj)
		{
			x1 = obj.x1;
			x2 = obj.x2;
			y1 = obj.y1;
			y2 = obj.y2;
			del_x = obj.del_x;
			del_y = obj.del_y;
			del_xy = obj.del_xy;
			del_yx = obj.del_yx;
			return *this;
		}
	};
		// Structs for 3D graphics
	struct mat4x4
	{
		float m[4][4] = { 0 };

		mat4x4 operator*(mat4x4& _m)
		{
			mat4x4 matrix;
			for (int16_t c = 0; c < 4; c++)
				for (int16_t r = 0; r < 4; r++)
					matrix.m[r][c] = m[r][0] * _m.m[0][c] + m[r][1] * _m.m[1][c] + m[r][2] * _m.m[2][c] + m[r][3] * _m.m[3][c];
			return matrix;
		}
	};
	struct fPoint3D										// Point struct, which have X,Y coords
	{
		float x, y, z, w;

	public:
		fPoint3D() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		fPoint3D(float x, float y, float z, float w = 1.0f) : x(x), y(y), z(z), w(w) {}

		fPoint3D& operator=(const fPoint3D& obj)
		{
			x = obj.x;
			y = obj.y;
			z = obj.z;
			w = obj.w;
			return *this;
		}
		fPoint3D& operator=(const fPoint2D& obj)
		{
			x = obj.x;
			y = obj.y;
			z = 0.0f;
			w = obj.w;
			return *this;
		}
		fPoint3D& operator=(float value)
		{
			x = value;
			y = value;
			z = value;
			return *this;
		}
		fPoint3D& operator+=(const fPoint3D& obj)
		{
			x += obj.x;
			y += obj.y;
			z += obj.z;
			return *this;
		}
		fPoint3D& operator-=(const fPoint3D& obj)
		{
			x -= obj.x;
			y -= obj.y;
			z -= obj.z;
			return *this;
		}
		fPoint3D& operator*=(float value)
		{
			x *= value;
			y *= value;
			z *= value;
			return *this;
		}
		fPoint3D& operator/=(float value)
		{
			x /= value;
			y /= value;
			z /= value;
			return *this;
		}

		bool operator==(fPoint3D& obj)
		{
			if (fabsf(x - obj.x) < 0.001f)
				if (fabsf(y - obj.y) < 0.001f)
					if (fabsf(z - obj.z) < 0.001f)
						return true;
			return false;
		}

		fPoint3D operator+(const fPoint3D& obj)
		{
			return fPoint3D(x + obj.x, y + obj.y, z + obj.z, w);
		}
		fPoint3D operator-(const fPoint3D& obj)
		{
			return fPoint3D(x - obj.x, y - obj.y, z - obj.z, w);
		}
		fPoint3D operator*(float value)
		{
			return fPoint3D(x * value, y * value, z * value, w);
		}
		fPoint3D operator/(float value)
		{
			return fPoint3D(x / value, y / value, z / value, w);
		}
	};
	struct triangle
	{
		fPoint3D points[3];

		int16_t sym = PIXEL_SOLID;
		int16_t col = FG_WHITE;

		triangle() {};
		triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
		{
			points[0].x = x1; points[0].y = y1; points[0].z = z1;
			points[1].x = x2; points[1].y = y2; points[1].z = z2;
			points[2].x = x3; points[2].y = y3; points[2].z = z3;
		}
		triangle(const fPoint2D& p1, const fPoint2D& p2, const fPoint2D& p3)
		{
			points[0] = p1;
			points[1] = p2;
			points[2] = p3;
		}
	};

	struct mesh
	{
		std::vector<triangle> tris;
	};


	// Drawing methods
public:
	void Draw(int16_t x, int16_t y, int16_t sym = ' ', int16_t col = BG_WHITE);
	void DrawLineBresenham(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t sym = ' ', int16_t col = BG_WHITE);
	void DrawPolygons(std::vector<fPoint2D>& points, int16_t sym = ' ', int16_t col = BG_WHITE);

		// Clear our console
	void Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t sym = PIXEL_SOLID, int16_t col = FG_BLACK);
	void Clip(int16_t& x, int16_t& y);

	void ShadingPolygonsScanLine(const std::vector<fPoint2D>& points, int16_t sym = ' ', int16_t col = BG_WHITE,
		int16_t y_min = -1, int16_t y_max = -1, int16_t x_min = -1, int16_t x_max = -1);
	void ShadingPolygonsFloodFillRecursion(const std::vector<fPoint2D>& points, int16_t sym = ' ',
		int16_t col = BG_WHITE, int16_t col_edges = BG_WHITE);

private:
	void FillingFloodFill(CHAR_INFO* console_ptr, int16_t x, int16_t y, int16_t sym, int16_t col, int16_t col_edges);

	// Actions methods
private:
	bool onSegment(const fPoint3D& p, const fPoint3D& q, const fPoint3D& r);
	bool checkPointAndSegment(const fPoint3D& start, const fPoint3D& p, const fPoint3D& end);

public:
	std::vector<triangle> RobertsAlgorithm(std::vector<triangle>& vecTrianglesToRaster, fPoint3D& view_point, fPoint3D& barycenter,
		int16_t sym = PIXEL_SOLID, int16_t col = FG_BLUE, int16_t col_edge = FG_GREY);

	void DrawShadow(std::vector<triangle>& vecTrianglesToRaster, fPoint3D& light);

	// Matrix methods (Use this for 3D)
public:
	float Vector_DotProduct(fPoint3D& v1, fPoint3D& v2);
	float Vector_Length(fPoint3D& v);
	fPoint3D Vector_Normalise(fPoint3D& v);
	fPoint3D Vector_CrossProduct(fPoint3D& v1, fPoint3D& v2);
	
	fPoint2D MultiplyMatrixVector(mat3x3& m, fPoint2D& v);
	fPoint3D MultiplyMatrixVector(mat4x4& m, fPoint3D& v);

	mat4x4 Matrix_MakeIdentity();
	mat4x4 Matrix_MakeRotationX(float fAngleRad);
	mat4x4 Matrix_MakeRotationY(float fAngleRad);
	mat4x4 Matrix_MakeRotationZ(float fAngleRad);
	mat4x4 Matrix_MakeScale(float x, float y, float z);
	mat4x4 Matrix_MakeTranslation(float x, float y, float z);
	mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
	mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2);
};

#endif // !_GRAPHICS_H_
