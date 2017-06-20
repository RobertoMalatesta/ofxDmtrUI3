#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//u.setup();
	// ALL Elements definitions are loaded from this file:
	u.createFromText("u.txt");

	// change to keepsettings
	u.load("default.xml");

	ofAddListener(u.settings.uiEvent,this, &ofApp::uiEvents);
	ofSetFrameRate(60);
	ofSetCircleResolution(120);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
//	ofBackground(ofFloatColor(u.pFloat["bg_r"],u.pFloat["bg_g"],u.pFloat["bg_b"]));
	ofBackground(ofColor(u.pFloat["bg_r"],u.pFloat["bg_g"],u.pFloat["bg_b"]));

	float raio = u.pFloat["raio"];
	float x = ofNoise(ofGetElapsedTimef()*.2) * ofGetWindowWidth();
	float y = ofNoise(ofGetElapsedTimef()*.3)* ofGetWindowHeight();
	if (u.pBool["drawCircle"]){
//		ofSetColor(ofFloatColor(u.pFloat["r"],u.pFloat["g"],u.pFloat["b"]));
		ofSetColor(ofColor(u.pFloat["r"],u.pFloat["g"],u.pFloat["b"]));
		ofDrawCircle(x, y, raio, raio);
	}
	u.settings.flowFree = u.pBool["flowFree"];
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::uiEvents(string & e) {
	cout << e << endl;
}
