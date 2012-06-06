/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ResourceDatabase;

REFLECT_DECLARE_CLASS(Project)

class Project : public Object
{
	REFLECT_DECLARE_OBJECT(Project)

public:
	
	ResourceDatabase* database;
};

//-----------------------------------//

class SceneDocument;

REFLECT_DECLARE_CLASS(ProjectPlugin)

class ProjectPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(ProjectPlugin)

public:

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;

	// Command callbacks.
	void onNewDocument(wxCommandEvent& event);
	void onOpenDocument(wxCommandEvent& event);
	void onSaveDocument(wxCommandEvent& event);
	void onSaveAsDocument(wxCommandEvent& event);

	// Creates a new document.
	Document* createDocument();

	// New toolbar button.
	wxAuiToolBarItem* newButton;
	wxMenuItem* newItem;

	// Open toolbar button.
	wxAuiToolBarItem* openButton;
	wxMenuItem* openItem;

	// Save toolbar button.
	wxAuiToolBarItem* saveButton;
	wxMenuItem* saveItem;
	wxMenuItem* saveAsItem;

protected:

	// Asks to save unsaved changes.
	bool askSaveChanges(Document* document);
};

//-----------------------------------//

NAMESPACE_EDITOR_END