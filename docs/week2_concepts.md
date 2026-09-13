# Computer Graphics Concepts — Week 2: 3D Scene & First Objects

This document provides a comprehensive theoretical and technical guide for the graphics concepts implemented in **Week 2** of the **Interactive 3D Solar System and Space Exploration Simulator**. It serves as an architectural reference and preparation for academic project evaluation (viva voce).

---

## 1. 3D Coordinate System Convention

The simulator adheres strictly to the standard **OpenGL Right-Handed Cartesian Coordinate System**:

```text
               +Y (Up)
                |
                |
                |
  (-X) Left <---+---> (+X) Right
               / \
              /   \
             /     \
  (+Z) Viewer       (-Z) Depth into scene
```

- **+X Axis**: Points horizontally to the right.
- **-X Axis**: Points horizontally to the left.
- **+Y Axis**: Points vertically upwards.
- **-Y Axis**: Points vertically downwards.
- **+Z Axis**: Points out of the screen toward the viewer.
- **-Z Axis**: Points into the screen away from the viewer (depth).
- **Origin `(0, 0, 0)`**: Fixed at the center of the Solar System, where the central Sun resides.

---

## 2. OpenGL Transformation Pipeline

In OpenGL, geometry defined in local object space is transformed through multiple coordinate systems before being rendered onto window pixels:

```text
Object Coordinates (Sphere definition)
      ↓  [Model Matrix: glTranslatef, glRotatef, glScalef]
World Coordinates (Solar System space)
      ↓  [View Matrix: gluLookAt]
Eye / Camera Coordinates (Relative to observer)
      ↓  [Projection Matrix: gluPerspective]
Clip Coordinates (Frustum clipping volume)
      ↓  [Perspective Division: (x/w, y/w, z/w)]
Normalized Device Coordinates (NDC: [-1, 1]^3)
      ↓  [Viewport Transformation: glViewport]
Window / Screen Coordinates (Pixels)
```

### Separation of Concerns:
- **View Transformation (`camera_apply()`)**: Modifies the modelview matrix to simulate moving and orienting the camera in world space.
- **Light Positioning (`lighting_apply()`)**: Called immediately after the view transformation so the point light (`GL_LIGHT0`) is fixed at world position `(0, 0, 0)`.
- **Model Transformation (`sun_render()`)**: Isolated using `glPushMatrix()` and `glPopMatrix()` so that translations and scaling applied to the Sun do not accumulate or leak into subsequent objects or camera transformations.

---

## 3. Perspective Projection

Perspective projection simulates human optical perception and camera lenses: objects farther away from the camera appear smaller than objects closer to the camera.

### Function: `gluPerspective(fovy, aspect, zNear, zFar)`

- **`fovy` (45.0°)**: Field of View angle in degrees in the vertical (Y) direction.
- **`aspect` (`width / height`)**: Aspect ratio of the viewport width to height. Recalculated dynamically in `reshape_callback` to prevent spatial distortion (stretching/squishing) when resizing the window. Zero-height edge cases are safely guarded.
- **`zNear` (0.1)**: Near clipping plane distance. Geometry closer than this distance is clipped.
- **`zFar` (100.0)**: Far clipping plane distance. Geometry farther than this distance is clipped.

---

## 4. Depth Testing and the Z-Buffer

### The Problem:
In 3D graphics, polygons can be drawn in arbitrary order. Without depth testing, polygons drawn later simply overwrite earlier ones regardless of whether they are physically behind or in front.

### The Solution:
- **`glEnable(GL_DEPTH_TEST)`**: Activates per-pixel depth comparison using the OpenGL Z-buffer.
- **`glDepthFunc(GL_LEQUAL)`**: Compares the incoming fragment's depth value with the stored depth value; the fragment is drawn only if its depth is less than or equal to the current buffer value.
- **Buffer Clearing (`glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)`)**: Resets both color and depth values at the start of every frame.

---

## 5. Fixed-Function Lighting Model

OpenGL's classic fixed-function lighting model approximates the interaction of light with surfaces using three primary reflection components plus material emission:

$$\text{Color} = \text{Emission} + \text{Ambient} + \text{Diffuse} + \text{Specular}$$

### Illumination Components:
1. **Ambient Light**: Non-directional, uniform light scattered throughout space by multiple diffuse reflections. Simulates baseline visibility in deep space:
   $$\text{Ambient} = I_a \times K_a$$
2. **Diffuse Reflection**: Directional light reflected uniformly in all directions according to **Lambert’s Cosine Law**:
   $$\text{Diffuse} = I_d \times K_d \times \max(\vec{N} \cdot \vec{L}, 0)$$
   where $\vec{N}$ is the surface normal vector and $\vec{L}$ is the vector pointing toward the light source.
3. **Specular Highlight**: Mirror-like reflection concentrated along the reflection vector, controlled by the viewer direction $\vec{V}$ and shininess exponent.
4. **Material Emission (`GL_EMISSION`)**:
   - Used for the Sun to give it a self-luminous golden glow (`1.0, 0.75, 0.1, 1.0`).
   - **Crucial Distinction**: In OpenGL's fixed-function pipeline, **material emission DOES NOT illuminate other objects**. The actual light emitter for the Solar System is `GL_LIGHT0` placed at `(0, 0, 0)`.

---

## 6. Surface Normals and `GL_NORMALIZE`

- Surface normals are perpendicular unit vectors $\vec{N}$ defined at every vertex.
- `glutSolidSphere()` automatically computes radial unit normals pointing outward from the sphere center.
- **`glEnable(GL_NORMALIZE)`**: When modelview transformations include non-uniform or uniform scaling (`glScalef`), vertex normal vectors can be distorted and lose unit length, resulting in incorrect diffuse and specular lighting. `GL_NORMALIZE` instructs OpenGL to automatically re-normalize normal vectors before lighting calculations.

---

## 7. Camera System Integration

- Week 1's camera system developed by Akshatha is integrated into the 3D pipeline.
- Camera Position: `(0.0, 2.0, 5.0)` — elevated along Y and pulled back along Z to provide an optimal oblique vantage point.
- Target Point: `(0.0, 0.0, 0.0)` — centered on the Sun.
- Up Vector: `(0.0, 1.0, 0.0)` — world up aligns with the +Y axis.
- Controls:
  - `W` / `S`: Move forward / backward along Z axis.
  - `A` / `D`: Move left / right along X axis.
  - `R`: Reset camera to initial position.
