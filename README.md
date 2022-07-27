# sfmEvac
Evacuation simulations using the Social Force Model, for binary mixtures of egoistic and cooperative pedestrians. Imitation of cooperative behaviour is implemented.
## Compile the cpp:
g++ sfm_Evac.cpp -std=c++11 -o sfmEvac

## Run the simulations:
./sfmEvac params.1

If you want to do mutiple simulations with the same parameters but different initial conditions, use "make_params.py" to generate the params file with different random seeds

(A folder is created with the name "sim_1")

"frames" folder contains the .dat files with the positions of every pedestrian in the room

"salida.txt" contains the parameters of the simulation, the evacuation time of each pedestrian and the total evacuation time

"dens_exit.txt" contains the density of pedestrians in the semicirce of radius r_dens arround the center of the exit, at each timestep 
### Params file
frameSaveRate -> frame saving frequency

seed -> for the random generator

dt -> timestep

tmax -> max duration of the simulation before ending it (in simulation time)

NCoop -> number of "cooperative" pedestrians

NEgo -> number of "egoist" pedestrians

tau_coop -> for the "cooperative" pedestrians' desire force

tau_ego -> for the "egoist" pedestrians' desire force

A_coop -> for the "cooperative" pedestrians' social force

A_ego -> for the "egoist" pedestrians' social force

B_coop -> for the "cooperative" pedestrians' social force

B_ego -> for the "egoist" pedestrians' social force

kappa -> friction force coefficient

desiredVel_coop -> "cooperative" pedestrians' desired velocity (desire force)

desiredVel_ego -> "egoist" pedestrians' desired velocity (desire force)

r -> pedestrians' radius (disks)

m -> pedestrians' mass

L -> length of the room

H -> height of the room

D -> width of the exit, located at the bottom-middle of the room

phi -> half of the pedestrians' field of view

cteFOV -> weight of the social force with pedestrians outside the field of view

vmax -> max velocity allowed for any pedestrian

r_dens -> radius of the semicircle arround the exit's center where to register the crowd density

r_imit -> radius of the "imitation area" around cooperative pedestrians

## Make the animation's frames:
Inside "sim_1/frames" run "pre_movie.py"

To make the animation, generate the .png files and use ffmpeg
### How to run it
python3 pre_movie.py (creates a .png for each frame in the folder)

python3 pre_movie.py 8 (creates a .png only for frm_000008.dat)

python3 pre_movie.py 34 178 (creates a .png for each frame in the folder between frm_000034.dat and frm_000178.dat)

#### Example of a simulation
![Video of a simulation and the live density in the room](https://media.giphy.com/media/PyWTI8CgAglDImKty8/giphy.gif)
