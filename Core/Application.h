#pragma once
#include "log.h"
#include <string>
#include <SDL3/SDL.h>

class Application {
       
private:
    

protected:
    Log* log = nullptr; // On passera par un pointeur ou une instance gérée

public:
// Ressources SDL3
    SDL_Window* mWindow = nullptr;
    SDL_Renderer* mRenderer = nullptr;

    // État de l'application
    bool mIsRunning = false;
    bool mIsInitialized = false;

    // Propriétés de la fenêtre
    const char* mWindowTitle = "Light Simulator 2D";
    int mWindowWidth = 800;
    int mWindowHeight = 600;

    Application();
    virtual ~Application(); // Toujours virtuel pour une classe parente

    bool Initialize();
    void Run();
    void Close();

    // Méthodes à redéfinir dans les classes enfants
    virtual void Setup() {}
    virtual void Update(float deltaTime) { (void)deltaTime; }
    virtual void Draw() {}

    // Utilitaires
    void SetWindowSize(int width, int height);
    void SetWindowTitle(const char* title);
};