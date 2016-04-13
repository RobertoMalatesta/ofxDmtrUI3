#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ui.flow.x = 20;
	ui.flow.y = 20;
	ui.createFromText("ui.txt");
	ui.setup();
	//ui.UINAME = "ui1";

	ui.pInt["gridAlpha"] = 127;
	ui.pInt["gridAlpha2"] = 255;
	ui.pInt["gridAresta"] = 10;

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0);
	ofSetColor(255, ui.pInt["gridAlpha"]);
	int w = 1920;
	int h = 1080;
	ofSetLineWidth(1);
	for (int a=0; a<w; a+= ui.pInt["gridAresta"]) {
		ofDrawLine(a, 0, a, h);
	}
	for (int a=0; a<h; a+= ui.pInt["gridAresta"]) {
		ofDrawLine(0, a, w, a);
	}

	ofSetColor(255, ui.pInt["gridAlpha2"]);
	ofSetLineWidth(1);
	for (int a=0; a<w; a+= 10*ui.pInt["gridAresta"]) {
		ofDrawLine(a, 0, a, h);
	}
	for (int a=0; a<h; a+= 10*ui.pInt["gridAresta"]) {
		ofDrawLine(0, a, w, a);
	}



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	ofImage img;

	img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
	img.save("grid.png");

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
