/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

//#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/resources/Resource.h"

namespace vapor { namespace script {

//-----------------------------------//

class State;

//-----------------------------------//

/**
 * Scripting context that can be used to execute scripts. Currently this
 * represents a Lua interpreter state.
 */

class VAPOR_API Script : public resources::Resource
{
public:

	Script( const std::string& text );
	virtual ~Script();

	// Execute the script.
	void execute();

	// Gets the source code of the script.
	const std::string& getSource();

	// Sets the state where the script will execute.
	void setState( State* state );

	// Gets the associated resource group.
	virtual resources::ResourceGroup::Enum getResourceGroup() const;

protected:

	// Source code of the script.
	std::string text;

	// State where the script will execute.
	State* state;
};

//-----------------------------------//

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Script );

//-----------------------------------//

} } // end namespaces

//#endif