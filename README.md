# Totally Legit Arcade Controller (TLAC)
ELAC's reborn big brother

# Description
This program consists of three parts:
##### Prepatch:
Used for applying the static patches from the `patch.txt` file to allow the game to successfully boot past all startup checks.
##### Divahook:
Installs a hook on the game's update routine to update its components.
##### TLAC:
Injects the divahook dll inside the game's process.

## Components
The available components can be toggled inside the `components.ini` config file. Currently available are:
* input_emulator
* touch_slider_emulator
* touch_panel_emulator
* sys_timer
* player_data_manager
* frame_rate_manager
* fast_loader

## Controls
Input bindings can be set inside the `keyconfig.ini` config file. Currently supported are keyboard and DualShock 4 controller input.
Key and button names are listed inside this config.

## Player Data
If enabled the PlayerDataManager writes constant values from the `playerdata.ini` config file to the game's memory.
This allows the user to set their player name, a level plate ID to be shown during the song select, equip a skin to be loaded during gameplay and button / slide sounds to be applied to all songs played.
This config file **must be** encoded using UTF-8.

# Usage
First **once per update** patch a clean `diva.exe` using the `prepatch.exe` program by passing the `diva.exe` path to it as its first command line argument; This can be done by dragging the diva executable on top of the prepatch executable inside the windows file explorer. It is strongly recommended to always create a backup of the original diva executable first.

To start diva run `diva.exe` like normal, to enable the components start `tlac.exe` **while** diva is running.
Do this **every time** you wish to run diva.

Diva has a few notable command line arguments including `-w` to start in windowed mode and `-hdtv1080` to increase the resolution to 1920x1080.

# Binaries
Pre-compiled binaries for the latest major updates can be found under the [releases](https://github.com/samyuu/TotallyLegitArcadeController/releases) tab.
