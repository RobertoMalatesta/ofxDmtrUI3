#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	u.setup();
	u.createFromText("u.txt");

	ofAddListener(u.settings.uiEvent,this, &ofApp::uiEvents);
	ofSetFrameRate(24);

	//u.onlyDrawOnRedraw = false;
	u.load("default.xml");
	//ofSetBackgroundAuto(false);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::desenha() {
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofBackground(40);
	ofBackground(ofFloatColor(u.pFloat["bg_r"],u.pFloat["bg_g"],u.pFloat["bg_b"]));
	float raio = 100;
	float x = ofNoise(ofGetElapsedTimef()*.2) * ofGetWindowWidth();
	float y = ofNoise(ofGetElapsedTimef()*.3)* ofGetWindowHeight();
	//if (u.getVal("verdadeiro")) {
	if (u.pBool["verdadeiro"]){
		ofSetColor(ofFloatColor(u.pFloat["r"],u.pFloat["g"],u.pFloat["b"]));
		ofDrawCircle(x, y, raio, raio);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'a') {
		u.save("uis.xml");
	}
	if (key == 's') {
		u.load("uis.xml");
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::uiEvents(string & e) {
	cout << "ofx :: " + e << endl;
}

//--------------------------------------------------------------
//void ofApp::uiEventsNeu(dmtrUIEvent & e) {
//
//}
