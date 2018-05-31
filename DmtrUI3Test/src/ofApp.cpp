#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowPosition(40, 40);

	u.createFromText("u.txt");
	u.load("default.xml");

	ofAddListener(u.settings.uiEvent,this, &ofApp::uiEvents);
	ofSetFrameRate(60);
	ofSetCircleResolution(120);
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor(u.pFloat["bg_r"],u.pFloat["bg_g"],u.pFloat["bg_b"]));

	if (*cena == "test") {
		float raio = u.pFloat["raio"];
		float x = ofNoise(ofGetElapsedTimef()*.2) * ofGetWindowWidth();
		float y = ofNoise(ofGetElapsedTimef()*.3)* ofGetWindowHeight();
		if (u.pBool["drawCircle"]){
			ofSetColor(ofColor(u.pFloat["r"],u.pFloat["g"],u.pFloat["b"]));
			ofDrawCircle(x, y, raio, raio);

			ofSetColor(255);
			ofDrawBitmapString(u.pString["blend"], x, y);
		}

		if (u.pBool["circle2"]){
			float x = u.pPoint["slider2d"].x * ofGetWindowWidth();
			float y = u.pPoint["slider2d"].y * ofGetWindowHeight();
			ofSetColor(ofColor(u.pFloat["r2"],u.pFloat["g2"],u.pFloat["b2"]));
			ofDrawCircle(x, y, 100);
		}


	}
	// modificar pra events
	u.settings.flowFree = u.pBool["flowFree"];
	u.settings.opacity = u.pFloat["opacity"];
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//cout << u.settings.pString["blend"] << endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::uiEvents(uiEv & e) {
	//cout << e.name << endl;
}
