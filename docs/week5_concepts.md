# Computer Graphics Concepts — Week 5: Texture Mapping

This document provides a comprehensive theoretical and technical guide for the graphics concepts implemented in **Week 5** of the **Interactive 3D Solar System and Space Exploration Simulator**. It serves as an architectural reference and preparation for academic project evaluation (viva voce).

---

## 1. Overview of Texture Mapping

In Weeks 1 to 4, celestial bodies were rendered using fixed-function Gouraud shading over solid-colored spheres. While the 3D geometry and day/night lighting were mathematically sound, planetary surfaces lacked realistic geographical and atmospheric features, and the central Sun visually resembled a flat yellow disc due to the lack of an external light source.

**Week 5** introduces **2D Texture Mapping**, wrapping authentic planetary photographic maps around 3D spherical geometry. This brings:
1. **Photorealistic Surface Realism**: Continents and oceans on Earth, storm bands and the Great Red Spot on Jupiter, cratered lunar highlands on the Moon, and atmospheric details across all planets.
2. **True Solar Photosphere Visualization**: The Sun displays convection granulation, sunspots, coronal brightness, and natural limb darkening, finally reading as a luminous star.
3. **Dynamic Day/Night Shading via Texture Modulation**: Textures integrate seamlessly with fixed-function point lighting (`GL_LIGHT0`), dynamically illuminating the sunlit hemisphere while shading the night side.
4. **Cosmic Environment**: A textured starfield sphere providing realistic astronomical immersion.

---

## 2. Spherical UV Coordinate Mapping

### 2.1 Equirectangular Projection to 3D Sphere

A 2D texture image is indexed by normalized parametric texture coordinates $(u, v) \in [0.0, 1.0]^2$:
* $u$: Horizontal coordinate corresponding to planetary longitude ($\theta \in [0, 2\pi]$).
* $v$: Vertical coordinate corresponding to planetary latitude ($\phi \in [-\pi/2, +\pi/2]$).

In `src/sphere.c`, the sphere is tessellated into latitudinal rings (stacks) and longitudinal subdivisions (slices). For each vertex at latitude $\phi$ and longitude $\theta$:

$$\phi = -\frac{\pi}{2} + \pi \cdot v, \quad \theta = 2\pi \cdot (1 - u)$$

The 3D Cartesian coordinates are computed as:

$$x = R \cos(\phi) \cos(\theta)$$
$$y = R \sin(\phi)$$
$$z = R \cos(\phi) \sin(\theta)$$

Surface normal vectors for lighting calculations are normalized outward unit vectors:

$$\vec{N} = (\cos(\phi) \cos(\theta), \; \sin(\phi), \; \cos(\phi) \sin(\theta))$$

```c
/* Texture coordinates emitted alongside normals and vertex positions */
glNormal3f(nx, ny, nz);
glTexCoord2f(u, v);
glVertex3f(radius * nx, radius * ny, radius * nz);
```

### 2.2 Polar and Seam Singularity Handling

1. **Equatorial Seam ($u = 0.0$ and $u = 1.0$)**: The longitude loop iterates from $i = 0$ to $i = \text{slices}$ (inclusive), so the final vertex of each strip shares the identical spatial coordinate as the first vertex, sampling $u = 0.0$ and $u = 1.0$. This ensures a continuous wrap without a missing sliver or visible seam.
2. **Poles ($\phi = \pm \pi/2$)**: At the North ($v = 1.0$) and South ($v = 0.0$) poles, $\cos(\phi) = 0$, causing all vertices along the pole ring to collapse into a single point $(0, \pm R, 0)$ with normals pointing straight up/down $(0, \pm 1, 0)$.
3. **Polar Clamping**: Vertical wrapping is set to `GL_CLAMP_TO_EDGE` so texture coordinates at the poles do not accidentally wrap and sample the opposite pole's pixels.

---

## 3. Texture Subsystem Architecture

### 3.1 Single-Header Image Loading (`stb_image.h`)

To maintain clean C99 code without heavy third-party GUI or image frameworks, the project integrates `stb_image.h` (v2.30, public domain / MIT license by Sean Barrett):
- Zero external library dependencies.
- Native decoding of JPEG and PNG image assets.
- Integrated into `src/texture.c` via `#define STB_IMAGE_IMPLEMENTATION`.
- `stbi_set_flip_vertically_on_load(1)` ensures image row 0 aligns with OpenGL bottom texture coordinate $v = 0.0$.

### 3.2 Texture Lifecycle & Management

To ensure optimal performance (~60 FPS), textures are never loaded per frame:
1. **Initialization (`textures_init`)**: Executed once during OpenGL startup. All image files are decoded into GPU memory. CPU buffers are immediately freed via `stbi_image_free()`.
2. **Binding (`texture_bind`)**: During each frame, `glBindTexture(GL_TEXTURE_2D, id)` binds the pre-allocated GPU texture ID and activates `GL_TEXTURE_2D`.
3. **State Isolation (`texture_unbind`)**: After drawing each celestial body, `glDisable(GL_TEXTURE_2D)` and `glBindTexture(GL_TEXTURE_2D, 0)` prevent texture bleeding onto non-textured geometry (such as orbit lines or Saturn's rings).
4. **Cleanup (`textures_cleanup`)**: Called upon simulator exit to delete all allocated GPU texture names via `glDeleteTextures()`.

---

## 4. Texture Filtering & Mipmapping

When 3D objects are viewed in perspective, the projected screen pixel size rarely matches the native texture texel size:

### 4.1 Magnification (`GL_TEXTURE_MAG_FILTER`)
When the camera is close to a planet, a screen pixel covers less than one texel:
- `GL_LINEAR` (Bilinear Interpolation): Computes a weighted average of the 4 nearest texels, eliminating blocky pixelation and providing smooth gradients.

### 4.2 Minification & Mipmapping (`GL_TEXTURE_MIN_FILTER`)
When viewing distant planets (e.g. Neptune or Uranus across the system), multiple texels map to a single screen pixel. Point sampling or simple bilinear filtering causes severe high-frequency aliasing, shimmering, and moiré patterns during camera movement.

**Mipmapping**:
The project generates a complete mipmap pyramid using `gluBuild2DMipmaps()`, creating downsampled levels: $2048 \times 1024 \to 1024 \times 512 \to 512 \times 256 \dots \to 1 \times 1$.
- `GL_LINEAR_MIPMAP_LINEAR` (Trilinear Filtering): Linearly interpolates between the two closest mipmap levels and performs bilinear filtering within each level. This delivers smooth, shimmer-free visualization across all viewing distances.

---

## 5. Interaction Between Textures and Fixed-Function Lighting

In the fixed-function OpenGL pipeline, the final fragment color is governed by the texture environment mode:

$$\text{glTexEnvi(GL\_TEXTURE\_ENV, GL\_TEXTURE\_ENV\_MODE, GL\_MODULATE)}$$

Under `GL_MODULATE`:

$$\mathbf{C}_{\text{final}} = \mathbf{C}_{\text{lighting}} \times \mathbf{C}_{\text{texture}}$$

where:
$$\mathbf{C}_{\text{lighting}} = \mathbf{M}_{\text{emission}} + \mathbf{M}_{\text{ambient}} \cdot \mathbf{L}_{\text{ambient}} + (\vec{N} \cdot \vec{L}) \cdot (\mathbf{M}_{\text{diffuse}} \cdot \mathbf{L}_{\text{diffuse}})$$

### Architectural Design:
1. **Planetary Diffuse Material**:
   - Set to neutral white `(1.0, 1.0, 1.0, 1.0)` with ambient `(0.2, 0.2, 0.2, 1.0)`.
   - On the Sun-facing side ($\vec{N} \cdot \vec{L} \approx 1.0$), $\mathbf{C}_{\text{lighting}} \approx 1.0$, allowing the authentic colors of the texture (oceans, continents, clouds) to display at full brilliance.
   - On the night side ($\vec{N} \cdot \vec{L} \le 0$), only the soft ambient light illuminates the planet, creating physically natural day/night terminators.
2. **Self-Luminous Sun**:
   - The Sun is a source of light, not a reflector.
   - During `sun_render()`, lighting is temporarily disabled while texturing is active with `glColor3f(1.0, 1.0, 1.0)`.
   - This displays the authentic photosphere photograph without artificial shadows or attenuation, while `GL_LIGHT0` at $(0, 0, 0)$ illuminates the revolving planets.
   - After rendering the Sun, `glEnable(GL_LIGHTING)` is immediately restored.

---

## 6. Matrix Hierarchy & Dynamic Motion Compatibility

Texture coordinates are defined in the local model space of the sphere. When the planetary transformation hierarchy executes:

$$\mathbf{M} = \mathbf{R}_y(\theta_{\text{orbit}}) \cdot \mathbf{T}_x(R) \cdot \mathbf{R}_y(\theta_{\text{rot}})$$

1. **Orbital Revolution**: The planet moves around the Sun along its orbit; texture coordinates remain anchored to the sphere's local frame.
2. **Axial Rotation**: The planet rotates around its polar $Y$-axis; the texture coordinates rotate with the geometry, causing continents and atmospheric storms to continuously rotate naturally across the view.
3. **Earth-Moon Hierarchy**: The Moon's local coordinate frame is translated to Earth's position and rotated in its orbit; the Moon's cratered texture orbits Earth and spins synchronously with its tidal rotation speed.
