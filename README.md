# Intro
This project is a graphics engine that I've built during my Computer Graphics (203.3710) course in the University of Haifa. The project was intended for either one or two people to work on. The master branch I worked on is left here as is with all my commits so my entire progress is on display. By the time I started working on this project I was a professional C# programmer for several months, and had some experience with C++ (mostly from my OOP course), but haven't had any experience with computer graphics whatsoever.

In this document I will go through the engine capabilities and show the progress I made.

## Progress
### Wireframe
In the beginning I started with simple tasks such as drawing lines and drawing triangles. I also parsed 3D models in .obj files, introduced cameras, projections and transformations. I combined the two so that I can parse .obj files, get a set of transformed triangles and draw each triangle on the screen, which gave me a wireframe viewer:
![](https://i.ibb.co/THYpx7K/image.png)
**A wireframe presentation of the cow model**

### Lighting & Shaders
The next assignment was to write and use C++ shaders, I assume that we can understand how shaders work before writing them in OpenGL in the final assignment, and add lighting to the scene. There was also a bonus for including post-effects I believe (which I did by adding fog). I was required to include point light sources as well as parallel light sources, and write Vertex & Fragment shaders. There's an option to switch between flat, Gouraud and Phong shaders. 
![alt text](https://i.ibb.co/tC6bJTD/image.png)
**A bunny model with some lighting on**
![alt text](https://i.ibb.co/2ZrnFsh/image.png)
**A fog post-effect**

### OpenGL shaders
The final assignment was to switch the implementation of the shaders to OpenGL. There were also some other bonus tasks such as including textures, including non-linear textures (for example a texture on a sphere), [toon shading](https://en.wikipedia.org/wiki/Cel_shading), bump mapping etc... We were supposed to only have one bonus task, and I believe I finished with 2 bonus tasks and another one which wasn't finished (bump mapping). I received a 100 grade for this task.
![alt text](https://i.ibb.co/FBgd94K/image.png)
**The same cow from the first assignment, along with a textured box and several light sources**
![](https://i.ibb.co/pnhPv5f/image.png)
**Same scene but with toon shading**
![](https://i.ibb.co/wyRqPNH/image.png)
**Sphere texture**

## Extra features
I added a lot of features for my personal convenience and challenge. Here are some that come to mind:
### WASD moving
At some point between assignment 2 and 3 I added an option to navigate around the mesh with the mouse (by pressing the mouse 3 key) and WASD keyboard buttons (Like in an FPS). I did it by first implementing simple "tilt" and "pan" interfaces which I used with GUI controls first, and then I linked it to mouse keys. I did the same but with "move" interfaces (that I used for several other things before I thought about WASD moving)
### Debug info
My program had a lot of debug info, mostly for evaluating run-time and performance. Before assignment 3, the shaders were written in C++ and in a non-parallel way. Loading even 2 high-detail models (such as the cow) would make the fps drop to around 1 FPS when building the project without any optimizations. I decided to analyze it by tracking the time that any high-level function takes. Interestingly, I discovered this way that the heaviest operation is actually clearing the buffer before rendering a new frame. This operation of course would take much less time when compiling on "Release" mode. 
I also added a menu option to show the calculated transformation and projection matrices, as well as compare the matrices I calculated and the glm library calculation results.
### Performance tweaking
As I mentioned, performance was a major issue before switching to OpenGL. I introduced a lot of tweaks to the code in some algorithms. 
I used some pragma commands to prevent optimization of some parts, so I can enjoy the performance boosts that I get with release mode as well as keeping the ability to thoroughly debug parts of my code. I also included some other tweaks such as skipping loops early when possible, and not rendering objects that are identified early as out of the camera view.

Here are some of the original README.md notes:
# Computer Graphics 203.3710, Winter 2018/2019 - Course Details
## Student 
Name: 'Asaf Agami'  

## Lecturer
*Roi Poranne*
[roiporanne@cs.haifa.ac.il](mailto:roiporanne@cs.haifa.ac.il)
URL: [https://www.inf.ethz.ch/personal/poranner/](https://www.inf.ethz.ch/personal/poranner/)

## Assistant
*Roy Velich*
[rvelich@campus.haifa.ac.il](mailto:rvelich@campus.haifa.ac.il)

## Course Description
This course provides an introduction to the foundations of Computer Graphics. We will cover the basic mathematical concepts, such as 2D and 3D transformations, shading models, and rendering techniques. The ultimate goal of this course is to develop a basic model viewer.

Students will experiment with modern graphics programming and build a renderer using C++ and OpenGL.

By the end of the course, you will be able to:

* Explain and apply the fundamental mathematical concepts used in rendering.
* Implement a basic graphics pipeline based on rasterization.
* Develop simple graphics programs in C++ using OpenGL and GLSL.

*Textbook*:
Interactive Computer Graphics: A Top-Down Approach with Shader-Based OpenGL ,6th Edition
