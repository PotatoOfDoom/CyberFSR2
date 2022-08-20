# DLSS2FSR
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
* Run the linuxinstall.sh script
* Or just run the following command with the appropriate file paths to install the tweaks manually:
```
WINEPREFIX=/path/where/the/steam/library/is/steamapps/compatdata/1091500/pfx /bin/wine64 regedit ../../common/Cyberpunk\ 2077/bin/x64/FidelityFx\ Super\ Resolution\ 2.0-3001-0-3-1656426926/EnableSignatureOverride.reg
```
* That's it. Now DLSS option should appear in settigs

## Uninstallation
* Just run `DisableSignatureOverride.reg`
* Linux users should refer to prior command
