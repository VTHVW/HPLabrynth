# HPLabrynth
Challenge #2 Second Semester 2025 HPC ing mat

### Autor :
Turco Vale 10809855

## Library
HPLabrynth.h has 2 namespaces **HPOpts** and **HPLabrynth**

The former is used to parse argc and argv and control the execution,
the latter is used to make the labyrinth and implements the DS to solve them.

### HPLabrynth::HPLab
This class implements the DS. It uses an array of the `HPLabrynt::Set` struct:
a simple struct containing the reference to the parent, the rank, and a valid parameter.
"valid" if going to be false iff we are on an "x".

This way the Set array can be of fixed size: N*M.
Furthermore, `link`,`find` and `unite` will just do nothing if they are called on an invalid cell
and therefore there is no added cost to it.

`HPLab` Has 2 constructors: The first one accept a pre-made Labyrinth in the form
of a struct `HPLabrynth::LabParams`. These can be fetched from a file with the appropriately named
`HPLabrynth::getLabParamsFromFile` function.
The second constructor generates a random Labyrinth using (if possible) a `random_device`
to initialize the seed for the PRNG. Then if `make_valid` it makes sure there is a path 
between start and exit using a simplified version of the algorithm described in [this wikipedia page](https://en.wikipedia.org/wiki/Maze_generation_algorithm#Iterative_randomized_Kruskal's_algorithm_(with_sets))

`HPLab::hasPath` execute the search for a path using the initialized DS.

`HPLab::toParams` return a printable version of the labyrinth. If `pretty` is set to true
it also add some color and special char to the output (it also prints walls all around the labyrinth).

### HPOpts
Please check the --help function to understand how to use the command.
(Examples will be provided in the .ipynb file)