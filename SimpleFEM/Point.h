#pragma once

#include <array>
#include <stdexcept>

// This is the class responsible for organizing any 3D point and its operations
// 1D points will only use X, 2D will use X and Y, and 3D uses all 3
template <std::size_t dim>
class Point
{
public:
	// Constructor
	Point()
	{
		for (std::size_t iDimIter = 0; iDimIter < dim; ++iDimIter)
		{
			m_arrCoords[iDimIter] = 0.0;
		}
	}

	// Constructor in 1 dimension
	Point(const double& x) : m_arrCoords({ x })
	{
		if (dim != 1)
		{
			throw std::runtime_error("Point::Point: Invalid dimension for this method of constructing a point.");
		}
	}

	// Constructor in 2 dimensions
	Point(const double& x, const double& y) : m_arrCoords({ x, y })
	{
		if (dim != 2)
		{
			throw std::runtime_error("Point::Point: Invalid dimension for this method of constructing a point.");
		}
	}

	// Constructor in 3 dimensions
	Point(const double& x, const double& y, const double& z) : m_arrCoords({ x, y, z })
	{
		if (dim != 3)
		{
			throw std::runtime_error("Point::Point: Invalid dimension for this method of constructing a point.");
		}
	}

	// Destructor
	~Point() = default;

	// Operations and overloads
	double operator[](const std::size_t& index)
	{
		if (index > dim)
		{
			throw std::runtime_error("Point::operator[]: Index is out of range.");
		}

		return m_arrCoords[index];
	}

	bool operator==(const Point<dim>& pt) 
	{
		bool bEqual = true;
		for (int iDimIter = 0; iDimIter < dim; ++iDimIter)
		{
			bEqual += m_arrCoords[iDimIter] == pt[iDimIter];
		}

		return bEqual;
	}

	Point operator+(const Point<dim>& pt)
	{
		std::array<double, dim> ptSum;
		for (int iDimIter = 0; iDimIter < dim; ++iDimIter)
		{
			ptSum[iDimIter] = m_arrCoords[iDimIter] + pt[iDimIter];
		}

		return Point<dim>(ptSum);
	}

	Point operator-(const Point<dim>& pt)
	{
		std::array<double, dim> ptSum;
		for (int iDimIter = 0; iDimIter < dim; ++iDimIter)
		{
			ptSum[iDimIter] = m_arrCoords[iDimIter] - pt[iDimIter];
		}

		return Point<dim>(ptSum);
	}

	// Getters and setters

private:
	std::array<double, dim> m_arrCoords;
};