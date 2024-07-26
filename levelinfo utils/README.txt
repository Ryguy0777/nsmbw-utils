ABOUT:
These utilities allow for several world and level numbers to be controlled by levelinfo's display icons, instead of being hardcoded like the original game.

Compatible with: NSMBWer+ (Newer 1.30 may be supported but not guaranteed to work)

INSTALLATION:
    Each utility requires a set of headers and code, otherwise they all have separate install instructions.
    Add levelnumber.h and utils.h to the include folder, in stage.h add "#include <levelnumber.h>" (no quotes)
    Add utils.cpp and utils.yaml to your project

NOTES:
    All of these utilities use level slot 39 in levelinfo for world information.
    All of the level icons used are different from newer. Refer to utils.cpp for a list of display ids.