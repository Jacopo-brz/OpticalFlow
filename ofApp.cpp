#include "ofApp.h"
#include <thread>
#define W 640
#define H 480

static bool isVGUpdated = false;
void ofApp::FirstVideoFrameCapt()
{
	using namespace std::literals::chrono_literals;
	std::cout << "started thread id = " << std::this_thread::get_id() << std::endl;
	while (!isVGUpdated)
	{
		vidGrabber.update();
		std:cout << "acquiring first frame...\n";
		isVGUpdated = vidGrabber.isFrameNew();
		//std::this_thread::sleep_for(1s); it takes several cycles for the videoGrabber to actually acquire the first frame	
	}
}
//--------------------------------------------------------------
void ofApp::setup(){
	//ofSetFrameRate(1);
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(1);
	vidGrabber.setup(W, H);
	color1.allocate(W, H);
	color2.allocate(W, H);

	flowX.allocate(W * decimate, H * decimate);
	flowY.allocate(W * decimate, H * decimate);

	decimate = 0.3;
	imageDecimated1.allocate(W * decimate, H*decimate);
	imageDecimated2.allocate(W * decimate, H * decimate);
	grey1.allocate(W * decimate, H * decimate, OF_IMAGE_COLOR);
	grey2.allocate(W * decimate, H * decimate, OF_IMAGE_COLOR);

	std::thread firstCapt(&ofApp::FirstVideoFrameCapt, this);
	firstCapt.join(); //wait until the first frame is acquired by the videoGrabber
	std::cout << "First frame acquired\n";
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	color1.setFromPixels(vidGrabber.getPixels()); //old frame
	imageDecimated1.scaleIntoMe(color1, CV_INTER_AREA); //decimated
	grey1.setFromPixels(imageDecimated1.getPixels()); //grey-scaled
	grey1.setImageType(OF_IMAGE_GRAYSCALE);
	w = grey1.getWidth();
	h = grey1.getHeight();

	vidGrabber.update();
	bNewFrame = vidGrabber.isFrameNew();
	//std::cout << "is the frame new? " << bNewFrame << std::endl; //Yes, since (at least in this version) the videoGrabber is able to acquire a new frame in a single update-draw cycle
	
	if (bNewFrame) 
	{
		bNewFrame_draw = true;
		bNewFrame = false;
		color2.setFromPixels(vidGrabber.getPixels()); //current frame
		imageDecimated2.scaleIntoMe(color2, CV_INTER_AREA); //decimated
		grey2.setFromPixels(imageDecimated2.getPixels()); //grey-scaled
		grey2.setImageType(OF_IMAGE_GRAYSCALE);

		
		img1 = ofxCv::toCv(grey1);
		img2 = ofxCv::toCv(grey2);
		cv::calcOpticalFlowFarneback(img1, img2, flow, 0.7, 3, 11, 5, 5, 1.1, 0); //default FOF values, some exception thrown
		//the output of this function is flow, a 2 channel image -same size as img1(and, of course, img2)- , at each point contains the x,y displacement
		//of img2 wrt img
		cv::split(flow, flowPlanes); //splits an n-channel image into n (single channel) arrays
		//conversion to IplImage format (why?)
		iplX = new IplImage (flowPlanes[0]);
		flowX = iplX; 
		iplY = new IplImage(flowPlanes[1]);
		flowY = iplY;
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	ofPushMatrix();
	ofTranslate(10, 5);
	ofScale(3, 3);
	color1.draw(0, 0, w, h); //reshaping the images with the decimated (w,h) -> keeps consistent the flow results
	color2.draw(w + 10, 0, w, h);

	if (bNewFrame_draw) 
	{
		bNewFrame_draw = false;
		float* flowXPixels = flowX.getPixelsAsFloats(); //pointer to the pixelvalues of flowX
		float* flowYPixels = flowY.getPixelsAsFloats();
		ofSetColor(0, 130, 200);

		for (int i = 0; i < w; i += 20)
		{
			for (int j = 0; j < h; j += 20) 
			{
				float fx = flowXPixels[i + w * j];
				float fy = flowYPixels[i + w * j];
				if ((fabs(int(fx)) + fabs(int(fy))) > 0.3) {
					ofDrawLine(i, j, i + fx, j + fy); //draw a line centered at x,y (pixel pos) --> flowXPixels, flowYPixels
					ofDrawCircle(i + fx, j + fy, 1);
				}
			}
		}
		delete iplX, iplY;
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
		
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
