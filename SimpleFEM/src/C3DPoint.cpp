#include "C3DPoint.h"

// Constructor with zeros
C3DPoint::C3DPoint() : m_dX(0.0), m_dY(0.0), m_dZ(0.0)
{}

// Constructor with initialized values
C3DPoint::C3DPoint(const double& x, const double& y, const double& z) : m_dX(x), m_dY(y), m_dZ(z)
{}