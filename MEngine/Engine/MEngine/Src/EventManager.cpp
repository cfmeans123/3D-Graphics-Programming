#include "Precompiled.h"
#include "EventManager.h"

using namespace MEngine;

void MEngine::EventManager::StaticInitialize()
{
	ASSERT(sEventManager == nullptr, "EventManager: is alreadt initialized");
	sEventManager = std::make_unique<EventManager>();
	sEventManager->Initialize();
}

void MEngine::EventManager::StaticTerminate()
{
	if (sEventManager != nullptr)
	{
		sEventManager->Terminate();
		sEventManager.reset();
	}
}

EventManager* MEngine::EventManager::Get()
{
	ASSERT(sEventManager != nullptr, "EventManager: is not initialized");
	return sEventManager.get();
}

void MEngine::EventManager::Broadcast(const Event* event)
{
}

MEngine::EventManager::~EventManager()
{
}

void MEngine::EventManager::Initialize()
{
	mEventListeners.clear();
}

void MEngine::EventManager::Terminate()
{
	mEventListeners.clear();
}

uint32_t MEngine::EventManager::AddListener(EventType eventType, const EventCallback& cb)
{
	mEventListeners[eventType][++mListenerId] = cb;
	return mListenerId;
}

void MEngine::EventManager::RemoveListener(EventType eventType, uint32_t listenerId)
{
	auto listenersIter = mEventListeners.find(eventType);
	if (listenersIter != mEventListeners.end())
	{
		auto iter = listenersIter->second.find(listenerId);
		if (iter != listenersIter->second.end())
		{
			listenersIter->second.erase(iter);
		}
	}
}

void MEngine::EventManager::BroadcastPrivate(const Event* event)
{
	auto& listeners = mEventListeners[event->GetType()];
	for (auto& cb : listeners)
	{
		cb.second(event);
	}
}
