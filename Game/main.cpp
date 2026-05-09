#include "application.h"

int main() {
    Application app(1280, 720, "Shrine Scene Scaffold");
    if (!app.initialize()) {
        return -1;
    }

    app.run();
    return 0;
}
