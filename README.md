# Interactive 3D Solar System and Space Exploration Simulator

An interactive, real-time 3D simulation of the Solar System and space exploration, built using standard C, OpenGL, and FreeGLUT.

---

## Current Status

**Week 6 - Lighting & Shading**

The project refines the fixed-function OpenGL illumination and material system to create deep, realistic 3D planetary shading while preserving photorealistic textures, dynamic animations, and camera navigation:
* **Differentiated Planetary Materials**: Calibrated Phong-style material reflection coefficients (ambient, diffuse, specular, and shininess) for each celestial body:
  - **Mercury**: Airless, heavily cratered silicate rock (very low specular $0.08$, low shininess $6.0$).
  - **Venus**: Dense, highly reflective sulfuric acid clouds with high albedo and soft cloud sheen (specular $0.35$, shininess $22.0$).
  - **Earth**: Distinct oceanic specular glint / sheen across 71% water coverage (high specular $0.50$, focused shininess $38.0$).
  - **Moon**: Fine silicate regolith with broad matte scattering (specular $0.06$, shininess $4.0$).
  - **Mars**: Dry, dusty iron-oxide basalt surface with matte reflection (specular $0.12$, shininess $10.0$).
  - **Jupiter**: Gaseous cloud bands with smooth atmospheric sheen (specular $0.25$, shininess $18.0$).
  - **Saturn**: Banded atmosphere with soft ammonia haze dispersion (specular $0.22$, shininess $16.0$).
  - **Uranus & Neptune**: Smooth icy methane atmospheres with crisp aerosol highlights (specular $0.30 - 0.32$, shininess $26.0 - 28.0$).
* **Clear Day / Night Hemispheres**: The central solar light (`GL_LIGHT0` at origin) dynamically illuminates each planet's inward-facing surface, creating a clearly defined subsolar point, transition terminator, and shaded night side.
* **Non-Black Night Sides**: Combined `GL_LIGHT_MODEL_AMBIENT` and light ambient terms maintain a subtle baseline ambient illumination ($\sim 0.20$), allowing continents, cloud bands, and planetary surface details to remain visible on the night side.
* **Local Viewer Specular Accuracy**: Configured `GL_LIGHT_MODEL_LOCAL_VIEWER` to calculate specular reflection vectors from the actual camera/eye position rather than an infinite $+Z$ viewer, ensuring specular highlights react naturally during camera orbits and zooms.
* **Gouraud Smooth Shading (`GL_SMOOTH`)**: Enabled smooth color interpolation across polygon faces, preventing faceted mesh appearances on spheres.
* **Preserved Luminous Textured Sun**: The Sun renders self-luminously with photographic photosphere textures, while `GL_LIGHT0` anchored at the origin casts light onto surrounding planets.
* **Strict State Isolation**: Orbit lines, Saturn rings, and cosmic starfield background remain unlit, preventing lighting or texture bleeding.

---

## Technology Stack

* **Language**: C (C99 standard)
* **Graphics API**: OpenGL (Desktop OpenGL with GLU)
* **Window & Event Management**: FreeGLUT (v3.8.0)
* **Image Loading**: `stb_image.h` (v2.30, public domain / MIT by Sean Barrett)
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
│   ├── sphere.c        # 3D sphere generator with normals and spherical UV mapping
│   ├── lighting.c      # Fixed-function OpenGL lighting initialization and positioning
│   ├── sun.c           # Central textured Sun rendering and axial rotation
│   ├── planet.c        # Dynamic 8-planet data structure, Moon hierarchy, and textures
│   ├── orbit.c         # Circular parametric orbit line rendering on X-Z plane
│   └── texture.c       # Texture loading, binding, mipmapping, and cleanup subsystem
├── include/
│   ├── camera.h        # Camera interface
│   ├── sphere.h        # Sphere renderer interface
│   ├── lighting.h      # Lighting subsystem interface
│   ├── sun.h           # Sun subsystem interface
│   ├── planet.h        # Planet subsystem interface and Moon accessors
│   ├── orbit.h         # Orbit subsystem interface
│   ├── texture.h       # Texture subsystem interface
│   └── stb_image.h     # Single-header image loader
├── textures/           # High-resolution planetary and celestial surface textures
│   ├── sun.jpg         # Solar photosphere map
│   ├── mercury.jpg     # Mercury surface map
│   ├── venus.jpg       # Venus atmosphere map
│   ├── earth.jpg       # Earth daytime surface map
│   ├── moon.jpg        # Lunar surface map
│   ├── mars.jpg        # Mars surface map
│   ├── jupiter.jpg     # Jupiter cloud bands and Great Red Spot map
│   ├── saturn.jpg      # Saturn cloud bands map
│   ├── uranus.jpg      # Uranus atmosphere map
│   ├── neptune.jpg     # Neptune atmosphere map
│   ├── stars.jpg       # Milky Way cosmic starfield map
│   └── saturn_ring.png # Saturn ring texture profile
├── docs/               # Technical guides and Computer Graphics concepts documentation
│   ├── week6_concepts.md # Comprehensive guide: Phong lighting model, materials, local viewer
│   ├── week5_concepts.md # Comprehensive guide: UV mapping, mipmaps, texture/light interaction
│   ├── texture_sources.md # Texture provenance, NASA mission sources, and CC BY 4.0 licenses
│   ├── week4_concepts.md # Week 4 guide: delta time, kinematics, Moon hierarchy
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
| `1` – `8` | Focus Camera on Selected Planet (Mercury to Neptune) |
| `0`       | Clear Focus and Return to Full Solar System Overview |
| `R` / `r` | Reset Camera to Default Overview `(0, 16, 28)` |
| `ESC` / `Q` / `q` | Exit Simulator Cleanly |

---

## Build & Run Instructions

Using Windows Batch Scripts (Recommended)

1. **Build**:
   ```cmd
   build.bat
   ```
2. **Run**:
   ```cmd
   run.bat
   ```
