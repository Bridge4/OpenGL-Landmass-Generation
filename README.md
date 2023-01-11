OpenGL-Landmass-Generation


Keybinds:

WASD and Arrow keys for movement and looking around
N to generate new maps
B to change biome coloring <- will add more variations later for now the proof of concept works
Shift button makes all these decrease rather than increase
1 increases Frequency of noise 
2 increases Lacunarity of noise 
3 increases Gain of noise
4 increases Octaves of noise leading to more stacked functions which can result in more rough looking terrain, too much or too little will increase and decrease map size



-Most Important: I think I made something really freaking cool and I'm very proud of the work I've done. 
-Created realistic looking and entirely unique landscapes at the click of a button that can be viewed however the user wants.
-I spent a considerable amount of time teaching myself how the shader pipeline works and getting shader compilation to work which took significantly more effort and time than the homeworks for camera and lighting concepts did. 
-I started from scratch with GLFW and learned how to adapt my code such that it worked with macOS as that was my primary development platform. 

Examples: 

![alt text](https://github.com/Bridge4/OpenGL-Landmass-Generation/blob/master/opengl 1.png?raw=true)

What I can improve:

-Mesh has to be redrawn each time modifications are made which results in brief frame rate drops while the new mesh is being rendered
-Could have created a more diverse color gradient for different elevations but at that point it was a matter of creating a large if-statement and didn't demonstrate understanding of any new concepts. 

Reused Code:
Provided more detailed explanations in each file with code reuse
https://learnopengl.com/Model-Loading/Mesh
https://github.com/emeiri/ogldev/blob/master/tutorial04/tutorial04.cpp
https://github.com/VictorGordan/opengl-tutorials/tree/main/YoutubeOpenGL%208%20-%20Camera
https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%209%20-%20Lighting/default.vert

------------
About the Project: 

This program I've created essentially constructs a detailed unique mesh and accurately lights it,
different meshes can be generated on the fly using the keybinds provided above. The bulk of the work went into generating initially a flat mesh out of triangles. In order to do that, I needed to figure out a way to store a large number of triangles and draw them in the correct order such that their faces aren't culled. This involved a lot of work finding out different methods of storing vertex data and resulted in the Mesh class. A Mesh is an object with its own VAO, VBO and EBO/IBO(element buffer object or index buffer object) which means that I could now create as many triangles as needed and fill the vertex vector with that information to represent them. The use of index buffers was essential to decreasing render time as the Mesh created by default is 1000x1000 vertices meanings 1,000,000 vertices total in the whole mesh. Duplicates would have destroyed the ability to create larger more intricate meshes. 

The way the landscapes are generated is by stacking Perlin Noise functions of varying frequencies and amplitudes. An increase in the frequency results in more island-like landmasses whereas an increase in lacunarity results in rougher or more detailed looking terrain TO AN EXTENT. Decreasing the Octaves variable results in a smoothing effect which demonstrates what happens when stacking smaller numbers of noise functions. The way this information is represented in the mesh is through the y value of each vertex. As a start I made each vertex flat on the y plane and then generated a Perlin Noise value for each vertex.

The height data was generated using a noise library called FastNoiseLite which saved me having to implement my own Perlin Noise functions. No need to reinvent the wheel.
