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
	Algorithm:

	1. Check the general validity of the data.
		a. Make sure that all location values are within the video's size.
			- This check may have to be adjusted if the scaling is different.
		b. For this use-case, make sure that the number of location points is the same as the number of frames.
			- This check will not be valid if there are gaps.
		c. The data is smooth. I.e. the location of the object in the frame cannot change 
		from the previous frames with high speed. Essentially the first derivative
		of the location trace should be monotonically changing. If it isn't, then 
		the location where this happents is/are the outliers.
			This can be implemnted in different ways depending on the available HW.

	2. For each frame:
		a. Calculate average grayscale value. Using mean() [http://docs.opencv.org/3.0.0/d2/de8/group__core__array.html#ga191389f8a0e58180bb13a727782cd461]
		b. Run the location algorithm using blob detector.
			The low threthold of the detector should be set to the above mean value.
			There could be many blobs.
		c. Compare all the located blobs' location with the expected location from the data, 
		within known precision value.
			- If none match - print an error.
			- If one matches, but there is more than one blob - print an error.
		


//-----------------------------------------------------------------------------
https://stackoverflow.com/questions/4953843/why-cant-visual-studio-find-my-dll

NOTES:

1. Since the release built OpenVC libraries were done with different version of MSVC linking with non-debug versions
will resolve the error of not finiding proper debug DLLs for that MSVC version.

2. 