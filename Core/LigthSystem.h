#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "UI/Color.h"
#include "Core/Application.h"

// Un segment pour bloquer la lumière
struct Edge { SDL_FPoint p1, p2; };

// Obstacle de base (Carré/Rectangle) // a develloper pour cercle et triangle et polygone
class Obstacle {
public:
    SDL_FRect rect;
    Color color;
    
    Obstacle(float x, float y, float w, float h, Color c) 
        : rect({x, y, w, h}), color(c) {}

    // Retourne les 4 bords pour le calcul d'ombre
    std::vector<Edge> getEdges() const {
        return {
            {{rect.x, rect.y}, {rect.x + rect.w, rect.y}},
            {{rect.x + rect.w, rect.y}, {rect.x + rect.w, rect.y + rect.h}},
            {{rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h}},
            {{rect.x, rect.y + rect.h}, {rect.x, rect.y}}
        };
    }
};

// Source de Lumière
class PointLight {
public:
    SDL_FPoint pos;
    Color color;
    float radius;
    float intensity; // 0.0f à 1.0f

    PointLight(float x, float y, float r, Color c, float i = 1.0f)
        : pos({x, y}), radius(r), color(c), intensity(i) {}
};

