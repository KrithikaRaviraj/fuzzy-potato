# Planetary and Celestial Texture Assets Provenance & Licensing

This document details the provenance, source references, licensing, and processing for all 2D texture assets utilized in **Week 5: Texture Mapping** of the **Interactive 3D Solar System and Space Exploration Simulator**.

---

## 1. Primary Source Overview

All planetary and celestial surface textures used in this simulator are sourced from **Solar System Scope** ([solarsystemscope.com/textures](https://www.solarsystemscope.com/textures/)), which compiles and creates digital planetary maps based on NASA imagery (from missions including SOHO, SDO, MESSENGER, Magellan, Apollo, Lunar Reconnaissance Orbiter, Viking, Mars Global Surveyor, Cassini-Huygens, and Voyager 1 & 2).

### License
* **License**: [Creative Commons Attribution 4.0 International (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/)
* **Attribution**: "Solar System Scope / Inove s.r.o. ([solarsystemscope.com](https://www.solarsystemscope.com/))"
* **Permitted Use**: Free to share, adapt, and use for commercial and non-commercial purposes with appropriate attribution.

---

## 2. Asset Manifest & Specifications

| Celestial Body | Asset File | Native Resolution | Projection | NASA Source Imagery / Mission | License |
|---|---|---|---|---|---|
| **Sun** | `textures/sun.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | NASA SDO / SOHO EIT Photosphere | CC BY 4.0 |
| **Mercury** | `textures/mercury.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | NASA MESSENGER MDIS Global Mosaic | CC BY 4.0 |
| **Venus** | `textures/venus.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | NASA Magellan Radar Global Topography | CC BY 4.0 |
| **Earth** | `textures/earth.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | NASA Blue Marble: Next Generation (MODIS) | CC BY 4.0 |
| **Moon** | `textures/moon.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | NASA Lunar Reconnaissance Orbiter (LROC) | CC BY 4.0 |
| **Mars** | `textures/mars.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | NASA Viking Orbiter / Mars Global Surveyor | CC BY 4.0 |
| **Jupiter** | `textures/jupiter.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | NASA Cassini ISS / Voyager Mosaic | CC BY 4.0 |
| **Saturn** | `textures/saturn.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | NASA Cassini ISS Global Map | CC BY 4.0 |
| **Uranus** | `textures/uranus.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | NASA Voyager 2 Narrow-Angle Camera | CC BY 4.0 |
| **Neptune** | `textures/neptune.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | NASA Voyager 2 Narrow-Angle Camera | CC BY 4.0 |
| **Starfield** | `textures/stars.jpg` | $2048 \times 1024$ | Equirectangular ($2:1$) | ESO / NASA Tycho-2 Star Catalog Milky Way Panorama | CC BY 4.0 |
| **Saturn Rings** | `textures/saturn_ring.png` | $2048 \times 125$ | Linear / Alpha Radial | NASA Cassini ISS Ring Plane Profile | CC BY 4.0 |

---

## 3. Formatting & Processing Notes

1. **Power-of-Two Dimensions**: All spherical maps are standard $2048 \times 1024$ ($2^{11} \times 2^{10}$), providing optimal compatibility with OpenGL texture hardware and automatic GLU mipmap generation.
2. **Equirectangular Projection**: Maps longitude $[0^\circ, 360^\circ]$ uniformly across the horizontal $U \in [0.0, 1.0]$ axis, and latitude $[-90^\circ, +90^\circ]$ uniformly across the vertical $V \in [0.0, 1.0]$ axis.
3. **Vertical Orientation**: Images are loaded with `stbi_set_flip_vertically_on_load(1)` so that image row 0 corresponds to the South Pole ($V = 0.0$) and the top row corresponds to the North Pole ($V = 1.0$), aligning with standard OpenGL Cartesian conventions.

