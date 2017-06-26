#include "ofApp.h"

void bang() {
	ofSystemAlertDialog("bang!");
}

void setBool(bool b) {
	ofSystemAlertDialog(b ? "yes" : "no");
}

void setFloat(float f) {
	cout << "invoke function setfloat returning :: " + ofToString(f) << endl;
}

void setString(string s) {
	cout << "invoke function setfloat returning :: " + s << endl;
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowPosition(40, 40);
	//u.setup();
	// ALL Elements definitions are loaded from this file:
	u.createFromText("u.txt");

	// change to keepsettings
	u.load("default.xml");

	ofAddListener(u.settings.uiEvent,this, &ofApp::uiEvents);
	ofAddListener(u.settings.uiEventString,this, &ofApp::uiEventsString);
	ofSetFrameRate(60);
	ofSetCircleResolution(120);
	ofEnableAlphaBlending();

	// OK
//	u.getElement("invoke")->invoke = &bang;
//	//u.getElement("invoke")->invokeBool = &setBool;
//	u.getElement("opacity")->invokeFloat = &setFloat;
//	u.getElement("testeInt")->invokeFloat = &setFloat;
//	u.getElement("blend")->invokeString = &setString;
	//u.getElement("invoke")->invoke = setBoolApp;
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
void ofApp::uiEventsString(string & e) {
	//cout << e << endl;
}


//--------------------------------------------------------------
void ofApp::uiEvents(uiEv & e) {
	cout << e.name << endl;

	// fazer um kind string no uiev
	//if (e.kind == TOGGLE) { cout << "TOGGLE" << endl; }
}
