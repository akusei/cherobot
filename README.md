# CHeroBot
Clicker Hero Bot with UI front-end. This Bot is in no way complete and does not account for all features in the game, it's very simple but does most of everything needed to advance the game through levels and upgrades.

## CHeroBot Project
C# front-end using WPF and MVVM. The front-end loads the clickerbot.dll to control the Bot.

## Clickerbot Project
Bot functionality written as a C++ DLL, using SendMessage for IPC to the injected window-hook DLL.

## Window-hook Project
A C++ DLL injected into the game to control the red/green border overlay and control user input so nothing will disturb the bot while it's running. Also receives IPC via SendMessage from the Clickerbot DLL.


![Bot waiting for game to launch](/screenshots/waiting.png?raw=true "Bot waiting for game to launch")

![Bot running](/screenshots/running.png?raw=true "Bot running")

![Bot injected and running](/screenshots/game-running.png?raw=true "Bot injected and running")

![Bot injected and paused](/screenshots/paused.png?raw=true "Bot injected and paused")