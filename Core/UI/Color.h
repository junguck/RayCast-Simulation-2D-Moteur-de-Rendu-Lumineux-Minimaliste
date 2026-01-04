#pragma once
#include <SDL3/SDL.h>

struct Color {
    uint8_t r, g, b, a;

    // Couleurs prédéfinies
    static Color Red() { return {255, 0, 0, 255}; }
    static Color Green() { return {0, 255, 0, 255}; }
    static Color Blue() { return {0, 0, 255, 255}; }
    static Color White() { return {255, 255, 255, 255}; }
    
    // Convertir en format SDL_Color
    SDL_Color toSDL() const { return {r, g, b, a}; }
};