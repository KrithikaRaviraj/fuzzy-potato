# Week 6 Computer Graphics Concepts: Fixed-Function Lighting, Shading & Materials

This document provides a comprehensive theoretical and implementation reference for **Week 6: Lighting & Shading** in the *Interactive 3D Solar System and Space Exploration Simulator*. It covers the mathematical formulation of the Phong illumination model, fixed-function OpenGL state pipelines, local viewer specular highlights, material calibration, and texture-lighting modulation.

---

## 1. Overview of the Fixed-Function Lighting Pipeline

In classic OpenGL (OpenGL 1.1 / 1.2), lighting calculations are evaluated on vertex coordinates within **eye space** (camera coordinate space). The fixed-function pipeline implements the **empirical Phong reflection model** (specifically the Blinn-Phong variant), where total surface radiance $I$ at a vertex is computed as the sum of four distinct components:

$$I = I_{\text{emission}} + I_{\text{ambient}} + I_{\text{diffuse}} + I_{\text{specular}}$$

In our simulator:
* The **Sun** acts as the central light emitter ($I_{\text{emission}} > 0$), while disabling external lighting to showcase its photographic photosphere details.
* The **Planets and Moon** act as celestial receivers ($I_{\text{emission}} = 0$), receiving ambient, diffuse, and specular illumination from the central solar light source `GL_LIGHT0`.

```text
       Sun (Emitter)
         [Origin (0,0,0)]
               │
        ┌──────┴──────┐
        │  GL_LIGHT0  │ (Point light at world origin)
        └──────┬──────┘
               ▼
   Illumination Radiance (L)
               │
     ┌─────────┴─────────┐
     ▼                   ▼
 Day Hemisphere     Terminator Boundary     Night Hemisphere
(Diffuse + Specular)   (N · L → 0)         (Ambient Only)
```

---

## 2. Mathematical Components of the Illumination Model

### 2.1 Ambient Reflection ($I_{\text{ambient}}$)
Ambient illumination approximates indirect, diffuse light that has scattered multiple times across the environment (in our case, cosmic background radiation and starlight). It is direction-independent and illuminates all surfaces equally regardless of orientation:

$$I_{\text{ambient}} = (k_a \times L_a) + (k_a \times A_{\text{global}})$$

Where:
* $k_a$ is the material's ambient reflection coefficient (`GL_AMBIENT`).
* $L_a$ is the ambient intensity of the light source (`GL_LIGHT0`, `GL_AMBIENT`).
* $A_{\text{global}}$ is the global scene ambient light (`GL_LIGHT_MODEL_AMBIENT`).

**Design Choice in Simulator**:
Rather than setting the night side to total pitch black ($I = 0$), we configure a baseline ambient contribution ($\sim 0.20$). When modulated with the photographic texture maps, this allows continents, cloud bands, and planetary surface details to remain faintly discernible on the night hemisphere, simulating starlight and secondary scatter.

---

### 2.2 Diffuse Reflection ($I_{\text{diffuse}}$) & Lambert's Cosine Law
Diffuse reflection models ideal matte reflection from rough surfaces where incident light is scattered uniformly in all directions over the hemisphere. According to **Lambert's Cosine Law**, the reflected luminous intensity is directly proportional to the cosine of the angle $\theta$ between the surface unit normal $\hat{N}$ and the light direction vector $\hat{L}$:

$$I_{\text{diffuse}} = k_d \times L_d \times \max(\hat{N} \cdot \hat{L}, 0)$$

Where:
* $k_d$ is the material's diffuse reflection coefficient (`GL_DIFFUSE`).
* $L_d$ is the light source's diffuse emission (`GL_LIGHT0`, `GL_DIFFUSE`).
* $\hat{N} \cdot \hat{L} = \cos\theta$. If $\theta > 90^\circ$ ($\hat{N} \cdot \hat{L} \le 0$), the surface faces away from the light source and receives zero direct diffuse illumination.

**The Planetary Terminator**:
The locus of points where $\hat{N} \cdot \hat{L} = 0$ forms the **planetary terminator**—the boundary line separating the sunlit day hemisphere from the darkened night hemisphere. Because the light source is at the Solar System center $(0,0,0)$, the subsolar point on each planet always points directly inward toward the Sun, naturally producing realistic terminator curves.

---

### 2.3 Specular Reflection ($I_{\text{specular}}$) & Blinn-Phong Model
Specular reflection models directional highlight gleams produced by smooth or shiny surfaces. OpenGL utilizes the **Blinn-Phong** formulation, which evaluates the cosine of the angle between the surface unit normal $\hat{N}$ and the **halfway vector** $\hat{H}$:

$$\hat{H} = \frac{\hat{L} + \hat{V}}{\|\hat{L} + \hat{V}\|}$$

$$I_{\text{specular}} = k_s \times L_s \times \max(\hat{N} \cdot \hat{H}, 0)^\alpha$$

Where:
* $k_s$ is the material's specular reflection coefficient (`GL_SPECULAR`).
* $L_s$ is the light source's specular intensity (`GL_LIGHT0`, `GL_SPECULAR`).
* $\hat{V}$ is the unit vector pointing toward the viewer/camera.
* $\alpha$ is the **shininess exponent** (`GL_SHININESS`, range $[0, 128]$). Higher values of $\alpha$ produce smaller, tighter, and sharper highlight spots (polished surfaces), while lower values create broad, diffuse glints.

---

## 3. Local Viewer vs. Infinite Viewer (`GL_LIGHT_MODEL_LOCAL_VIEWER`)

In default OpenGL fixed-function lighting:
```c
glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
```
The view direction $\hat{V}$ is assumed to be parallel to the $+Z$ axis $(0, 0, 1)$ for every vertex in the scene (infinite viewer assumption). While computationally faster on legacy 1990s hardware, this creates severe optical distortions on nearby 3D spherical planets, because the perspective angle to the camera changes dramatically across the sphere's surface.

By configuring:
```c
glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
```
OpenGL computes the true line-of-sight vector $\hat{V}_i = \frac{E - P_i}{\|E - P_i\|}$ from each vertex $P_i$ to the camera eye position $E$. As the user zooms in, orbits, or focuses on Earth or Jupiter, specular highlights shift realistically across the planetary curved horizons.

---

## 4. Normal Vectors & Unit Normalization (`GL_NORMALIZE`)

The parametric UV sphere generator (`src/sphere.c`) computes exact outward normal vectors analytically at each latitude ($\phi$) and longitude ($\theta$):

$$\hat{N} = (\cos\phi \cos\theta, \; \sin\phi, \; \cos\phi \sin\theta)$$

Because $\cos^2\phi \cos^2\theta + \sin^2\phi + \cos^2\phi \sin^2\theta = \cos^2\phi (\cos^2\theta + \sin^2\theta) + \sin^2\phi = 1$, the analytical normals generated by the sphere module are strictly unit length ($1.0$).

However, during scene modeling:
1. Scaling transformations (`glScalef`) non-uniformly alter vector magnitudes.
2. In the fixed-function pipeline, normal vectors are transformed by the **inverse transpose** of the upper-left $3 \times 3$ modelview matrix.

Enabling:
```c
glEnable(GL_NORMALIZE);
```
guarantees that OpenGL renormalizes all normal vectors to length $1.0$ before lighting evaluation, preventing lighting under-exposure or saturation caused by geometric scaling.

---

## 5. Smooth Gouraud Shading (`GL_SMOOTH`)

OpenGL supports two primary shading models:
* `GL_FLAT`: The color computed at the first (or last) vertex of a polygon is applied uniformly across the entire face, resulting in faceted polygonal silhouettes.
* `GL_SMOOTH` (Gouraud Shading): Lighting equations are evaluated at each vertex using its outward normal $\hat{N}_i$, and the resulting color values are bilinearly interpolated across the rasterized interior fragments of each polygon.

In `src/lighting.c`:
```c
glShadeModel(GL_SMOOTH);
```
This produces seamless, continuous lighting gradients across latitude and longitude quad strips on all planetary spheres.

---

## 6. Texture Modulation (`GL_MODULATE`)

In Week 5, photographic texture maps were introduced. In Week 6, textures and illumination are fused using OpenGL's texture environment modulation mode:

```c
glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
```

Under modulation, the final fragment color $C$ combines the lit material color $I$ with the texture texel color $T$:

$$C_{\text{RGB}} = I_{\text{primary, RGB}} \times T_{\text{RGB}}$$

Where $I_{\text{primary}} = I_{\text{ambient}} + I_{\text{diffuse}}$.

### Why Material Diffuse Must Be Neutral White
If a planet's diffuse material color was tinted (e.g., pure red for Mars or blue for Neptune), modulation would multiply the photographic texels by that color filter:
$$T_{\text{texel}} \times (0.88, 0.30, 0.15)$$
This would destroy the authentic spectral colors captured by NASA cameras. By setting planetary diffuse material to neutral white $(1.0, 1.0, 1.0, 1.0)$:
$$T_{\text{texel}} \times (1.0, 1.0, 1.0) = T_{\text{texel}}$$
The photographic texture is rendered at 100% color fidelity on the sunlit side, while the scalar factor $\hat{N} \cdot \hat{L}$ naturally controls the day/night light level.

---

## 7. Planetary Material Calibration Table

Different celestial bodies possess radically different physical compositions. Week 6 implements scientifically calibrated material properties for each body:

| Celestial Body | Physical Composition | Ambient ($k_a$) | Diffuse ($k_d$) | Specular ($k_s$) | Shininess ($\alpha$) | Visual Interpretation |
|---|---|---|---|---|---|---|
| **Mercury** | Airless cratered basalt | `0.15, 0.15, 0.15` | `0.95, 0.95, 0.95` | `0.08, 0.08, 0.08` | `6.0` | Very dark, matte silicate rock; negligible specular scatter. |
| **Venus** | Sulfuric acid cloud deck | `0.22, 0.22, 0.20` | `1.00, 1.00, 1.00` | `0.35, 0.35, 0.30` | `22.0` | Highest planetary albedo (~0.75); bright, soft atmospheric cloud sheen. |
| **Earth** | 71% liquid oceans + clouds | `0.20, 0.20, 0.22` | `1.00, 1.00, 1.00` | `0.50, 0.50, 0.55` | `38.0` | Pronounced ocean glint reflection; sharp, focused liquid highlight. |
| **Moon** | Fine basaltic regolith | `0.14, 0.14, 0.14` | `0.95, 0.95, 0.95` | `0.06, 0.06, 0.06` | `4.0` | Dusty airless surface; broad, diffuse retro-reflective scatter. |
| **Mars** | Iron-oxide dusty soil | `0.18, 0.16, 0.15` | `0.98, 0.95, 0.95` | `0.12, 0.10, 0.10` | `10.0` | Dry, rusty oxidized rock; low matte scattering. |
| **Jupiter** | Dense ammonia/gas clouds | `0.20, 0.20, 0.18` | `1.00, 1.00, 0.98` | `0.25, 0.23, 0.20` | `18.0` | Smooth cloud bands; moderate upper-deck atmospheric sheen. |
| **Saturn** | Banded gas + ammonia haze | `0.20, 0.20, 0.18` | `1.00, 0.98, 0.95` | `0.22, 0.22, 0.18` | `16.0` | Soft golden haze dispersion overlying planetary belts. |
| **Uranus** | Methane aerosol haze | `0.22, 0.24, 0.24` | `1.00, 1.00, 1.00` | `0.30, 0.35, 0.35` | `26.0` | Smooth, highly uniform icy methane surface reflection. |
| **Neptune** | Deep methane + cirrus ice | `0.20, 0.22, 0.25` | `1.00, 1.00, 1.00` | `0.32, 0.35, 0.40` | `28.0` | Crisp azure atmospheric sheen with subtle high-altitude glaze. |

---

## 8. Light Positioning & Eye-Space Transformation Order

In OpenGL, the position parameter passed to `glLightfv(GL_LIGHT0, GL_POSITION, pos)` is **immediately transformed** by the current **Modelview matrix** at the exact time the function is called:

$$P_{\text{eye}} = M_{\text{modelview}} \times P_{\text{light}}$$

If `lighting_apply()` were called *before* `camera_apply()` (or when the modelview matrix is identity), the light source would be fixed in eye space at the camera's location (behaving like a headlamp).

In `src/main.c`:
```c
/* 1. Set up Camera View Matrix */
camera_apply();        /* gluLookAt sets Modelview to View Matrix V */

/* 2. Position Light Source */
lighting_apply();      /* glLightfv transforms (0,0,0,1) by V */
```
Because `lighting_apply()` is called directly after `camera_apply()`, world-space origin $(0, 0, 0, 1)$ is multiplied by the view matrix $V$. As the camera moves around the Solar System, the light source remains firmly anchored at the center of the world coincident with the Sun.

---

## 9. Light Attenuation in Astronomical Simulators

Fixed-function OpenGL supports distance attenuation via:
$$\text{attenuation} = \frac{1}{k_c + k_l \cdot d + k_q \cdot d^2}$$

In physical reality, radiant flux follows the **inverse-square law** ($1/d^2$). However, the true Solar System spans vast dynamic ranges:
* Mercury distance: $2.5$ normalized units
* Neptune distance: $15.0$ normalized units
* Inverse-square ratio: $(\frac{2.5}{15.0})^2 = \frac{1}{36} \approx 2.7\%$

If true physical inverse-square attenuation were applied, Neptune and Uranus would receive less than $3\%$ of Mercury's light, rendering them pitch black and invisible on computer monitors.

For educational 3D graphics applications:
```c
glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
```
We maintain uniform incident intensity while letting each planet's physical surface materials, day/night orientation, and distance from the viewer govern its visual contrast.

---

## 10. Summary of State Management & Isolation Patterns

To guarantee zero visual cross-contamination, OpenGL state transitions follow strict isolation blocks:

```c
/* Sun Rendering */
glPushMatrix();
glDisable(GL_LIGHTING);          /* Unlit self-luminous emission */
texture_bind(texture_get_sun());
sphere_draw(SUN_RADIUS, 48, 48);
texture_unbind();
glEnable(GL_LIGHTING);           /* Restore lighting for planets */
glPopMatrix();

/* Orbit Path Rendering */
glDisable(GL_LIGHTING);          /* Orbits are non-reflective lines */
glDisable(GL_TEXTURE_2D);
glBegin(GL_LINE_LOOP); ... glEnd();
glEnable(GL_LIGHTING);

/* Starfield Background */
glDisable(GL_LIGHTING);
glDisable(GL_DEPTH_TEST);
glDepthMask(GL_FALSE);           /* Prevent writing to Z-buffer */
glDisable(GL_CULL_FACE);         /* Draw inside of sky sphere */
texture_bind(texture_get_stars());
sphere_draw(70.0f, 32, 24);
texture_unbind();
glDepthMask(GL_TRUE);
glEnable(GL_DEPTH_TEST);
glEnable(GL_LIGHTING);
```
This guarantees clean rendering across all platforms with 0 artifacts.
