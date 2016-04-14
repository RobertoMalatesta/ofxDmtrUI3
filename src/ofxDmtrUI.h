
#pragma once
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"


enum flowDir {
	VERT, HORIZ, NO_FLOW
};

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
	float 		valor;
	int			valorInt;
	float 		*val;
	float 		val2;
	int			valorPixels = 0;
	bool			isInt = false;

	void update(int x) {
		valorPixels = ofClamp(x - rect.x, 0, rect.width);
		valor = ofMap(valorPixels, 0, rect.width, min, max);

		if (isInt) {
			valorInt = ofMap(valorPixels, 0, rect.width, min, max);
		}
	}

	void setValue(float v) {
		if (isInt) {
			valorInt = v;
		} else {
			valor = v;
		}
		valorPixels = ofMap(v, min, max, 0, rect.width);
	}

	void draw() {
		ofSetColor(cor);
		ofDrawRectangle(rect);

		// value rectangle black transparent
		ofSetColor(0,128);
		ofDrawRectangle(rect.x, rect.y, valorPixels, rect.height);
		string label = nome + " "+ofToString(isInt ? valorInt : valor);
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
	bool				showLabel = true;

	void draw() {
		ofSetColor(cor);
		ofDrawRectangle(rect);

		// toggle Label
		if (showLabel) {
			ofSetColor(255);
			ofDrawBitmapString(nome, rect.x + rect.width + 5, rect.y+16);
		}

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
		ofDrawBitmapString(nome, rect.x + 0, rect.y+15);
	}
};


class radio  {
public:
	string 			nome;
	ofRectangle 		rect;
	ofColor 			cor;
	vector <string>	opcoes;
	vector <ofRectangle> 	rects;
	string		selecionado;

	string *_val;

	int offx = 0;
	int offy = 0;
	int height = 20;
	ofEvent<string> uiEvent;

	void checkMouse(int x, int y) {
		int i = 0;
		for (auto & r : rects) {
			if (r.inside(x,y)) {
				if (selecionado != opcoes[i]) {
					selecionado = opcoes[i];
					*_val = selecionado;
					string ev = "updateRadio" + nome;
					ofNotifyEvent(uiEvent, ev, this);
				}
			}
			i ++;
		}
	}

	void init() {
		for (auto & o : opcoes) {
			int largura = 6*2 + o.size() * 8;
			ofRectangle tr = ofRectangle(rect.x + offx, rect.y + offy, largura, height);

			// mudar para sliderwidth
			if ((offx + tr.width) > rect.width) {
				offx = 0;
				offy += 20 + 1;
				tr = ofRectangle(rect.x + offx, rect.y + offy, largura, height);
			}
			rect.height = MAX(rect.height, offy + 21);
			offx += largura + 1;
			rects.push_back(tr);
		}

		// recalculando a largura total do rect sem mexer no offx e offy.
		//rect.width = widthAll;
		// fazer com que o rect principal seja da largura de todos os outros rects.
	}

	void draw() {
		ofSetColor(255);
		int offy = 15;
		int offx = 0;
//				ofSetColor(255,0,60,128);
//				ofDrawRectangle(rect);

		int i = 0;
		for (auto & r : rects) {
			auto & o = opcoes[i];
			ofSetColor(selecionado == o ? cor : ofColor(80,120));
			ofDrawRectangle (r);
			ofSetColor(selecionado == o ? 0 : 255);
			ofDrawBitmapString(o, r.x + 6, r.y + offy);
			i++;
		}
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
	void 	mousePressedDragged(int x, int y, int button);

	void 	onDraw(ofEventArgs &data);
	void 	onUpdate(ofEventArgs &data);
	void 	onKeyPressed(ofKeyEventArgs &data);
	void 	onKeyReleased(ofKeyEventArgs &data);
	void 	onMousePressed(ofMouseEventArgs &data);
	void 	onMouseDragged(ofMouseEventArgs &data);
	void 	onMouseReleased(ofMouseEventArgs &data);
	void 	onMouseMoved(ofMouseEventArgs &data);
	void 	onExit(ofEventArgs &data);

	// rename to createelement
	void		create(string nome, string tipo="slider", string valores = ""); // NULL

	void		save(string xml);
	void		load(string xml);

	// this is only used in the next function
	vector <string> textToVector(string file);
	void		createFromText(string file);

	void		expires(int dataInicial, int dias = 10);

	//void		uiEvents(ofEventArgs & args);
	void		uiEvents(string & e);

	map <string,float>			pEasy;
	map <string,float>			pFloat;
	map <string,int>				pInt;
	map <string,bool>			pBool;
	map <string,string>			pString;

	vector <slider> 	sliders;
	vector <toggle> 	toggles;
	vector <label> 	labels;
	vector <radio> 	radios;

	ofFbo fbo;
	float easing = 30;
	bool showGui = true;
	bool redraw = true;
	bool columnOver = false;

	ofRectangle coluna = ofRectangle(0,0,620,560);
	float marginx = 20;
	float marginy = 20;
	ofPoint flow = ofPoint(marginx, marginy);

	flowDir flowDirection = VERT;
	bool	 saveLoadShortcut = true;

	string UINAME = "";

	int sliderHeight = 20;
	int sliderWidth = 150;

	ofEvent<string> uiEvent;

	map< string, map <string, string> > dirListMap;


	ofEvent<string> evento;

};


