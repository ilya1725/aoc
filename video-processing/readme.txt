Setup

A moving camera captures a scene that has an object moving freely. 
The location of the object in a given time is given in the text file. 
Write a program that validates the coordinates in the text file. 
An object is “present” by making sure that the pixels in a 3x3 grid 
are darker than the average grayscale value of the frame. 
Print out all timestamps of the video where this case is not true. 

Three sample videos and accompanying text file 

Input: A video and a file with the location of the object in every frame

Expected output: Either command line output or text file output with timestamps of the video whenever this isn’t true. 

Assumptions: The frame rate is high enough (and the camera and object speeds are low
enough) so the changes in apparent size are smooth. 

Because this is a naïve approach to identifying objects, 
it will not be accurate (so false positives and false negatives will occur). 

//-----------------------------------------------------------------------------
https://stackoverflow.com/questions/4953843/why-cant-visual-studio-find-my-dll

NOTES:

1. Since the release built OpenVC libraries were done with different version of MSVC linking with non-debug versions
will resolve the error of not finiding proper debug DLLs for that MSVC version.

2. 