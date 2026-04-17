#include <Engine.h>

#include <memory>
#include <functional>

class GameAppInstance : public Engine::ApplicationInstance
{
    virtual std::string GetNameApp() override {return "Game";}
};

std::unique_ptr<Engine::ApplicationInstance> GenerateApplicationInstance()
{
    return std::make_unique<GameAppInstance>();
}