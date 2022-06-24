# FidelityFx Super Resolution 2.0 for Cyberpunk 2077

 This mod is a library for Cyberpunk which replaces Nvidia DLSS with AMD FidelityFX Super Resolution 2.0

## Compilation

* Clone this repo including all submodules
* Compile the FSR 2.0 submodule in external/FidelityFX-FSR2 like it is described in their Readme https://github.com/GPUOpen-Effects/FidelityFX-FSR2#quick-start-checklist. Note: I used the GenerateSolutionsDLL.bat but I am sure static libraries will work fine too
* Open the CyberFSR.sln with Visual Studio 2022
* Hit Compile
* Copy the compiled DLL, ffx_fsr2_api_dx12_x64.dll and ffx_fsr2_api_x64.dll from the FidelityFX Directory to your Cyberpunk 2077 executable directory.
* Rename the compiled DLL to nvngx.dll
* Run the EnableSignatureOverride.reg to allow Cyberpunks DLSS implementation to load unsigned DLSS versions
* Run the game and set the quality in the DLSS settings
* Play the game with FSR 2.0
