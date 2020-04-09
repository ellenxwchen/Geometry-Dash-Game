# ECE243-Project

## Updates ##
Our project recreates the game “Geometry Dash” with C code with KEYs as user input, and using the pixel buffer for animation. The player needs to jump to avoid different obstacles, like the triangles, or spikes. Instead of allowing the player to win like we stated in our proposal, the game is continuous until the player hits an obstacle.


When the game is first loaded, the opening screen appears with the “Geometry Dash” graphic. There are 3 levels to the game. The first level can be selected if no switches are on, or SW_2 to SW_9 are on. To select the second level, turn SW_0 on, and then LEDR_0 will also be on. To select the third level, turn SW_1 on, which will also turn LEDR_1 on as well. To start playing, use any of the KEY buttons. You can also use the enter key on the PS/2 keyboard to start the game.


To control the player, use the KEY buttons to jump up to avoid obstacles. You can also use the space bar on the PS/2 keyboard to jump. Your score can be seen in the upper left corner as you play. When you die, you are taken to a “Game Over” screen, which also displays your final score. You can restart the game by selecting a level using the same instructions above, and using the KEY buttons to start a new game. 
