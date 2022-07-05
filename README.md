# FidelityFx Super Resolution 2.0 for Cyberpunk 2077

 This mod is a library for Cyberpunk which replaces Nvidia DLSS with AMD FidelityFX Super Resolution 2.0

## Compilation

* Clone this repo including all submodules
* Compile the FSR 2.0 submodule in external/FidelityFX-FSR2 like it is described in their [Readme](https://github.com/GPUOpen-Effects/FidelityFX-FSR2#quick-start-checklist). Note: I used the GenerateSolutionsDLL.bat but I am sure static libraries will work fine too
* Open the CyberFSR.sln with Visual Studio 2022
* Hit Compile
* Copy the compiled DLL, ffx_fsr2_api_dx12_x64.dll and ffx_fsr2_api_x64.dll from the FidelityFX Directory to your Cyberpunk 2077 executable directory.
* Rename the compiled DLL to nvngx.dll
* Run the EnableSignatureOverride.reg to allow Cyberpunks DLSS implementation to load unsigned DLSS versions
* Run the game and set the quality in the DLSS settings
* Play the game with FSR 2.0

## special instructions to install the mod on Linux

* download the mod from the [Nexusmods page](https://www.nexusmods.com/cyberpunk2077/mods/3001?tab=files)
* extract the files and install them normally as stated in the nexusmod page
* with the appropriate file paths run the following command 
  
  

```
WINEPREFIX=/path/where/the/steam/library/is/steamapps/compatdata/1091500/pfx /bin/wine64 regedit ../../common/Cyberpunk\ 2077/bin/x64/FidelityFx\ Super\ Resolution\ 2.0-3001-0-3-1656426926/EnableSignatureOverride.reg
```

After this command the mod should work fine