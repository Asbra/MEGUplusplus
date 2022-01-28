# MEGU++
Mod the PC Steam release of [Marc Ecko's Getting Up](https://store.steampowered.com/app/260190/Marc_Ecks_Getting_Up_Contents_Under_Pressure/)

## Installation
- Requires current Steam version of Getting Up (due to using ASI loader)
- Put the `MEGUplusplus.asi` file into the game binaries folder, eg. `C:\Steam\steamapps\common\Marc Ecko's Getting Up 2\_Bin\`


## Features
- You can now put up to 127 tags/stickers/marker tags on a map, raised from the games original limit of 25
- Debug console (F1 to open/close)


## Notes

There's an included post-build event
```
    <PostBuildEvent>
      <Command>copy /Y "$(TargetDir)$(TargetName).dll" "$(GameDir)\_Bin\$(TargetName).asi"</Command>
    </PostBuildEvent>
```

Set the global variable `GameDir` to your correct path
```
<GameDir>C:\Steam\steamapps\common\Marc Ecko's Getting Up 2</GameDir>
```
for the post-build event to work properly or disable it.
