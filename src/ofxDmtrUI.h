
#pragma once
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"



class slider {
public:
	string nome;
	ofRectangle rect;
	ofColor cor;
	float min, max, def;
	double valor = 0;

	void update(int x) {
		valor = (x - rect.x)/(float)rect.width;
	}
};

class toggle {
public:
	string nome;
	ofRectangle rect;
	ofColor cor;
	bool valor = false;
	bool inside = false;
};


class ofxDmtrUI : public ofBaseApp
{
public:
	

	// teste virtual void setup - 29 de marco de 2016
	void		setup();
	void		keyPressed(int key);
	void		keyReleased(int key);
	void		update();
	void		draw();
	void		exit();

	void		save(string xml);
	void		load(string xml);

	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);


	void onDraw(ofEventArgs &data);
	void onUpdate(ofEventArgs &data);
	void onKeyPressed(ofKeyEventArgs &data);
	void onKeyReleased(ofKeyEventArgs &data);
	void onExit(ofEventArgs &data);

	void onMousePressed(ofMouseEventArgs &data);
	void onMouseDragged(ofMouseEventArgs &data);
	void onMouseReleased(ofMouseEventArgs &data);

	
	vector <slider> sliders;
	vector <toggle> toggles;

	
	map <string,float>			pEasy;
	map <string,float>			pFloat;
	map <string,int>				pInt;
	map <string,bool>			pBool;
	map <string,string>			pString;

	bool showGui = true;
	bool redesenha = true;

	ofFbo fbo;

};
