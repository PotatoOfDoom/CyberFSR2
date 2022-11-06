# DLSS2FSR (CyberFSR/PotatoFSR)
Drop-in DLSS replacement with FSR 2.1.2 for various games such as Cyberpunk 2077, Red Dead Redemption 2, etc.

It includes a DLL wrapper/injector (winmm.dll) to disable Nvidia GeForce RTX GPU checking, so AMD Radeon / Nvidia GTX users can enjoy the mod.

## Official Discord Channel: https://discord.gg/2JDHx6kcXB


## List of Supported Games
* A Plague Tale Requiem (https://www.nexusmods.com/aplaguetalerequiem/mods/9)
* Amid Evil (https://steamcommunity.com/app/673130/discussions/0/4960174866676781850)
* Aron's Adventure
* Assetto Corsa Competizione
* Batora - Lost Haven
* Bright Memory: Infinite (https://www.nexusmods.com/brightmemoryinfinite/mods/4)
* Chernobylite 
* DOOM Eternal (https://www.nexusmods.com/doometernal/mods/1170)
* Deep Rock Galactic
* Destroy All Humans! 2 - Reprobed
* Diablo II - Resurrected (https://www.nexusmods.com/diablo2resurrected/mods/269)
* GTA Liberty City Definitive Edition
* GTA San Andreas Definitive Edition
* GTA Vice City Definitive Edition
* Ghostrunner (https://www.nexusmods.com/ghostrunner/mods/17)
* Ghostwire Tokyo
* Gotham Knights
* Hell Pie
* Horizon Zero Dawn (https://www.nexusmods.com/horizonzerodawn/mods/133)
* Hot Wheels Unleashed
* Into the Radius VR
* Kena: Bridge of Spirits (https://www.nexusmods.com/kenabridgeofspirits/mods/18)
* Marvel's Spider-Man Remastered (https://www.nexusmods.com/marvelsspidermanremastered/mods/683)
* Minecraft Bedrock 1.19.40.2
* Myst
* No Man's Sky (https://www.nexusmods.com/nomanssky/mods/2485)
* Red Dead Redemption 2 v1436.28 (https://www.nexusmods.com/reddeadredemption2/mods/1640)
* Rise of The Tomb Raider (https://www.nexusmods.com/riseofthetombraider/mods/82)
* Soulstice (https://www.nexusmods.com/soulstice/mods/3)
* Succubus
* The Ascent (https://www.nexusmods.com/theascent/mods/11)
* The Last Oricru
* The Orville - Interactive Fan Experience
* WRC Generations – The FIA WRC Official Game


## Installation
### Windows 
* Download the latest relase from [Release Page](https://github.com/MOVZX/CyberFSR2/releases)
* Extract the contents of the archive next to the game EXE file in your games folder
* Run/double-click `EnableSignatureOverride.reg` and confirm merge
* Make sure the game is running in DX12 mode:  
![Steam setting](https://i.imgur.com/a8Sybru.png)
* That's it. Now DLSS option should appear in settigs

### GNU/Linux
* Download the latest relase from [Release Page](https://github.com/MOVZX/CyberFSR2/releases)
* Extract the contents of the archive next to the game EXE file in your games folder
* Run the ```linuxinstall.sh``` script or manually import `EnableSignatureOverride.reg` into the Registry
* Set game launch options (Steam) to: `WINEDLLOVERRIDES="winmm=n,b" %command%`
* Or if you're using Lutris/Heroic/etc you can set it like this:  
![WINE setting](https://i.imgur.com/v7JLSUY.png)
* Make sure the game is running in DX12 mode:  
![Steam setting](https://i.imgur.com/a8Sybru.png)
* That's it. Now DLSS option should appear in settigs


## Tweaks
### UE4 Ghosting/Flickering Fixes

Edit **Engine.ini** file located in ```%LOCALAPPDATA%\GAMENAME\Saved\Config\WindowsNoEditor\Engine.ini``` and paste the tweaks below to the very bottom of the line:
```
[SystemSettings]
r.NGX.DLSS.DilateMotionVectors=0
r.SceneColorFormat=5
```
#### Location for each games is different, but here are some locations for the games supported by this Mod:

- **Amid Evil**: ```%LOCALAPPDATA%\AmidEvil\Saved\Config\WindowsNoEditor\Engine.ini```
- **Aron's Adventure**: ```%LOCALAPPDATA%\AronsAdventure\Saved\Config\WindowsNoEditor\Engine.ini```
- **Chernobylite**: ```%LOCALAPPDATA%\ChernobylGame\Saved\Config\WindowsNoEditor\Engine.ini```
- **Deep Rock Galactic**: ```GAME_INSTALL_LOCATION\Deep Rock Galactic\FSD\Saved\Config\WindowsNoEditor\Engine.ini```
- **Ghostrunner**: ```%LOCALAPPDATA%\Ghostrunner\Saved\Config\WindowsNoEditor\Engine.ini```
- **GhostWire Tokyo**: ```%USERPROFILE%\TangoGameworks\GhostWire Tokyo (STEAM)\Saved\Config\WindowsNoEditor\Engine.ini```
- **Hot Wheels Unleashed**: ```%LOCALAPPDATA%\hotwheels\Saved\Config\WindowsNoEditor\Engine.ini```
- **Ready or Not**: ```%LOCALAPPDATA%\ReadyOrNot\Saved\Config\WindowsNoEditor\Engine.ini```
- **The Ascent**: ```%LOCALAPPDATA%\TheAscent\Saved\Config\WindowsNoEditor\Engine.ini```


## Uninstallation
* Just run `DisableSignatureOverride.reg`
* Delete nvngx.dll, nvngx.ini, and winmm.dll
* GNU/Linux users should refer to prior command
