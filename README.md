#Musical Fractals

This project aims at creating 3D fractals evolving to the sound of music.
The fractals will be rendered on OpenGL using ray marching 
and respond to the different frequencies of the music,
and hopefully to better algorithms that just a plain FFT... Maybe

Currently, the GLSL fragment shaders has not been included as it was not my own
and might be liscenced (got no clue but didn't want to tickle the devil anyway)
will be rewriting one that I can claim as my own anyway as I dislike working with 
code I don't understand, and this project is my project, 
why would I have code I don't understand in it???

I don't know if I'll be using a library for FFT yet, 
maybe only one to read the audio file and I'll do the FFT by hand? would be more fun...
Don't think I'll be bothered to parse audio files by hand tho, so there is definitly going to be somthing involved there

currently need gtk4, glew and gcc, though you can probably change the compiler just fine in the Makefile


