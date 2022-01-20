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



# Maze-Generator whit A*

![ezgif-7-5cbe21f07c](https://user-images.githubusercontent.com/24572084/150222048-7bf05f3f-17ef-4867-8d15-8b6d9e6f5d46.gif)
