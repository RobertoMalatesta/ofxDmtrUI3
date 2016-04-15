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
	ui.createFromText("areiasliders.txt");
	ui2.setup();
	ui2.createFromText("master.txt");
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(30);
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
