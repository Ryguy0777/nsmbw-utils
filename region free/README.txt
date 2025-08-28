ABOUT:
This utility allows for region unlocked save files and a single titlescreen layout for all regions & languages.

Compatible with: NSMBWer+, Newer 1.30

INSTALLATION:
    
    Add the following symbols to kamek_pal.x
        getRootPane__Q23m2d17EmbedLayoutBase_cFv = 0x80007220;
	    saveFileReadSuccess = 0x800cf8cc;
    Add regionFree.S and regionFree.yaml to your project.

NOTES:
    openingTitle.arc must be placed inside of the Layout folder. Remove any region indicator from brlyt and brlan names.
    Example: openingTitle_EU_00.brlyt -> openingTitle_00.brlyt
    PLEASE NOTE if converting a japan openingTitle to a region free layout the _13 in the brlyt and brlan names must be changed to _00.