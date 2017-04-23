#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	u.setup();
	u.createFromText("u.txt");
	u.load("default.xml");

	ofAddListener(u.settings.uiEvent,this, &ofApp::uiEvents);
	ofSetFrameRate(60);

}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::desenha() {
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofFloatColor(u.pFloat["bg_r"],u.pFloat["bg_g"],u.pFloat["bg_b"]));

	float raio = u.pFloat["raio"];
	float x = ofNoise(ofGetElapsedTimef()*.2) * ofGetWindowWidth();
	float y = ofNoise(ofGetElapsedTimef()*.3)* ofGetWindowHeight();
	if (u.pBool["drawCircle"]){
		ofSetColor(ofFloatColor(u.pFloat["r"],u.pFloat["g"],u.pFloat["b"]));
		ofDrawCircle(x, y, raio, raio);
	}

//	ofSetColor(255);
//	ofDrawBitmapString(ofToString(raio), 100, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::uiEvents(string & e) {
}
