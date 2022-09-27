# DLSS2FSR (CyberFSR)
Drop-in DLSS replacement with FSR 2.0 for various games such as Cyberpunk 2077, Red Dead Redemption 2, etc.

It includes a DLL wrapper/injector (winmm.dll) to disable Nvidia GeForce GPU checking, so AMD Radeon GPU users can enjoy the mod.

## List of Supported Games
* Marvel's Spider-Man Remastered (https://www.nexusmods.com/marvelsspidermanremastered/mods/683)
* Horizon Zero Dawn (https://www.nexusmods.com/horizonzerodawn/mods/133)
* No Man's Sky (https://www.nexusmods.com/nomanssky/mods/2485)
* Rise of the Tomb Raider (https://www.nexusmods.com/riseofthetombraider/mods/82)
* Red Dead Redemption 2 v1436.28 (https://www.nexusmods.com/reddeadredemption2/mods/1640)
* DOOM Eternal (https://www.nexusmods.com/doometernal/mods/1170)
* Deep Rock Galactic
* Chernobylite
* Ghostrunner (https://www.nexusmods.com/ghostrunner/mods/17)
* Amid Evil (https://steamcommunity.com/app/673130/discussions/0/4960174866676781850)
* Hot Wheels Unleashed
* The Ascent (https://www.nexusmods.com/theascent/mods/11)
* Ghostwire Tokyo
* Aron's Adventure
* Minecraft Bedrock Edition
* Destroy All Humans! 2 - Reprobed
* Diablo II - Resurrected (https://www.nexusmods.com/diablo2resurrected/mods/269)
* Kena: Bridge of Spirits (https://www.nexusmods.com/kenabridgeofspirits/mods/18)
* Soulstice (https://www.nexusmods.com/soulstice/mods/3)
* Succubus

## Installation
### Windows 
* Download the latest relase from [Release Page](https://github.com/MOVZX/CyberFSR2/releases)
* Extract the contents of the archive next to the `nvngx_dlss.dll` file in your games folder
* Run/double-click `EnableSignatureOverride.reg` and confirm merge
* That's it. Now DLSS option should appear in settigs

### Linux
* Download the latest relase from [Release Page](https://github.com/MOVZX/CyberFSR2/releases)
* Extract the contents of the archive next to the `nvngx_dlss.dll` file in your games folder
* Run the ```linuxinstall.sh``` script
* Or just run the following command with the appropriate file paths to install the tweaks manually:
```
WINEPREFIX=/path/where/the/steam/library/is/steamapps/compatdata/1091500/pfx /bin/wine64 regedit ../../common/Cyberpunk\ 2077/bin/x64/FidelityFx\ Super\ Resolution\ 2.0-3001-0-3-1656426926/EnableSignatureOverride.reg
```
* That's it. Now DLSS option should appear in settigs

## Tweaks
### UE4 Ghosting/Flickering Fixes

Edit **Engine.ini** file located in ```%LOCALAPPDATA%\GAMENAME\Saved\Config\WindowsNoEditor\Engine.ini``` and paste the tweaks below to the very bottom of the line:
```
[SystemSettings]
r.NGX.DLSS.DilateMotionVectors=0
r.SceneColorFormat=5
```
#### Location for each game is different, but here are all the location which has the game supported by this Mod:

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
* Linux users should refer to prior command
