# DLSS2FSR (CyberFSR/PotatoFSR)
Replacing DLSS 2.x with FSR 2.1.2 for various games such as Cyberpunk 2077, Red Dead Redemption 2, etc.

It includes a DLL wrapper/injector (winmm.dll) to disable Nvidia GeForce RTX GPU checking, so AMD Radeon / Nvidia GTX users can enjoy the mod.

## Join our Cult here: https://discord.gg/2JDHx6kcXB


## List of Supported Games
* A Plague Tale Requiem (https://www.nexusmods.com/aplaguetalerequiem/mods/9)
* Amid Evil (https://steamcommunity.com/app/673130/discussions/0/4960174866676781850)
* Amok Runner
* Aron's Adventure
* Assetto Corsa Competizione
* Batora - Lost Haven
* BLACKTAIL
* Blind Fate: Edo no Yami
* Bright Memory: Infinite (https://www.nexusmods.com/brightmemoryinfinite/mods/4)
* Dakar Desert Rally
* DOOM Eternal (https://www.nexusmods.com/doometernal/mods/1170)
* Deep Rock Galactic
* Deliver Us the Moon
* Diablo II - Resurrected (https://www.nexusmods.com/diablo2resurrected/mods/269)
* Dream Cycle
* Evil West
* F.I.S.T. - Forged In Shadow Torch (https://www.nexusmods.com/fistforgedinshadowtorch/mods/2)
* Fobia - St.Dinfna Hotel
* Frozen Flame
* GTA Liberty City Definitive Edition
* GTA San Andreas Definitive Edition
* GTA Vice City Definitive Edition
* Ghostrunner (https://www.nexusmods.com/ghostrunner/mods/17)
* Gotham Knights
* Gungrave G.O.R.E
* Hell Pie
* Hello Neighbor 2
* Horizon Zero Dawn (https://www.nexusmods.com/horizonzerodawn/mods/133)
* Hot Wheels Unleashed
* Hydroneer
* ICARUS
* INDUSTRIA
* Into the Radius VR
* Kena: Bridge of Spirits (https://www.nexusmods.com/kenabridgeofspirits/mods/18)
* Land of the Vikings
* Marvel's Spider-Man Remastered (https://www.nexusmods.com/marvelsspidermanremastered/mods/683)
* Marvel's Spider-Man Miles Morales (https://www.nexusmods.com/spidermanmilesmorales/mods/33)
* Minecraft Bedrock 1.19.40.2 (and up)
* Myst (https://www.nexusmods.com/myst/mods/3)
* Necromunda: Hired Gun (https://www.nexusmods.com/necromundahiredgun/mods/8)
* No Man's Sky (https://www.nexusmods.com/nomanssky/mods/2485)
* Ready or Not (https://www.nexusmods.com/readyornot/mods/1953)
* Red Dead Redemption 2 (https://www.nexusmods.com/reddeadredemption2/mods/1640)
* Rise of the Tomb Raider (https://www.nexusmods.com/riseofthetombraider/mods/82)
* Severed Steel
* Soulstice (https://www.nexusmods.com/soulstice/mods/3)
* Succubus
* Sword and Fairy 7
* The Ascent (https://www.nexusmods.com/theascent/mods/11)
* The Chant
* The Last Oricru
* The Orville - Interactive Fan Experience
* WRC Generations – The FIA WRC Official Game


## Installation
### Windows 
* Download the latest relase from [Release Page](https://github.com/MOVZX/CyberFSR2/releases)
* Extract the contents of the archive next to the game EXE file in your games folder
* Make sure the game is running in DX12 mode:  
![Steam setting](https://i.imgur.com/a8Sybru.png)
* That's it. Now DLSS option should appear in settigs

### GNU/Linux
* Download the latest relase from [Release Page](https://github.com/MOVZX/CyberFSR2/releases)
* Extract the contents of the archive next to the game EXE file in your games folder
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
- **Amok Runner**: ```%LOCALAPPDATA%\Amok\Saved\Config\WindowsNoEditor\Engine.ini```
- **Assetto Corsa Competizione**: ```%LOCALAPPDATA%\AC2\Saved\Config\WindowsNoEditor\Engine.ini```
- **Aron's Adventure**: ```%LOCALAPPDATA%\AronsAdventure\Saved\Config\WindowsNoEditor\Engine.ini```
- **Batora - Lost Haven**: ```%LOCALAPPDATA%\Batora\Saved\Config\WindowsNoEditor\Engine.ini```
- **BLACKTAIL**: ```%LOCALAPPDATA%\BLACKTAIL\Saved\Config\WindowsNoEditor\Engine.ini```
- **Blind Fate: Edo no Yami**: ```%LOCALAPPDATA%\Yami\Saved\Config\WindowsNoEditor\Engine.ini```
- **Bright Memory: Infinite**: ```%LOCALAPPDATA%\BrightMemoryInfinite\Saved\Config\WindowsNoEditor\Engine.ini```
- **Dakar Desert Rally**: ```%LOCALAPPDATA%\Dakar2Game\Saved\Config\WindowsNoEditor\Engine.ini```
- **Deep Rock Galactic**: ```GAME_INSTALL_LOCATION\Deep Rock Galactic\FSD\Saved\Config\WindowsNoEditor\Engine.ini```
- **Deliver Us the Moon**: ```%LOCALAPPDATA%\MoonMan\Saved\Config\WindowsNoEditor\Engine.ini```
- **Dream Cycle**: ```%LOCALAPPDATA%\DreamCycle\Saved\Config\WindowsNoEditor\Engine.ini```
- **F.I.S.T. - Forged In Shadow Torch**: ```%LOCALAPPDATA%\ZingangGame\Saved\Config\WindowsNoEditor\Engine.ini```
- **Fobia - St.Dinfna Hotel**: ```%LOCALAPPDATA%\Fobia\Saved\Config\WindowsNoEditor\Engine.ini```
- **Frozen Flame**: ```%LOCALAPPDATA%\FrozenFlame\Saved\Config\WindowsNoEditor\Engine.ini```
- **Ghostrunner**: ```%LOCALAPPDATA%\Ghostrunner\Saved\Config\WindowsNoEditor\Engine.ini```
- **Gotham Knights**: ```%LOCALAPPDATA%\Mercury\Saved\Config\WindowsNoEditor\Engine.ini```
- **Gungrave G.O.R.E**: ```%LOCALAPPDATA%\GunGraveGore\Saved\WindowsNoEditor\Engine.ini```
- **Hell Pie**: ```%LOCALAPPDATA%\HellPie\Saved\Config\WindowsNoEditor\Engine.ini```
- **Hello Neighbor 2**: ```%LOCALAPPDATA%\HelloNeighbor2\Saved\Config\WindowsNoEditor\Engine.ini```
- **Hot Wheels Unleashed**: ```%LOCALAPPDATA%\hotwheels\Saved\Config\WindowsNoEditor\Engine.ini```
- **Hydroneer**: ```%LOCALAPPDATA%\Mining\Saved\Config\WindowsNoEditor\Engine.ini```
- **ICARUS**: ```%LOCALAPPDATA%\Icarus\Saved\Config\WindowsNoEditor\Engine.ini```
- **INDUSTRIA**: ```%LOCALAPPDATA%\Industria\Saved\Config\WindowsNoEditor\Engine.ini```
- **Into the Radius VR**: ```%LOCALAPPDATA%\IntoTheRadius\Saved\Config\WindowsNoEditor\Engine.ini```
- **Kena: Bridge of Spirits**: ```%LOCALAPPDATA%\Kena\Saved\Config\WindowsNoEditor\Engine.ini```
- **Land of the Vikings**: ```%LOCALAPPDATA%\VikingOyunu\Saved\Config\WindowsNoEditor\Engine.ini```
- **Myst**: ```%LOCALAPPDATA%\Myst\Saved\Config\WindowsNoEditor\Engine.ini```
- **Necromunda: Hired Gun**: ```%LOCALAPPDATA%\Streumon\Necromunda\Saved\Config\WindowsNoEditor\Engine.ini```
- **Ready or Not**: ```%LOCALAPPDATA%\ReadyOrNot\Saved\Config\WindowsNoEditor\Engine.ini```
- **Severed Steel**: ```%LOCALAPPDATA%\ThankYouVeryCool\Saved\Config\WindowsNoEditor\Engine.ini```
- **Soulstice**: ```GAME_INSTALL_LOCATION\Soulstice\Saved\Config\WindowsNoEditor\Engine.ini```
- **Succubus**: ```%LOCALAPPDATA%\Succubus\Saved\Config\WindowsNoEditor\Engine.ini```
- **Sword and Fairy 7**: ```%LOCALAPPDATA%\Pal7\Saved\Config\WindowsNoEditor\Engine.ini```
- **The Ascent**: ```%LOCALAPPDATA%\TheAscent\Saved\Config\WindowsNoEditor\Engine.ini```
- **The Chant**: ```%LOCALAPPDATA%\Chant\Saved\Config\WindowsNoEditor\Engine.ini```
- **The Last Oricru**: ```%LOCALAPPDATA%\LostHero\Saved\Config\WindowsNoEditor\Engine.ini```
- **The Orville - Interactive Fan Experience**: ```GAME_INSTALL_LOCATION\OrvilleFanGame\Saved\Config\WindowsNoEditor\Engine.ini```


## Uninstallation
* Delete nvngx.dll, nvngx.ini, and winmm.dll
* GNU/Linux users should refer to prior command
