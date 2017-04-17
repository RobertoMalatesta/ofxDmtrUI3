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

	//void uiEventsNeu(dmtrUIEvent & e);

//	ofxDmtrUI u;
//	ofxDmtrUI *ui;
//	ofxDmtrUI *uiC;

	ofFbo *fbo;
	string *cena;

	void desenha();

	void uiEvents(string & e);

	ofxDmtrUI3 u;
};
