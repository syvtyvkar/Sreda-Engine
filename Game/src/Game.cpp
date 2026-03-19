#include <Engine.h>

class GameLayer : public Engine::EngineLayer
{
    public:
        GameLayer() = default;
        virtual ~GameLayer() = default;
        virtual void OnInit(){};
        virtual void DeInit(){};
        virtual void Update(float DeltaTime){};
        virtual void OnRender(){};
};

Engine::EngineLayer* GenerateGame()
{
    Engine::EngineLayer* NewLayer = new GameLayer();
    return NewLayer;
};