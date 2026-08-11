# Shrine Scene Setup

## Current scope

- Standalone CMake target using GLFW + GLAD + OpenGL + GLM.
- Free camera navigation.
- Scene renderer with shrine yard layout.
- Basic ambient + diffuse + specular lighting.
- Procedural cubemap skybox.
- Reflective/refractive pond water.
- Lantern toggle interaction placeholder.
- frame-buffer and toon shading

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

* CMake uses 3rdParty dependencies from the exercise folder; put 3rdParty in the root folder before building. And make sure glad, glfw, glm, and stb are not empty.
