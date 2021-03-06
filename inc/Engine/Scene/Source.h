/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#ifdef ENABLE_AUDIO_OPENAL

#include "Engine/Scene/Component.h"
#include "Engine/Audio/Source.h"

FWD_DECL_INTRUSIVE(Sound)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

API_SCENE REFLECT_DECLARE_ENUM(SourceState)

enum struct SourceState
{
	Play,
	Pause,
	Stop
};

//-----------------------------------//

API_SCENE REFLECT_DECLARE_ENUM(SourceMode)

enum struct SourceMode
{
	Static,
	Streaming
};

//-----------------------------------//

class AudioSource;

/**
 * Uses the position and orientation defined in the node 
 * as the sound source location.
 */

API_SCENE REFLECT_DECLARE_CLASS(Source)

class API_SCENE Source : public Component 
{
	REFLECT_DECLARE_OBJECT(Source)
	DECLARE_UNCOPYABLE(Source)

public:

	Source();
	virtual ~Source();

	// Sets the state of the source.
	void setState( SourceState state );

	// Sets the mode of the source.
	void setMode( SourceMode mode );

	// Sets the volume of the source. Volume is in the range [0.0-1.0].
	void setVolume( float volume );

	// Sets the pitch of the source. Pitch is in the range [0.0-1.0].
	void setPitch( float pitch );

	// Sets the minimum rolloff distance of the source.
	void setMinDistance( float distance );

	// Sets the maximum rolloff distance of the source.
	void setMaxDistance( float distance );

	// Sets the rolloff factor of the source. Rolloff is in the range [0.0-1.0].
	void setRolloff( float rolloff );

	// Sets the rolloff mode of the source.
	void setRolloffMode( AudioRolloffMode mode );

	// Sets the loop of the source.
	void setLoop( bool state );

	// Sets the sound of the source.
	void setSound( const SoundHandle& sound );

	// Updates the component.
	virtual void update( float delta ) OVERRIDE;

public:

	// State of the source.
	SourceState state;

	// State of the source.
	SourceMode mode;

	// Loops the source.
	bool loop;

	// Volume of the source.
	float volume;

	// Pitch of the source.
	float pitch;

	// Minimum rolloff distance.
	float minDistance;

	// Maximum rolloff distance.
	float maxDistance;

	// Rolloff mode of the source.
	AudioRolloffMode rolloffMode;

	// Rolloff of the source.
	float rolloff;

	// Sound resource.
	SoundHandle sound;

	// Audio source.
	AudioSource* audioSource;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Source );

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif