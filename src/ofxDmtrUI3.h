/**********************************************************************************

 Copyright (C) 2018 Dimitre Lima (dmtr.org)

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
//#include "ofxXmlSettings.h"


#if defined( TARGET_ANDROID ) || defined( TARGET_LINUX ) || defined( TARGET_LINUX64 ) || defined( TARGET_LINUX64 ) 
	#define DMTRUI_TARGET_NOARB
#endif

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
	void save2(string xml);
	void load2(string xml);

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

	void onTouchUp(ofTouchEventArgs & touch);
	void onTouchDown(ofTouchEventArgs & touch);
	void onTouchMoved(ofTouchEventArgs & touch);

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

	// novidade teste 10 de março de 2018
	map <string,ofImage>		pImage;

	void reFlow();
	void fboClear();

	// Straight from old ofxDmtrUI
	// eventualmente rever isto

	map <string, ofxDmtrUI3> uis;
	vector <ofxDmtrUI3 *> allUIs;

	ofxDmtrUI3 *_uiLast = NULL;
	ofxDmtrUI3 *_uiNext = NULL;
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

	void addUI(string nome, bool down = false, string valores = "");
//	void nextTo(ofxDmtrUI3 & uiNext);
//	void downTo(ofxDmtrUI3 & uiNext);

	void addRadio(string name, vector<string> options, string sel = "") {
		elements.push_back(new radio(name, settings, options));
		if (sel != "") {
			elements.back()->set(sel);
		}
	}
	
	void autoFit();
	void reFlowUis();
	void reFlowUiNeue();

	string getPresetsPath(string ext="");
	void clear(bool keepVars = false);
	
	void allocateAndClearFbo(ofFbo &fbo, ofPoint dimensions = ofPoint(0,0));

	/*
     LOOKUP SECTION
    */

	void set(string e, string s, bool notifyEvent = true);
	void set(string e, bool b, bool notifyEvent = true);
	void set(string e, int i, bool notifyEvent = true);
	void set(string e, float f, bool notifyEvent = true);
	void set(string e, ofPoint p, bool notifyEvent = true);
	
	map <string, slider *> slidersLookup;
	map <string, toggle *> togglesLookup;
	map <string, radio  *> radiosLookup;
	map <string, slider2d *> sliders2dLookup;
	map <string, bang *> bangsLookup;
	
	map <string, element *> elementsLookup;

	//--------------------------------------------------------------
	slider * getSlider(string n) {
		return slidersLookup.find(n) != slidersLookup.end() ? slidersLookup[n] : NULL;
	}
	
	//--------------------------------------------------------------
	radio * getRadio(string n) {
		return radiosLookup.find(n) != radiosLookup.end() ? radiosLookup[n] : NULL;
	}
	
	//--------------------------------------------------------------
	toggle * getToggle(string n) {
		return togglesLookup.find(n) != togglesLookup.end() ? togglesLookup[n] : NULL;
	}
	
	//--------------------------------------------------------------
	slider2d * getSlider2d(string n) {
		return sliders2dLookup.find(n) != sliders2dLookup.end() ? sliders2dLookup[n] : NULL;
	}

	//--------------------------------------------------------------
	bang * getBang(string n) {
		return bangsLookup.find(n) != bangsLookup.end() ? bangsLookup[n] : NULL;
	}
	
    //--------------------------------------------------------------
    element * getElement(string n) {
        return elementsLookup.find(n) != elementsLookup.end() ? elementsLookup[n] : NULL;
    }
    
    //--------------------------------------------------------------
    void updateLookup() {
        togglesLookup.clear();
        radiosLookup.clear();
        slidersLookup.clear();
        sliders2dLookup.clear();
        bangsLookup.clear();
        
        elementsLookup.clear();
        
        for (auto & e : elements) {
            if (e->kind == TOGGLE) {
                togglesLookup[e->name] = (toggle*)e;
            }
            else if (e->kind == RADIO) {
                radiosLookup[e->name] = (radio*)e;
            }
            else if (e->kind == SLIDER) {
                slidersLookup[e->name] = (slider*)e;
            }
            else if (e->kind == SLIDER2D) {
                sliders2dLookup[e->name] = (slider2d*)e;
            }
            else if (e->kind == BANG) {
                bangsLookup[e->name] = (bang*)e;
            }
            elementsLookup[e->name] = e;
        }
    }
    
    /*
     END LOOKUP SECTION
     */
    
    //--------------------------------------------------------------
	void changeUI(string ui, string path) {
		if ( _uiFather->uis.find(ui) != _uiFather->uis.end() ) {
			_uiFather->uis[ui].clear();
			_uiFather->uis[ui].createFromText(path);
		} else {
			//"uis key not found";
		}
	}
	
	void loadImageList(string name, string file) {
//		cout << "loadImagelist function" << endl;
//		cout << name << endl;
//		cout << file << endl;
//		cout << "-------" << endl;
		pImage[name].load(file);
	}

	float easing = 5.0;

	void expires(int dataInicial, int dias = 10) {
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		cout << "-------- Dmtr Expires: " ;
		cout << rawtime;
		int segundosPorDia = 86400;
		int segundosExpira = segundosPorDia * dias;
		float diasExpira = (segundosExpira - (difftime(rawtime,dataInicial))) / (float)segundosPorDia;

        cout << ": expira em " + ofToString(diasExpira) + " dias" << endl;
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
	void showUINeue(bool show);
	bool showInterface = true;
	bool hasPresets = false;

	// talvez desencanar disso...
	float scale = 1;
	
	
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

	string uiTag = "";
	
	loadSaveType loadMode = PRESETSFOLDER;
	loadSaveType saveMode = PRESETSFOLDER;

	
	bool visible = true;

	// somente serve pro midi controller, pro push and hold
	int lastPreset = -1;
	
	
	bool notifyEventOnLoad = true;

	// so tem parametro pq to usando o invokeBool
	void saveMaster(bool save=true);

	string fixedLabel = "";
	
	vector < std::function<void()> > presetupFunctions;
	vector < std::function<void()> > setupFunctions;
	vector < std::function<void()> > updateFunctions;
	vector < std::function<void()> > drawFunctions;
	vector < std::function<void(uiEv &)> > uiEventFunctions;
	
//	void addUpdateFunction(std::function<void()> & func) {
//		updateFunctions.push_back(std::bind(func, this));
//	}
	//std::function<void(string,string)> updateUI = NULL

	void saveActualPreset() {
//		cout << "saveActualPreset" << endl;
		int slot = ofToInt(getElement("allPresets")->getValString());
		savePresetAll(slot);
	}
	
	void mouseRelease();
	

	bool isDown = false;
	ofFbo::Settings fboSettings;


	void messageBox(string s) {
		vector <string> linhas = ofSplitString(s, "\r");
		int size = 0;
		for (auto & l : linhas) {
			size = MAX(size, l.size());
		}
		// cout << "messagebox :: " << s << endl;
		// cout << "size = " << size << endl;
		for (int a=0; a<size+4; a++) {
			cout << "-" ;
		}
		cout << endl;

		for (auto & l : linhas) {
			string spaces = "";
			int difSize = (size - l.size());
			//cout << difSize << endl;
			if (difSize) {
				for (int a=0; a<difSize; a++) {
					spaces += " ";
				}
			}
			cout << "| " << l << spaces << " |" << endl;
		}
		for (int a=0; a<size+4; a++) {
			cout << "-" ;
		}
		cout << endl;
	}

};
