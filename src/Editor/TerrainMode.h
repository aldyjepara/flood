/************************************************************************
*
* vapor3D Editor � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Mode.h"
#include "Operation.h"

namespace vapor { namespace editor {

//-----------------------------------//

namespace TerrainTool
{
	enum Enum
	{
		Raise,
		Lower,
		Paint,
		Smooth
	};
}

//-----------------------------------//

class TerrainOperation : public Operation
{
public:

	TerrainOperation( TerrainTool::Enum tool );

	void redo();
	void undo();

	float size;
	float strength;
	TerrainTool::Enum tool;
	
	RayTriangleQueryResult res;

	std::vector<float> heights;
	bool savedState;

protected:

	void processState( bool save );
	void applyTerrainTool();
};

//-----------------------------------//

class TerrainMode : public Mode, public wxEvtHandler
{
public:

	TerrainMode( EditorFrame* frame );

	virtual void onModeInit(wxToolBar* toolbar, ModeIdMap& map);
	
	virtual void onMouseDrag( const MouseDragEvent& mde );
	virtual void onMouseButtonPress( const MouseButtonEvent& mbe );
	virtual void onMouseButtonRelease( const MouseButtonEvent& );
	virtual void onMouseLeave();

	// Callback used when editing the terrain by holding the mouse button.
	void onTimer( wxTimerEvent& event );

	// Ray picks the terrain and returns the intersection point.
	bool pickTerrain( const MouseButtonEvent& mb, RayTriangleQueryResult& res );

	// Checks if the ray hits and deforms.
	void deformTerrain( const MouseButtonEvent& mb );

	// Creates a new operation if there is none currently.
	void createOperation();

	// Finishes the operation and registers the event in the history stack.
	void registerEvent();

protected:

	// Holds the current terrain operation.
	TerrainOperation* op;
	
	// Callback used when editing the terrain by holding the mouse button.
	wxTimer timer;

private:

    DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces