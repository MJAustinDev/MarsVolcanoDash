# Mars Volcano Dash

Simple game I've built using the Box2D physics engine, GLFW and stb_image. Currently in a WIP stage, but the base mechanics and most of the graphics are all working as I intent.

---

## Important (Licenses)

See LICENSE.md for the official license, but here's the tldr

I'm releasing my own work under the MIT license and you're free to do with it as you wish so long as you follow the license's copyright notice and permissions properly.
As far as I can tell that means stick said license in any released work you've built off my work in relation to my work, but other than that you should be free to do whatever you want :)

https://www.tawesoft.co.uk/kb/article/mit-license-faq

My background is computer science and programming not law, but that's the gist I've gathered about how the license works.
Although I'm not really expecting too many people are going to be making and releasing games based directly off my code anyhow lol


**Everything inside of the "include" and "libraries" are not my work.
And as such are under their corresponding licenses, see below.**



### Box2D

Box2D is released under its own MIT license, you can check out the GitHub repo here: https://github.com/erincatto/box2d

And the website here: https://box2d.org/

Obviously the actual Box2D engine was developed by Erin Catto and the code belongs to him.
Make sure if you are using any of the Box2D files here in your own project you correctly credit and attach the correct MIT license with it.

All the functionality from the DLL comes from Box2D so I'm not claiming any ownership over the lib and DLL files,
hence I've attached the original Box2D MIT licence to them.

All I've done to the original Box2D code is recompile it into a single DLL hence removing the dependency on cmake.
I did this using a separate CodeBlocks project that contained all the .cpp and .h files.
I did this months ago for a uni project so I can't 100% remember, but I might have had to modify some of the #include "... .h" lines to compensate for their new positions.
Going over the code I can't seem to find any differences so I'm pretty sure the way I set up the compiler and files structure compensated for it instead.
(Tbf I'm probably miss remembering/confusing this with an earlier toy around project but I want to disclaim just in case)

Hence all Box2D header files are either exactly the same as the original or the same as the original with a few #include lines changed.



### GLFW

GLFW is licensed under the zlib/libpng license, see here: https://www.glfw.org/license

I've used version 3.3.7 as that was the latest release at time of download.
I downloaded the 64-bit Windows binaries from the GLFW website, https://www.glfw.org/

Regarding the downloaded binaries,
I've used the "glfw3.dll" and the "libglfw3dll.a" files found inside of the "lib-mingw-64" folder.
As well as the header files found inside the "include/GLFW" directory.
Obviously these files are not my work and are under the aforementioned zlib/libpng license,
with copy right going to Marcus Geelnard and Camilla Löwy.


### stb_image

Stb_image can be licensed under two different licenses depending on choice, I've opted to use the Public Domain license.

You can check out the GitHub repo here: https://github.com/nothings/stb

I needed a quick and easy way to load png images hence I've only used the image loader library "stb_image.h".
Hence I copied the header file off the GitHub repository.
Obviously this file is Sean T barrett's work and is under the aforementioned Public Domain License.



**The above Box2D, GLFW and stb_image licenses are placed inside of this projects LISCENSE.md file with indication to where they apply,
they can also be found in the directories that they exist as well as in any code taken directly from the libraries (i.e., their header files).**

---

## Gameplay

Game is not yet complete, at least concerning graphics and content, but it is playable and contains all the underpinning mechanics I want in the final game.

You work for a mining company on that operates on the surface of Mars, for your work you drive an armoured pickup truck that you use to transport mined materials.
A volcanic eruption has occurred near your current position, sending massive boulders and meteors into the sky as well as sending molten lava across the Martian surface.
Your vehicle is armoured and can withstand the force from incoming debris, however it cannot withstand being completely submerged in lava.
The lava from the eruption is progressively moving towards you while incoming debris from the eruption is falling down and littering your path!
You must navigate the volatile terrain in an attempt to save your life...


### Menus

Use W & S to scroll up and down, and Enter to select an option


### In game controls

W ⇒ Forwards (right along the track)

S ⇒ Backwards (left along the track)

A ⇒ Apply force to tilt car CCW

D ⇒ Apply force to tilt car CW

Player 2 controls are the same but with the following mappings (basically moving WASD along your keyboard to IJKL)

I ⇒ W,
K ⇒ S,
J ⇒ A,
L ⇒ D

---

## To come

- Some minor graphics updates
- Terrain Variety
- Clean up a few loose ends/unclean code
- Guides

---

## Repository break down

- GameControl, contains files associated with controlling the game either on menu or while running
- GameEntities, contains files associated with creating, controlling and destroying game entities e.g., player's car, boulders, etc
- include, contains all the external .h files required to compile the program (Box2D, GLFW, stb_image)
- libraries, contains the .a and .dll files associated with the external libraries (Box2d, GLFW)
- Utility, contains basic utility functions/classes that are used throughout the code
- Visuals, contains files associated with drawing graphics to the screen


---

## Final notes

I'm releasing and licensing what I've done under MIT, feel free to do what you like with it.
**Just be careful when using parts of the program which I haven't built, (Box2D, GLFW, stb_image) and make sure to reference their licensing properly.**
Other than that I encourage you to play around and modify the likes of the game mode configuration, the players car, or even add in new enemies.
Do so to your hearts content!
I've left the CodeBlocks project in the repo that way you should be able to toy around with the code without any knowledge of setting up the compiler/linker

Also a final thanks to my friend Mike for the idea of a chasing game when I asked for possible ideas for physics engine based games!








