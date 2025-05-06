ABOUT:
This allows for the coin battle/free mode level select's level list, display icons, and world names to be controlled by levelinfo

Version 1.0

INSTALLATION:
    Add the following symbols to kamek_pal.x
        continueMultiCourseButtonSetup = 0x8079E178;
		returnFromTextureCheck = 0x8079E044;
		colorTitleText__20dMultiCourseSelect_cFii = 0x807999E0;
		GameFlag = 0x8042A260;
    Add multicourseselect yaml, S, h, and cpp to your project

NOTE:
    The Coin Battle/Free Mode worlds in levelinfo must be the last two sections in the list for the recommended courses screen to work.