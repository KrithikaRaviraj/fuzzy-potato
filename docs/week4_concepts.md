# Computer Graphics Concepts — Week 4: Rotation, Revolution & Moon

This document provides a comprehensive theoretical and technical guide for the graphics concepts implemented in **Week 4** of the **Interactive 3D Solar System and Space Exploration Simulator**. It serves as an architectural reference and preparation for academic project evaluation (viva voce).

---

## 1. Overview of Week 4 Dynamics

In Week 3, the simulator established the spatial composition of the complete Solar System in static equilibrium. **Week 4** transforms this static model into a dynamic, physics-inspired kinematic simulation featuring:
1. **Planetary Orbital Revolution**: Continuous orbital motion around the Sun along circular paths on the ecliptic plane ($Y = 0$).
2. **Planetary Axial Rotation**: Continuous spinning of each planet around its own local polar axis.
3. **Multi-Level Hierarchical Modeling (Earth-Moon System)**: A multi-tier transformation hierarchy where the Moon acts as a child satellite orbiting Earth while Earth simultaneously orbits the Sun.
4. **Frame-Rate-Independent Animation**: Robust delta-time calculation driven by FreeGLUT timer callbacks.

---

## 2. Frame-Rate Independent Animation & Event Loops

### 2.1 Fixed Frame Rate vs. Variable Delta Time

In computer graphics and game engines, moving objects according to fixed increments per frame ($x \leftarrow x + \Delta x$) leads to frame-rate-dependent simulation speeds:
- A monitor running at 144 Hz will run more than twice as fast as a monitor running at 60 Hz.
- Any drop in frame rate slows down the virtual passage of time.

To ensure consistent simulation physics across varying hardware, the simulator uses **Delta-Time Integration ($\Delta t$)**:

$$\theta(t + \Delta t) = \theta(t) + \omega \cdot \Delta t$$

where:
- $\theta$ is the angular position in degrees.
- $\omega$ is the angular velocity in degrees per second ($\text{deg/s}$).
- $\Delta t$ is the elapsed real-world wall-clock time in seconds.

### 2.2 FreeGLUT Timer Callback (`glutTimerFunc`)

FreeGLUT provides two mechanisms for animation:
1. **`glutIdleFunc()`**: Executes whenever the application has no window messages to process. It consumes 100% of a CPU core and uncapped redraws.
2. **`glutTimerFunc(ms, callback, value)`**: Schedules a one-shot callback to trigger after approximately $ms$ milliseconds. This maintains a targeted frame rate (~60 FPS with $16\text{ ms}$ intervals) while giving the CPU time to rest.

```c
static void timer_callback(int value) {
    (void)value;

    int current_time = glutGet(GLUT_ELAPSED_TIME);

    /* Safe first-frame initialization to prevent abnormal jumps */
    if (previous_time == 0) {
        previous_time = current_time;
    }

    float delta_time = (float)(current_time - previous_time) / 1000.0f;
    previous_time = current_time;

    /* Clamp delta_time to 0.1s maximum to prevent large jumps after pauses */
    if (delta_time > 0.1f) {
        delta_time = 0.1f;
    } else if (delta_time < 0.0f) {
        delta_time = 0.0f;
    }

    /* Update dynamic state */
    planets_update(delta_time);

    /* Request window redisplay */
    glutPostRedisplay();

    /* Reschedule timer for ~60 FPS (16 ms) */
    glutTimerFunc(16, timer_callback, 0);
}
```

### 2.3 Safeguards Against Stalls & Debugging Pauses

1. **First-Frame Initialization**: `glutGet(GLUT_ELAPSED_TIME)` returns milliseconds since `glutInit()`. If `previous_time` is initialized to 0 and the first timer fires several milliseconds later, without proper initialization, an abrupt jump could occur. The simulator initializes `previous_time = glutGet(GLUT_ELAPSED_TIME)` in `main()` and guards against `previous_time == 0`.
2. **Delta Clamping**: If the user drags or resizes the window, or if a breakpoint is hit during debugging, hundreds or thousands of milliseconds might elapse. Without clamping, $\Delta t$ would be enormous, causing planets to skip orbits or wrap erratically. The simulator enforces $\Delta t \in [0.0\text{s}, 0.1\text{s}]$.

---

## 3. Kinematics of Orbital Revolution

### 3.1 Parametric Orbital Mechanics

In our educational visualization, planetary orbits are modeled as circular coplanar paths on the horizontal $X-Z$ plane ($Y = 0$).

The 3D Cartesian coordinates for a planet with orbital distance $R$ and revolution angle $\theta_{\text{orbit}}$ are:

$$X = R \cdot \cos(\theta_{\text{orbit}})$$
$$Y = 0$$
$$Z = -R \cdot \sin(\theta_{\text{orbit}})$$

Rather than computing trigonometric coordinates directly in software and passing vertices, OpenGL allows this positioning via coordinate frame transformations:

$$\mathbf{M}_{\text{orbit}} = \mathbf{R}_y(\theta_{\text{orbit}}) \cdot \mathbf{T}_x(R)$$

### 3.2 Pedagogical Angular Speed Progression

In celestial mechanics, Kepler's Third Law implies that the orbital period $T \propto a^{3/2}$, meaning planets closer to the Sun move significantly faster than distant outer planets. The simulator reflects this natural hierarchy:

| Celestial Body | Semi-Major Axis / Dist ($R$) | Orbital Speed ($\text{deg/s}$) | Period in Sim ($360^\circ / \omega$) | Educational Relationship |
|---|---|---|---|---|
| **Mercury** | 2.5 | 48.0 | 7.5 s | Fastest revolution |
| **Venus** | 3.6 | 35.0 | 10.3 s | Faster than Earth |
| **Earth** | 4.8 | 25.0 | 14.4 s | Reference inner planet |
| **Mars** | 6.0 | 20.0 | 18.0 s | Slower than Earth |
| **Jupiter** | 8.2 | 12.0 | 30.0 s | Slow outer giant |
| **Saturn** | 10.5 | 8.5 | 42.4 s | Slower than Jupiter |
| **Uranus** | 12.8 | 5.5 | 65.5 s | Very slow |
| **Neptune** | 15.0 | 3.5 | 102.9 s | Slowest planet |

### 3.3 Angle Normalization $[0^\circ, 360^\circ)$

Floating-point precision in single-precision IEEE 754 (`float`) degrades as numbers grow large. If $\theta$ were allowed to increment indefinitely without bound (e.g. into thousands of degrees over minutes of simulation), precision loss causes stuttering and catastrophic cancellation. 

The update function maintains all angles strictly within $[0.0^\circ, 360.0^\circ)$:

```c
planets[i].orbit_angle += planets[i].orbit_speed * delta_time;
while (planets[i].orbit_angle >= 360.0f) {
    planets[i].orbit_angle -= 360.0f;
}
while (planets[i].orbit_angle < 0.0f) {
    planets[i].orbit_angle += 360.0f;
}
```

---

## 4. Multi-Level Hierarchical Modeling (Earth-Moon System)

### 4.1 Kinematic Hierarchy Tree

Hierarchical modeling organizes objects in a tree of parent-child relationships where transformations applied to a parent automatically propagate to all descendants:

```text
Global World Frame (Sun at 0, 0, 0)
│
├── glRotatef(Earth orbit_angle, 0, 1, 0)
└── glTranslatef(Earth orbit_distance, 0, 0)  <-- Earth Orbital Position
    │
    ├── [Earth Axial Rotation Frame]
    │   ├── glRotatef(Earth rotation_angle, 0, 1, 0)
    │   └── sphere_draw(Earth radius)
    │
    └── [Moon Hierarchy Frame (Earth Local Space)]
        ├── glRotatef(Moon orbit_angle, 0, 1, 0)
        └── glTranslatef(Moon orbit_distance, 0, 0) <-- Moon Orbital Position
            │
            └── [Moon Axial Rotation Frame]
                ├── glRotatef(Moon rotation_angle, 0, 1, 0)
                └── sphere_draw(Moon radius)
```

### 4.2 The Critical Transformation Isolation Rule

In fixed-function OpenGL, matrix transformations accumulate sequentially from right to left on the current matrix $\mathbf{M}$.

If Earth's axial rotation ($\mathbf{R}_{\text{earth\_spin}}$) were applied *before* the Moon's orbital translation, the Moon would spin around Earth at Earth's rapid daily rotation rate ($50^\circ/\text{s}$), violently whipping the Moon's entire orbit around every few seconds!

**Correct Architectural Solution:**
Earth's axial rotation must be isolated inside its own `glPushMatrix()` / `glPopMatrix()` block:
1. Translate to Earth's orbital location: $\mathbf{M}_E = \mathbf{R}_y(\theta_{E}) \cdot \mathbf{T}_x(D_E)$.
2. Under $\mathbf{M}_E$, branch the **Moon child frame**:
   $$\mathbf{M}_M = \mathbf{M}_E \cdot \mathbf{R}_y(\theta_{\text{moon\_orbit}}) \cdot \mathbf{T}_x(D_M)$$
3. Under $\mathbf{M}_E$, separately branch **Earth's body frame**:
   $$\mathbf{M}_{\text{earth\_body}} = \mathbf{M}_E \cdot \mathbf{R}_y(\theta_{\text{earth\_rot}})$$

```c
/* 1. Position at Earth's orbital coordinate */
glRotatef(earth->orbit_angle, 0.0f, 1.0f, 0.0f);
glTranslatef(earth->orbit_distance, 0.0f, 0.0f);

/* 2. Moon Satellite Branch (Inherits Earth position, NOT Earth spin) */
glPushMatrix();
    glRotatef(moon_orbit_angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(moon_orbit_distance, 0.0f, 0.0f);
    
    glPushMatrix();
        glRotatef(moon_rotation_angle, 0.0f, 1.0f, 0.0f);
        sphere_draw(moon_radius, 20, 20);
    glPopMatrix();
glPopMatrix();

/* 3. Earth Body Branch */
glPushMatrix();
    glRotatef(earth->rotation_angle, 0.0f, 1.0f, 0.0f);
    sphere_draw(earth->radius, 32, 32);
glPopMatrix();
```

---

## 5. Dynamic Lighting & Day/Night Phases

In Week 2 and Week 3, the simulator established `GL_LIGHT0` as a fixed point light at $(0, 0, 0)$ inside the luminous Sun.

With Week 4's dynamic revolution and rotation:
1. **Dynamic Hemispherical Illumination**: Because the light vector $\vec{L} = \text{normalize}(\mathbf{P}_{\text{light}} - \mathbf{P}_{\text{surface}})$ always originates from the center $(0, 0, 0)$, the hemisphere of any planet facing the Sun receives full diffuse illumination ($\mathbf{N} \cdot \vec{L} > 0$), while the outward hemisphere lies in ambient darkness ($\mathbf{N} \cdot \vec{L} \le 0$).
2. **Naturally Emerging Lunar Phases**: As the Moon revolves around Earth, its sunlit hemisphere continuously points toward the Sun at $(0, 0, 0)$, not Earth. An observer on Earth (or viewing from our oblique camera) sees natural crescents, gibbous shapes, and full moon illumination dynamically emerge from standard Phong diffuse reflection without any procedural hacks.

---

## 6. Separation of Concerns Architecture

The project maintains strict modular separation between simulation state updates and graphics rendering:

- `planets_update(float delta_time)`:
  - Pure kinematic logic.
  - No OpenGL calls.
  - Deterministic and testable via automated unit tests in headless environments.
- `planets_render(void)`:
  - Pure rendering and matrix manipulation.
  - Consumes read-only state.
  - Applies material properties, matrix stack pushes/pops, and sphere draw calls.
- `src/main.c`:
  - Serves as the orchestration layer binding FreeGLUT timer callbacks to `planets_update()`.
