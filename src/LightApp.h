#include "Core/Application.h"
#include "Core/UI/Color.h"
#include <vector>
#include <cmath>
#include "Core/Math/MathConstants.h"
// --- STRUCTURES DE SCÈNE ---

Constants math = Constants();
//les objects qui subisse un ombrage
struct Obstacle {
    SDL_FRect rect;
    SDL_Color color;
};

//les point lumineux qui soummette des ombrages
struct Light {
    SDL_FPoint pos;
    SDL_Color color;
    float radius;
    float intensity; // [0.0f - 1.0f]
};

// -----------------------------------------------------------------------------
// Classe : LightApp
// Rôle global :
// LightApp est une application dérivée de Application qui gère une scène
// composée de lumières et d'obstacles. Elle permet de configurer la scène,
// de mettre à jour l'état (interactions utilisateur), et de dessiner les
// lumières, leurs ombres et les obstacles. Elle illustre la gestion
// graphique de sources lumineuses et de projections d'ombres en temps réel
// avec SDL3.
// -----------------------------------------------------------------------------

class LightApp : public Application {
private:
    std::vector<Light> m_lights;       // Liste des lumières présentes dans la scène
    std::vector<Obstacle> m_obstacles; // Liste des obstacles (rectangles) bloquant la lumière
    int m_selectedLightIndex = -1;     // Index de la lumière sélectionnée par l'utilisateur (-1 si aucune)

public:
    // Setup :
    // Configure la scène initiale en ajoutant des obstacles et des lumières.
    // Cette fonction est appelée au démarrage de l'application.
    void Setup() override{
        log->message("Configuration de la scène lumineuse...");

        // Ajout de deux obstacles rectangulaires
        m_obstacles.push_back({{200, 150, 100, 100}, {255, 0, 0, 255}});
        m_obstacles.push_back({{500, 300, 150, 50}, {255, 0, 0, 255}});

        // Ajout d'une lumière initiale (exemple en bleu)
        m_lights.push_back({{600, 150}, {0, 255, 255, 255}, 200, 500.0f});
        // m_lights.push_back({{300, 600}, {255, 0, 255, 255}, 20, 250.0f});
    }

    // Update :
    // Met à jour l'état de la scène en fonction du temps écoulé et des
    // interactions utilisateur. Ici, la logique est à compléter (UI, réglages).
    void Update(float deltaTime) override {
        // réglages utilisateur et UI
        //interaction lumiere souris
    }

    // Draw :
    // Fonction de rendu principale. Dessine les lumières, leurs ombres et
    // les obstacles. Utilise différents modes de mélange pour simuler
    // l'addition des couleurs et la transparence.
    void Draw() override {
        // Dessin des lumières
        for (const auto& light : m_lights) {
            DrawPointLight(light);
        }

        // Dessin des ombres projetées par les obstacles
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        for (const auto& light : m_lights){
            DrawShadows(mRenderer, light, m_obstacles);
        }

        // Dessin des obstacles eux-mêmes
        for (const auto& obs : m_obstacles) {
            SDL_SetRenderDrawColor(mRenderer, obs.color.r, obs.color.g, obs.color.b, 255);
            SDL_RenderFillRect(mRenderer, &obs.rect);
        }

        // Rétablir le mode de mélange par défaut
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    }

    // DrawCircle :
    // Dessine un cercle en utilisant l'algorithme de Bresenham.
    // Paramètres : renderer, coordonnées du centre, rayon et couleur.
    void DrawCircle(SDL_Renderer* mRenderer, int centerX, int centerY, int radius, SDL_Color color) {
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, color.a);
        int x = radius;
        int y = 0;
        int err = 0;

        while (x >= y) {
            SDL_RenderPoint(mRenderer, centerX + x, centerY + y);
            SDL_RenderPoint(mRenderer, centerX + y, centerY + x);
            SDL_RenderPoint(mRenderer, centerX - y, centerY + x);
            SDL_RenderPoint(mRenderer, centerX - x, centerY + y);
            SDL_RenderPoint(mRenderer, centerX - x, centerY - y);
            SDL_RenderPoint(mRenderer, centerX - y, centerY - x);
            SDL_RenderPoint(mRenderer, centerX + y, centerY - x);
            SDL_RenderPoint(mRenderer, centerX + x, centerY - y);

            if (err <= 0) {
                y += 1;
                err += 2*y + 1;
            }
            if (err > 0) {
                x -= 1;
                err -= 2*x + 1;
            }
        }
    }

    // DrawPointLight :
    // Dessine une lumière ponctuelle en utilisant un dégradé radial.
    // La lumière est représentée par des triangles formant un disque
    // avec une couleur centrale opaque et des bords transparents.
    void DrawPointLight(const Light& light) {
        const int segments = 64; 
        std::vector<SDL_Vertex> vertices;

        // Couleur au centre (opaque)
        SDL_FColor centerColor = { 
            light.color.r / 255.0f, 
            light.color.g / 255.0f, 
            light.color.b / 255.0f, 
            1.0f
        };

        // Couleur au bord (transparente)
        SDL_FColor edgeColor = centerColor;
        edgeColor.a = 0.0f; 

        // Construction des triangles du disque lumineux
        for (int i = 0; i < segments; ++i) {
            float angle1 = i * (math.PI * 2.0f) / segments;
            float angle2 = (i + 1) * (math.PI * 2.0f) / segments;

            vertices.push_back({{light.pos.x, light.pos.y}, centerColor, {0,0}});
            vertices.push_back({{
                light.pos.x + std::cos(angle1) * light.intensity, 
                light.pos.y + std::sin(angle1) * light.intensity
            }, edgeColor, {0,0}});
            vertices.push_back({{
                light.pos.x + std::cos(angle2) * light.intensity, 
                light.pos.y + std::sin(angle2) * light.intensity
            }, edgeColor, {0,0}});
        }

        // Rendu du disque lumineux
        SDL_RenderGeometry(mRenderer, nullptr, vertices.data(), (int)vertices.size(), nullptr, 0);

        // Dessin du cercle de contour
        DrawCircle(mRenderer, light.pos.x, light.pos.y, light.radius, light.color);
    }

    // Normalize :
    // Normalise un vecteur (le ramène à une longueur de 1).
    SDL_FPoint Normalize(SDL_FPoint v) {
        float len = std::sqrt(v.x * v.x + v.y * v.y);
        if (len == 0.0f) return {0, 0};
        return {v.x / len, v.y / len};
    }

    // Dot :
    // Calcule le produit scalaire entre deux vecteurs.
    float Dot(SDL_FPoint a, SDL_FPoint b) {
        return a.x * b.x + a.y * b.y;
    }

    // ProjectPoint :
    // Projette un point dans la direction de la lumière à une certaine distance.
    SDL_FPoint ProjectPoint(SDL_FPoint point, SDL_FPoint lightPos, float distance) {
        SDL_FPoint dir = { point.x - lightPos.x, point.y - lightPos.y };
        dir = Normalize(dir);
        return { point.x + dir.x * distance, point.y + dir.y * distance };
    }

    // GetObstacleCorners :
    // Récupère les quatre coins d'un obstacle rectangulaire.
    void GetObstacleCorners(const Obstacle& obs, SDL_FPoint out[4]) {
        out[0] = {obs.rect.x, obs.rect.y};                         // Haut-gauche
        out[1] = {obs.rect.x + obs.rect.w, obs.rect.y};            // Haut-droite
        out[2] = {obs.rect.x + obs.rect.w, obs.rect.y + obs.rect.h}; // Bas-droite
        out[3] = {obs.rect.x, obs.rect.y + obs.rect.h};            // Bas-gauche
    }

    // EdgeCastsShadow :
    // Détermine si une arête d'obstacle projette une ombre par rapport à la position de la lumière.
    bool EdgeCastsShadow(SDL_FPoint a, SDL_FPoint b, SDL_FPoint lightPos) {
        SDL_FPoint edge = {b.x - a.x, b.y - a.y};
        SDL_FPoint normal = { -edge.y, edge.x }; // normale gauche
        SDL_FPoint toLight = { lightPos.x - a.x, lightPos.y - a.y };
        return Dot(normal, toLight) < 0.0f;
    }

       // DrawShadows :
    // Dessine les ombres projetées par les obstacles en fonction de la position de la lumière.
    // Les ombres sont représentées par des triangles noirs semi-transparents qui s'étendent
    // à partir des arêtes des obstacles vers l'extérieur de la scène.
void DrawShadows(SDL_Renderer* renderer, const Light& light, const std::vector<Obstacle>& obstacles) {
    std::vector<SDL_Vertex> vertices;

    // L'intensité définit l'opacité maximale de l'ombre
    float maxAlpha = light.intensity; 
    SDL_FColor colorStart = {0.0f, 0.0f, 0.0f, maxAlpha};
    SDL_FColor colorEnd = {0.0f, 0.0f, 0.0f, 0.0f};

    for (const Obstacle& obs : obstacles) {
        SDL_FPoint corners[4];
        GetObstacleCorners(obs, corners);

        for (int i = 0; i < 4; ++i) {
            SDL_FPoint a = corners[i];
            SDL_FPoint b = corners[(i + 1) % 4];

            if (!EdgeCastsShadow(a, b, light.pos)) continue;

            // Calcul de la longueur de l'ombre proportionnelle à la taille/puissance
            float shadowLength = light.radius; // On utilise la portée de la lumière

            // Projection standard
            SDL_FPoint aProj = ProjectPoint(a, light.pos, shadowLength);
            SDL_FPoint bProj = ProjectPoint(b, light.pos, shadowLength);

            // --- AJOUT DE LA PÉNOMBRE (basé sur light.radius) ---
            // On décale légèrement les projections pour simuler une source large
            SDL_FPoint aProjSoft = ProjectPoint(a, {light.pos.x - light.radius, light.pos.y}, shadowLength);
            SDL_FPoint bProjSoft = ProjectPoint(b, {light.pos.x + light.radius, light.pos.y}, shadowLength);

            // Triangle 1 (Core)
            vertices.push_back({a, colorStart, {0, 0}});
            vertices.push_back({b, colorStart, {0, 0}});
            vertices.push_back({bProj, colorEnd, {0, 0}});

            // Triangle 2 (Core)
            vertices.push_back({a, colorStart, {0, 0}});
            vertices.push_back({bProj, colorEnd, {0, 0}});
            vertices.push_back({aProj, colorEnd, {0, 0}});
            
            // Note: En mode "propre", on ajouterait des triangles de dégradé 
            // sur les côtés aProj/aProjSoft pour l'effet de flou.
        }
    }

    SDL_RenderGeometry(renderer, nullptr, vertices.data(), (int)vertices.size(), nullptr, 0);
}

    // PointInLight :
    // Vérifie si un point (par exemple la position de la souris) se trouve à l'intérieur
    // de l'aire d'influence d'une lumière. Utile pour sélectionner ou interagir avec une lumière.
    bool PointInLight(const SDL_FPoint& p, const Light& light) {
        float dx = p.x - light.pos.x;
        float dy = p.y - light.pos.y;

        // Compare la distance au carré avec le rayon d'influence de la lumière
        return (dx * dx + dy * dy) <= (light.intensity/2 * light.intensity/2);
    }
};
