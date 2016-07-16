/**********************************************************************************

 Copyright (C) 2016 Dimitre Lima (www.dmtr.org)

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 **********************************************************************************/


#pragma once
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"

//#if defined( TARGET_OF_IPHONE ) || defined( TARGET_OF_IOS ) || defined( TARGET_ANDROID )
//	#define DMTRUI_TARGET_TOUCH
//#endif

enum flowDir {
	VERT, HORIZ, NO_FLOW
};

// 27 de maio de 2016
enum elementType {
	SLIDER, SLIDERINT, TOGGLE, LABEL, RADIO, RADIOITEM, SLIDER2D, FBO, PRESETS
};

enum eventoType {
	// bang fica aqui mesmo?
	UPDATE, LOAD, SET, BANG
};

enum varType {
	FLOAT, INT, STRING, BOOLEANO
};



// 27 de maio de 2016, ainda n‹o sei se vai dar certo.
class dmtrUIEvent {
public:
	string *_nome;
	string nome;
	string ui;
	elementType element;
	eventoType tipo;
	varType var = FLOAT;
	//ofxDmtrUI *_ui;
	// unknown type name.
	//string action; //update, load, etc.
};


// 28 de maio. rascunho pro futuro - acho que provisorio.
//dmtrUIEvent sendEvent(string & nome, varType var, elementType element , eventoType tipo ) { //string ui,
//	dmtrUIEvent te;
//	te._nome = &nome;
//	te.tipo = UPDATE;
//	te.element = element;
//	te.tipo = tipo;
//	te.var = var;
//	return te;
//}


class slider {
public:
	string 		nome;
	elementType tipo;
	ofRectangle rect;
	bool 		inside = false;
	ofColor 		cor;
	float		min = 0;
	float		max = 1;
	float		def = .5;
	//float 		min, max, def;

	// com o tempo remover o valor e valorInt e so usar os outros.
//	float 		valor;
//	int			valorInt;

	// automatic so it can choose between float and integer :)
	// doesnt work
	// const auto			*_val;
	float 		*_val;
	int			*_valInt;
	// temporario
	unsigned char			*_valChar;

	int			valorPixels = 0;
	bool			isInt = false;
	// 15 04 2016 - boolean for vertical sliders
	bool			vert = false;

	ofEvent<string> uiEvent;

	// 27 05 2016 - tipo de evento mais completo
	ofEvent<dmtrUIEvent> evento;

	void update(int x, int y) {
		dmtrUIEvent te;
		te.nome = nome;
		te._nome = &nome;
		te.tipo = UPDATE;

		// nome pode ser um pointer.
		// fazer algo que cheque se o valor foi modificado da ultima vez pra disparar o evento?
		if (vert) {
			valorPixels = ofClamp(rect.height - (y - rect.y), 0, rect.height);
		} else {
			valorPixels = ofClamp(x - rect.x, 0, rect.width);
		}

		if (isInt) {
			te.element = SLIDERINT;
			te.var = INT;
			if (vert) {
				*_valInt = ofMap(valorPixels, 0, rect.height, min, max);
			} else {
				*_valInt = ofMap(valorPixels, 0, rect.width, min, max);
			}
			string ev = "updateInt_" + nome;
			ofNotifyEvent(uiEvent, ev, this);
		} else {
			te.element = SLIDER;

			if (vert) {
				*_val = ofMap(valorPixels, 0, rect.height, min, max);
			} else {
				*_val = ofMap(valorPixels, 0, rect.width, min, max);
			}
			string ev = "updateFloat_" + nome;
			ofNotifyEvent(uiEvent, ev, this);
		}

		ofNotifyEvent(evento, te, this);
	}

	void setValue(float v, string eventName = "set") {

		// posso colocar o evento aqui mas no caso vou dar um trigger qdo estiver carregando XML
		// trigando todos os All e baguncando presets.

		// hoje coloquei eventos aqui pra poder usar o pan_All desde o midi controller
		string ev;
		if (isInt) {
			*_valInt = v;
			ev = eventName + "Int_" + nome;
		} else {
			*_val = v;
			ev = eventName + "Float_" + nome;
		}
		valorPixels = ofMap(v, min, max, 0, rect.width);

		if (ev != "") {
			ofNotifyEvent(uiEvent, ev, this);
		}

//		dmtrUIEvent e = sendEvent(nome, isInt ? INT : FLOAT, isInt ? SLIDERINT : SLIDER, SET);
//		ofNotifyEvent(evento, e, this);

	}

//	void draw() {
	void draw(bool learn = false) {
		auto vvv = (isInt ? *_valInt : *_val);
		valorPixels = ofMap(vvv, min, max, 0, rect.width);

		ofSetColor(cor);
		ofDrawRectangle(rect);

		// value rectangle black transparent
		ofSetColor(0,128);
		if (vert) {
			valorPixels = ofMap(vvv, min, max, 0, rect.height);

			//ofDrawRectangle(rect.x, rect.y, rect.width, valorPixels);
			ofDrawRectangle(rect.x, rect.y + rect.height-valorPixels, rect.width, valorPixels);
		} else {
			ofDrawRectangle(rect.x, rect.y, valorPixels, rect.height);
			string label = nome + " "+ofToString(isInt ? *_valInt : *_val);
			ofSetColor(0,128);
			int offy = rect.height - 2;
			ofDrawBitmapString(label, rect.x+11, rect.y+offy);
			ofSetColor(255);
			ofDrawBitmapString(label, rect.x+10, rect.y+offy-1);
		}

		if (learn) {
			ofSetColor(255,0,70);
			ofNoFill();
			//ofSetColor(0,50,200,50);
			ofDrawRectangle(rect);
			ofFill();
		}
	}
};

class toggle {
public:
	string 			nome;
	ofRectangle 		rect;
	bool 			inside = false;
	ofColor 			cor;
	//bool 			valor = false;
	bool				showLabel = true;
	bool				*_val;
	bool				def = false;
	bool				bang = false;

	ofEvent<string> uiEvent;
	ofEvent<dmtrUIEvent> evento;

	void flip() {
		*_val = !*_val;
		string ev = bang ? "bang_" + nome : "updateBool_" + nome;
		ofNotifyEvent(uiEvent, ev, this);

		// 28 maio 2016
		dmtrUIEvent te;
		te._nome = &nome;
		te.tipo = BANG;
		te.var = BOOLEANO;
		ofNotifyEvent(evento, te, this);

	}

	// acho que nao preicsa mais? precisa?
	// setvalue aqui ta triggering o evento.
	void setValue(bool v) {
		*_val = v;
		// 19 de abril. vai atrapalhar?
		string ev = "loadBool_" + nome;
		ofNotifyEvent(uiEvent, ev, this);
	}

	void draw() {
		ofSetColor(cor);
		ofDrawRectangle(rect);

		// toggle Label
		if (showLabel) {
			ofSetColor(255);
			ofDrawBitmapString(nome, rect.x + rect.width + 5, rect.y+16);
		}

		if (*_val) {
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
	string			*_val;

	void draw() {
		ofSetColor(cor);
		//ofDrawRectangle(rect);
		ofDrawBitmapString(nome + " " + *_val, rect.x + 0, rect.y+15);
	}
};

class radio  {
public:
	string 			nome;
	ofRectangle 		rect;
	ofColor 			cor;
	string 			*_val;

	vector <string>	opcoes;
	vector <ofRectangle> 	rects;


	// tentativa de multiple
	bool multiple = false;
	vector <bool *> _vals;
	vector <bool> clicked;

	int offx = 0;
	int offy = 0;
	int height = 20;
	ofEvent<string> uiEvent;
	ofEvent<dmtrUIEvent> evento;


	// pra fazer da mareh o lookup de cores.
	vector <ofColor> cores;
	bool useColors = false;

	void setValue(string v, int notify = 0) {
		*_val = v;
		dmtrUIEvent te;
		te.nome = nome;
		te._nome = &nome;
		// vamos ver direito tudo isso.
		// mudar tudo pra ingles?
		te.element = RADIO;
		te.var = STRING;

		if (notify == 1) {
			te.tipo = UPDATE;
			string ev = "updateRadio_" + nome;
			ofNotifyEvent(uiEvent, ev, this);
		}

		if (notify == 2) {
			te.tipo = LOAD;
			string ev = "loadRadio_" + nome;
			ofNotifyEvent(uiEvent, ev, this);
		}
		ofNotifyEvent(evento, te, this);
	}

	void checkMouse(int x, int y) {
		int i = 0;
		for (auto & r : rects) {
			if (r.inside(x,y) ) {
				if (multiple) {
					if (!clicked[i]) {
						*_vals[i] ^= 1;

						// ver uma maneira mais firmeza de modularizar isso:
						dmtrUIEvent te;
						te.nome = nome;
						te._nome = &nome;
						te.element = RADIO; // fazer RADIOMULT?
						te.tipo = UPDATE;
						te.var = STRING;
						ofNotifyEvent(evento, te, this);
					}
					clicked[i] = true;
				}

				else {
					if (*_val != opcoes[i]) {
						*_val = opcoes[i];
						string ev = "updateRadio_" + nome;
						ofNotifyEvent(uiEvent, ev, this);
						dmtrUIEvent te;
						te.nome = nome;
						te._nome = &nome;
						te.element = RADIO;
						te.tipo = UPDATE;
						te.var = STRING;
						ofNotifyEvent(evento, te, this);					}
				}
			}
			// 12 de maio de 2016. tentando evento do multiple radio
			else {
				clicked[i] = false;
			}
			i ++;
		}
	}

	void init() {
		for (auto & o : opcoes) {
			clicked.push_back(false);
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

			// temporario pras cores de moving heads mareh
			cores.push_back(cor);
		}

		// recalculando a largura total do rect sem mexer no offx e offy.
		//rect.width = widthAll;
		// fazer com que o rect principal seja da largura de todos os outros rects.
	}

	void draw() {
		ofSetColor(255);
		int offy = 15;
		int offx = 0;
		//ofSetColor(255,0,60,128);
		//ofDrawRectangle(rect);

		int i = 0;
		if (multiple) {
			for (auto & r : rects) {
				auto & o = opcoes[i];
				ofSetColor(*_vals[i] ? cor : ofColor(80,120));
				if (useColors) {
					ofSetColor(cores[i]);
				}
				ofDrawRectangle (r);
				ofSetColor(*_vals[i] ? 0 : 255);
				ofDrawBitmapString(o, r.x + 6, r.y + offy);
				i++;
			}
		} else {

			for (auto & r : rects) {
				auto & o = opcoes[i];
				ofSetColor(*_val == o ? cor : ofColor(80,120));
				if (useColors) {
					ofSetColor(cores[i]);
				}
				ofDrawRectangle (r);
				ofSetColor(*_val == o ? 0 : 255);
				ofDrawBitmapString(o, r.x + 6, r.y + offy);
				i++;
			}
		}
	}
};


class slider2d {
public:
	string nome;
	ofPoint	 *_val;
	//	float *_valx;
	//	float *_valy;
	float defx = .5;
	float defy = .5;
	ofFbo *_fbo;
	bool		fboSet = false;
	ofRectangle rect;
	ofColor cor;
	ofEvent<string> uiEvent;
	bool inside = false;
	bool isSlider = true;

	void setFbo(ofFbo &fbo) {
		_fbo = &fbo;
		fboSet = true;
	}

	void setValue(ofPoint xy) {
		//cout << "setValue Slider2d " + nome << endl;
		*_val = xy;
		// UNIFICAR EVENTOS DEPOIS
		string ev = "loadSlider2d_" + nome;
		ofNotifyEvent(uiEvent, ev, this);
	}

	void checkMouse(int mx, int my) {
		float x = mx - rect.x;
		float y = my - rect.y;
		*_val = ofPoint(
						ofClamp(ofMap(x, 0, rect.width, 0, 1), 0, 1),
						ofClamp(ofMap(y, 0, rect.height, 0, 1), 0, 1)
						);
		string ev = "updateSlider2d_" + nome;
		ofNotifyEvent(uiEvent, ev, this);
	}

	void draw() {
		if (fboSet) {
			//ofSetColor(255);
			_fbo->draw(rect.x, rect.y);
		}
		else
		{
			//ofSetColor(cor);
			ofDrawRectangle(rect);
		}

		if (isSlider) {
			//ofSetColor(255,0,0);
			ofNoFill();
			//ofPushMatrix();
			ofPoint xy = *_val;
			float x = xy.x * rect.width + rect.x;
			float y = xy.y * rect.height + rect.y;
			ofDrawLine(x, rect.y, x, rect.y + rect.height);
			ofDrawLine(rect.x, y, rect.x + rect.width, y);
			ofFill();
			float raio = 4;
			ofDrawRectangle(x -raio/2, y-raio/2, raio,raio);
		}
	}
};

class preset {
public:
	int index;
	string nome;
	ofRectangle	rect;
	ofImage	img;
	ofFbo fbo;
	bool selecionado = false;
	void draw() {
		ofSetColor(255);
		fbo.begin();
		ofClear(0,255);
		if (img.isAllocated()) {
			img.draw(0,0);
		}
		if (selecionado) {
			ofSetColor(255,0,80);
			ofDrawRectangle(5,5,20,20);
		}
		fbo.end();
		fbo.draw(rect.x, rect.y, rect.width, rect.height);
	}
};

class presets {
public:
	string nome = "presets";
	ofRectangle rect;
	vector <preset> presets;
	//int	*_val;
	// temporario
	int valor = -1;
	bool ok = false;
	ofEvent<string> uiEvent;

	void draw() {
		for (auto & p : presets) {
			p.draw();
		}
	}

	void update(int novovalor) {
	}

	// 10 de maio, Mareh Marisco, apenas testando.
	void set(int slot) {
		//cout << "set : " + ofToString(slot) << endl;
		if (valor != -1) {
			presets[valor].selecionado = false;
			presets[valor].draw();
		}
		if (presets.size() > 0) {
			valor = slot;
			presets[valor].selecionado = true;
			presets[valor].draw();
		}
	}

	void checkMouse(int x, int y) {
		for (auto & p : presets) {
			if (p.rect.inside(x,y)) {
				// only trigger events if clicked in different preset slot
				if (ofGetKeyPressed(OF_KEY_COMMAND)) {
					if (valor != -1) {
						presets[valor].selecionado = false;
						presets[valor].draw();
					}
					valor = p.index;
					p.selecionado = true;
					string ev = "savePreset_" + ofToString(valor);
					ofNotifyEvent(uiEvent, ev, this);
					p.draw();
				} else {
					if (valor != p.index) {
						if (valor != -1) {
							presets[valor].selecionado = false;
							presets[valor].draw();
						}
						valor = p.index;
						p.selecionado = true;
						string ev = "loadPreset_" + ofToString(valor);
						ofNotifyEvent(uiEvent, ev, this);
						p.draw();
					}
				}
			}
		}
	}
};

class color {
public:
	string nome;
	float *_h;
	float *_s;
	float *_b;
	float *_a;
	slider *_sh;
	slider *_ss;
	slider *_sb;
	slider *_sa;

};


class element {
public:
	string nome;
	elementType tipo;
	ofRectangle *_rect;

	slider *_slider = NULL;
	toggle *_toggle = NULL;
	label *_label = NULL;
	radio *_radio = NULL;
	presets *_presets = NULL;
	slider2d *_slider2d = NULL;

	void set (slider2d &e) {
		nome = e.nome;
		_rect = &e.rect;
		_slider2d = &e;
		tipo = SLIDER2D;
	}

	void set (presets &e) {
		nome = e.nome;
		_rect = &e.rect;
		_presets = &e;
		tipo = PRESETS;
	}

	void set (slider &e) {
		nome = e.nome;
		_rect = &e.rect;
		_slider = &e;
		// ou sliderint?
		tipo = SLIDER;
	}

	void set (toggle &e) {
		nome = e.nome;
		_rect = &e.rect;
		_toggle = &e;
		tipo = TOGGLE;
	}

	void set (label &e) {
		nome = e.nome;
		_rect = &e.rect;
		_label = &e;
		tipo = LABEL;
	}

	void set (radio &e) {
		nome = e.nome;
		_rect = &e.rect;
		_radio = &e;
		tipo = RADIO;
	}


	/*
	 busquei o std::bind acho que da pra fazer ponteiro de funcao pro draw
	*/
	//void *_draw;
	// function pointer.
	void (*_draw)(void);

//	typedef void (*funcao)();
//	funcao _draw;

	void draw() {
		if (_slider != NULL) {
			_slider->draw();
		}
	}
};

//void funcao() {
//	cout << "funcao" << endl;
//}


class ofxDmtrUI : public ofBaseApp
{
public:
	void setup(string uiName = "");
	void keyPressed(int key);
	void keyReleased(int key);
	void update();
	void draw();
	void exit();
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseAll(int x, int y, int button);
	void mousePressedDragged(int x, int y, int button);

	void onDraw(ofEventArgs &data);
	void onUpdate(ofEventArgs &data);
	void onKeyPressed(ofKeyEventArgs &data);
	void onKeyReleased(ofKeyEventArgs &data);
	void onMousePressed(ofMouseEventArgs &data);
	void onMouseDragged(ofMouseEventArgs &data);
	void onMouseReleased(ofMouseEventArgs &data);
	void onMouseMoved(ofMouseEventArgs &data);
	void onExit(ofEventArgs &data);

	void createFromText(string file);
	void createFromLine(string line);

	// rename to createelement
	void create(string nome, string tipo="slider", string valores = "", string valores2 = ""); // NULL
	void save(string xml);
	void load(string xml);
	void expires(int dataInicial, int dias = 10);
	void uiEvents(string & e);
	void uiEventsNeu(dmtrUIEvent & e);
	void autoFit(bool w = true, bool h = true);

	void setFloat(string nome, float val);
	void setBool(string nome, bool val);
	void setRadio(string nome, string val);

	void loadPreset(int n);
	void loadPresetAll(int n);
	void savePreset(int n);
	void savePresetAll(int n);
	void setFbo(ofFbo &fbo);

	vector <string> textToVector(string file);

	vector <slider> 	sliders;
	vector <toggle> 	toggles;
	vector <label> 	labels;
	vector <radio> 	radios;
	vector <slider2d> sliders2d;

	// WOW
	vector <element> elements;

	map <string,float>			pEasy;
	map <string,float>			pFloat;
	map <string,int>				pInt;
	map <string,bool>			pBool;
	map <string,string>			pString;
	map <string,string>			pLabel;
	map <string,ofPoint>			pPoint;
	map <string,ofFloatColor>	pColor;
	// NOVO
	map <string,string>			pFolder;
	// only internal use to backup some variables.
	map <string,float>			pFloatBak;
	

	// Fbo to draw GUI column
	ofFbo fbo;
	float easing = 5;
	bool showGui = true;
	bool redraw = true;
	bool columnOver = false;

	// Layout, default
	bool bw = false;
	float marginx = 20;
	float marginy = 20;
	int sliderHeight = 20;
	int sliderWidth  = 150;
	int	sliderMargin = 5;
	// precisa?
	int	lastHeight, lastWidth;
	
	ofPoint flow = ofPoint(marginx, marginy);
	flowDir flowDirection = VERT;

	bool	 saveLoadShortcut = false;
	bool keepSettings = false;
	bool useShortcut = false;
	string presetsFolder = "_presets/";
	string presetsFolderNumber = "";
	string UINAME = "ui";

	bool debug = false;
	int hueStart = 100;
	bool learnMode = false;
	float opacity = 255;
	float opacityRest = 255;
	bool dirListEntireName = false;

	ofColor colunaBackground = ofColor(0,100);
	ofRectangle coluna = ofRectangle(0,0,250,250);
	ofPoint presetDimensions = ofPoint(100,25);


	ofEvent<string> uiEvent;
	ofEvent<dmtrUIEvent> evento;

	// ainda ver direitiho se isso vai rolar.
	map <string, map <string, string> > dirListMap;
	map <string, int> indexElement;

	// 26 04 2016 - presets
	presets allPresets;

	// fbo pointer to save presets.
	ofFbo *_fbo;
	int presetLoaded;

	vector <ofxDmtrUI *> _presetsUIs;
	ofxDmtrUI * uiC = NULL;
	ofxDmtrUI * uiM = NULL;

	void re();
	float getNoise(string nome, float a);
	void clear(bool keepVars = false);
	void addAllListeners();
	vector<string> colors;
	string getPresetsFolder();


};

