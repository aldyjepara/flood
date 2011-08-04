/************************************************************************
*
* vapor3D Editor � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Protocol/API.h"
#include "Network/MessagePlugin.h"
#include "Protocol/Users.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

enum API_PROTOCOL UserAuthType
{
	None,
	Password,
	Certificate
};

struct API_PROTOCOL UserAuthMessage : MessageDefinition
{
	String name;
	UserAuthType type;
};

struct API_PROTOCOL UserJoinMessage : MessageDefinition
{
	UserId user;
	String name;
};

struct API_PROTOCOL UserLeaveMessage : MessageDefinition
{
	UserId user;
};

struct API_PROTOCOL UserChangeNameMessage : MessageDefinition
{
	UserId user;
	String name;
};

#define USER_INITIAL_ID 600

REFLECT_DECLARE_ENUM(UserMessageIds)

struct API_PROTOCOL UserMessageIds
{
	enum Enum : MessageId
	{
		UserAuth = USER_INITIAL_ID + 4,
		UserChangeName = USER_INITIAL_ID + 2,

		UserJoin = USER_INITIAL_ID + 0,
		UserLeave = USER_INITIAL_ID + 1,
		UserNotifyName = USER_INITIAL_ID + 3,
	};
};

REFLECT_DECLARE_CLASS(UserMessagePlugin)

//-----------------------------------//

class API_PROTOCOL UserMessagePlugin : public MessagePlugin
{
	REFLECT_DECLARE_OBJECT(UserMessagePlugin)

public:

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Gets the messages that this plugin handles.
	const MessagesTable& getMessagesTable() OVERRIDE;

	// Gets the enum with the messages that this plugin handles.
	Enum* getMessagesEnum() OVERRIDE;

	// Message callbacks.
	virtual void handleUserAuth(const SessionPtr&, const MessagePtr&) {}
	virtual void handleUserChangeName(const SessionPtr&, const MessagePtr&) {}

	virtual void handleUserJoin(const SessionPtr&, const MessagePtr&) {}
	virtual void handleUserLeave(const SessionPtr&, const MessagePtr&) {}
	virtual void handleUserNotifyName(const SessionPtr&, const MessagePtr&) {}

	Users users;
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END