# Computer Graphics Concepts — Week 3: Complete Basic Solar System

This document provides a comprehensive theoretical and technical guide for the graphics concepts implemented in **Week 3** of the **Interactive 3D Solar System and Space Exploration Simulator**. It serves as an architectural reference and preparation for academic project evaluation (viva voce).

---

## 1. 3D Coordinate System & Scene Hierarchy

The simulator employs standard **OpenGL Right-Handed Cartesian Coordinates**:

```text
               +Y (Up / Orbital Normal)
                |
                |
                |
  (-X) Left <---+---> (+X) Right
               / \
              /   \
             /     \
  (+Z) Viewer       (-Z) Depth into scene
```

- **Orbital Plane (Ecliptic)**: The primary orbital plane is defined by the **X-Z plane** ($Y = 0$). All planetary orbits lie on this horizontal plane.
- **Scene Origin `(0, 0, 0)`**: Fixed at the center of the Solar System, where the self-luminous Sun and the central point light source (`GL_LIGHT0`) reside.

---

## 2. Hierarchical Modeling and Matrix Stacks

In Computer Graphics, hierarchical modeling allows complex compound scenes to be constructed by composing coordinate frames. 

### Matrix Isolation (`glPushMatrix` / `glPopMatrix`)

Without matrix isolation, translating to render one planet would permanently displace the coordinate system for all subsequent objects. The simulator uses the OpenGL matrix stack to achieve strict transformation isolation:

```text
Modelview Matrix Stack
│
├── [Global View Matrix: gluLookAt]
│   │
│   ├── glPushMatrix()
│   │   ├── Model Transform: Sun at (0, 0, 0)
│   │   └── sun_render()
│   └── glPopMatrix()
│
│   ├── [orbits_render_all(): 8 circular loops at y = 0]
│
│   ├── For each planet:
│   │   ├── glPushMatrix()
│   │   │   ├── glRotatef(orbit_angle, 0, 1, 0)   [Orbital orientation]
│   │   │   ├── glTranslatef(orbit_distance, 0, 0) [Radial displacement]
│   │   │   ├── [Saturn ring: tilted local frame if i == 5]
│   │   │   ├── glMaterialfv(...)                 [Material properties]
│   │   │   └── sphere_draw(radius, 32, 32)       [Local geometry]
│   │   └── glPopMatrix()                         [Restores global frame]
```

By pushing and popping the matrix, each planet's local transformations are completely independent of its peers.

---

## 3. Parametric Orbit Geometry

Planetary orbits in Week 3 are modeled as circular paths on the ecliptic plane ($Y = 0$).

### Parametric Equations of a Circle in 3D:

$$x(\theta) = R \cdot \cos(\theta)$$
$$y(\theta) = 0$$
$$z(\theta) = R \cdot \sin(\theta)$$

where $R$ is the planetary orbital distance and $\theta \in [0, 2\pi)$.

### Rendering Technique:
- Approximated using $N = 100$ line segments connected via `GL_LINE_LOOP`.
- Lighting is temporarily disabled (`glDisable(GL_LIGHTING)`) during orbit line rendering so that the lines display a crisp, non-attenuated cosmic blue-grey color `(0.25, 0.32, 0.42)` without specular or diffuse lighting artifacts.

---

## 4. Geometric Modeling of Saturn's Ring

Saturn is uniquely identified by its planetary ring system. In Week 3, this is implemented using pure geometric primitives (without textures):

1. **Local Frame Alignment**: Rendered inside Saturn's isolated modelview transformation frame.
2. **Axial Tilt**: Tilted at approximately $25^\circ$ using `glRotatef(25.0f, 1.0f, 0.0f, 0.4f)`.
3. **Geometry**:
   - A flat ring disk generated via `GL_QUAD_STRIP` connecting inner radius ($r_{\text{in}} = 1.35 \times R_{\text{Saturn}}$) to outer radius ($r_{\text{out}} = 2.15 \times R_{\text{Saturn}}$).
   - Inner and outer bounding circles rendered with `GL_LINE_LOOP` for sharp silhouette definition.

---

## 5. Normalized Educational Visualization Scale

Real astronomical dimensions span multiple orders of magnitude (the Sun's diameter is $\approx 109 \times$ Earth's diameter, while astronomical distances are tens of thousands of times planetary radii). Rendering at true physical scale would render planets sub-pixel specks and leave the screen largely empty.

The simulator uses a **normalized educational scale** that preserves fundamental visual relationships:

### Planetary Size Ordering:
$$\text{Mercury (0.18)} < \text{Mars (0.22)} < \text{Venus (0.28)} \approx \text{Earth (0.30)} < \text{Neptune (0.40)} \approx \text{Uranus (0.42)} < \text{Saturn (0.58)} < \text{Jupiter (0.70)} < \text{Sun (1.20)}$$

### Orbital Distance Ordering:
$$\text{Mercury (2.5)} < \text{Venus (3.6)} < \text{Earth (4.8)} < \text{Mars (6.0)} < \text{Jupiter (8.2)} < \text{Saturn (10.5)} < \text{Uranus (12.8)} < \text{Neptune (15.0)}$$

---

## 6. Point-Light Illumination & Material Properties

### Light Source (`GL_LIGHT0`):
- Configured as a **point light** located at world origin `(0, 0, 0, 1.0)` (the Sun's center) with warm white diffuse light `(1.0, 1.0, 0.95, 1.0)`.
- `lighting_apply()` sets the light position immediately after the camera view matrix (`gluLookAt`) is loaded, anchoring it in world space.

### Day / Night Hemispherical Lighting:
- In fixed-function OpenGL, vertex normals pointing outward from each planet's sphere are evaluated against the light vector $\vec{L}$ pointing from the vertex to `(0, 0, 0)`:
  $$\text{Diffuse} = I_d \cdot K_d \cdot \max(\vec{N} \cdot \vec{L}, 0)$$
- Consequently, the hemisphere of each planet facing the Sun is brightly illuminated, while the hemisphere facing deep space remains in soft ambient shadow (`K_a = 0.25 \times K_d`).
- The Sun itself uses material emission (`GL_EMISSION`) to appear self-luminous.

---

## 7. Camera Positioning & Perspective Projection

### Projection (`gluPerspective`):
- **Vertical FOV**: $45.0^\circ$
- **Near Clip Plane**: $0.1$
- **Far Clip Plane**: $100.0$
- **Aspect Ratio**: Dynamically updated in `reshape_callback` ($W / H$).

### Viewing Position (`gluLookAt`):
- **Position**: `(0.0, 16.0, 28.0)`
- **Look-At Target**: `(0.0, 0.0, 0.0)`
- **Up Vector**: `(0.0, 1.0, 0.0)`
- This vantage point looks down obliquely at an angle of $\approx 29.7^\circ$ from the horizontal plane, transforming the circular orbits into clear ellipses and capturing the Sun and all 8 planets within the viewing frustum on launch.
