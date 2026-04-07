#pragma once
#include <CppUnitTestAssert.h>

#include "../SimpleFEM/Point.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Class definition for some extra definitions for testing
class CAssert 
{
public:
	// Verify that two objects are equal.
	template<typename T> static void AreEqual(const T& expected, const T& actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
	{
		Assert::AreEqual(expected, actual, message, pLineInfo);
	}

	// double equality with tolerance comparison:
	static void AreEqual(double expected, double actual, double tolerance = 0.0, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
	{
		Assert::AreEqual(expected, actual, tolerance, message, pLineInfo);
	}

	// int equality comparison:
	static void AreEqual(int expected, int actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
	{
		Assert::AreEqual(expected, actual, message, pLineInfo);
	}

	// Verify that two 1D Points are equal.
	static void AreEqual(Point<1> expected, Point<1> actual, double tolerance = 0.0, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
	{
		Assert::AreEqual(expected[0], actual[0], tolerance, message, pLineInfo);
	}

	// Verify that two 2D Points are equal.
	static void AreEqual(Point<2> expected, Point<2> actual, double tolerance = 0.0, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
	{
		Assert::AreEqual(expected[0], actual[0], tolerance, message, pLineInfo);
		Assert::AreEqual(expected[1], actual[1], tolerance, message, pLineInfo);
	}

	// Verify that two 3D Points are equal.
	static void AreEqual(Point<3> expected, Point<3> actual, double tolerance = 0.0, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
	{
		Assert::AreEqual(expected[0], actual[0], tolerance, message, pLineInfo);
		Assert::AreEqual(expected[1], actual[1], tolerance, message, pLineInfo);
		Assert::AreEqual(expected[2], actual[2], tolerance, message, pLineInfo);
	}
};

