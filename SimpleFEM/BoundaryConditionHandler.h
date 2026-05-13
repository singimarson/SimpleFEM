#pragma once

#include "FESquareMatrix.h"
#include "FEVector.h"

#include <unordered_map>

namespace BoundaryConditionHandler
{

// Write the main function for 1D meshes next time
void HandleBoundaryConditions(FESquareMatrix& mSystemMatrix, FEVector& vSystemRHS, const std::unordered_map<int, double>& mapBoundaryConditions);

} // namespace BoundaryConditionHandler