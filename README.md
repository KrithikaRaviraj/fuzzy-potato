# Interactive 3D Solar System and Space Exploration Simulator

An interactive, real-time 3D simulation of the Solar System and space exploration, built using standard C, OpenGL, and FreeGLUT.

---

## Current Status

**Week 2 — 3D Scene & First Objects**

The project has transitioned from initial setup to establishing the core 3D scene foundation:
* **3D Coordinate System**: Consistent right-handed Cartesian coordinate system with origin at the Solar System center.
* **Perspective Projection**: Dynamic perspective projection responding smoothly to window resizing.
* **Reusable Sphere Renderer**: Generic sphere drawing module (`sphere.h` / `sphere.c`) generating outward-facing vertex normals.
* **Sun (Central Object)**: Central 3D sphere rendered at `(0, 0, 0)` with isolated model transformations and self-luminous emission.
* **Basic Lighting**: Fixed-function OpenGL lighting setup (`GL_LIGHT0`, `GL_NORMALIZE`, ambient and diffuse components).
* **Camera Integration**: Seamless integration with Akshatha's Week 1 camera navigation system.

---

## Technology Stack

* **Language**: C (C99 standard)
* **Graphics API**: OpenGL (Desktop OpenGL with GLU)
* **Window & Event Management**: FreeGLUT (v3.8.0)
* **Compiler / Toolchain**: GCC 15.2.0 (MSYS2 UCRT64 `x86_64-w64-mingw32`)
* **Version Control**: Git + GitHub

---

## 3D Coordinate System Convention

The project employs the standard **OpenGL Right-Handed Cartesian Coordinate System**:

* **+X axis**: Extends to the right
* **-X axis**: Extends to the left
* **+Y axis**: Extends upwards
* **-Y axis**: Extends downwards
* **+Z axis**: Extends out of the screen (toward the viewer)
* **-Z axis**: Extends into the screen (away from the viewer)
* **Origin `(0, 0, 0)`**: Center of the Solar System, where the Sun resides.

---

## Project Structure

```text
fuzzy-potato/
│
├── src/
│   ├── main.c          # Application entry point and FreeGLUT event loop
│   ├── camera.c        # Camera navigation and view matrix management
│   ├── sphere.c        # Reusable 3D sphere rendering abstraction with normals
│   ├── lighting.c      # Fixed-function OpenGL lighting initialization and positioning
│   └── sun.c           # Central Sun 3D object rendering and material properties
├── include/
│   ├── camera.h        # Camera interface
│   ├── sphere.h        # Reusable sphere renderer interface
│   ├── lighting.h      # Lighting subsystem interface
│   └── sun.h           # Sun subsystem interface
├── textures/           # Surface textures and skybox maps (reserved for future weeks)
├── assets/             # 3D object models and simulation assets (reserved for future weeks)
├── docs/               # Technical guides and Computer Graphics concepts documentation
│   ├── week2_concepts.md # Detailed CG concepts: transformations, lighting, projection
│   ├── compiler_setup.md # MSYS2 GCC UCRT64 environment guide
│   ├── opengl_setup.md   # OpenGL configuration guide
│   └── freeglut_setup.md # FreeGLUT configuration guide
├── bin/                # Compiled executable binaries (git ignored)
├── build.bat           # Windows one-click build script
├── run.bat             # Windows one-click execution script
├── Makefile            # GNU Make build configuration
├── README.md           # Project documentation and current status
└── .gitignore          # Git exclusion rules
```

---

## Controls

| Key | Action |
|---|---|
| `W` / `w` | Move Camera Forward (+Z) |
| `S` / `s` | Move Camera Backward (-Z) |
| `A` / `a` | Move Camera Left (-X) |
| `D` / `d` | Move Camera Right (+X) |
| `R` / `r` | Reset Camera to Default Position `(0, 2, 5)` |
| `ESC` / `Q` / `q` | Exit Simulator Cleanly |

---

## Build & Run Instructions

### Option 1: Using Windows Batch Scripts (Recommended)

1. **Build**:
   ```cmd
   build.bat
   ```
2. **Run**:
   ```cmd
   run.bat
   ```

### Option 2: Using GNU Make

```bash
make
make run
```
