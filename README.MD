

# About This Repository
This repository is a general collection of graph-based algorithms. For this moment, the focus is algorithms for finding dense subgraphs. In particular, I aim to contribute to the study of nucleus decomposition.

## Navigating The Repository
The repository is set up as follows
 - `/bin` holds all compiled binaries.
 - `/data` holds all input and output data. 
   - `/data/input` contains formatted files used by our algorithms
   - `/data/input_raw` contains files from online databases such as [SNAP](https://snap.stanford.edu/data/) which our algorithms cannot yet use as they are not yet formatted (see "Formatting Raw Inputs" below). 
 - `/src` contains all of our graph-based data structures and algorithms; respectively, these are stored in /src/collections and /src/algorithms.
 - `/test` contains all unit tests for our data structures and algorithms. 

## Inputs and Outputs
### Formatting Raw Inputs
To format files from [SNAP](https://snap.stanford.edu/data/), we provide /tools/process_raw_input.py which formats every file in /data/input_raw into a supported format, and saves the new format to /data/input. Note that every file is processed every time the script is ran.

### Outputs
Outputs are categorized by their timestamp. Each instance of running an algorithm with an output will generate a folder named with the current timestamp formatted as `mm/dd/yyyy-hh:mm:ss:AM/PM` (`%m/%d/%Y-%I:%M:%S%p`). Each folder contains a series of files that contain edge lists, which I call a generation. Each generation (file) is numerically named and this name corresponds to the current state of the graph at that generation. For example, if the file name is `0`, then that file contains the state of the graph at iteration `0` for whatever algorithm is being ran. 

## Unit Testing
As mentioned, `/test` contains all unit tests for our data structures and algorithms. Most unit test was written before the algorithms were constructed. `/test/test.c` runs all unit tests.

## Compiling
You can compile this project using the `Makefile`  provided by using the command `make`. `Makefile` produces two binaries, `/bin/graphs` and `/bin/tests`. `/bin/graphs` will run the main file at `/src/main.c` and serves as a general entry point for the project, while `/bin/tests` runs `/test/test.c` which runs all unit tests within `/test`.

# Papers Referenced
This section contains references to all referenced algorithms that are (+), will be (-), or might be (&) implemented in this repository. I apologize to those whose name could not be properly written out. 

**A. K-Clique**
   1. (+) [Efficient k-clique Listing with Set Intersection Speedup](https://arxiv.org/pdf/2203.13512.pdf) (Yuan et al.)
   2. (&) [Parallel Clique Counting and Peeling Algorithms](https://arxiv.org/pdf/2002.10047.pdf) (Shi, Dhulipala, Shun)
   
**B. Nucleus Decomposition**
   1. (-) [Finding the Hierarchy of Dense Subgraphs using Nucleus Decompositions (Sarıyuce, Seshadhri, Catalyurek)](https://arxiv.org/pdf/1411.3312.pdf) (Sarıyuce, Seshadhri, Catalyurek)
   2. (-) [Local Algorithms for Hierarchical Dense Subgraph Discovery](https://arxiv.org/pdf/1704.00386.pdf) (Sarıyuce, Seshadhri, Pinar)
   3. (-) [Theoretically and Practically Efficient Parallel Nucleus Decomposition](https://arxiv.org/pdf/2111.10980.pdf) (Shi, Dhulipala, Shun)
   4. (-) [Julienne: A Framework for Parallel Graph Algorithms using Work-efficient Bucketing](https://people.csail.mit.edu/jshun/bucketing.pdf) (Dhulipala, Blelloch, Shun)
   
**C. Other**
   1.  Bucketing used in B.4..
       1. (-) [Parallel Algorithms for Butterfly Computations](https://arxiv.org/pdf/1907.08607.pdf) (Shi, Shun)
       2. (&) [High-Performance Parallel Graph Coloring with Strong Guarantees on Work, Depth, and Quality](https://arxiv.org/pdf/2008.11321.pdf) (Besta et al.)
