# Multimupen
Tool that allows to control multiple mupen windows at once.
Download it from releases tab

# How to setup
1.Place Multimupen.exe and TASDI.dll in one directory, then copy ReceiverPlugin.dll to mupen plugin folder and select it as input plugin in settings.

2.To properly use savestates you must copy whole mupen directory as many times as you need (e.g. you need 5 mupen windows then you need 5 copies). This is because simply running one .exe multiple times (or more than one exe but in same directory) causes overwriting same savestate with every mupen, despite TASing different "branches". Copying whole directory sounds weird but works.

3.If you're using Win10 then you will experience tasinput going out of focus issue, just like it happens without multimupen. To fix this simply click and hold on tasinput, drag to the multimupen window and click. I can't fix this because that's how tasinput was designed, it worked fine in the past so nobody questioned it.

What do these checkboxes do:

-Process keyup messages: makes program react to releasing keys, better have this checked.

-Force input update: Basically spams mupens with input data, currently this will cause crazy desyncs, but also allows to use gamepad.
# Known bugs
-DeSyNcS

-Sometimes you can't savestate, ~~but restarting everything seems to fix this~~ the problem lies in how mupen checks for hotkeys, if you have savestate defined as multiple key combination, like a default Shift+F#, you must hold shift long enough so every mupen gets to see the shift. I hope its possible to fix it.

# Compiling
I've provided a Code Blocks project file because that's what I use to code (I think VS should be able to import it? If no then I will make a .sln later). There are 3 build targets: - "Release"/"Debug" for main .exe (debug shows a cmd window with debug output) and "Receiver" for compiling the .dll.
