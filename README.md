# Interactive 3D Solar System and Space Exploration Simulator

An interactive, real-time 3D simulation of the Solar System and space exploration, built using standard C, OpenGL, and FreeGLUT.

---

## Technology Stack

* **Language**: C (C99 standard)
* **Graphics API**: OpenGL (Desktop OpenGL with GLU)
* **Window & Event Management**: FreeGLUT (v3.8.0)
* **Compiler / Toolchain**: GCC 15.2.0 (MSYS2 UCRT64 `x86_64-w64-mingw32`)
* **Version Control**: Git + GitHub

---

## Current Status

**Week 1 — Setup & OpenGL Foundation**

This project is currently in its Week 1 milestone. The core focus is establishing a verified C compiler setup, configuring OpenGL development libraries, integrating FreeGLUT, and implementing the fundamental 3D rendering pipeline.

---

## Project Structure

```text
fuzzy-potato/
│
├── src/            # C source code files (application entry, rendering logic)
├── include/        # Header files and module interface definitions (.h)
├── textures/       # Surface textures and skybox maps (for future stages)
├── assets/         # 3D object models, planetary data, and simulator assets
├── docs/           # Technical setup guides and verification documentation
├── bin/            # Output binary directory (ignored by git)
├── build.bat       # Windows one-click build script
├── run.bat         # Windows one-click launch script
├── Makefile        # GNU Make build configuration
├── README.md       # Project overview, documentation, and build instructions
└── .gitignore      # Git exclusion rules for compiled artifacts and editor configs
```

### Directory Purposes

* **`src/`**: Contains the C implementation files. Currently includes `main.c`, which initializes FreeGLUT, creates the application window, configures OpenGL 3D state, and manages display/reshape/keyboard callbacks.
* **`include/`**: Reserved for modular header files as features expand in upcoming weeks (e.g., camera definitions, planet data structures, math utilities).
* **`textures/`**: Dedicated folder for planet surface textures, rings, and background starry skybox images.
* **`assets/`**: Dedicated directory for 3D model meshes, mission trajectory data, and other space assets.
* **`docs/`**: Technical documentation detailing compiler configuration (`compiler_setup.md`), OpenGL development setup (`opengl_setup.md`), and FreeGLUT configuration (`freeglut_setup.md`).

---

## Build and Run Instructions

The project is configured and tested on Windows using **MSYS2 UCRT64** with **GCC**.

### Prerequisites

Ensure MSYS2 UCRT64 toolchain and FreeGLUT are installed:
* GCC compiler: `C:\msys64\ucrt64\bin\gcc.exe`
* FreeGLUT package: `mingw-w64-ucrt-x86_64-freeglut`

### Option 1: Using the Automated Scripts (Recommended on Windows)

1. **Build the project**:
   ```cmd
   build.bat
   ```
   This compiles `src/main.c` and produces `bin/solar_sim.exe`.

2. **Run the simulator**:
   ```cmd
   run.bat
   ```

### Option 2: Using GNU Make

```bash
make
make run
```

### Option 3: Direct GCC Compilation (PowerShell / Command Prompt)

1. Add the MSYS2 UCRT64 toolchain to your session path:
   ```powershell
   $env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH
   ```
2. Compile the application:
   ```powershell
   gcc -Wall -Wextra -std=c99 src/main.c -Iinclude -o bin/solar_sim.exe -lfreeglut -lopengl32 -lglu32
   ```
3. Run the binary:
   ```powershell
   .\bin\solar_sim.exe
   ```

### Controls

* **`ESC`** or **`q` / `Q`**: Cleanly exit the simulator.

---

## Current Functionality

The Week 1 implementation provides a clean, robust 3D graphics foundation:

* **FreeGLUT Windowing**: Initializes FreeGLUT, configures double buffering (`GLUT_DOUBLE`), RGB color (`GLUT_RGB`), and a depth buffer (`GLUT_DEPTH`).
* **Clean Event Loop**: Manages window lifecycle with graceful exit support via `glutSetOption` and `glutLeaveMainLoop`.
* **3D Depth Testing**: Enables hardware depth buffering (`glEnable(GL_DEPTH_TEST)` and `glDepthFunc(GL_LEQUAL)`) to ensure correct spatial occlusion.
* **Perspective Projection**: Configures a 3D perspective frustum using `gluPerspective(45.0, aspect, 0.1, 100.0)`.
* **Modelview & Camera Setup**: Sets a camera position using `gluLookAt` overlooking the origin.
* **3D Geometry Rendering**: Renders a 3D wireframe primitive tilted in 3D space to verify depth, perspective, and wireframe rasterization.
* **Window Resizing**: Implements a reshape callback (`glutReshapeFunc`) that dynamically updates the OpenGL viewport and recalculates the perspective aspect ratio upon window resizing without distortion.

---

## Future Development Roadmap

In subsequent weeks, the simulator will be expanded to include:

* **Solar System Model**: Sun and all 8 major planets (Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune).
* **Planetary Dynamics**: Accurate orbital paths, orbital revolution, and axial rotation calculations.
* **Natural Satellites**: The Moon orbiting Earth and key planetary moons.
* **Texture Mapping**: Realistic planetary surface textures and deep-space celestial skybox.
* **Lighting & Shading**: Positional lighting emitted from the Sun, ambient space lighting, and material specular/diffuse properties.
* **Camera System**: Free exploration camera (orbital rotation, pan, zoom) and cockpit/first-person view.
* **Interactive Exploration**: Planet selection, targeted zoom, orbital data overlays, and information panels.
* **Spacecraft & Missions**: Controllable spacecraft with realistic trajectory simulation and space exploration missions.
* **Celestial Features**: Asteroid belt (Kuiper belt / main asteroid belt) with particle and instanced geometry rendering.
