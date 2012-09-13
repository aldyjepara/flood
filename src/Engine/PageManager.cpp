/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Paging/PageManager.h"
#include "Engine/Paging/Page.h"

#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Transform.h"
#include "Engine/Scene/Camera.h"

#if 0

NAMESPACE_ENGINE_BEGIN

bool loaded[100][100];

//-----------------------------------//

PageManager::PageManager( uint pageSize )
	: pageSize(pageSize)
{ }

//-----------------------------------//

PageManager::PageManager( uint pageSize, CameraWeakPtr weakCamera )
	: pageSize(pageSize)
	, weakCamera(weakCamera)
{ }

//-----------------------------------//

PageManager::~PageManager()
{
	for( size_t i = 0; i < pages.size(); i++ )
	{
		Page* const page = pages[i];
		delete page;
	}
}

//-----------------------------------//

void PageManager::update( float delta )
{
	const CameraPtr& camera = weakCamera.lock();

	// No associated current camera.
	if(!camera)
		return;

	Entity* entity = camera->getEntity();
	const TransformPtr& transform = entity->getTransform();

	Vector3 pos = transform->getPosition();
	Vector2 newPage = convertWorldToPage(pos);

	if( page != newPage )
	{
		page = newPage;

		PageEvent event;
		event.pos = page;
	
		onPageSwitch(event);

		if(!loaded[(int)page.x][(int)page.y])
		{
			loaded[(int)page.x][(int)page.y] = true;

			onPageLoading(event);
		}
	}
}

//-----------------------------------//

Vector2 PageManager::convertWorldToPage(const Vector3& pos)
{
	int x = floor(pos.x / pageSize); 
	int y = floor(pos.z / pageSize);

	return Vector2(x, y);
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif