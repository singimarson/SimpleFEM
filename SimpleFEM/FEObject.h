#pragma once

#include "FETypes.h"

#include <combaseapi.h>
#include <string>

class FEObject
{
public:
	FEObject() { CoCreateGuid(&m_gID); };
	virtual ~FEObject() = default;

	virtual bool SupportsObjectType(const long objType)
	{
		return FE_OBJ_TYPE_OBJECT;
	}

	// Getters and Setters
	std::string GetName() { return m_strName; }
	void SetName(const std::string& strName) { m_strName = strName; }

	// Should not ever need to set the guid from outside the constructor
	GUID GetID() { return m_gID; }

protected:
	std::string m_strName;
	GUID m_gID;

private:
	// Todo: Implement an ID functionality for FEObjects. Probably need a GUID class
	// Not necessary right now, but it'll probably be useful down the road whenever we work
	// with GUIs and want to store and use specific objects.

};

