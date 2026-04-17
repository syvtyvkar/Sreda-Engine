#include "EditorEngineInstance.h"

void EditorAppInstance::OnStart()
{
    __super::OnStart();
    m_EditorInputHotKey = std::make_unique<EditorInputComponent>();
    m_EditorInputHotKey.get()->Init();
}

void EditorAppInstance::OnEnd()
{
    __super::OnEnd();
    m_EditorInputHotKey.get()->DeInit();
    m_EditorInputHotKey.reset();
}
