# Randomized Prim's Maze-Generator

Here i will make a Grig based Maze generater.
Whit the Randomized Prim's algorithem.

Build on the GamePlay Programing framework from school.

This is for my GamePlay Programing research exam.

![ezgif-7-e62f9d283b](https://user-images.githubusercontent.com/24572084/150207546-5af80af5-586e-4e95-a4fd-2b6b7796210e.gif)

this Maze Generaroe uses the Randomized Prim's algorithem. So it adds more random path, turns and unpredictability into the maze.

## prosses:

The maze starts out as just a grid full of walls. And 1 free space.
It well then take 1 of the walls next to the free space randomly.

Then it will firstle make sure it does not have 2 or more free spaces next to this wall.
If it does have 2 or more it will ignore this space and keep it a wall. 
So that it does not make the maze to easy. Whit to mutch options to the desired location.
If it als less then 2 free spaces next to the wall. It will them turn the wall into a free space.
And it will add the neighboring walls into the possible changeable walls.

So it can take a random one out of that list. To turn into a free space.


## New Maze:

It will always try to make a new free space so long als there is a wall in the possible changeable walls list.

When making a new maze it just refils the whole grid whit wals. And frees up the 1st space. Witch in turn adds 2 walls into the possible changeable walls list.

And due to adding more into that list. it will automatacly make a new maze.

## Diferent algorithems

There are multiple different maze generation algorithems. That are sometime's used.

###  depth-first search:
Like the Randomized Prim's algorithem. Depth-first search also takes a random wall. 
But unlike the Randomized Prim's algorithem. Depth-first search only takes a random wall from the neighboring walls of the most curently freed up space.
When it cant make a new free space next to the most current freed up space. It will go to the most current one that can make new free spaces next to himself.

This however does give the problem that there will be less/shorter branching paths. That leed to dead ends.

But this algorithm can lead to a bit more variety in solutions.

###  Randomized Kruskal's algorithm:
Like the Randomized Prim's algorithem. Randomized Kruskal's algorithm does not take a neighboring wall of the most curently viseted free space.
But unlike the Randomized Prim's algorithem. Randomized Kruskal's algorithm does also not take a wall next to a already opend free space.

It will take absolutely any wall still left in the grid that can be made into a freesapce.

# Maze-Generator whit A*

![ezgif-7-5cbe21f07c](https://user-images.githubusercontent.com/24572084/150222048-7bf05f3f-17ef-4867-8d15-8b6d9e6f5d46.gif)

Finaly i added the maze generator whit the A* pathfinding algorithm.

A few thing needed to be added whit importing the maze generator to the A* algorithm.
Mainly that i needed to actualy spesefy the wals are actual walls. so the character does not just simply walk over the walls.

So i simply went over all the walls after the maze generation was done. And removed any connections to eatch wall.
