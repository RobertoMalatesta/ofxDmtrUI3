
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
	float		min = 0;
	float		max = 1;
	float		def = .5;
	//float 		min, max, def;
	float 		valor = 0;
	float 		*val;
	int			valorPixels = 0;

	void update(int x) {
		valorPixels = ofClamp(x-rect.x, 0, rect.width);
		valor = ofMap(valorPixels, 0, rect.width, min, max);
	}

	void setValue(float v) {
		valor = v;
		valorPixels = ofMap(valor, min, max, 0, rect.width);
	}

	void draw() {
		ofSetColor(cor);
		ofDrawRectangle(rect);

		// value rectangle black transparent
		ofSetColor(0,128);
		ofDrawRectangle(rect.x, rect.y, valorPixels, rect.height);
		string label = nome + " "+ofToString(valor);
		ofSetColor(0,128);
		ofDrawBitmapString(label, rect.x+11, rect.y+15);
		ofSetColor(255);
		ofDrawBitmapString(label, rect.x+10, rect.y+14);
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

	void draw() {
		ofSetColor(cor);
		ofDrawRectangle(rect);

		// toggle Label
		//			ofSetColor(255);
		//			ofDrawBitmapString(t.nome, t.rect.x+30, t.rect.y+14);

		if (valor) {
			ofSetColor(0);
			ofNoFill();
			int off = 3;
			ofDrawLine(rect.x + off, rect.y + off, 			     rect.x + rect.width -off, rect.y + rect.height -off);
			ofDrawLine(rect.x + off, rect.y + rect.height - off, rect.x + rect.width -off, rect.y + off);
			ofFill();
		}
	}
};

class label {
public:
	string 			nome;
	ofRectangle 		rect;
	ofColor 			cor;

	void draw() {
		ofSetColor(cor);
		//ofDrawRectangle(rect);
		ofDrawBitmapString(nome, rect.x+11, rect.y+15);
	}
};

class ofxDmtrUI : public ofBaseApp
{
public:
	void		setup();
	void		keyPressed(int key);
	void		keyReleased(int key);
	void		update();
	void		draw();
	void		exit();
	void 	mouseDragged(int x, int y, int button);
	void 	mousePressed(int x, int y, int button);
	void 	mouseReleased(int x, int y, int button);
	void 	mouseAll(int x, int y, int button);

	void 	onDraw(ofEventArgs &data);
	void 	onUpdate(ofEventArgs &data);
	void 	onKeyPressed(ofKeyEventArgs &data);
	void 	onKeyReleased(ofKeyEventArgs &data);
	void 	onMousePressed(ofMouseEventArgs &data);
	void 	onMouseDragged(ofMouseEventArgs &data);
	void 	onMouseReleased(ofMouseEventArgs &data);
	void 	onMouseMoved(ofMouseEventArgs &data);
	void 	onExit(ofEventArgs &data);
	void		createFromText(string file);


	// rename to createelement
	void		create(string nome, string tipo="slider", ofVec3f valores = ofVec3f(0,1,0)); // NULL

	// generate some elements
	void		generate(string text="");

	void		save(string xml);
	void		load(string xml);

	vector <string> textToVector(string file);

	map <string,float>			pEasy;
	map <string,float>			pFloat;
	map <string,int>				pInt;
	map <string,bool>			pBool;
	map <string,string>			pString;

	vector <slider> sliders;
	vector <toggle> toggles;
	vector <label> labels;

	ofFbo fbo;
	float easing = 30;
	bool showGui = true;
	bool redraw = true;
	bool columnOver = false;

	ofRectangle coluna = ofRectangle(0,0,620,560);
	float marginx = 20;
	float marginy = 20;
	ofPoint flow = ofPoint(marginx, marginy);

};
