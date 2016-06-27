#include "ofApp.h"

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
