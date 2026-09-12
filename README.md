# ParalleL-System :
ParalleL-System is a L-system interpreter and render in parallel using kokkos written in C++.

## What is a L-system ?
A L-system or Lindermayer system is rewriting system. It consists of an alphabet that is recursively rewritten that make a string expanded by rules from an axiom. It can represent plants or fractals. 
For exemple let consider, 
- **alphabet** : V = {A, B}
- **axiom** : w = A
- **Rules** : (A -> AB), (B -> A)
  
All A will be replaced by AB, and all B by A. Starting with axiom A we have

`n = 0` : A

`n = 1` : AB
 
`n = 2` : AB A
 
`n = 3` : AB A AB

And can continue. Thoses letters can also represent an action. For example `F` is often understood by a step forward, or `+` and `-` is a rotation by a given angle. The Koch curve can draw beautiful fractals with the **axiom** F and the **rule** (F -> F+F-F-F+F), then with `n = 4` it shows this fractal :

![Alt text](./show.svg)

## Parallel aspect of the L-system :
An L-system is by nature **sequential implemantation**, CPU must replace rule by rule its own string. Even the render turtle is sequential. But reading the litterature about L-system can gives us a insight of how to implement a parallel version of [it](https://www.cg.tuwien.ac.at/research/publications/2009/LIPP-2009-PGL/LIPP-2009-PGL-paper.pdf). String interpration can by parallelised with managing an array of offset to fill with parallel thread in it. For the render part with turtle, the key aspect of the algorithm is by using 4x4 matrix for deplacment representation where the associativity plays a central role.

## How to install :
First download the github project, ```git clone https://github.com/Pierre-Bartoli/ParalleL-System.git```.
You can build the CPU (sequential version) or GPU version, `cd CPU` or `cd GPU`.
### for CPU (sequential) : 
`cmake -S . -B build` and `cmake --build build`, the executable is in `build/L-system_CPU`

### for GPU (parallel) :
- You must choose your backend `-DKokkos_ENABLE_<BACKEND>=ON` and architecture `-DKokkos_ARCH_<ARCHITECTURE>=ON` (find for your configuration see [here](https://kokkos.org/kokkos-core-wiki/get-started/building-from-source.html#configuring-kokkos)),

for example with CUDA with RTX 5070 : `cmake -S . -B build -DKokkos_ENABLE_CUDA=ON -DCMAKE_CUDA_ARCHITECTURES=120`.
- And compile it : `cmake --build build`.
The executable is in build `build\L-system_GPU`

## How to use :
You must launch the executable with an input file and output file. Input file is a text file that contains in this order :
- The angle of rotations in degrees
- The number of recursive loop (denoted by `n` is previous sections)
- The axiom
- Rules, one per line, in the form F=AB, where F is called predecessor (which letter is remplaced) and AB is called the successor (by what the predecessor will be remplaced). `=` symbol is required.
Then launch the programme with ```.\build\L-system_GPU -i ..\example -o Lsystem.svg```.
