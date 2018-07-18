# Energetic Vibrations Audio Visualiser
This project was developed by [MindBuffer](https://www.mindbuffer.net/) for the Energetic Vibrations Projection Mapping Installation as part of the Beyond Perception exhibition at Scienceworks. It is developed using openFrameworks and GLSL.

![Figure 1-1](https://github.com/JoshuaBatty/EnergeticVibrationsAV/blob/master/images/main_setup.png "Main Projection Screen")

The projection consists of 21 individual objects that represent the chair layout in the exhibition space. When a chair is sat on, the corresponding 3D object changes from a black and white box to a colourful rotating object. This can be simulated using the maxmsp patch that comes with this repo. Using the max patch, you can either click on the individual toggles, or turn on randomise mode. In doing so, you are able to experience the different generative modes that are activated depending on various seat trigger combinations. Try it out! 

![Figure 1-2](https://github.com/JoshuaBatty/EnergeticVibrationsAV/blob/master/images/max_test_patch.png "Max Test Patch")

The software also comes with a user interface displayed in a second window. These are the parameters that define how the object looks and behaves. The second panel is responsible for the various effects that are applied to active chairs. These effects include:
* Pixilation  
* Alpha Trails
* Super Feedback 
* Reaction Diffusion 

Each of the effects can be mixed into each other leading to a wide variety of possible visual aesthetics.  

![Figure 1-3](https://github.com/JoshuaBatty/EnergeticVibrationsAV/blob/master/images/user_interface.png "User Interface")

Finally, the final graphics have a mask applied to them in order to contain them within the projection surface on the wall. This shape can be found in bin/data/models

The below image demonstrates how this mask contains the shapes within the borders of the projection surface. 

![Figure 1-4](https://github.com/JoshuaBatty/EnergeticVibrationsAV/blob/master/images/mask_demo.png "Mask Demo")

