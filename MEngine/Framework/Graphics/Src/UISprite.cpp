#include "Precompiled.h"
#include "UISprite.h"

using namespace MEngine;
using namespace MEngine::Graphics;

void UISprite::Initialize(const std::filesystem::path& filePath)
{
	TextureManager* tm = TextureManager::Get();
	mTextureId = tm->LoadTexture(filePath);
	const Texture* texture = tm->GetTexture(mTextureId);
	ASSERT(texture != nullptr, "UISprite: texture [%s] was not found", filePath.u8string().c_str());
	SetRect(0, 0, texture->GetWidth(), texture->GetHeight());
}

void UISprite::Terminate()
{
}

void UISprite::SetPosition(const Math::Vector2& position)
{
	mPosition.x = position.x;
	mPosition.y = position.y;
}

void UISprite::SetScale(const Math::Vector2& scale)
{
	mScale.x = scale.x;
	mScale.y = scale.y;
}

void UISprite::SetRect(uint32_t top, uint32_t left, uint32_t right, uint32_t bottom)
{
	mRect.top = top;
	mRect.left = left;
	mRect.right = right;
	mRect.bottom = bottom;
	UpdateOrigin();
}

void UISprite::SetPivot(Pivot pivot)
{
	mPivot = pivot;
	UpdateOrigin();
}

void UISprite::SetFlip(Flip flip)
{
	switch (flip)
	{
	case Flip::None: mFlip = DirectX::SpriteEffects_None; break;
	case Flip::Horizontal: mFlip = DirectX::SpriteEffects_FlipHorizontally; break;
	case Flip::Vertical: mFlip = DirectX::SpriteEffects_FlipVertically; break;
	case Flip::Both: mFlip = DirectX::SpriteEffects_FlipBoth; break;
	}
}

void UISprite::SetColor(const Color& color)
{
}

void UISprite::SetRotation(float rotation)
{
}

bool UISprite::IsInSprite(float x, float y)
{
	return false;
}

void UISprite::UpdateOrigin()
{
}
