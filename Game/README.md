# Shrine Scene Setup

## Current scope

- Standalone CMake target using GLFW + GLAD + OpenGL + GLM.
- Free camera navigation.
- Scene renderer with placeholder shrine yard layout.
- Basic ambient + diffuse + specular lighting.
- Lantern toggle interaction placeholder.

## Controls

- `W/A/S/D`: move camera
- `Q/E`: down/up
- `Mouse`: look around
- `Mouse wheel`: zoom
- `Left Shift`: sprint
- `L`: toggle lantern light
- `Esc`: quit

## Build and run

```powershell
cmake -S Game -B build/game
cmake --build build/game --config Release
cd build/game/Release
.\ShrineScene.exe
```

## Asset folders

- `assets/models`
- `assets/textures`
- `assets/cubemaps`

These are ready for model and texture import in the next step.

* CMake uses 3rdParty dependencies from the exercise folder; put info-h502 in the root folder before building. And make sure glad, glfw, glm, and stb are not empty, otherwise you can clone them as stated in the exercises on GitLab.
