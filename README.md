# 2SPIM131-1 24H Spillmotorarkitektur Compulsory 1

Compulsory 1 for the 2SPIM131-1 24H Spillmotorarkitektur course.
This repository is merely meant as an assignment delivery.

## Pre-notes
This project utilizes OpenFramework, which we used for graphics in 2PRO101-1 22V Programmering I.
Due to the way this dependency is implemented, it's very likely it will not compile on remote machines.

A compiled version is supplied.

OpenFramework contains built-in graphics handling, making it a quick solution to screen rendering, and optimal for testing for this compulsory.
Future projects for this course will likely utilize OpenGL for both 2D and 3D graphics.

## Program
The program is a simple simulation of ball/ball collision. 
10 balls (can be changed via command line) with random sizes and mass are scattered around the board.
You can shoot one ball using the mouse (click to fire) similar to a Billiards game.

There is no friction in this system.

## Problems
- Ball/wall interaction on the edge of the board is not correctly simulated and may cause incorrect behavior.
- Balls are not settled at the start of the simulation, and they may overlap.
- There is no friction.
- No graph is used to separate the balls for collision checking. (part of compulsory)

## Command Line
guiTest.exe [number of balls]