#include "Precompiled.h"
#include "SoundEffectComponent.h"

#include "SaveUtil.h"

using namespace MEngine;
using namespace MEngine::Audio;

void MEngine::SoundEffectComponent::Initialize()
{
    ASSERT(!mFileName.empty(), "SoundEffectComponent: no sound file loaded");
    mSoundId = SoundEffectManager::Get()->Load(mFileName);
}

void MEngine::SoundEffectComponent::Terminate()
{
    Stop();
}

void MEngine::SoundEffectComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);
    SaveUtil::SaveString("FileName", mFileName.c_str(), doc, componentValue);
    SaveUtil::SaveBool("Looping", mLooping, doc, componentValue);
    value.AddMember("SoundEffectComponent", componentValue, doc.GetAllocator());
}

void MEngine::SoundEffectComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("FileName"))
    {
        mFileName = value["FileName"].GetString();
    }
    if (value.HasMember("Looping"))
    {
        mLooping = value["Looping"].GetBool();
    }
}

void MEngine::SoundEffectComponent::DebugUI()
{
    ImGui::Text(mFileName.c_str());
    if (ImGui::Button("Play"))
    {
        Play();
    }
    if (ImGui::Button("Stop"))
    {
        Stop();
    }
}

void MEngine::SoundEffectComponent::Play()
{
    SoundEffectManager::Get()->Play(mSoundId);
}

void MEngine::SoundEffectComponent::Stop()
{
    SoundEffectManager::Get()->Stop(mSoundId);
}
