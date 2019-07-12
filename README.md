#2048 Solver

Based on code from Maurits van der Schee (available at: <https://github.com/mevdschee/2048.c>), I developed a program that plays 2048 using a depth-first search algorithm. This simple program was then taught a bunch of heuristics inspired by my own 2048-playing experience, until it could consistently beat my high score. 

How to use: Firstly, run `make` to create an executable called 2048. Running `./2048` alone will allow you to play the game in terminal with your arrow keys. See how well you can do, then run `./2048 ai avg 7 slow` to see if you can beat my AI!  

**AI options:** Several different settings are available for the AI. The general syntax is `./2048 ai <max/avg> <max_depth> slow`

* `<max/avg>` tells the AI whether to use the maximum possible score generated from a given move, or the average possible score that can be generated from all the things that can happen from one move. Generally, `max` is more risky, and `avg` performs better.
* `<max_depth>` is an integer that tells the AI how deeply to perform the depth first search. Larger numbers yield more intelligent programs, but perform exponentially slower.
* `slow`, when toggled on, adds a slight delay to the robot's moves. This is good if you want to watch it play - but if you just want the results, toggle this off. 

A small output file (output.txt) is created every time the AI is run, containing the score of the AI, the largest tile it achieved, the number of nodes generated/expanded, and its runtime.

This code was originally developed for the undergraduate computing subject COMP20003 - Algorithms and Data Structures. I have included my original report as well. This provides plots showing how the performance of my AIs depend on the input parameters that are chosen, and also details about the implemented heuristics.

Happy coding!  
~Benjamin Metha