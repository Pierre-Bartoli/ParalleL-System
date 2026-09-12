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

And can continue. Thoses letters can also represent an action. For example `F` is often understood by a step forward, or `+` and `-` is a rotation by a given angle. The Koch curve can draw beautiful fractals with the **axiom** F and the **rule** (F -> F+F-F-F+F), then with `n = 4` its shows this fractal :


