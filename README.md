# Circuitry

NOTE: This project is still work in progress and mostly for my own amusement/education.

This is a little sandbox where I try to create specifications for logic circuits for different integer number formats (see [Wikipedia](https://en.wikipedia.org/wiki/Signed_number_representations) for a good summary of those).

I implemented these specifications in software instead of using a program like logisim because a) I can easily create automated tests in software and b) I prefer writing code over using UIs for creating diagrams.

## How to read the code

The entire code is currently in [numbers.c](./numbers.c).

The code starts with a bunch of macro definitions. These serve to a) make code using bit-trickery more readable and b) make it very clear what building blocks are available for creating the logic circuits.
