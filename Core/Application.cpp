#include "Application.h"

Application::Application() {
    log = new Log(); // Initialisation du logger
}

Application::~Application() {
    Close();
    if (log) delete log;
}

bool Application::Initialize() {
    if (mIsInitialized) return true;

    log->message("Initialisation de la SDL3...");

    // 1. Initialisation Vidéo
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return false;
    }

    // 2. Création fenêtre + Renderer
    // Correction : Largeur d'abord, puis Hauteur
    if (!SDL_CreateWindowAndRenderer(mWindowTitle, mWindowWidth, mWindowHeight, 0, &mWindow, &mRenderer)) {
        log->erreur("Erreur Creation Fenetre/Renderer");
        return false;
    }

    mIsInitialized = true;
    log->message("Fenêtre créée avec succès.");
    return true;
}

void Application::Run() {
    if (!mIsInitialized) {
        if (!Initialize()) return;
    }

    mIsRunning = true;
    Setup(); // Appel utilisateur unique

    SDL_Event event;
    uint64_t lastTime = SDL_GetTicks(); // Pour le deltaTime

    while (mIsRunning) {
        // 1. Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) mIsRunning = false;
        }

        // 2. Calcul du temps écoulé (DeltaTime)
        uint64_t currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // 3. Mise à jour de la logique
        Update(deltaTime);

        // 4. Rendu
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // Fond Noir
        SDL_RenderClear(mRenderer);

        Draw();

        SDL_RenderPresent(mRenderer);
    }
}

void Application::Close() {
    if (mRenderer) {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
    if (mWindow) {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    SDL_Quit();
    mIsInitialized = false;
    log->message("Application fermée proprement.");
}

void Application::SetWindowSize(int width, int height) {
    mWindowWidth = width; mWindowHeight = height;
    if (mWindow) SDL_SetWindowSize(mWindow, width, height);
}


void Application::SetWindowTitle(const char* title) {
    mWindowTitle = title;
    if (mWindow) {
        SDL_SetWindowTitle(mWindow, title);
    }
}
