#pragma once

#include "FEMesh.h"

// Class representing a line mesh in finite element analysis
// A line mesh is a one-dimensional mesh consisting of line elements, so this is the only class needed for 1D meshing
// Note: This class might need to be final, but who knows what'll be needed later on
class FELineMesh : public FEMesh
{
public:
	FELineMesh(double dLength);
	~FELineMesh() = default;

	virtual bool SupportsObjectType(const long objType) const override
	{
		return (FEMesh::SupportsObjectType(objType) || objType == FE_OBJ_TYPE_LINE_MESH);
	}

	// Getters and Setters
	double GetLength() const { return m_dLength; }
	bool SetLength(const double length)
	{
		m_dLength = length;
		return true;
	}

private:
	double m_dLength = 0.0;
};

