#pragma once

#include <array>
#include <cmath>
#include <stdexcept>

// This is the class responsible for organizing any 3D point and its operations
// 1D points will only use X, 2D will use X and Y, and 3D uses all 3
template <std::size_t dim>
class Point
{
public:
	// Constructor
	template<std::size_t D = dim, typename std::enable_if<D == 1 || D == 2 || D == 3, std::size_t>::type = 0>
	Point() : m_arrCoords({})
	{
	}

	// Constructor with array
	template<std::size_t D = dim, typename std::enable_if<D == 1 || D == 2 || D == 3, std::size_t>::type = 0>
	Point(const std::array<double, dim>& ptArray) : m_arrCoords(ptArray)
	{
	}

	// Constructor in 1 dimension
	template <std::size_t D = dim, typename std::enable_if<D == 1, std::size_t>::type = 0>
	explicit Point(const double& x) : m_arrCoords({x})
	{
	}

	// Constructor in 2 dimensions
	template <std::size_t D = dim, typename std::enable_if<D == 2, std::size_t>::type = 0>
	explicit Point(const double& x, const double& y) : m_arrCoords({x, y})
	{
	}

	// Constructor in 3 dimensions
	template <std::size_t D = dim, typename std::enable_if<D == 3, std::size_t>::type = 0>
	explicit Point(const double& x, const double& y, const double& z) : m_arrCoords({x, y, z})
	{
	}

	// Destructor
	~Point() = default;

	// Operations and overloads
	double operator[](const std::size_t& index) const
	{
		if (index >= dim)
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
		std::array<double, dim> ptDiff;
		for (int iDimIter = 0; iDimIter < dim; ++iDimIter)
		{
			ptDiff[iDimIter] = m_arrCoords[iDimIter] - pt[iDimIter];
		}

		return Point<dim>(ptDiff);
	}

	// Miscellaneous operations

	// Distance between two points
	double Distance(const Point<dim>& pt)
	{
		double dDistanceSquared = 0.0;
		for (int iDimIter = 0; iDimIter < dim; ++iDimIter)
		{
			dDistanceSquared += (m_arrCoords[iDimIter] - pt[iDimIter]) * (m_arrCoords[iDimIter] - pt[iDimIter]);
		}
		return std::sqrt(dDistanceSquared);
	}

private:
	std::array<double, dim> m_arrCoords;
};