#include "Application.hpp"

int main() {
    AppSpecs specs {
        .width = 1280,
        .height = 720,
        .title = "BepInFetch",
    };

    Application app(specs);
    app.Run();

    return 0;
}
