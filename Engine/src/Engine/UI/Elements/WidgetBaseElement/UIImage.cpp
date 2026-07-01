#include "UIImage.h"

#include "BaseEngine.h"
#include "Engine/Render/Renderer2D.h"
#include "Engine/Render/Font/Font.h"
#include "Engine/Render/Renderer.h"

#include "Engine/Render/RenderCommand.h"

namespace Engine::UI 
{
    using namespace Engine::Render;

    UIImage::UIImage(const std::string &InImageAsset)
    {
        m_Texture = Texture2D::Create("/Resources/Textures/" + InImageAsset);
    }

    void UIImage::OnRender()
    {
         if (!IsVisible() || !m_Texture) return;

        UIWidget::OnRender();

        const Vector2 pos = GetComputedPosition();
        const Vector2 size = GetComputedSize();

        //const TRef<Texture2D> LTexture = m_Texture;
        Renderer2D::DrawQuad(pos,size, m_Texture,1.f,TColor::White);
    }
    
    void UIImage::OnUpdate(float deltaTime)
    {
    }
}