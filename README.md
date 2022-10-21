#Musical Fractals

This project aims at creating 3D fractals evolving to the sound of music.
The fractals will be rendered on OpenGL using ray marching 
and respond to the different frequencies of the music,
and hopefully to better algorithms that just a plain FFT... Maybe

I don't know if I'll be using a library for FFT yet, 
maybe only one to read the audio file and I'll do the FFT by hand? would be more fun...
Don't think I'll be bothered to parse audio files by hand tho, so there is definitly going to be somthing involved there, almost 100% gonna be ffmpeg

currently need gtk4-x11, X11, glew and gcc, though you can probably change the compiler just fine in the Makefile...
I wish I could have gone without X11, but gtk decided to remove support for grabbing cursors and warping them.. which is kind of a must if you do any 3D controls using the mouse.... I have no clue why they did that, it means I would have to reimplement it in every graphical server if I wanted compatibility... yay...

That's probably gonna mark the end of me using gtk for any graphical project... I thought they were cross platform, well not for 3D stuff it seems

I have started reading the source code of SFML to check on how they interacted with Xlib (as the doc of Xlib is attrocious to look at on a web browser, and the man pages aren't really well done for navigating them), and oh god that code seems clean... I think I might consider going to SFML later on, when I have the rest of the project implemented. Gonna be a pain in the ass to port, but they seem to have a nice openGL interaction, so it might end up making the code cleaner in the end, and anything that makes cleaner code is good. I wouldn't have access to widgets tho, I don't want to use Qt, so I might end up with either another lib for widgets, or make the whole thing myself... That would be a fun project tho..

I've seen people integrate SFML within GTK but it seems like more of a hack than anything pertinent, so would be a pain in the ass whenever something updates and breaks on this or that configuration.


