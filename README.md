# Interactive 3D Solar System and Space Exploration Simulator

An interactive, real-time 3D simulation of the Solar System and space exploration, built using standard C, OpenGL, and FreeGLUT.

---

## Current Status

**Week 5 - Texture Mapping**

The project introduces photorealistic 2D texture mapping across all celestial bodies while fully preserving all dynamic kinematics, hierarchy, lighting, and camera systems:
* **All 8 Textured Planets**: Authentic equirectangular photographic surface maps for Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, and Neptune (sourced from NASA / Solar System Scope).
* **Luminous Textured Sun**: The Sun is mapped with high-resolution solar photosphere imagery, showcasing surface granulation, flares, sunspots, and natural limb darkening, eliminating the uniform flat-yellow appearance.
* **Textured Moon**: High-resolution lunar surface map showing craters and lunar maria, orbiting Earth in a multi-level hierarchy.
* **Starfield Cosmic Background**: Immersive panoramic Milky Way starfield background sphere rendered behind all scene elements.
* **Spherical UV Mapping**: Reusable parametric 3D sphere generator with analytical surface normals and equirectangular spherical $(u, v)$ coordinates.
* **Trilinear Mipmapping & Filtering**: Full mipmap pyramid generation via `gluBuild2DMipmaps`, configured with `GL_LINEAR_MIPMAP_LINEAR` minification to eliminate aliasing and moiré shimmering at all distances.
* **Dynamic Day/Night Lighting Integration**: Fixed-function `GL_LIGHT0` point lighting modulates texture colors (`GL_MODULATE`), dynamically illuminating sunlit hemispheres while casting natural shadows on the night side.
* **Preserved Dynamic Kinematics**: All Week 4 orbital revolution, axial rotation, Earth-Moon hierarchy, and Saturn ring geometry remain intact.
* **Strict State Management**: Clean OpenGL state isolation prevents texture bleeding onto non-textured geometry (orbits and Saturn rings).
* **Robust Resource Management**: Textures load once at initialization into GPU memory, and are freed cleanly on simulator exit.

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
