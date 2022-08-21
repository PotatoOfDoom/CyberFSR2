# DLSS2FSR (CyberFSR)
Drop-in DLSS replacement with FSR 2.0 for various games such as Cyberpunk 2077, Red Dead Redemption 2, etc.

It includes a DLL wrapper/injector (winmm.dll) to disable Nvidia GeForce GPU checking, so AMD Radeon GPU users can enjoy the mod.

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
