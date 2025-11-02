#pragma once

#include "FEMesh.h"

// Class representing a rectangular mesh in finite element analysis
// ToDo: Work on this after the 1D meshing is complete
class FERectangularMesh : public FEMesh
{
public:
	FERectangularMesh(int iDim, double dLengthX, double dLengthY, double dLengthZ);
	~FERectangularMesh() = default;

private:
	double m_dLengthX = 0.0;
	double m_dLengthY = 0.0;
	double m_dLengthZ = 0.0;
};

