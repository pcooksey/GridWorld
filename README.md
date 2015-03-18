Creating a simple 2D world for my grad AI class based off my Flat world classes. 

For setting up SDL 2.0, I recommend using this website: http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php
Basics:
<ol>
<li> Project : build options : search directory tab </li>
<li> Compiler subtab : add : ex. <code>C:\SDL2\include</code> </li>
<li> Linker subtab : add : ex. <code>C:\SDL2\lib</code> </li>
<li> Linker setting tab : <code> -lmingw32 -lSDL2main -lSDL2 </code> in other linker options box </li>
<li> [Link is more detailed so follow that if confused] </li>
</ol>
There is a bug with current version below is a link to the fixed copy of SDL_platform.h:

https://hg.libsdl.org/SDL/raw-file/e217ed463f25/include/SDL_platform.h

If you drop the file into SDL 2.0.3's include\SDL2\ directory, overwriting the original it should compile.

<ol>
<li>Project is built in CodeBlocks environment</li>
<li>The program is coded in C++ and uses SDL as its graphic library.</li>
<li>Linker setting <code> -lmingw32 -lSDLmain -lSDL</code></li>
</ol>
