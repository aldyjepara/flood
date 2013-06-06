/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Host.h"
#include "Packet.h"
#include "Peer.h"
#include "ResourceHandle.h"
#include "Session.h"
#include "SessionManager.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::Host::Host(::Host* native)
{
    NativePtr = native;
}

Flood::Host::Host(System::IntPtr native)
{
    auto __native = (::Host*)native.ToPointer();
    NativePtr = __native;
}

bool Flood::Host::DestroySocket()
{
    auto ret = ((::Host*)NativePtr)->destroySocket();
    return ret;
}

void Flood::Host::BroadcastPacket(Flood::Packet^ _0, unsigned char channel)
{
    auto arg0 = (::Packet*)_0->NativePtr;
    auto arg1 = (uint8)channel;
    ((::Host*)NativePtr)->broadcastPacket(arg0, arg1);
}

void Flood::Host::ProcessEvents(unsigned int timeout)
{
    auto arg0 = (uint32)timeout;
    ((::Host*)NativePtr)->processEvents(arg0);
}

bool Flood::Host::HasContext()
{
    auto ret = ((::Host*)NativePtr)->hasContext();
    return ret;
}

void Flood::Host::PeerConnect::add(System::Action<Flood::Peer^>^ evt)
{
    if (!_PeerConnectDelegateInstance)
    {
        _PeerConnectDelegateInstance = gcnew _PeerConnectDelegate(this, &Flood::Host::_PeerConnectRaise);
        auto _fptr = (void (*)(const ::PeerPtr&))Marshal::GetFunctionPointerForDelegate(_PeerConnectDelegateInstance).ToPointer();
        ((::Host*)NativePtr)->onPeerConnect.Connect(_fptr);
    }
    _PeerConnect = static_cast<System::Action<Flood::Peer^>^>(System::Delegate::Combine(_PeerConnect, evt));
}

void Flood::Host::PeerConnect::remove(System::Action<Flood::Peer^>^ evt)
{
    _PeerConnect = static_cast<System::Action<Flood::Peer^>^>(System::Delegate::Remove(_PeerConnect, evt));
}

void Flood::Host::PeerConnect::raise(Flood::Peer^ _1)
{
    _PeerConnect(_1);
}

void Flood::Host::_PeerConnectRaise(const ::PeerPtr& _1)
{
    PeerConnect::raise(gcnew Flood::Peer((::Peer*)_1.get()));
}

void Flood::Host::PeerDisconnect::add(System::Action<Flood::Peer^>^ evt)
{
    if (!_PeerDisconnectDelegateInstance)
    {
        _PeerDisconnectDelegateInstance = gcnew _PeerDisconnectDelegate(this, &Flood::Host::_PeerDisconnectRaise);
        auto _fptr = (void (*)(const ::PeerPtr&))Marshal::GetFunctionPointerForDelegate(_PeerDisconnectDelegateInstance).ToPointer();
        ((::Host*)NativePtr)->onPeerDisconnect.Connect(_fptr);
    }
    _PeerDisconnect = static_cast<System::Action<Flood::Peer^>^>(System::Delegate::Combine(_PeerDisconnect, evt));
}

void Flood::Host::PeerDisconnect::remove(System::Action<Flood::Peer^>^ evt)
{
    _PeerDisconnect = static_cast<System::Action<Flood::Peer^>^>(System::Delegate::Remove(_PeerDisconnect, evt));
}

void Flood::Host::PeerDisconnect::raise(Flood::Peer^ _3)
{
    _PeerDisconnect(_3);
}

void Flood::Host::_PeerDisconnectRaise(const ::PeerPtr& _3)
{
    PeerDisconnect::raise(gcnew Flood::Peer((::Peer*)_3.get()));
}

void Flood::Host::PeerPacket::add(System::Action<Flood::Peer^, Flood::Packet^, int>^ evt)
{
    if (!_PeerPacketDelegateInstance)
    {
        _PeerPacketDelegateInstance = gcnew _PeerPacketDelegate(this, &Flood::Host::_PeerPacketRaise);
        auto _fptr = (void (*)(const ::PeerPtr&, const ::PacketPtr&, int))Marshal::GetFunctionPointerForDelegate(_PeerPacketDelegateInstance).ToPointer();
        ((::Host*)NativePtr)->onPeerPacket.Connect(_fptr);
    }
    _PeerPacket = static_cast<System::Action<Flood::Peer^, Flood::Packet^, int>^>(System::Delegate::Combine(_PeerPacket, evt));
}

void Flood::Host::PeerPacket::remove(System::Action<Flood::Peer^, Flood::Packet^, int>^ evt)
{
    _PeerPacket = static_cast<System::Action<Flood::Peer^, Flood::Packet^, int>^>(System::Delegate::Remove(_PeerPacket, evt));
}

void Flood::Host::PeerPacket::raise(Flood::Peer^ _4, Flood::Packet^ _5, int _7)
{
    _PeerPacket(_4, _5, _7);
}

void Flood::Host::_PeerPacketRaise(const ::PeerPtr& _4, const ::PacketPtr& _5, int _7)
{
    PeerPacket::raise(gcnew Flood::Peer((::Peer*)_4.get()), gcnew Flood::Packet((::Packet*)_5.get()), _7);
}

Flood::HostConnectionDetails::HostConnectionDetails(::HostConnectionDetails* native)
{
    Address = clix::marshalString<clix::E_UTF8>(native->address);
    Port = native->port;
    ChannelCount = native->channelCount;
}

Flood::HostConnectionDetails::HostConnectionDetails(System::IntPtr native)
{
    auto __native = (::HostConnectionDetails*)native.ToPointer();
    Address = clix::marshalString<clix::E_UTF8>(__native->address);
    Port = __native->port;
    ChannelCount = __native->channelCount;
}

Flood::HostConnectionDetails::HostConnectionDetails(System::String^ address, unsigned short port, unsigned char channelCount)
{
    auto _native = ::HostConnectionDetails(clix::marshalString<clix::E_UTF8>(address), (uint16)port, (uint8)channelCount);
    this->Address = clix::marshalString<clix::E_UTF8>(_native.address);
    this->Port = _native.port;
    this->ChannelCount = _native.channelCount;
}

Flood::HostClient::HostClient(::HostClient* native)
    : Flood::Host(native)
{
}

Flood::HostClient::HostClient(System::IntPtr native)
    : Flood::Host(native)
{
    auto __native = (::HostClient*)native.ToPointer();
}

Flood::HostClient::HostClient()
    : Flood::Host(nullptr)
{
    NativePtr = new ::HostClient();
}

bool Flood::HostClient::Connect(Flood::HostConnectionDetails _0)
{
    auto _marshal0 = ::HostConnectionDetails();
    _marshal0.address = clix::marshalString<clix::E_UTF8>(_0.Address);
    _marshal0.port = (uint16)_0.Port;
    _marshal0.channelCount = (uint8)_0.ChannelCount;
    auto arg0 = _marshal0;
    auto ret = ((::HostClient*)NativePtr)->connect(arg0);
    return ret;
}

Flood::Peer^ Flood::HostClient::GetPeer()
{
    auto ret = ((::HostClient*)NativePtr)->getPeer();
    return gcnew Flood::Peer((::Peer*)ret.get());
}

Flood::Session^ Flood::HostClient::GetSession()
{
    auto ret = ((::HostClient*)NativePtr)->getSession();
    return gcnew Flood::Session((::Session*)ret);
}

Flood::HostServer::HostServer(::HostServer* native)
    : Flood::Host(native)
{
}

Flood::HostServer::HostServer(System::IntPtr native)
    : Flood::Host(native)
{
    auto __native = (::HostServer*)native.ToPointer();
}

bool Flood::HostServer::CreateSocket(Flood::HostConnectionDetails _0)
{
    auto _marshal0 = ::HostConnectionDetails();
    _marshal0.address = clix::marshalString<clix::E_UTF8>(_0.Address);
    _marshal0.port = (uint16)_0.Port;
    _marshal0.channelCount = (uint8)_0.ChannelCount;
    auto arg0 = _marshal0;
    auto ret = ((::HostServer*)NativePtr)->createSocket(arg0);
    return ret;
}

System::Collections::Generic::List<Flood::Peer^>^ Flood::HostServer::GetPeers()
{
    auto &ret = ((::HostServer*)NativePtr)->getPeers();
    auto _tmpret = gcnew System::Collections::Generic::List<Flood::Peer^>();
    for(auto _element : ret)
    {
        auto _marshalElement = gcnew Flood::Peer((::Peer*)_element.get());
        _tmpret->Add(_marshalElement);
    }
    return _tmpret;
}

Flood::SessionManager^ Flood::HostServer::GetSessionManager()
{
    auto &ret = ((::HostServer*)NativePtr)->getSessionManager();
    return gcnew Flood::SessionManager((::SessionManager*)&ret);
}

Flood::HostServer::HostServer()
    : Flood::Host(nullptr)
{
    NativePtr = new ::HostServer();
}

