/************************************************************************
*
* vapor3D Editor � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "UndoOperation.h"

FWD_DECL_INTRUSIVE(Entity)
FWD_DECL_INTRUSIVE(Component)
FWD_DECL_INTRUSIVE(Scene)
FWD_DECL_INTRUSIVE(Model)
FWD_DECL_INTRUSIVE(Mesh)

namespace vapor { namespace editor {

//-----------------------------------//

enum 
{
	ID_MenuSceneEntityDelete = wxID_HIGHEST+472,
	ID_MenuSceneEntityDuplicate,
	ID_MenuSceneEntityVisible,
	ID_MenuSceneEntityWireframe,
	ID_MenuSceneEntityTerrain,
};

//-----------------------------------//

class EntityOperation : public UndoOperation
{
public:

	void undo();
	void redo();

	bool added;
	EntityPtr entity;
	SceneWeakPtr weakScene;
};

//-----------------------------------//

class EntityItemData : public wxTreeItemData
{
public:

	EntityItemData() : entity(nullptr) {}

	Entity* entity;
	ComponentWeakPtr component;
};

//-----------------------------------//

class EditorFrame;
class EntityOperation;

typedef std::map<Entity*, wxTreeItemId> EntityIdsMap;

//-----------------------------------//

/**
 * This control is responsible for mantaining and updating a TreeCtrl
 * with all the contents of the scene tree in the passed instance of
 * the engine. The events of this control (right-click context menus)
 * will be used to change some settings of the scene tree, and also
 * properties of the associated scene nodes.
 */

class ScenePage : public wxPanel 
{
public:

	ScenePage( wxWindow* parent, wxWindowID id = wxID_ANY );
	virtual ~ScenePage();

	// Gets the tree control.
	GETTER(TreeCtrl, wxTreeCtrl*, treeCtrl)

	// Sets the current scene.
	void setScene(const ScenePtr& scene);

	// Gets the entity associated with the tree item.
	EntityPtr getEntityFromTreeId( wxTreeItemId id );

	// Gets the tree item from the entity.
	wxTreeItemId getTreeIdFromEntity(const EntityPtr& entity);

	// Gets the component associated with the tree item.
	ComponentPtr getComponentFromTreeId( wxTreeItemId id );

	// Did we send the last selection event.
	bool sentLastSelectionEvent;

protected:

	// Initializes the control.
	void initTree();
	void initButtons();

	// Initializes the icons list.
	void initIcons();

	// Adds a group node to the tree.
	void addGroup( wxTreeItemId id, const EntityPtr& node, bool createGroup = true );

	// Adds a node to the tree.
	wxTreeItemId addEntity( wxTreeItemId id, const EntityPtr& node );

	// Adds a component to the tree item.
	void addComponent( wxTreeItemId id, ComponentPtr component );

	// Creates a new node operation.
	EntityOperation* createEntityOperation(const EntityPtr& node, const std::string& desc);

	// Cleans the current scene.
	void cleanScene();

	// wxWidgets event callbacks.
	void onItemChanged( wxTreeEvent& );
	void onItemMenu( wxTreeEvent& );
	void onLabelEditBegin( wxTreeEvent& );
	void onLabelEditEnd( wxTreeEvent& );
	void onDragBegin( wxTreeEvent& );
	void onDragEnd( wxTreeEvent& );
	void onContextMenu( wxContextMenuEvent& );
	void onMenuSelected( wxCommandEvent& );
	void onComponentAdd( wxCommandEvent& );
	void onButtonEntityAdd(wxCommandEvent&);
	void onButtonEntityDelete(wxCommandEvent&);
	void onButtonEntityDeleteUpdate(wxUpdateUIEvent&);

	// Event helpers.
	void populateEntityItemMenu(wxMenu& menu, const EntityPtr& node);
	void populateComponentItemMenu(wxMenu& menu, const ComponentPtr& component);
	void onAnimationMenuSelected(wxCommandEvent&);
	void onAttachmentMenuSelected(wxCommandEvent&);
	wxMenu* createMenuAnimation(const MeshPtr& node);
	wxMenu* createMenuAttachment(const MeshPtr& node);
	int firstAnimationId;
	int firstAttachmentId;
	MeshHandle meshHandle;
	Mesh* mesh;
	ModelPtr model;

	// Scene-monitoring callbacks.
	void onEntityAdded( const EntityPtr& node );
	void onEntityRemoved( const EntityPtr& node );

	// Scene tree.
	wxBoxSizer* sizer;
	wxTreeCtrl* treeCtrl;
	wxTreeItemId rootId;
	wxTreeItemId menuItemId;
	wxTreeItemId dragItemId;
	EntityIdsMap nodeIds;

	// Tree icons.
	wxImageList* imageList;
	std::map<const Type*, int> icons;
	std::map<const Type*, wxBitmap> bitmaps;

	// Entity buttons.
	wxBitmapButton* buttonEntityAdd;
	wxBitmapButton* buttonEntityDelete;

	// Current menu.
	wxMenu* currentMenu;

	// Scene instance.
	SceneWeakPtr weakScene;

	// Entity counter.
	int nodeCounter;
};

//-----------------------------------//

} } // end namespaces