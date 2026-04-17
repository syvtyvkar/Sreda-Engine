#include <Engine.h>

#include <memory>
#include <functional>

class GameAppInstance : public ApplicationInstance
{
    virtual std::string GetNameApp() override {return "Game";}
};

TUniquePtr<ApplicationInstance> GenerateApplicationInstance()
{
    return CreateUniquePtr<GameAppInstance>();
}