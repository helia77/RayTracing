# RayTracing
 ### Simple ray tracing algorithm for direct illumination of spheres.  
 (ECE 6360 GPU and Heterogeneous Programming course assignment)

<br>

* For a given list of sphere and light coordinates (txt file), renders and saves the ray-traced image as Targa (TGA) file.
* Parallelized using C++ std::thread library
* To open TGA images I recommend installing the [Honeyview](https://en.bandisoft.com/honeyview/) software.
* No additional libraries required
* sphere.txt order: `x, y, z, r, B, G, R`<br>
  - RGB is flipped to align with the TARGA file format<br>
* light.txt order: `x, y, z, B, G, R` <br>
  - This code is written for white light only (RGB = (1,1,1))
<br>
 <img src = "spheres.jpg" width=300></td>
