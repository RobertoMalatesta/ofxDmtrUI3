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

	void uiEvents(string & e);
	ofxDmtrUI3 u;

	string * cena = &u.settings.pString["scene"];

	void setBoolApp(bool b) {
		ofSystemAlertDialog(b ? "yes" : "no");
	}
};
