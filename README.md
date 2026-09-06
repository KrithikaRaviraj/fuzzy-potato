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
