# OpticalFlow
Implementation of Farneback's optical flow algorithm in openFrameworks using OpenCV. In order to run correctly the program you have to:
- Create an openFrameworks project using projectGenerator, using ofxCv and ofxOpenCv addons
- Substitute the src folder content with this repo content
- Compile and... RRRRRRRRRRRUN!
On the top left of the canvas you should see the optical flow (rendered as lines with a circle along the direction where it is pointing) computed for the old frame (superimposed to it) and the current frame (on the right side of the canvas).

#ISSUES
- Unknown exception thrown by the optical flow function
- Not robust to glitches, probably given by light conditions between two consecutive frames
