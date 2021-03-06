/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_AUDIO_OPENAL

#include "Engine/Scene/Source.h"
#include "Engine/Audio/Source.h"
#include "Engine/Audio/Device.h"
#include "Engine/Resources/Sound.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(SourceState)
	ENUM(Play)
	ENUM(Pause)
	ENUM(Stop)
REFLECT_ENUM_END()

REFLECT_ENUM(SourceMode)
	ENUM(Static)
	ENUM(Streaming)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(Source, Component)
	FIELD_ENUM_SETTER(4, SourceState, state, State)
	FIELD_ENUM_SETTER(5, SourceMode, mode, Mode)
	FIELD_PRIMITIVE_SETTER(6, bool, loop, Loop)
	FIELD_PRIMITIVE_SETTER(7, float, volume, Volume)
	FIELD_PRIMITIVE_SETTER(8, float, pitch, Pitch)
	FIELD_PRIMITIVE_SETTER(9, float, minDistance, MinDistance)
	FIELD_PRIMITIVE_SETTER(10, float, maxDistance, MaxDistance)
	FIELD_ENUM_SETTER(11, AudioRolloffMode, rolloffMode, RolloffMode)
	FIELD_PRIMITIVE_SETTER(12, float, rolloff, Rolloff)
	FIELD_CLASS_PTR_SETTER(13, Sound, SoundHandle, sound, Handle, Sound)
REFLECT_CLASS_END()

//-----------------------------------//

Source::Source()
	: state(SourceState::Stop)
	, mode(SourceMode::Static)
	, volume(1.0f)
	, pitch(1.0f)
	, minDistance(20.0f)
	, maxDistance(40.0f)
	, rolloffMode(AudioRolloffMode::Logarithmic)
	, rolloff(1.0f)
	, loop(false)
	, sound(HandleInvalid)
	, audioSource(nullptr)
{ }

//-----------------------------------//

Source::~Source()
{
	Deallocate(audioSource);
}

//-----------------------------------//

void Source::setVolume(float volume)
{
	this->volume = volume;
	if(audioSource) audioSource->setVolume(volume);
}

//-----------------------------------//

void Source::setPitch(float pitch)
{
	this->pitch = pitch;
	if(audioSource) audioSource->setPitch(pitch);
}

//-----------------------------------//

void Source::setMinDistance( float distance )
{
	this->minDistance = distance;
	if(audioSource) audioSource->setReferenceDistance(distance);
}

//-----------------------------------//

void Source::setMaxDistance( float distance )
{
	this->maxDistance = distance;
	if(audioSource) audioSource->setMaxDistance(distance);
}

//-----------------------------------//

void Source::setRolloff(float rolloff)
{
	this->rolloff = rolloff;
	if(audioSource) audioSource->setRolloff(rolloff);
}

//-----------------------------------//

void Source::setRolloffMode(AudioRolloffMode mode)
{
	this->rolloffMode = mode;
	if(audioSource) audioSource->setRolloffMode(rolloffMode);
}

//-----------------------------------//

void Source::setLoop(bool state)
{
	this->loop = state;
	if(audioSource) audioSource->setLoop(state);
}

//-----------------------------------//

void Source::setState( SourceState state )
{
	this->state = state;
	
	if(!audioSource) return;

	if(state == SourceState::Play && !audioSource->isPlaying())
		audioSource->play();
	
	else if(state == SourceState::Pause)
		audioSource->pause();
	
	else if(state == SourceState::Stop)
		audioSource->stop();
}

//-----------------------------------//

void Source::setMode( SourceMode mode )
{
	this->mode = mode;
	if(!audioSource) return;
}

//-----------------------------------//

void Source::setSound( const SoundHandle& soundHandle )
{
	sound = soundHandle;

	Sound* sound = soundHandle.Resolve();
	if( !sound ) return;

	if( sound->getStreamed() )
		mode = SourceMode::Streaming;

	if( !audioSource )
	{
		AudioDevice* device = GetAudioDevice();
		AudioContext* context = device->getMainContext();
		audioSource = AllocateThis(AudioSource, context);
	}

	audioSource->setSound(soundHandle);

	setVolume(volume);
	setPitch(pitch);
	setRolloff(rolloff);
	setMode(mode);
	setState(state);
}

//-----------------------------------//

void Source::update( float delta )
{
	if( !audioSource ) return;

	audioSource->update();

	if( audioSource->isPlaying() )
		state = SourceState::Play;
	else if( audioSource->isPaused() )
		state = SourceState::Pause;
	else
		state = SourceState::Stop;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif