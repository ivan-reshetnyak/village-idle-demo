# Village Idle Demo

## What is it
This is a test project done for [REDACTED], sanitized to remove all mentions of [REDACTED].  
It is essentially a barebones demo for an idle game - a game map can be filled with two types of buildings - generating either Happiness or Gold resources, and generating Experience on purchase. Map and game world are configurable and persistent (saved in .xml every session).
Since I've not been contacted by [REDACTED] in a long time (and I've put a fair bit of effort into this) I've figured I shouldn't let it all go to waste and just put it up here. (if you're an employee of [REDACTED] and think I shouldn't have done this - feel free to reach out)

## Work involved
This was done from scratch by me alone, during my free time from 20.05.2023 to 27.05.2023.  
Most code is done in C++, with some (obvious) parts in GLSL. All code outside of `src/include` is written by me.   
Raw WINAPI was used for managing application lifecycle, with OpenGL with minimal tools (GLEW) for rendering. LibPNG and LibJPG were used for dealing with image loading, and RapidXML was used for processing XML files.  

## CONTROLS
Some of the controls may not be immediately obvious, so I'll quickly go over them.

### MAP:
1. Map can be dragged by clicking an empty space on the map and dragging your mouse.
2. Map can be zoomed in and out by clicking appropriate buttons on the left side of the screen.
3. Keyboard key 'G' toggles map grid visibility.
4. Left-clicking a building on the map will show "X" prompt on top of it. Left-click that prompt to delete a building, right-click to cancel.

### SHOP:
1. Shop can be brought up by clicking its icon in the bottom-right corner of the screen.
2. "WORKSHOPS" and "DECORATIONS" in it are clickable, switching shop tabs.
3. Left-clicking an item in the shop will pick it up (no player gold/level check is performed here as it was not required and for more convenient showcase).
After that the item will stick to your mouse cursor. Left-clicking it again will attempt to place the item on the map and buy it (gold is always paid, can go negative).
