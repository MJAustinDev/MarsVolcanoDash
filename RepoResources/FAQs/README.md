# FAQs 
(not really asked but questions I'm guessing people may have)

## Why make this game in the first place?

I wanted to toy around with and learn the basics of opengl, to do this I decided to make a reasonably scoped game.
As I already had experience with Box2D from a university project, I choose to make a 2D physics game.
When I asked some friends for possible ideas my friend Mike suggested a 'run away from' game,
from there the idea of a car out running a volcanic eruption evolved.



## Why use textures for text rather than a font rendering library?

The short answer is that I did look into this but the more restrictive licenses put me off.

This project was meant to help me learn opengl, as such I didn't want to waste anymore time getting my head around legal Jargon and copyleft licensing.
As I already had plans to use image loaded textures to create the dust storm effect,
it made more sense to use the same method rather than dealing with more unnecessary coding.



## Why provide PNG images for textures rather than encode/compress them?

Again this project was more about learning the basics of opengl for future more ambitious game projects,
as such I didn't want to spend too much time on loading in textures.
Hence, I used 'stb_image' as it allowed me to easily load in textures without a restrictive license.

Also the game is designed to handle non-existent image files, you'll just end up with blank menu buttons and no dust storm lol



## What version of opengl?

This project scoped with opengl 1.1 in mind, for two reasons:
- The project's purpose was to help me get the basics of opengl, hence I didn't want to use functions only available in later releases
- I wanted the game to be compatible with older machines that may not have the drivers/hardware to run later releases



## When/what are the next updates

For when, I can't give an exact date or time frame as I have a few other projects that I want to work towards as well as other life commitments to take care of.
However the updates themselves shouldn't be too taxing so the wait won't be too long.

For what, just a few graphical updates, nothing that'll change the gameplay.
Mainly just distance/score markers and some background decoration that'll make it look like a space mining corp operates there.
I may also add some easter eggs at some point, but thats not a promise.



## Why not statically link Box2D and GLFW?

I did in the release version, all the licenses of the libraries that I've used allow for this.
I used a separate Codeblocks project that links to static versions of Box2D and GLFW that I have on my machine.
If you look at the '.gitignore' file you'll see the .cbp referenced there.

I've left the dynamic libraries inside the project repository so people can easily recompile the game on their machine.

Box2D from the original git requires cmake, this can be daunting if you're new to C++ or programming in general.
Providing recompiled (Box2D) and original (GLFW) DLL copies as well as a pre-configured codeblocks project,
makes the game more accessible to those inexperienced with C++ and compiler configuration.












