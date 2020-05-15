# Multimupen
Tool that allows to control multiple mupen windows at once

# How to setup
1.Place Multimupen.exe and TASDI.dll in one directory

2.To properly use savestates you must copy whole mupen directory as many times as you need (e.g. you need 5 mupen windows then you need 5 copies). This is because simply running one .exe multiple times (or more than one exe but in same directory) causes overwriting same savestate with every mupen, despite TASing different "branches". Copying whole directory sounds weird but works.

What do these checkboxes do:
-Process keyup messages: makes program react to releasing keys, better have this checked.

-Force input update: Basically spams mupens with input data, currently this will cause crazy desyncs, but also allows to use gamepad.
# Known bugs
-DeSyNcS

-Sometimes you can't savestate, but restarting everything seems to fix this

# Compiling
I've provided a Code Blocks project file because that's what I use to code (I think VS should be able to import it? If no then I will make a .sln later). There are 3 build targets: - "Release"/"Debug" for main .exe (debug shows a cmd window with debug output) and "Plugin" for compiling the .dll.
