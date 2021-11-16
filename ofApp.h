#pragma once

#include "ofMain.h"
#include "ofxOpencv.h"
#include "ofxCv.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void FirstVideoFrameCapt();

		ofVideoGrabber vidGrabber;
		ofxCvColorImage color1, color2; //First and second original images
		ofImage grey1, grey2; //Decimated grayscaled images
		ofxCvFloatImage flowX, flowY; //Resulted optical flow
		
		float decimate; //Decimate grey-scaled images to 30% for improve performances
		ofxCvColorImage imageDecimated1; 
		ofxCvColorImage imageDecimated2;

		cv::Mat img1;
		cv::Mat img2;
		cv::Mat flow;
		vector<cv::Mat> flowPlanes;

		int w, h;							//Decimated size of input images

		IplImage* iplX;
		IplImage* iplY;

		bool bNewFrame = false; //for not deleting iplX, iplY if the frame is not updated (deleting 2 times the same memory location)
		bool bNewFrame_draw = false;
};
