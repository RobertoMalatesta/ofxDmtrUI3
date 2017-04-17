#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	u.setup();
	u.createFromText("u.txt");

	ofAddListener(u.settings.uiEvent,this, &ofApp::uiEvents);
	ofSetFrameRate(60);

	u.onlyDrawOnRedraw = false;

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
	float raio = 100;
	float x = ofNoise(ofGetElapsedTimef()*.2) * ofGetWindowWidth();
	float y = ofNoise(ofGetElapsedTimef()*.3)* ofGetWindowHeight();
	//if (u.getVal("verdadeiro")) {
	if (u.settings.pBool["verdadeiro"]){
		ofDrawCircle(x, y, raio, raio);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
