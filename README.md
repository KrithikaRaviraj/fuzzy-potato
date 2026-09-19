# Interactive 3D Solar System and Space Exploration Simulator

An interactive, real-time 3D simulation of the Solar System and space exploration, built using standard C, OpenGL, and FreeGLUT.

---

## Current Status

**Week 3 - Complete Basic Solar System**

The project has transitioned from the central Sun foundation to the complete basic Solar System scene:
* **All 8 Planets**: Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, and Neptune rendered as solid 3D spheres.
* **Planetary Orbits**: Eight circular orbit paths rendered on the horizontal X-Z plane (`y = 0`) using smooth parametric line loops.
* **Relative Scale & Spacing**: Normalized educational visualization scale preserving relative planetary size relationships and strictly increasing orbital distances outward from the Sun.
* **Distinct Colors**: Clearly distinguishable, physically inspired base colors for every celestial body.
* **Saturn's Ring**: Clean geometric representation of Saturn's tilted planetary ring system using basic OpenGL geometry.
* **Point-Light Illumination**: Fixed-function OpenGL point light (`GL_LIGHT0`) positioned at the Sun's center, producing realistic day/night hemispherical illumination on planets.
* **Camera & Viewport**: Elevated oblique perspective view `(0.0, 16.0, 28.0)` providing an immediate, unclipped overview of all 8 planets and their orbital paths.

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
│   ├── main.c          # Application entry point and FreeGLUT event loop
│   ├── camera.c        # Camera navigation and view matrix management
│   ├── sphere.c        # Reusable 3D sphere rendering abstraction with normals
│   ├── lighting.c      # Fixed-function OpenGL lighting initialization and positioning
│   ├── sun.c           # Central Sun 3D object rendering and material properties
│   ├── planet.c        # Reusable 8-planet data structure, rendering, and Saturn ring
│   └── orbit.c         # Circular parametric orbit line rendering on X-Z plane
├── include/
│   ├── camera.h        # Camera interface
│   ├── sphere.h        # Reusable sphere renderer interface
│   ├── lighting.h      # Lighting subsystem interface
│   ├── sun.h           # Sun subsystem interface
│   ├── planet.h        # Planet subsystem interface and data structures
│   └── orbit.h         # Orbit subsystem interface
├── textures/           # Surface textures and skybox maps (reserved for future weeks)
├── assets/             # 3D object models and simulation assets (reserved for future weeks)
├── docs/               # Technical guides and Computer Graphics concepts documentation
│   ├── week3_concepts.md # Comprehensive CG guide: hierarchical modeling, orbits, lighting
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
