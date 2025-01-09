# Controls
| WASD | Shift | Space | E | Right Click |
| - | - | - | - | - |
| Movement | Sprint | Jump | Interact | Zoom |

# Building (Unix)
Required packges:
* cJSON (vendering soon)
```fish
git clone https://github.com/DinoNuggies7/gilded-cove.git
cd gilded-cove
git submodule update --init
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP -j$(nproc) # (Add for Windows) PLATFORM_OS=WINDOWS RAYLIB_LIBTYPE=SHARED
cd ../..
make
```
For cross compiling to Windows, you can simply replace `make` with `mingw32-make` or `mingw64-make`.
