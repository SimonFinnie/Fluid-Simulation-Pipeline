# CMakeList.txt : CMake project for FluidSimulationPipeline, include source and define
# project specific logic here.
#
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()
find_package(glm CONFIG REQUIRED)
if(WIN32)
	find_package(glad CONFIG REQUIRED)
endif(WIN32)
find_package(glfw3 CONFIG REQUIRED)
# Add source to this project's executable.

if(WIN32)
add_executable (FluidSimulationPipeline "FluidSimulationPipeline.cpp" 
"AdvancedFluidSim2D.cpp" "AdvancedFluidSim2D.h" "BaseFluidSimulation.cpp" "BaseFluidSimulation.h" "ComputeShader.cpp" "ComputeShader.h" 
"Controller.cpp" "Controller.h" "FluidController2D.cpp" "FluidController2D.h" "RenderCamera.cpp" "RenderCamera.h" "Renderer.h" 
"Renderer.cpp" "Shader.cpp" "Shader.h" )
elseif(LINUX)
add_executable (FluidSimulationPipeline "FluidSimulationPipeline.cpp" 
"AdvancedFluidSim2D.cpp" "AdvancedFluidSim2D.h" "BaseFluidSimulation.cpp" "BaseFluidSimulation.h" "ComputeShader.cpp" "ComputeShader.h" 
"Controller.cpp" "Controller.h" "FluidController2D.cpp" "FluidController2D.h" "RenderCamera.cpp" "RenderCamera.h" "Renderer.h" 
"Renderer.cpp" "Shader.cpp" "Shader.h" "glad.h" "glad.c" )
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AdvancedAdvection.comp
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/SOR.comp
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ForceApplication.comp
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MACDivergence.comp
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MACGradientSubtraction.comp
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CopyTexture.comp
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TextureVisualiser.fgs
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TextureVisualiser.vs
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

if(WIN32)
target_link_libraries(FluidSimulationPipeline PRIVATE glm::glm glad::glad glfw)
elseif(LINUX)
target_link_libraries(FluidSimulationPipeline PRIVATE glm::glm glfw)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FluidSimulationPipeline PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
