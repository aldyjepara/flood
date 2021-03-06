/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/TextureManager.h"
#include "Graphics/RenderBackend.h"
#include "Resources/ResourceManager.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

TextureManager::TextureManager()
{
	ResourceManager* res = GetResourceManager();
	res->onResourceLoaded.Connect( this, &TextureManager::onLoaded );
	res->onResourceRemoved.Connect( this, &TextureManager::onUnloaded );
	res->onResourceReloaded.Connect( this, &TextureManager::onReloaded );
}

//-----------------------------------//

TextureManager::~TextureManager()
{
	ResourceManager* res = GetResourceManager();
	res->onResourceLoaded.Disconnect( this, &TextureManager::onLoaded );
	res->onResourceRemoved.Disconnect( this, &TextureManager::onUnloaded );
	res->onResourceReloaded.Disconnect( this, &TextureManager::onReloaded );

	#pragma TODO("Make sure all textures are released on exit")
	TextureMap::const_iterator it;
	for( it = textures.begin(); it != textures.end(); it++ )
	{
		const Texture* texture = it->second.get();
		//assert( texture->getReferenceCount() == 2 );
	}
}

//-----------------------------------//

void TextureManager::removeTexture(Image* image)
{
	TextureMap::iterator it = textures.find(image);
	
	if( it == textures.end() )
		return;

	textures.erase(it);
}

//-----------------------------------//

static const uint8 TEX_SIZE = 64;

TexturePtr TextureManager::getTexture( const String& name )
{
	ResourceManager* res = GetResourceManager();
	ImageHandle handle = res->loadResource<Image>(name);
	return getTexture(handle);
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( Image* image )
{
	if( !image )
	{
		// Image not valid.
		return nullptr;
	}

	// Image already has texture.
	else if( textures.find(image) != textures.end() )
	{
		return textures[image];
	}

	// Image not loaded yet.
	else if( !image->isLoaded() ) 
	{
		Texture* texture = backend->createTexture();
		texture->allocate(Vector2i(TEX_SIZE, TEX_SIZE), PixelFormat::R8G8B8A8);

		textures[image] = texture;
		return texture;
	}

	return nullptr;
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( const ImageHandle& imageHandle )
{
	Image* image = imageHandle.Resolve();

	if (TexturePtr texture = getTexture(image))
		return texture;

	// Create a new texture from image.
	Texture* texture = backend->createTexture();
	texture->setImage(imageHandle);

	textures[image] = texture;

	return texture;
}

//-----------------------------------//

void TextureManager::onLoaded( const ResourceEvent& event )
{
	ImageHandle handleImage = HandleCast<Image>(event.handle);
	Image* image = handleImage.Resolve();

	if( image->getResourceGroup() != ResourceGroup::Images )
		return;

	if( textures.find(image) == textures.end() )
		return;

	Texture* texture = textures[image].get();
	texture->setImage(handleImage);

	backend->uploadTexture(texture);
}

//-----------------------------------//

void TextureManager::onUnloaded( const ResourceEvent& event )
{
	ImageHandle handleImage = HandleCast<Image>(event.handle);
	Image* image = handleImage.Resolve();

	if( image->getResourceGroup() != ResourceGroup::Images )
		return;

	if( textures.find(image) == textures.end() )
		return;

	LogDebug( "Removing texture '%s'", image->getPath().c_str() );

	removeTexture(image);
}

//-----------------------------------//

void TextureManager::onReloaded( const ResourceEvent& event )
{
	ImageHandle handleImage = HandleCast<Image>(event.handle);
	Image* newImage = handleImage.Resolve();

	if( newImage->getResourceGroup() != ResourceGroup::Images )
		return;
	
	Image* oldImage = (Image*) event.oldResource;

	if( textures.find(oldImage) == textures.end() )
		return;

	LogDebug( "Reloading texture '%s'", newImage->getPath().c_str() );

	Texture* texture = textures[oldImage].get();
	texture->setImage(handleImage);

	textures.erase(oldImage);
	textures[newImage] = texture;
}

//-----------------------------------//

uint TextureManager::getMemoryUsage()
{
	uint total = 0;

	TextureMap::const_iterator it;
	for( it = textures.begin(); it != textures.end(); it++ )
		total += it->first->getBuffer().size();

	return total;
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END