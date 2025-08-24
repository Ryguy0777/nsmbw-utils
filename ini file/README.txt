ABOUT:
This utility allows for ini files to be read and parsed by the game.
Currently all logic for this must be on a case-by-base basis, however this utility provides the necessary headers to do so.

Compatible with: NSMBWer+, Newer 1.30

INSTALLATION:
    
    Add the following symbols to kamek_pal.x
        ReadNextCharUTF8__Q34nw4r2ut14CharStrmReaderFv = 0x80229380;
	    strstr = 0x802E1F78;
	    XStrToU32___FPCUc = 0x80235B50;
	    atoi = 0x802E23F8;
	    GetCharPtr_ = 0x8042AF68;
	    GetCharOnMem___FPCUc = 0x80235960;
    Add ini.h to either the src or include folder of your project.

NOTES:
    By itself, this does nothing. You will need to install custom code that uses this utility for it to serve any purpose.
    It is provided here to simplify the installation process for any custom code that reads an ini file.