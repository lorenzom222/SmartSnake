#!/bin/bash

# Compile the program
g++ driver.cpp smart.cpp snake.cpp -lncurses -o snake_game

# Run the program
./snake_game
