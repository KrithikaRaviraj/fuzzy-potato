# Interactive 3D Solar System and Space Exploration Simulator

An interactive, real-time 3D simulation of the Solar System and space exploration, built using standard C, OpenGL, and FreeGLUT.

---

## Current Status

**Week 4 - Rotation, Revolution & Moon**

The project has introduced dynamic kinematics, orbital revolution, axial spin, and multi-level hierarchical satellite modeling:
* **Planetary Orbital Revolution**: Continuous orbital revolution of all 8 planets around the Sun along their circular coplanar orbits ($Y = 0$). Revolution speeds follow Keplerian-inspired pedagogical scaling (Mercury fastest at $48^\circ/\text{s}$ down to Neptune at $3.5^\circ/\text{s}$).
* **Planetary Axial Rotation**: Continuous axial spinning of every planet around its own local polar axis.
* **Earth-Moon Hierarchical Modeling**: The Moon is modeled as a child satellite in a multi-level transformation hierarchy (`Sun -> Earth Orbit -> Earth Translation -> Moon Orbit -> Moon Translation -> Moon Spin`).
* **Transformation Matrix Isolation**: Earth's axial rotation is isolated with `glPushMatrix` / `glPopMatrix` so that Earth's fast daily spin does not rotate the Moon's orbital plane.
* **Frame-Rate Independent Animation**: Time-driven animation utilizing FreeGLUT's `glutTimerFunc` (~60 FPS) and wall-clock delta-time calculation (`glutGet(GLUT_ELAPSED_TIME)`), with safe first-frame initialization and stall clamping (max 0.1s).
* **Angle Normalization**: All planetary and lunar orbital/rotational angles wrap safely within $[0^\circ, 360^\circ)$ to prevent floating-point precision degradation.
* **Preserved Week 3 Starting Positions**: Planets start from their Week 3 reference positions, seamlessly animating onward.
* **Dynamic Lighting & Day/Night Cycles**: The central point light (`GL_LIGHT0`) illuminates the inward-facing hemisphere of each revolving planet, producing dynamic day/night cycles and natural lunar phases.

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
* **+Y axis**: Extends upwards (perpendicular to the orbital plane)
* **-Y axis**: Extends downwards
* **+Z axis**: Extends out of the screen (toward the viewer)
* **-Z axis**: Extends into the screen (away from the viewer)
* **Origin `(0, 0, 0)`**: Center of the Solar System, where the Sun resides.
* **Orbital Plane**: The ecliptic horizontal plane ($Y = 0$, X-Z plane).

---

## Project Structure

```text
fuzzy-potato/
│
├── src/
│   ├── main.c          # Application entry point, FreeGLUT timer and event loop
│   ├── camera.c        # Camera navigation and view matrix management
│   ├── sphere.c        # Reusable 3D sphere rendering abstraction with normals
│   ├── lighting.c      # Fixed-function OpenGL lighting initialization and positioning
│   ├── sun.c           # Central Sun 3D object rendering and material properties
│   ├── planet.c        # Dynamic 8-planet data structure, Moon hierarchy, and Saturn ring
│   └── orbit.c         # Circular parametric orbit line rendering on X-Z plane
├── include/
│   ├── camera.h        # Camera interface
│   ├── sphere.h        # Reusable sphere renderer interface
│   ├── lighting.h      # Lighting subsystem interface
│   ├── sun.h           # Sun subsystem interface
│   ├── planet.h        # Planet subsystem interface, animation update, and Moon accessors
│   └── orbit.h         # Orbit subsystem interface
├── textures/           # Surface textures and skybox maps (reserved for future weeks)
├── assets/             # 3D object models and simulation assets (reserved for future weeks)
├── docs/               # Technical guides and Computer Graphics concepts documentation
│   ├── week4_concepts.md # Comprehensive guide: delta time, kinematics, Moon hierarchy
│   ├── week3_concepts.md # Week 3 guide: hierarchical modeling, orbits, lighting
│   ├── week2_concepts.md # Week 2 concepts: coordinate systems, transformations, projection
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
| `W` / `w` | Move Camera Forward (-Z) |
| `S` / `s` | Move Camera Backward (+Z) |
| `A` / `a` | Move Camera Left (-X) |
| `D` / `d` | Move Camera Right (+X) |
| `Z` / `z` | Zoom In / Decrease Viewing Distance |
| `X` / `x` | Zoom Out / Increase Viewing Distance |
| `R` / `r` | Reset Camera to Default Overview `(0, 16, 28)` |
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
