
#pragma once
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"



class slider {
public:
	string 		nome;
	ofRectangle rect;
	bool 		inside = false;
	ofColor 		cor;
	float 		min, max, def;
	float 		valor = 0;
	float 		*val;

	void update(int x) {

		valor = ofClamp((x - rect.x)/(float)rect.width, 0, 1);
		//val = valor;
//		cout << valor << endl;
//		cout << val << endl;
	}
};

class toggle {
public:
	string 			nome;
	ofRectangle 		rect;
	bool 			inside = false;
	ofColor 			cor;
	bool 			valor = false;
	bool				showLabel = false;
};


class ofxDmtrUI : public ofBaseApp
{
public:
	

	// teste virtual void setup - 29 de marco de 2016
	void		setup();
	void		generate(string text="");
	void		keyPressed(int key);
	void		keyReleased(int key);
	void		update();
	void		draw();
	void		exit();

	// rename to createelement
	void		create(string nome, string tipo="slider");

	void		save(string xml);
	void		load(string xml);

	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	void mouseAll(int x, int y, int button);


	void onDraw(ofEventArgs &data);
	void onUpdate(ofEventArgs &data);
	void onKeyPressed(ofKeyEventArgs &data);
	void onKeyReleased(ofKeyEventArgs &data);
	void onExit(ofEventArgs &data);

	void onMousePressed(ofMouseEventArgs &data);
	void onMouseDragged(ofMouseEventArgs &data);
	void onMouseReleased(ofMouseEventArgs &data);

	void onMouseMoved(ofMouseEventArgs &data);

	
	map <string,float>			pEasy;
	map <string,float>			pFloat;
	map <string,int>				pInt;
	map <string,bool>			pBool;
	map <string,string>			pString;

	vector <slider> sliders;
	vector <toggle> toggles;

	ofFbo fbo;
	float easing = 30;
	bool showGui = true;
	bool redraw = true;
	bool columnOver = false;

	ofRectangle coluna = ofRectangle(0,0,620,560);
	ofPoint flow = ofPoint(0,0);


};
