#pragma once

// This is the class responsible for organizing any 3D point and its operations
// 1D points will only use X, 2D will use X and Y, and 3D uses all 3
class C3DPoint
{
public:
	C3DPoint();
	C3DPoint(const double& x, const double& y, const double& z);
	~C3DPoint() = default;

	// Getters and setters
	double GetX() const { return m_dX; }
	bool SetX(const double dX)
	{
		m_dX = dX;
		return true;
	}

	double GetY() const { return m_dY; }
	bool SetY(const double dY)
	{
		m_dY = dY;
		return true;
	}
	
	double GetZ() const { return m_dZ; }
	bool SetZ(const double dZ)
	{
		m_dZ = dZ;
		return true;
	}

private:
	double m_dX = 0.0;
	double m_dY = 0.0;
	double m_dZ = 0.0;
};

