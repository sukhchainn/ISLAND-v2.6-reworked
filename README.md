# ISLAND-v2.6-reworked
ISLAND 2 is a text adventure game. This repository is a port of the original DOS game into GCC.

I am just porting the game and this game is public domain so I am neither responsible nor liable for anything.
You can only download the source code, compile the code and make an executable and share the code present in this repository.
The portion of the code that I have written is MIT licenced, but the original code is not.

COMPILE
for Windows
  ```gcc
  gcc -o main ISLAND.C include/conio.o -trigraphs 
  ```
  
for Linux
  ```gcc
  gcc main -o ISLAND.C include/conio.o
  ```
