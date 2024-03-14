[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/c8ng1gdc)
# Course project - Mine Navigation Search and Rescue (Tomas and Thomas)
## Table of Contents
- [Project Challenge and Instructions](#project-challenge-and-instructions)
- [Introduction](#introduction)
- [Demonstration Video Final Maze](#demonstration-video-final-maze)
- [Buggy Hardware and Set-up](#buggy-hardware-and-set-up)
- [General Functionality](#general-functionality)
  - [Overview/Instructions for Operation](#overviewinstructions-for-operation)
  - [In Depth View of Code Path](#in-depth-view-of-code-path)
- [Movement Calibration](#movement-calibration)
- [Colour Calibration](#colour-calibration)
- [Exemptions?](#exemptions)

## Project Challenge and Instructions
<a name="project-challenge-and-instructions"></a>

### Challenge brief

Your task is to develop an autonomous robot that can navigate a "mine" using a series of instructions coded in coloured cards and return to its starting position.  Your robot must be able to perform the following: 

1. Navigate towards a coloured card and stop before impacting the card
1. Read the card colour
1. Interpret the card colour using a predefined code and perform the navigation instruction
1. When the final card is reached, navigate back to the starting position
1. Handle exceptions and return back to the starting position if final card cannot be found

### "Mine" environment specification

A "mine" is contstructed from black plywood walls 100mm high with some walls having coloured cards located on the sides of the maze to assist with navigation. The following colour code is to be used for navigation:

Colour | Instruction
---------|---------
Red | Turn Right 90
Green | Turn Left 90
Blue | Turn 180
Yellow | Reverse 1 square and turn right 90
Pink | Reverse 1 square and turn left 90
Orange | Turn Right 135
Light blue | Turn Left 135 
White | Finish (return home)
Black | Maze wall colour

Mine courses will vary in difficulty, with the simplest requiring 4 basic moves to navigate. More advanced courses may require 10 or moves to navigate. The mines may have features such as dead ends but colour cards will always direct you to the end of the maze. Once the end of the maze has been reached, you must return to the starting position. An example course to navigate is shown below. You do not know in advance which colours will be in the course or how many.

![Navi Diagram](gifs/maze.gif)

## Testing

Final testing will take place in the CAGB 6th Floor Drawing Room and testing areas will be provided around the mechatronics lab. You will not know the exact layout of the maze(s) in advance. You will also be asked to give a short presentation on the testing day.

## Introduction
<a name="introduction"></a>
[Mention code management using GitHub - working on separate functionalities and then pulling them together]

## Demonstration Video Final Maze
<a name="demonstration-video-final-maze"></a>
[Link to demonstration video showcasing the final maze]

## Buggy Hardware and Set-up
<a name="buggy-hardware-and-set-up"></a>
[Description of buggy hardware and set-up instructions]

## General Functionality
<a name="general-functionality"></a>
[Explanation of general functionality of the project]

### Overview/Instructions for Operation
<a name="overviewinstructions-for-operation"></a>
[Overview and instructions for operating the project]

### In Depth View of Code Path
<a name="in-depth-view-of-code-path"></a>
[Detailed explanation of the code path]

## Movement Structure
<a name="movement-calibration"></a>

### Movement Process
[Explanation of movement process]

### Movement Calibration

## Colour Sensing Structure
<a name="colour-calibration"></a>

### Colour Sensing Process
[Explanation of movement process]

### Colour Calibration
[Explanation of colour calibration process]

## Exemptions?
<a name="exemptions"></a>
[Explanation of any exemptions or exceptions]


