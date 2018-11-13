#pragma once

#include "ofMain.h"
#include "ofxDmtrUI3.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);

	void uiEvents(uiEv & e);

	ofxDmtrUI3 u;
	ofxDmtrUI3 *ui = &u.uis["ui"];
	ofxDmtrUI3 *uiC = &u.uis["uiscene"];
	ofFbo *fbo = &u.mapFbos["fbo"];
	string * cena = &ui->pString["scene"];
	void desenha();
};
