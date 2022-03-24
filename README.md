# sfmEvac
Evacuation simulations using the Social Force Model, for binary mixtures of pedestrian attitudes
## Compile the cpp:
g++ sfm_Evac.cpp -std=c++11 -o sfmEvac
## Run the simulations:
./sfmEvac params.1

(A folder is created with the name "sim_1")

"frames" folder contains the .dat files with the positions of every pedestrian in the room

"salida.txt" contains the parameters of the simulation, the evacuation time of each pedestrian and the total evacuation time

"dens_exit.txt" contains the density of pedestrians in the semicirce of radius r_dens arround the center of the exit, at each timestep 
## Make the movie frames, density frames or both:
Inside "sim_1/frames" run "pre_movie.py", "pre_density.py" or "pre_movie+density.py"

To make the animation, generate the .png files and use ffmpeg
### How to run it (pre_density and pre_movie+density works the same way)
python3 pre_movie.py (creates a .png for each frame in the folder)

python3 pre_movie.py 8 (creates a .png only for frm_000008.dat)

python3 pre_movie.py 34-178 (creates a .png for each frame in the folder between frm_000034.dat and frm_000178.dat)

#### Example of movie+density video
![Video of a simulation and the live density in the room](https://media.giphy.com/media/PyWTI8CgAglDImKty8/giphy.gif)
