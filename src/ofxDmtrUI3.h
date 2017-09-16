/**********************************************************************************

 Copyright (C) 2017 Dimitre Lima (www.dmtr.org)

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


//#define NOXML



#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"




#if defined( TARGET_OF_IPHONE ) || defined( TARGET_OF_IOS ) || defined( TARGET_ANDROID )
	#define DMTRUI_TARGET_TOUCH
#endif

#include "ofxDmtrUIElements.h"

class ofxDmtrUI3 : public ofBaseApp
{
public:

	~ofxDmtrUI3();
	ofxDmtrUI3();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);

	void createFromText(string file, bool notify = true);
	void createFromLines(vector<string>lines);
	void createFromLines(string lines);
	void createFromLine(string line);

	void save(string xml);
	void load(string xml);
	
	void reload();

	vector <string> textToVector(string file);
	vector <element*> elements;
	//vector <element> newElements;

	// experimental 02 07 2017
	map <string, slider *> slidersMap;

	void onDraw(ofEventArgs &data);
	void onUpdate(ofEventArgs &data);

	void onMouseMoved(ofMouseEventArgs &data);

	void onMousePressed(ofMouseEventArgs &data);
	void onMouseDragged(ofMouseEventArgs &data);
	void onMouseReleased(ofMouseEventArgs &data);
	void onKeyPressed(ofKeyEventArgs &data);
	void onKeyReleased(ofKeyEventArgs &data);

//	void onMouseMoved(ofMouseEventArgs &data);
	void onExit(ofEventArgs &data);
	void onWindowResized(ofResizeEventArgs &data);

	void uiEventsString(string & e);
	void uiEvents(uiEv & e);

	uiConfig settings;

	ofFbo fboUI, fboUI2;
	ofShader shader;

	map <string,float>		pFloat;
	map <string,float>		pEasy;
	map <string,int>			pInt;
	map <string,bool>		pBool;
	map <string,string>		pString;
	map <string,ofPoint>		pPoint;
	map <string,ofColor>		pColor;

	void reFlow();
	void fboClear();


	// Straight from old ofxDmtrUI
	// eventualmente rever isto

	map <string, ofxDmtrUI3> uis;
	vector <ofxDmtrUI3 *> allUIs;

	ofxDmtrUI3 *_uiUnder = NULL;
	ofxDmtrUI3 *_uiRight = NULL;
	ofxDmtrUI3 *_uiLast = NULL;
	ofxDmtrUI3 *_uiFather = NULL;

	map <string, ofFbo> mapFbos;
	void createSoftwareFromText(string file);
	void loadPresetAll(int n, bool fromKey = false);
	void savePresetAll(int n);
	void setFbo(ofFbo &fbo);
	// pointer to save presets from
	ofFbo *_fbo;
	soft software;

	string UINAME = "";

	// remover no futuro
	//int minimumWidth = 100;
	bool keepSettings = false;

	void addUI(string nome, bool down = false, string valores = "");
	void nextTo(ofxDmtrUI3 & uiNext);
	void downTo(ofxDmtrUI3 & uiNext);
	void addRadio(string name, vector<string> options, string sel = "") {
		elements.push_back(new radio(name, settings, options));
		if (sel != "") {
			elements.back()->set(sel);
		}
	}
	void autoFit();
	void reFlowUis();

	string getPresetsPath(string ext="");
	void clear(bool keepVars = false);
	
	
	void allocateAndClearFbo(ofFbo &fbo);



	//--------------------------------------------------------------
	element * getElement(string n) {
		bool result = false;
		for (auto & e : elements) {
			if (e->name == n) {
				result = true;
				return e;
			}
		}
		if (!result) {
			return NULL;
		}
	}

	//--------------------------------------------------------------
	radio * getRadio(string n) {
		bool result = false;
		for (auto & e : elements) {
			if (e->name == n && e->kind == RADIO) {
				result = true;
				return (radio*)e;
			}
		}
		if (!result) {
			return NULL;
		}
	}

	void changeUI(string ui, string path) {
		if ( _uiFather->uis.find(ui) != _uiFather->uis.end() ) {
			_uiFather->uis[ui].clear();
			_uiFather->uis[ui].createFromText(path);
			_uiFather->uis[ui].autoFit();

			if (_uiFather->uis[ui]._uiUnder != NULL) {
				_uiFather->uis[ui]._uiUnder->autoFit();
			}

			//_uiFather->uis[ui].settings.redraw = true;
		} else {
			//"uis key not found";
		}
	}

	float easing = 5.0;

	void expires(int dataInicial, int dias = 10) {
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		cout << "-------- Dmtr Expires: " ;
		cout << rawtime << endl;
		int segundosPorDia = 86400;
		int segundosExpira = segundosPorDia * dias;
		float diasExpira = (segundosExpira - (difftime(rawtime,dataInicial))) / (float)segundosPorDia;

		cout << "--------- expira em " + ofToString(diasExpira) + " dias" << endl;
		if (diasExpira < 0 || diasExpira > dias) {
			ofSystemAlertDialog("Dmtr.org Software Expired ~ " + ofToString(dataInicial) + "\rhttp://dmtr.org/");
			std::exit(1);
		}
	}


	ofColor stringHexToColor(string corString) {
		int corInt = ofHexToInt(corString.substr(1));
		ofColor cor = ofColor::fromHex(ofHexToInt(corString.substr(1)));
		return cor;
	}

	string createdFromTextFile = "";

	void notify(string e);

	void redraw();

	// ofxDmtrUIMidiController
	vector <int> nextPreset;

	vector <future> futureCommands;
	void loadMaster();

	// created for ofxdmtrui3syphonin
	void createRadio(string name, vector<string> options, string sel = "");
	string getFileFullPath(string n);

	void showUI(int show);

	bool showInterface = true;

	void set(string e, bool b);
	void set(string e, int i);
	void set(string e, float f);
	void set(string e, string s);


	map <string, slider *> slidersLookup;
	map <string, toggle *> togglesLookup;
	map <string, radio  *> radiosLookup;
	
	void updateLookup();


	bool hasPresets = false;

	// talvez desencanar disso...
	int scale = 1;
	
	
	string loadedXmlFile = "";
	string loadedTextFile = "";


	map <string, vector<string> > templateUI;

	string buildingTemplate = "";
	
	void mouseUI(int x, int y, bool pressed = false);
	
	
	
	//17 agosto de 2017 - experimental
	map <string, vector <string> > templateVectorString;
	
	//22 de agosto de 2017 - experimental
	map <string, string> vars;
	
	
	void updateSoftwareRect() {
		software.rect = ofRectangle(0,0,0,0);
		software.rect = software.rect.getUnion(settings.rect);
		for (auto & u : uis) {
			software.rect = software.rect.getUnion(u.second.settings.rect);
		}
	}
	
	ofPoint velOffXUI = ofPoint(0,0);
	
	struct scrolling {
		ofPoint mouse;
		ofPoint vel;
		ofPoint dif;
	} scroll;
	
	string templatesString =
R"(#======
beginTemplate	audioBpmControls
bool	audioOuBpm	0
int	BPM	1 200 120
radio	ondaBeats	1 2 4 8
radio	onda	s w ww r
slider2d	freq
float	audioGanho	0 .5 0.25
float	audioOffset	-1 0 -.2
float	peakhold	0 20 2
float	decay	0 .98 .85
bool	invertAudio	0
endTemplate
#======
beginTemplate	audioControls
slider2d	freq
float	audioGanho	0.0 .5 0.25
float	audioOffset	-1 0 -.2
float	peakhold	0 20 2
float	decay	0 .98 .85
bool	invertAudio	0
endTemplate
#======
beginTemplate	colorLicht
bool	$UsaPaleta	0
tag	paleta
slider2d	$Paleta	.5 .5
tag	hsv
slider2d	$Hsv	.5 .5
tag
fbo	$PaletaAtual	200 10
float	$S	0 255 255
float	$HRange	0 720 100
float	$HRangeAudio	0 360 0
float	$BRange	0 512 0
float	$BStop	0 1 1
int	$HStep	0 6 0
float	$Alpha	0 255 255
float	$AlphaAudio	0 255 0
float	$AlphaRange	0 255 0
endTemplate
#======)";


	
	string uiTag = "";
	bool loadSave = true;
	bool loadPreset = true;
	bool savePreset = true;
};
