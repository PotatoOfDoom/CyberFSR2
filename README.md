# DLSS2FSR
Drop-in DLSS replacement with FSR 2.0 for various games such as Cyberpunk 2077.



## Installation
*Following instructions were written for Cyberpunk 2077 and may differ for other games.*

### Windows 
* Download the latest relase from [Nexusmods](https://www.nexusmods.com/cyberpunk2077/mods/3001?tab=files).
* Extract the contents of the archive next to the `nvngx_dlss.dll` file in your games folder.
* Run `EnableSignatureOverride.reg` and confirm merge.
* That's it. Now DLSS option should appear in settigs.

### Linux
* Download the latest relase from [Nexusmods](https://www.nexusmods.com/cyberpunk2077/mods/3001?tab=files).
* Extract the contents of the archive next to the `nvngx_dlss.dll` file in your games folder.
* Run the following command with the appropriate file paths: 
```
WINEPREFIX=/path/where/the/steam/library/is/steamapps/compatdata/1091500/pfx /bin/wine64 regedit ../../common/Cyberpunk\ 2077/bin/x64/FidelityFx\ Super\ Resolution\ 2.0-3001-0-3-1656426926/EnableSignatureOverride.reg
```
* That's it. Now DLSS option should appear in settigs.

### Uninstallation
* Just run `DisableSignatureOverride.reg`
* Linux users should refer to prior command.

## Compilation

### Requirements
* Visual Studio 2022
* latest Vulkan SDK (1.3.216.0)

### Instructions
* Clone this repo with all of its submodules.
* Compile the FSR 2.0 submodule in external/FidelityFX-FSR2 as instructed in [official documentation](https://github.com/GPUOpen-Effects/FidelityFX-FSR2#quick-start-checklist). Note: I used the GenerateSolutionsDLL.bat but I am sure static libraries will work fine too.
* Open the CyberFSR.sln with Visual Studio 2022.
* Hit Compile.
* Copy the compiled `ffx_fsr2_api_dx12_x64.dll` and `ffx_fsr2_api_x64.dll` from the FidelityFX Directory to your Cyberpunk 2077 executable directory.
* Rename the compiled DLL to `nvngx.dll`.
* Run the `EnableSignatureOverride.reg` to allow Cyberpunks DLSS implementation to load unsigned DLSS versions
* Run the game and set the quality in the DLSS settings
* Play the game with FSR 2.0

