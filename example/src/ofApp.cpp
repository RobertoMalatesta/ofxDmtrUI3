#include "ofApp.h"

/*

 Example using easing parameters.
 First 3 sliders control RGB
 We are using pEasy which is pFloat values with easing

 Presets can be loaded using 1, 2, 3, 4 keys
 and saved using COMMAND + 1, 2, 3, 4 keys

 */


//--------------------------------------------------------------
void ofApp::setup(){
	ui.setup();
	ui.generate();
	ofSetCircleResolution(94);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(30);

	ofSetColor(255);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);

	float tempo = ofGetElapsedTimef();
	float x = ofNoise(tempo/10.0)*ofGetWindowWidth();
	float y = ofNoise(tempo/12.5)*ofGetWindowHeight();
	float raio = 200;
	ofSetColor(ui.pEasy["Slider1"]*255.0, ui.pEasy["Slider2"]*255.0, ui.pEasy["Slider3"]*255.0);
	ofDrawEllipse(x,y,raio,raio);
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}
