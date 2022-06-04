# Conways game of life (C++)

This repository contains a version of the conway's game of life that implements the [HashLife](https://www.dev-mind.blog/hashlife/) algorithm in C++.

![](https://i.ibb.co/YD8njFP/Screenshot-from-2022-06-04-21-08-54.png)

# What the conway's game of life is ?

The conway's game of life is a cellular automate made by John Horton Conway in 1970. It's a complex game simulation dispite the easy rules.

The rules of the conway's game of life are very easy to understand, the game is composed by a grid of tiles called cells, each cells cans to have 2 differents states, dead or alive, and their state depend by the neighbors:

- **2 neighbors**: the cells' state doesn't change.
- **3 neighbors**: the cell's state become / stay alive
- **others**: the cell's state become / stay dead