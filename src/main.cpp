#include "Core/Application.h"
#include "Core/UI/Color.h"
#include "LightApp.h"
#include <vector>
#include <cmath>

int main(int argc, char* argv[]) {
    LightApp app;
    app.SetWindowTitle("Simulateur Lumiere Additive SDL3");
    app.SetWindowSize(800, 600);

    if (app.Initialize()) {
        app.Run();
    }

    return 0;
}