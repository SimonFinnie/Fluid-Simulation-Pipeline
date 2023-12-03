# FluidSimulationPipeline

This project is a custom fluid simulation pipeline that replicates the properties of fluids by solving the discretised navier stokes equations for velocity.
This project utilises compute shader based implementations of fluid simulations for improved performance.
Currently, this project only supports 2D fluid simulations, with the plan to implement both 3D rendering and 3D fluid simulations in the near future.
The visual output of this project is currently the velocity vector fields produced from this fluid system, with x-y values being mapped to red-green colouration.

Notable features of the fluid system:
* Cubic interpolation and RK3 based advection implementation.
* MAC grid structure.
* SOR based PDE solver.
* Highly parallelised OpenGL compute shader implementation.
* Solid wall boundary conditions for the outer walls of the textures.

<p float="left" align="center">
  <img src="/Images/FluidInteraction.gif" width="300" />
  <img src="/Images/FluidRun.gif" width="300" /> 
  <img src="/Images/FluidSpiral.gif" width="300" />
</p>


  # How to Build the Project
  This project is setup with a CMake file, which allows it to be compiled on multiple different platforms easily.
  Currently, this project has only been properly tested on Windows, but Linux build instructions are planned for the future.


## Dependencies

  This project relies on the following libraries:

  * OpenGL 4.3
  * glm
  * glfw3
  * glad
 
  ## Windows

  It is recommended that as an initial step, the above libraries be installed using vcpkg.
  Thus is not a requirement, and the libraries can optionally be installed manually.
  
  ### Visual Studio
  The project can be setup with the following steps:
  * If using vcpkg make sure to have applied the following command: "vcpkg integrate install" to connect libraries with visual studio
  * Clone the project into a visual studio project.
  * Click the dropdown at the top that says x64-Debug or x64-Release and select Manage Configurations
  * Find the section that says "CMake toolchain file" and replace what is in it with your vcpkg.cmake directory
  * Then build the project.
  This process will build a executable in out/build/x64-*****/FluidSimulationPipeline/
  From here you are free to edit the code as you wish, and rebuild and so on.
  
