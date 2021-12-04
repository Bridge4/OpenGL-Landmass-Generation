OpenGL-Landmass-Generation

I made a landscape generation engine, code for camera, shader compilation and the mesh class was 
inspired by the following:

https://github.com/VictorGordan/opengl-tutorials/tree/main/YoutubeOpenGL%208%20-%20Camera
https://github.com/emeiri/ogldev/blob/master/tutorial04/tutorial04.cpp
https://learnopengl.com/Model-Loading/Mesh

What I have Complete:
-Map generation is complete
-Lighting is complete 
-Camera complete

What's left to do:
-Planning on creating different plants procedurally using L-systems but if that doesn't work 
then I'll make the objects by hand manually and texture them
-As a stretch goal I want to create a particle shader that simulates snowfall, over the course
of this project I've become familiar with how shaders work and by extension how to manipulate uniforms so I'm decently confident I can get this done on time. 

Keybinds:
WASD and Arrow keys for movement and looking around
N to generate new maps
B to change biome coloring <- will add more variations later for now the proof of concept works
Shift button makes all these decrease rather than increase
1 increases Frequency of noise 
2 increases Lacunarity of noise 
3 increases Gain of noise
4 increases Octaves of noise leading to more stacked functions which can result in more rough looking terrain, too much or too little will increase and decrease map size
