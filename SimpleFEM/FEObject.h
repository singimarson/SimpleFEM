#pragma once

#include "FETypes.h"

class FEObject
{
public:
	FEObject() {}
	virtual ~FEObject() = default;

	virtual bool SupportsObjectType(const long objType)
	{
		return FE_OBJ_TYPE_OBJECT;
	}

private:
	// Todo: Implement an ID functionality for FEObjects. Probably need a GUID class
	// Not necessary right now, but it'll probably be useful down the road whenever we work
	// with GUIs and want to store and use specific objects.
};

