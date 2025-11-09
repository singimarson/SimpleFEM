#pragma once
#include <CppUnitTestAssert.h>

#include "C3DPoint.h"

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

	// double equality comparison:
	static void AreEqual(double expected, double actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
	{
		Assert::AreEqual(expected, actual, message, pLineInfo);
	}

	// int equality comparison:
	static void AreEqual(int expected, int actual, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
	{
		Assert::AreEqual(expected, actual, message, pLineInfo);
	}

	// Verify that two C3DPoints are equal.
	static void AreEqual(C3DPoint expected, C3DPoint actual, double tolerance = 0.0, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
	{
		Assert::AreEqual(expected.GetX(), actual.GetX(), tolerance, message, pLineInfo);
		Assert::AreEqual(expected.GetY(), actual.GetY(), tolerance, message, pLineInfo);
		Assert::AreEqual(expected.GetZ(), actual.GetZ(), tolerance, message, pLineInfo);
	}
};

