/*
ofxDmtrUISyphonIn 
 Created by Dimitre Lima http://dmtr.org/
 Copyright 2016 Dmtr.org. All rights reserved.
 Only for Dmtr.org projects use.
*/

#pragma once
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxDmtrUI3.h"
#include "ofxSyphon.h"

class ofxDmtrUISyphonIn : public ofBaseApp {
public:
	void setup();
	void keyPressed(int key);
	void keyReleased(int key);
	void onKeyPressed(ofKeyEventArgs &data);
	void onKeyReleased(ofKeyEventArgs &data);

	void syphonUpdated(ofxSyphonServerDirectoryEventArgs &arg);

	ofxSyphonClient	syphonIn;
	ofxSyphonServerDirectory syphonList;

	ofxDmtrUI3 ui;
	ofxDmtrUI3 *uiSyphon = NULL;

	void uiEvents(uiEv & e);
	
	void updateList();

	string ignoreApp = "";
};
