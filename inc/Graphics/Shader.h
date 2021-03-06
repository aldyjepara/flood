/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/References.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Shaders can be of different types, depending on what they deal with.
 *  - Vertex shaders manipulate vertex properties such as position.
 *  - Pixel shaders calculate the color of individual pixels (fragments).
 *  - Geometry shaders can be used to generate geometry procedurally.
 */

enum struct ShaderType
{
	Vertex,
	Fragment,
	Geometry
};

//-----------------------------------//

/**
 * Shaders are used to program the GPU programmable rendering pipeline.
 * It is a set of software instructions that instruct how to calculate
 * each vertex and each pixel processed by the graphics card.
 */

class API_GRAPHICS Shader : public ReferenceCounted
{
public:

	// Gets the shader type.
	ACCESSOR(ShaderType, ShaderType, type)

	// Gets the shader text.
	ACCESSOR(Text, const String&, text)

	// Gets the compilation log.
	GETTER(Log, const String&, log)

	// Creates and loads the shader.
	virtual bool create() = 0;

	// Compiles the shader.
	virtual bool compile() = 0;

	// Forces the shader to be recompiled.
	virtual void forceRecompile();
	
	// Is this shader already compiled?
	bool isCompiled() const;

	// Did this shader throw compile errors?
	bool gotCompileErrors() const;

	// Shader type.
	ShaderType type;

	// Holds the shader text.
	String text;

	// Holds the compilation log.
	String log;

	// Is this shader already compiled?	
	bool compiled;
	bool compileErrors;

protected:

	Shader();
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Shader );

//-----------------------------------//

NAMESPACE_GRAPHICS_END