# cpu_voxel_raycaster
Very barebones CPU Voxel Raycaster

This is the basic raycaster I used to generate visualizations for my Out-Of-Core Sparse Voxel Octree Builder ([ooc_svo_builder repo](https://github.com/Forceflow/ooc_svo_builder)) publication. It is designed to work with the _.octree_ file format that is a result of that pipeline.

The code was never published as part of the publication. People have been mailing me over the years for a tool to visualize the output of ooc_svo_builder, so here it is. Note that this code is +5 yrs old and has been unmaintained since, so it will require some work to get started, but can help you if you want to research the topic further.

![vizualization_example](https://raw.githubusercontent.com/Forceflow/cpu_voxel_raycaster/master/img/viz_example.png)

# Building & Requirements
I'm trying to get the project in a workable state state again. Here are some build notes for other adventurers:
 * Converted to VS2019 solution
 * Requires **trimesh2**: My ready-made fork can be found [here](https://github.com/Forceflow/trimesh2))
 * Requires **[GLFW](https://www.glfw.org/)**: Only very basic window creation / keyboard functionality / OpenGL context setup is used, so it _should_ be fine to use any recent version of GLFW.
 * Requires **[glew](http://glew.sourceforge.net/)**: Again, only using this to grab OpenGL extensions. Any recent version should do.

