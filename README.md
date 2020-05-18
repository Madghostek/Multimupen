# Multimupen
Tool that allows to control multiple mupen windows at once.
Download it from releases tab

# How to setup
1.Place Multimupen.exe and TASDI.dll in one directory, then copy ReceiverPlugin.dll to mupen plugin folder and select it as input plugin in settings.

2.To properly use savestates you must copy whole mupen directory as many times as you need (e.g. you need 5 mupen windows then you need 5 copies). This is because simply running one .exe multiple times (or more than one exe but in same directory) causes overwriting same savestate with every mupen, despite TASing different "branches". Copying whole directory sounds weird but works.

3.If you're using Win10 then you will experience tasinput going out of focus issue, just like it happens without multimupen. To fix this simply click and hold on tasinput, drag to the multimupen window and click. I can't fix this because that's how tasinput was designed, it worked fine in the past so nobody questioned it.

What do these checkboxes do:

~~* Process keyup messages: makes program react to releasing keys, better have this checked.~~ Now set by default

* Disable key repeat - Makes you unable to spam key by holding, makes tasing in frame advance desync-free afaik.

* Force input update: Basically spams mupens with input data, currently this will cause crazy desyncs, but also allows to use gamepad.

# How to TAS without desyncs
Due to how this program works it is common for desyncs to happen, so far I wasn't able to fix all causes but here are some tips to minimize them:
1. Don't record input when mupen is running at full speed, It will easily desync your run! Always use frame advance.
2. It's helpful to record .m64 on all windows at once instead of only one, so you can track if one window desynced, however you must start recording at the same time (so just pause before starting recording).
3. You can fast forward, but make sure to sync every window afterwards so the frame count is the same everywhere. (This is why tasing at full speed desyncs too btw)
4. Don't load savestates that weren't made during recording (same thing as in normal TASing). A good idea is to save 1-9 slots at the start

If a desync happened anyway you will have to start recording from the point where it desynced, so it's a good idea to check every some time if everything looks good.

# Known bugs
* Version v1.1 still desyncs when running at full speed, this probably won't be fixed, read previous section.

Report any issues pls

# Compiling
I've provided a Code Blocks project file because that's what I use to code (I think VS should be able to import it? If no then I will make a .sln later). There are 3 build targets: - "Release"/"Debug" for main .exe (debug shows a cmd window with debug output) and "Receiver" for compiling the .dll.
