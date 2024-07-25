ABOUT:
This code is intended to work with mkwcat's P1 Character Change patch.
It causes the file select to display the icon and lives of the last character used for Player 1 instead of Mario.

INSTALLATION:
Game Files:
    add dateFile.arc to your Others folder 

Code:
    in kamek_pal.x add the symbols:
	returnFScrap = 0x8077da14;
	CSMonExecute = 0x8077aca0;
    add fileselecticons yaml, S, and cpp
    in game.h, you will find a SaveBlock class, in it add "u8 lastP1Character;" (no quotes) in a safe place
    (I reccomend adding it to a union inside the padding)