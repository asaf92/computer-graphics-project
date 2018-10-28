# Computer Graphics 203.3710, Winter 2018/2019
## Students Team data

Name: 'Asaf Agami'  
Student Number: '203941661'

### Lecturer
*Roi Poranne*

[roiporanne@cs.haifa.ac.il](mailto:roiporanne@cs.haifa.ac.il)

URL: [https://www.inf.ethz.ch/personal/poranner/](https://www.inf.ethz.ch/personal/poranner/)

Office Hours: By appointment.

### Assistant
*Roy Velich*

[rvelich@campus.haifa.ac.il](mailto:rvelich@campus.haifa.ac.il)

Office Hours: By appointment.

### Lectures:
Sundays at 16:00pm - 20:00pm

Main Building, Room 713

## Course Description

This course provides an introduction to the foundations of Computer Graphics. We will cover the basic mathematical concepts, such as 2D and 3D transformations, shading models, and rendering techniques. The ultimate goal of this course is to develop a basic model viewer.

Students will experiment with modern graphics programming and build a renderer using C++ and OpenGL.

By the end of the course, you will be able to:

* Explain and apply the fundamental mathematical concepts used in rendering.
* Implement a basic graphics pipeline based on rasterization.
* Develop simple graphics programs in C++ using OpenGL and GLSL.

*Textbook*:
Interactive Computer Graphics: A Top-Down Approach with Shader-Based OpenGL ,6th Edition

## Assignments

* [Assignment 1: Wireframe Viewer](Assignment1/homework1.pdf)

* [Assignment 2: Basic Shading](Assignment2/homework2.pdf)

* [Assignment 3: OpenGL Renderer](Assignment3/homework3.pdf)

## General Instructions

### Companion Videos
All of the instructions below are demonstrated in the two following videos:

* [Setup Environment](https://youtu.be/irAP4DGwvPM)
* [Generate Project with CMake](https://youtu.be/84wIbGCKYgA)

The videos were made using a Windows system and Visual Studio 2017. CMake also supports project generation for other environments and IDEs, such as CodeBlocks and Eclipse - You should use one of those if you're not running a Windows system.

### Installing Git and CMAKE
Before we can begin, you must install Git, a version control system which you need for handing in your assignments, and for keeping track of your progress. We refer you to the online [Pro Git book](https://git-scm.com/book/en/v2) for more information. There you will also find [instructions](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git]) on how to to install it. We recommend using [SourceTree](https://www.sourcetreeapp.com/) or [GitHub Desktop](https://desktop.github.com/) as a GUI for Git.

We use CMake for cross-platform builds. On Windows you can download it from: [https://cmake.org/download/](https://cmake.org/download/). If you are using Linux or macOS, we recommend installing it with a package manager instead of the CMake download page. E.g. on Debian/Ubuntu:
```
sudo apt-get install cmake
```
or with MacPorts on macOS:
```
sudo port install cmake.
```

### Visual Studio
For those of you who work on Microsoft Windows, we recommand to use Visual Studio 2017 as your IDE. You can download Visual Studio 2017 Community Edition from here [https://visualstudio.microsoft.com/vs/community/](https://visualstudio.microsoft.com/vs/community/).

### Cloning the Assignment Repository
Before you can clone your private assignment repository, you need to have an active [Github](https://github.com/) account. You can then create your own private repository or join and existing one by following the link we sent you via moodle. The name of the repository will always have the form "computergraphics2018-'Team_name'", where 'Team_name' is replace by the name you chose.

Next, you will need to clone it to your computer, either by using SourceTree, or by typing the following in command line.
```
git clone --recursive https://github.com/computer-graphics-fall-2018-2019-haifa/project-'Team_Name'.git
```
This can take several minutes.

Next, go into the newly created folder, and add the base assignment repository as a remote (pay attention to capital letters):
```
cd project-'Team_Name'
git remote add base https://github.com/computer-graphics-fall-2018-2019-haifa/project.git
```

Note: You only have to do this once. This will allow you to recieve updates to the repository from us.


You should now have your local clone of the assignment repository. Take a look at the new repository folder and open 'README.md'. It contains the text you are currently reading. Please fill in your name and student number (for both team members) at the top of this file and save. Then, you need to stage, commit and push the changes:
```
git add README.md
git commit -m "Adjust README.md"
git push
```
**Never push large files to the remote repository! You should almost always only push code. Never push executables or build data**

Note: This is how you commit changes manually from the command line. You can also commit changes using a dedicated graphical interface, such as GitHub Desktop. You can see how to do it in the videos linked above.

Please refer to the git book for an explanation on the differences between those steps.
You should be able to see your name online on your private repository: https://github.com/computer-graphics-fall-2018-2019-haifa/project-'Team_Name'.git

### Building the code
Use the CMAKE gui as explained in class to create a visual studio solution, or use your compiler/IDE of choice. For a better workflow, we recommend placing a /build folder inside your code folder, where all the build information will be generates.
**Never push the build folder to the remote repository!**

Once the solution has been generated, open it with visual studio. Check that it compiles and runs.

### Workflow
In general, you should use Git to commit your changes as often as possible. This will help you to backtrack bugs and also serve as a backup mechanism. For more information we refer you to the [Pro Git book](https://git-scm.com/book/en/v2/Git-Basics-Recording-Changes-to-the-Repository).

We will notify you when new assignment are up. New assignments can be pulled from the base repository by entering the command:
```
git pull base master
```

### Solution Submission

To submit your final solution of the assignment please add the following commit message: "Solution assignment X". E.g:
```
git commit -m "Solution assignment X"
git push
```
