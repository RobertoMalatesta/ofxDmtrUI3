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

	// old way of creating a wall of toggle buttons.
	int y = 20;
	for (int b=0; b<4; b++) {
		for (int a=0; a<10; a++) {
			toggle tt;
			tt.nome = "Toggle" + ofToString(a) + ofToString(b);
			float altura = 20;
			float altura2 = altura*1.25;
			float w = 20;
			tt.rect = ofRectangle(10 + b *(altura*1.25), 40 + a*(altura*1.25), w,altura); //(y+=altura2)
			tt.cor = ofColor::fromHsb(a*4+b*5,255,255);
			tt.showLabel = false;
			ui.toggles.push_back(tt);
		}
	}

	ui.flow.x = 150;
	ui.flow.y = 40;
	for (int a=0; a<20; a++) {
		ui.create("Slider" + ofToString(a));
	}

	ofSetCircleResolution(94);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(30);
	ofSetColor(255);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
	float tempo = ofGetElapsedTimef();
	float x = ofNoise(tempo/5.0 * ui.pEasy["Slider3"])*ofGetWindowWidth();
	float y = ofNoise(tempo/7.0 * ui.pEasy["Slider4"])*ofGetWindowHeight();
	float raio = 200;
	ofSetColor(ui.pEasy["Slider0"]*255.0, ui.pEasy["Slider1"]*255.0, ui.pEasy["Slider2"]*255.0);
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
