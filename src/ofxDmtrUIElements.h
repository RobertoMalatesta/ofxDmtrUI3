/**********************************************************************************

 Copyright (C) 2018 Dimitre Lima (www.dmtr.org)

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




// naming conflicts? namespace?
enum elementType {
	SLIDER, LABEL, TOGGLE, BANG, RADIO, RADIOITEM, SLIDER2D, PRESET, PRESETS, COLOR, COLORITEM, IMAGE
};

// naming conflicts? namespace?
enum dmtrUIVarType {
	DMTRUI_FLOAT, DMTRUI_INT, DMTRUI_STRING, DMTRUI_BOOLEAN, DMTRUI_POINT
};

// teste ainda podemos remover. 18 de novembro de 2017
enum loadSaveType {
	NONE, PRESETSFOLDER, MASTER
};




struct customColor {
	ofColor label;
	ofColor slider;
	ofColor activeSlider;
	ofColor column;
	
	customColor() {}
	customColor(ofColor l, ofColor s, ofColor a, ofColor c) : label(l), slider(s), activeSlider(a), column(c) {}
};




struct soft {
public:

// 16 oct 2017, areia
	bool eventOnClick = true;

	bool visible = true;
	float opacity = 180;
	ofPoint presetDimensions = ofPoint(72,48);
	string presetsFolder = "_presets/";
	int presetLoaded = -1;
	int multiSampling = 0;
	int w = 0;
	int h = 0;
	
	// porque isso ta aqui no software?
	ofPoint sliderDimensions = ofPoint(220, 20);

	ofPoint offset = ofPoint(0,0);
	soft() {}
	
	//new, 25 08 2017, frejat
	ofRectangle rect;
	bool autoScrolling = true;
	
	bool customFont = false;
	ofTrueTypeFont font;
	
	bool needsRedraw = false;
	
	//11 de setembro de 2017, RIR
	string uiTag = "";
	
	
	bool customColors = false;
	customColor colors;
};



struct future {
public:
	string uiname;
	string name;
	string action;
	int valInt;
	future (string u, string n, string a, int v) : uiname(u), name(n), action(a), valInt(v) {}
};


// rename to a more significant name?
struct uiEv {
public:
	string name;
	// STILL TO BE COMPLETED
	string uiname;
	elementType kind;
	dmtrUIVarType varType;

	bool b;
	int i;
	float f;
	ofPoint p;
	string s;
	
	bool isDir = false;
	bool uiGlobal = false;
	bool onClick = true;
	string tag;

	// doens't work, element is declared afterwards
//	element * e;
//	uiEv(element & ee) { e = ee };
	
	uiEv(string n) : name(n) { uiGlobal = true; }

	uiEv(string n, string u, elementType k, dmtrUIVarType t, string ta) 				: name(n), uiname(u), kind(k), varType(t), tag(ta) {}
	uiEv(string n, string u, elementType k, dmtrUIVarType t, string ta, float ff)		: name(n), uiname(u), kind(k), varType(t), tag(ta), f(ff) {}
	uiEv(string n, string u, elementType k, dmtrUIVarType t, string ta, int ii) 		: name(n), uiname(u), kind(k), varType(t), tag(ta), i(ii) {}
	uiEv(string n, string u, elementType k, dmtrUIVarType t, string ta, bool bb)		: name(n), uiname(u), kind(k), varType(t), tag(ta), b(bb) {}
	uiEv(string n, string u, elementType k, dmtrUIVarType t, string ta, ofPoint pp) 	: name(n), uiname(u), kind(k), varType(t), tag(ta), p(pp) {}
	uiEv(string n, string u, elementType k, dmtrUIVarType t, string ta, string ss)		: name(n), uiname(u), kind(k), varType(t), tag(ta), s(ss) {}
};


struct uiConfig {
public:
	
	
	bool notifyEventWhenSetDoesntChange = true;

	string uiname;
	string tag;
	
	//	ofColor bgColor = ofColor(120, 220);
	//ofColor bgColor = ofColor(80, 200);
	
	
	//ofColor bgColor = ofColor(80, 140);
	ofColor bgColor = ofColor(80, 220);
//	ofColor bgColor = ofColor(0, 140);
	
	
	ofColor activeColor = ofColor(0,0,0,190);
//	ofColor activeColor = ofColor(0,255);
	
	ofColor bwElementColor = ofColor(120, 220); //127
//	ofColor bwElementColor = ofColor(90, 255); //127

	//senao fazer pointer to function aqui...
	//std::function<void(string,string)> updateUI = NULL;

	//void (*updateUI)(string,string) = NULL;
	//map <string, string> radioUIMap;

	int minimumWidth = 200;

	ofPoint sliderDimensions; // = ofPoint(200, 20);
	ofPoint margin = ofPoint(10,10);
	float opacity = 200;
	ofPoint flow = margin;
	ofPoint flowBak;
	float colx;
	int spacing = 5;
	int spacingChildren = 1;


	ofRectangle rect;
	int nPresets = 21;
	// ponteiro pro addUI geral.
	soft * software = NULL;
	
	void setSoftware(soft & s) {
		//cout << "setSoftware " << endl;
		software = &s;
		sliderDimensions = software->sliderDimensions;
		//cout << sliderDimensions << endl;
	}

	void setSliderWidth(int w) {
		//cout << "setSliderWidth " << w <<  " uiname:" << uiname << endl;
		sliderDimensions.x = w;
		minimumWidth = MAX(minimumWidth, sliderDimensions.x);
	}

	string getPresetsPath(string ext) {
		if (software != NULL) {
			return software->presetsFolder + ext;
		}
	}


	// mouse flows free from one item to another.
	bool flowFree = true;
	bool flowVert = true;

	// legacy
	//ofEvent<string> uiEventString;

	// complete
	ofEvent<uiEv> uiEvent;
	ofEvent<string> uiGeneralEvent;

	// 5 agosto 2017 pra testar a uiremote
	void notifyString(string s) {
		//ofNotifyEvent(settings->uiGeneralEvent, s);
	}

	bool needsRedraw = false;
	bool redraw = false;

	map <string, float>		* pFloat;
	map <string, int>		* pInt;
	map <string, bool>		* pBool;
	map <string, string> 	* pString;
	map <string, ofPoint>	* pPoint;
	map <string, ofColor>	* pColor;

	float hueStart = 60;
	float hue;
	int hueStep = 4;
	ofColor color;
	bool bw = false;
	//float flowxbak = -100;


	bool obeyColumn = true;


	void updateColor() {
		hue = int(hue + hueStep)%255;

	}

//	void updateColorValue() {
//		color = ofColor::fromHsb(hue, 155, 210, 255);
//		if (bw) {
//			color = bwElementColor;
//		}
//	}

	ofColor getColor() {
		color = ofColor::fromHsb(hue, 155, 210, 230);
		if (bw) {
			color = bwElementColor;
		}
		
		if (software->customColors) {
			color =  software->colors.slider;
		}
		
		return color;
	}

	uiConfig() {
		reset();
		updateColor();
 	}

	void reset() {
		flow = flowBak = margin;
		// starthue no futuro
		hue = hueStart;
		needsRedraw = true;
		colx = margin.x;
	}

	int getSpacing() {
		return useMarginChildren ? spacingChildren : spacing;
	}

	bool useMarginChildren = false;
	// only to make children items of radio.
	void setMarginChildren(bool m) {
		useMarginChildren = m;
		hueStep = m ? 3 : 6;
	}

	void update(ofRectangle & r) {
		if (flowVert) {
			flow.y += r.height + getSpacing();
		} else {
			if ((flow.x + r.width) > (colx + sliderDimensions.x) && obeyColumn) {
				flow.x = colx;
				flow.y += r.height + getSpacing();
			} else {
				flow.x += r.width + getSpacing();
			}
		}
		updateColor();
		//rect.growToInclude(r.getBottomRight());
	}

	void newLine() {
		flow.y += sliderDimensions.y + getSpacing();
	}
	void newCol() {
		colx +=  sliderDimensions.x + margin.x;
		//flow.x += sliderDimensions.x + margin.x;
		flow.x = colx;
		flow.y =  margin.y;
	}

	void setFlowVert(bool f) {
		flowVert = f;
		if (f) {
			flow.x = colx;

			// consertar aqui pra isVert
		}
	}
};




class element {
protected:
	//ofColor activeRectColor = ofColor(0, 90);
	ofPoint labelPos;

public:
	ofColor labelColor = ofColor(255);
	bool isVert = false;
	uiConfig * settings = NULL;
	bool saveXml = true;
	bool showLabel = true;
	ofColor color;

	bool alwaysRedraw = false;
	dmtrUIVarType varType;

	std::function<void(bool)> invokeBool = NULL;
	// invokeFloat, invokeInt, invokeString

	// 24 june 2017 - webcams
	int selectedId = -1;
	
	// 27 aug 2017
	string tag = "";


	// deixar somente pro que tem fbo
	virtual void setFbo (ofFbo &fbo) {}
	virtual void setFolder(string s) {};

	bool useLabelShadow = true;
	// medida provisoria ate resolver o radioitem
	string valString = "";


	// passei pra publico pra melhorar pro radio
	ofRectangle rect, activeRect, boundsRect;

	// 21 de junho, usando no radioitem
	element * _parent = NULL;
	elementType kind = SLIDER;
	bool isPressed = false;
	bool redraw = false;
	string name;
	string label = "";

	// 19 june 2017
	bool firstClicked = false;

	// pode ser temporario
	bool isDir = false;

	void notify() {
		// fires any kind of void on any change
//		if ((*invoke) != NULL) {
//			(*invoke)();
//		}

		if (varType == DMTRUI_STRING) {
			uiEv e = uiEv(name, settings->uiname, kind, varType, tag, getValString());
			e.isDir = isDir;
			e.onClick = settings->software->eventOnClick;
			e.uiname = settings->uiname;
			ofNotifyEvent(settings->uiEvent, e);
		}
		else if (varType == DMTRUI_BOOLEAN) {
			uiEv e = uiEv(name, settings->uiname, kind, varType, tag, getValBool());
			e.onClick = settings->software->eventOnClick;
			e.uiname = settings->uiname;
			ofNotifyEvent(settings->uiEvent, e);
		}
		else if (varType == DMTRUI_INT) {
			uiEv e = uiEv(name, settings->uiname, kind, varType, tag, (int)getVal());
			e.onClick = settings->software->eventOnClick;
			e.uiname = settings->uiname;
			ofNotifyEvent(settings->uiEvent, e);
		}
		else if (varType == DMTRUI_POINT) {
			uiEv e = uiEv(name, settings->uiname, kind, varType, tag, getValPoint());
			e.onClick = settings->software->eventOnClick;
			e.uiname = settings->uiname;
			ofNotifyEvent(settings->uiEvent, e);
		}
		else if (varType == DMTRUI_FLOAT)  {
			uiEv e = uiEv(name, settings->uiname, kind, varType, tag, getVal());
			e.onClick = settings->software->eventOnClick;
			e.uiname = settings->uiname;
			ofNotifyEvent(settings->uiEvent, e);
		}
	}

	void needsRedraw(bool need = true) {
		redraw = need;
		settings->needsRedraw = need;

		if (_parent != NULL) {
			_parent->needsRedraw();
		}
	}

	void getProperties() {
		//cout << "getProperties " << name << endl;
		
		if (settings->software->customColors) {
			labelColor = settings->software->colors.label;
			color = settings->software->colors.slider;
		}
		
		
		tag = settings->tag;
		
		int x = settings->flow.x;
		int y = settings->flow.y;
		color = settings->getColor();

		if (kind != LABEL) {
			rect.x = x;
			rect.y = y;
			rect.width = (kind == TOGGLE || kind == BANG) ? settings->sliderDimensions.y : settings->sliderDimensions.x;
			rect.height = settings->sliderDimensions.y;
			activeRect.x = x;
			activeRect.y = y;
			activeRect.width = settings->sliderDimensions.x;
			activeRect.height = settings->sliderDimensions.y;
		}

		// never draw.
		boundsRect.x = x;
		boundsRect.y = y;
		boundsRect.width = settings->sliderDimensions.x;
		boundsRect.height = settings->sliderDimensions.y;
		
		if (isVert) {
			rect.width = 		settings->sliderDimensions.y;
			rect.height = 		settings->sliderDimensions.x;
			activeRect.width = 	settings->sliderDimensions.y;
			activeRect.height = settings->sliderDimensions.x;
			boundsRect.width = 	settings->sliderDimensions.y;
			boundsRect.height = settings->sliderDimensions.x;
			showLabel = false;
		}

		// generic
		labelPos.x = x + 5;
//		labelPos.y = y + 16;
		labelPos.y = y + settings->sliderDimensions.y / 2 + 7;


		// temporario
		if (kind == PRESETS) {
			rect.width = activeRect.width = 0;
		}

		if (kind == PRESET) {
			boundsRect.width = settings->software->presetDimensions.x;
			boundsRect.height = settings->software->presetDimensions.y;
			activeRect = rect = boundsRect;
			color = ofColor(0,255);
		}

		else if (kind == SLIDER2D) {
			boundsRect.height = settings->sliderDimensions.y * 2 + settings->getSpacing();
			rect.height = boundsRect.height;
			activeRect.width = 0;
			activeRect.height = 0;
		}

		else if (kind == LABEL) {
			labelPos.x = x;
		}

		else if (kind == RADIO) {
			labelPos.x = x;
			rect.width = 0;
			rect.height = 0;
		}

		else if (kind == SLIDER) {
			varType = DMTRUI_FLOAT;
		}

		else if (kind == TOGGLE || kind == BANG ) {
			//labelPos.x = x + 25;
			int retanguloMaisOffset = settings->sliderDimensions.y * 1.3;
			labelPos.x = x + retanguloMaisOffset;

			float margem = settings->sliderDimensions.y/5;
			activeRect = ofRectangle(
				rect.x + margem, rect.y + margem,
				rect.width-margem*2, rect.height-margem*2
			);

			if (!showLabel) {
				boundsRect.width = boundsRect.height;
			} else {

				
				if (settings->software->customFont) {
					ofRectangle r = settings->software->font.getStringBoundingBox(name, 0, 0);
					int largura = r.width + margem*2;
					boundsRect.width = retanguloMaisOffset + largura;
				} else {
					int contaletras = 0;
					for(auto c: ofUTF8Iterator(name)){
						contaletras++;
					}
					boundsRect.width = retanguloMaisOffset + margem*2 + contaletras * 8;
				}
			}
		}

		if (kind == RADIOITEM || kind == COLORITEM || kind == PRESET) {
			float margem = 4;
			labelPos.x = x + margem;

			if (kind != PRESET)
			{

				
				// CUSTOM FONT
				if (settings->software->customFont) {
					//cout << name << endl;
					ofRectangle r = settings->software->font.getStringBoundingBox(name, 0, 0);
					int largura = r.width + margem * 2;
					boundsRect.width = rect.width = largura;
					activeRect = rect;

				} else {
					int contaletras = 0;
					for(auto c: ofUTF8Iterator(name)){
						contaletras++;
					}
					boundsRect.width = margem*2 + contaletras * 8;
					rect.width = boundsRect.width;
					activeRect = rect;
				}
			}

			// ugly solution to overflow but working
			if ((settings->flow.x + boundsRect.width) >
				(settings->colx + settings->sliderDimensions.x)) {
				settings->flow.x = settings->colx;
				settings->flow.y += boundsRect.height + 1;

				// TODO :: avoid infinite loop to long filenames
				// 20525524_10154923017472183_4837044839922028887_n.jpg

				getProperties();
				//cout << "ugly stuff :: " + name << endl;
			} else {
				//cout << "updateflow :: " + name << endl;
				//cout << name << endl;
				
				updateFlow();
			}

		}

		if (kind != PRESETS && kind != PRESET &&
			kind != RADIOITEM && kind != RADIO &&
			kind != COLOR && kind != COLORITEM) {
			updateFlow();
		}

		// pra todos que precisam disso na inicializacao
		// posteriormente remover pro toggle laembaixo;
		needsRedraw();
	}

	void updateFlow() {
		settings->update(boundsRect);
	}

	void addSettings (uiConfig & u) {
		settings = &u;
	}

	element() {}
	~element() {}


	void setColor(ofColor c) {
		color = c;
	}

	virtual void updateFromPixels(ofPixels & p) {}
	virtual void updateImage() {}

	virtual void drawLabel() {
		string n = name + label;
		
		if (settings->software->customFont) {
			if (useLabelShadow) {
				ofSetColor(0,130);
				settings->software->font.drawString(n, labelPos.x - 1, labelPos.y + 1 - 3);
			}
			
			//decidir se remover o labelcolor totalmente
			ofSetColor(labelColor);
			//ofSetColor(255);
			settings->software->font.drawString(n, labelPos.x , labelPos.y - 3);
			
		} else {
			if (useLabelShadow) {
				ofSetColor(0,180);
				ofDrawBitmapString(n, labelPos.x - 1, labelPos.y + 1);
			}

			//decidir se remover o labelcolor totalmente
			ofSetColor(labelColor);
			//ofSetColor(255);
			ofDrawBitmapString(n, labelPos.x, labelPos.y);
		}
	}



	virtual void set(bool i, bool notifyEvent = true) {
		cout << "set function on primitive element, using bool " + name << endl;
	}

	virtual void set(float i, bool notifyEvent = true) {
		cout << "set function on primitive element, using float " + name << endl;
	}

	virtual void set(string i, bool notifyEvent = true) {
		cout << "set function on primitive element, using string " + name << endl;
	}

	virtual void set(ofPoint i, bool notifyEvent = true) {
		cout << "set function on primitive element, using ofPoint " + name << endl;
	}
	
	// novidade 7 de agosto
	virtual void set(int i, bool notifyEvent = true) {
		cout << "set function on primitive element, using int " + name << endl;
	}


	virtual float getVal() { cout << "never to be used getval float :: " + name << endl; return 1; } //return 1;
	virtual bool getValBool() { cout << "never to be used getvalbool :: " + name << endl; return true; }
	virtual string getValString() { cout << "never to be used getValString :: " + name << endl; return ""; }
	virtual ofPoint getValPoint() { cout << "never to be used getvalpoint:: " + name << endl; return ofPoint(0,0); }

	virtual void draw() {

		ofPushStyle();
		ofPushMatrix();
//		ofSetColor(settings->bgColor);
		ofSetColor(0, 255);
		ofDrawRectangle(rect);
		ofSetColor(color);
		ofDrawRectangle(rect);
		drawSpecific();

		if (showLabel) {
			drawLabel();
		}
//		ofSetColor(255,0,0, 50);
//		ofDrawRectangle(boundsRect);
		
		ofPopMatrix();
		ofPopStyle();
	}

	// temporario somente pra resolver
	bool dragging = false;

	virtual void drawSpecific() {}

	virtual void setValFromMouse(int x, int y) {}

	virtual void setRadioVal(element * e) {}

	virtual void checkMouseNeu(int x, int y, bool first = false) {
		if (first) {
			dragging = false;
			if (boundsRect.inside(x,y)) {
				firstClicked = true;
			}
		} else {
			dragging = true;
		}

		if (firstClicked || settings->flowFree)
		{
			if (boundsRect.inside(x,y)) {
				setValFromMouse(x,y);
				isPressed = true;
			} else {
				if (isPressed) {
					setValFromMouse(x,y);
					draw();
					isPressed = false;
				}
			}
		}
	}

	void restoreVal() {
		if (varType == DMTRUI_FLOAT) {
			set((*settings->pFloat)[name]);
		}
		else if (varType == DMTRUI_INT) {
			// ainda igual ao int. vamos ver o que acontece.
			set(float((*settings->pInt)[name]));
		}
		else if (varType == DMTRUI_STRING) {
			set((*settings->pString)[name]);
		}
		else if (varType == DMTRUI_POINT) {
			set((*settings->pPoint)[name]);
		}
//		else if (varType == COLOR) {
//			set((*settings->pColor)[name]);
//		}

	}
};



class slider2d : public element {
private:
	ofPoint min = ofPoint(0,0);
	ofPoint max = ofPoint(1,1);
public:
	ofPoint val = ofPoint(0.5, 0.5);
	ofPoint lastVal;

	ofFbo * _fbo = NULL;
	bool fboSet = false;

	void setFbo(ofFbo &fbo) {
		_fbo = &fbo;
		fboSet = true;
		//alwaysRedraw = true;
		//activeRect.width = 0;
		//rect.width = 0;
	};

	slider2d(string n, uiConfig & u, ofPoint mi = ofPoint(0,0), ofPoint ma = ofPoint(1,1), ofPoint v = ofPoint(.5,.5))
	: min(mi), max(ma) {
		kind = SLIDER2D;
		varType = DMTRUI_POINT;
		settings = &u;
		name = n;
		getProperties();
		set(v);
		showLabel = false;
	}

	void set(ofPoint v, bool notifyEvent = true) {
		val = v;
		if (lastVal != val || settings->notifyEventWhenSetDoesntChange) {
			(*settings->pPoint)[name] = val;
			lastVal = val;
			needsRedraw();
			label = " " + ofToString(val.x) + ":" + ofToString(val.y);
			if (notifyEvent) {
				notify();
			}
		}
	}

	ofPoint getValPoint() {
		return val;
	}

	// slider2d
	void drawSpecific() {
		// tem como melhorar performance? acho q tem.
		ofSetColor(255);
		if (_fbo != NULL) {
//			_fbo->draw(rect.x + settings->rect.x , rect.y + settings->rect.y);
			
//			cout << "drawing fbo " + name << endl;
//			cout << rect << endl;
			_fbo->draw(rect.x, rect.y);
		} else {
			float x = rect.x + val.x * rect.width;
			float y = rect.y + val.y * rect.height;
			ofDrawLine(x, rect.y, x, rect.y + rect.height);
			ofDrawLine(rect.x, y, rect.x + rect.width, y);
		}
	}

	void setValFromMouse(int x, int y) {
		int xx = ofClamp(x, rect.x, rect.x + rect.width);
		int yy = ofClamp(y, rect.y, rect.y + rect.height);
		ofPoint xy = ofPoint (xx,yy) - ofPoint(rect.x, rect.y);
		ofPoint wh = ofPoint (rect.width, rect.height);
		ofPoint val = min + (max-min)*(xy/wh);
		set(val);
	}
};


class plotter : public slider2d {
public:
	vector <float> vars;

	plotter(string n, uiConfig & u) : slider2d(n, u) {
		alwaysRedraw = true;
	}
	
	void update(float v) {
		vars.push_back(v);
		if (vars.size() > rect.width) {
			vars.erase(vars.begin());
		}
	}
	
	void drawSpecific() {
		//ofClear(0,255);
		ofSetColor(0,255);
		ofDrawRectangle(rect);
		ofSetColor(255);
		ofNoFill();
		
//		glBegin(GL_POINTS);
		ofBeginShape();
		int x=0;
		for (auto & v : vars) {
			float y = rect.height - v*rect.height;
//			glVertex2f(rect.x + x,  rect.y + y);
			ofVertex(rect.x + x,  rect.y + y);
			x++;
		}
		ofEndShape();
//		glEnd();
	}
};




class slider : public element {
private:
	bool isInt;
	
public:
	float min = 0;
	float max = 1;
	float val = .5;

	// temporary, just to fire set event on initialization
	float lastVal = -12349871234;

	slider(string n, uiConfig & u, float mi, float ma, float v, bool i=false, bool vert=false) : min(mi), max(ma), isInt(i) {
		settings = &u;
		isVert = vert;
		name = n;
		getProperties();
		set(v);
		varType = isInt ? DMTRUI_INT : DMTRUI_FLOAT;
	}

	// slider
	
	void internalSet(float v, bool notifyEvent = true) {
		val = v;
		if (lastVal != val || settings->notifyEventWhenSetDoesntChange) {
			if (isInt) {
				val = int(val);
				(*settings->pInt)[name] = val;
			} else {
				(*settings->pFloat)[name] = val;
			}
			if (isVert) {
				float yy = ofMap(val, max, min, rect.y, rect.y + rect.height);
				activeRect.height = rect.height - (yy - rect.y);
				activeRect.y = yy;
			} else {
				activeRect.width = rect.width * ((val-min) / (max-min));
			}
			lastVal = val;
			needsRedraw();
			label = " " + ofToString(val);
			if (notifyEvent) {
				notify();
			}
		}

	}
	
	void set(float v, bool notifyEvent = true) {
		internalSet(v, notifyEvent);
	}
	void set(int v, bool notifyEvent = true) {
		internalSet(v, notifyEvent);
	}

	float getVal() {
		return val;
	}

	// slider
	void drawSpecific() {
		ofSetColor(settings->activeColor);
		ofDrawRectangle(activeRect);
	}

	void setValFromMouse(int x, int y) {
		int xx;
		if (isVert) {
			xx = ofClamp(y, rect.y, rect.y + rect.height);
			activeRect.height = rect.height - (xx - rect.y);
			activeRect.y = xx;
			if (isInt) {
				int valInt = min + ((max+1)-min)*(float(xx-rect.y)/(float)rect.height);
				valInt = ofClamp(valInt, min, max);
				set(valInt);
			} else {
				float valFloat = ofMap(xx, rect.y, rect.y+rect.height, max, min);
				//float valFloat = min + (max-min)*(float(xx-rect.y)/(float)rect.height);
				set(valFloat);
			}
		} else {
			xx = ofClamp(x, rect.x, rect.x + rect.width);
			//this one is not needed, repeat on function set
			activeRect.width = xx - rect.x;
			
			if (isInt) {
				int valInt = min + ((max+1)-min)*(float(xx-rect.x)/(float)rect.width);
				valInt = ofClamp(valInt, min, max);
				set(valInt);
			} else {
				float valFloat = min + (max-min)*(float(xx-rect.x)/(float)rect.width);
				set(valFloat);
			}
		}

	}
};



class label : public element {
public:
	label (string n, uiConfig & u) {
		kind = LABEL;
		settings = &u;
		name = n;
		getProperties();
		saveXml = false;
	}
};


// TYPE TO HANDLE BOTH RADIOITEM AND TOGGLE
class booleano : public element {
public:
	bool val = false;

	bool getValBool() {
		return val;
	}

	void set(bool v, bool notifyEvent = true) {
		if (val != v || settings->notifyEventWhenSetDoesntChange) {
			val = v;
			(*settings->pBool)[name] = val;
			needsRedraw();
			if (notifyEvent) {
				if (kind == BANG) {
					if (val) {
						notify();
						if (invokeBool != NULL) {
							invokeBool(val);
						}
					}
                    //val = false;
				} else {
					notify();
					if (invokeBool != NULL) {
						invokeBool(val);
					}
				}
			}
		}
	}

	void setValFromMouse(int x, int y) {
		if (!isPressed) {
			set(val ^ 1);
			isPressed = true;
		}
	}

	float getVal() {
		return val;
	}
};


class toggle : public booleano {
public:

	toggle(string n, uiConfig & u, bool v, bool l = true)  {
		kind = TOGGLE;
		varType = DMTRUI_BOOLEAN;
		settings = &u;
		name = n;
		showLabel = l;
		getProperties();
		set(v);
		needsRedraw();
	}

	void drawSpecific() {
		if (val) {
			ofSetColor(settings->activeColor);
			ofDrawRectangle(activeRect);
		}
	}
};


class bang : public booleano {
public:
	bang(string n, uiConfig & u, bool l = true)  {
		kind = BANG;
		varType = DMTRUI_BOOLEAN;
		settings = &u;
		name = n;
		showLabel = l;
		getProperties();
		//set(v);
		needsRedraw();
		saveXml = false;
	}

	void drawSpecific() {
		if (val) {
			ofSetColor(settings->activeColor);
			ofDrawRectangle(activeRect);
            
            val = false;
            needsRedraw();
		}
	}
};


class radioitem : public booleano {
public:
	radioitem (string n, uiConfig & u) {
		kind = RADIOITEM;
		//varType = BOOLEAN;
		settings = &u;
		name = n;
		getProperties();
	}

	void drawSpecific() {
		if (val) {
			ofSetColor(settings->activeColor);
			ofDrawRectangle(activeRect);
		}
	}
};



class coloritem : public booleano {
public:
	
	coloritem (string n, uiConfig & u) {
		kind = COLORITEM;
		settings = &u;
		name = n;

		getProperties();
		nameToColor();
	}

	void nameToColor() {
		istringstream stream("0x" + name.substr(1));
		stream.unsetf(ios_base::dec);
		int corInt;
		stream >> corInt;
		ofColor corzinha = ofColor::fromHex(corInt);
		ofFill();
		ofSetColor(corzinha);
		color = corzinha;
	}

	void drawSpecific() {
		if (val) {
			ofSetColor(settings->activeColor);
			ofDrawRectangle(activeRect);
		}
	}
};



// primitive class to radio and presets, classes that has children elements
class mult : public element {
public:

	// adicionado 8 de junho de 2018, teste pra Mareh Marisco
	bool isFloat = false;

	
	vector <element*> elements;

	vector <string> items;
	string lastVal;

	bool eventWhenSameSelectedIndex = false;

	ofPoint flowBak;
	string folder;

	std::function<void(string, string)> changeUI = NULL;
	std::function<void(string, string)> loadImageList = NULL;

	
	
	void startChildren() {
		flowBak = settings->flow;
		if (showLabel) {
			settings->newLine();
		}
		settings->setFlowVert(false);
		settings->setMarginChildren(true);
	}

	void endChildren() {
		settings->setMarginChildren(false);
		settings->setFlowVert(true);
		settings->flow = flowBak;
	}


	void setFolder(string s) {
		folder = s;
	}

	void drawSpecific() {
		for (auto & e : elements) {
			e->draw();
		}

//		ofSetColor(255,160);
//		ofDrawRectangle(boundsRect);
	}

	void setValFromMouse(int x, int y) {
		for (auto & e : elements) {
			if (e->boundsRect.inside(x,y)) {
				set(e->name);
			}
		}
	}

	string getValString() {
		return valString;
	}

//	void getVal() {
//		return (float)selectedIndex;
//	}
	
	
	// mult (radio and presets)
	void set(string s, bool notifyEvent = true) {
		int index = 0;
		bool updated = false;

		for (auto & e : elements) {
			if (e->name == s) {
				if (valString != e->name || (eventWhenSameSelectedIndex && !dragging))
				{
					selectedId = index;

					updated = true;
					valString = e->name;
					(*settings->pString)[name] = valString;
					
					if (isFloat) {
						(*settings->pFloat)[name] = ofToFloat(valString);
					}
					
					if (e->kind == COLOR) {
						(*settings->pColor)[name] = e->color;
					}

					e->set(true);
					needsRedraw();
					if (notifyEvent) {
						notify();
					}

					if (changeUI != NULL) {
						string f = folder + "/" + valString + ".txt";
						string uiname = "ui" + name;
						changeUI(uiname, f);
					}
					
					if (loadImageList != NULL) {
						string f = folder + "/" + valString;
						loadImageList(name, f);
					}

					// invoke pointer to functions (string and id)
				}
			} else {
				if (e->getVal())
				{
					e->set(false);
				}
			}
			index++;
		}
		if (!updated) {
			valString = (*settings->pString)[name] = s;
			needsRedraw();
			if (notifyEvent) {
				notify();
			}
		}
	}

	string getNameFromIndex(int index) {
		int i = 0;
		string name = "";
		for (auto & e : elements) {
			if ((e->kind == RADIOITEM) && i == index) {
				name = e->name;
			}
			i++;
		}
		if (name != "") {
			return name;
		}
	}

	int getIndex(string s) {
		int i = 0;
		int index = -1;
		for (auto & e : elements) {
			if (e->name == s) {
				index = i;
			}
			i++;
		}
		if (index != -1) {
			return index;
		}
	}

	void setFromIndex(int i, bool notifyEvent = true) {
		int index = 0;
		string elname;
		for (auto & e : elements) {
			if (index == i) {
				elname = e->name;
			}
			index++;
		}
		if (elname != "") {
			set(elname, true);
		}
	}


	string getFullFileName() {
		return folder + "/" + valString;
	}
};


class radio : public mult {
public:
	
	// teste 12 agosto 2017
	//radio () = default;
	//radio ();
	
	// teste 8 de marco de 2018 // babel
	bool isImage = false;
	
	radio (string n, uiConfig & u, vector <string> its, bool showThisLabel = true) {
		showLabel = showThisLabel;
		kind = RADIO;
		varType = DMTRUI_STRING;
		settings = &u;
		name = n;
		items = its;
		getProperties();

		startChildren();
		for (auto & i : items) {
			elements.push_back(new radioitem(i, u));
			elements.back()->_parent = this;
			boundsRect.growToInclude(elements.back()->boundsRect.getBottomRight());
		}
		endChildren();
		updateFlow();
	}

};



class preset : public booleano {
public:
	ofImage img;
	ofFbo fbo;
	ofPoint dimensions; // = ofPoint(72, 48);

	void imageOrVoid() {
		string file = settings->getPresetsPath(name + ".tif");
		fbo.begin();
		ofClear(30,255);
		if (ofFile::doesFileExist(file)) {
			img.load(file);
			ofSetColor(255);
			img.draw(0,0);
		} else {

			ofTranslate(fbo.getWidth()/2, fbo.getHeight()/2);
			//ofSetColor(255,0,40);
			//int n = 12;
			ofSetColor(180);
			int n = 5;
			ofDrawLine(-n, n, n, -n);
			ofDrawLine(-n, -n, n, n);
		}
		fbo.end();
	}

	preset (string n, uiConfig & u) {
		kind = PRESET;
		varType = DMTRUI_BOOLEAN;

		settings = &u;
		name = n;

		dimensions = settings->software->presetDimensions;
		getProperties();
		fbo.allocate(dimensions.x, dimensions.y, GL_RGBA);

		imageOrVoid();
	}

	void updateFromPixels(ofPixels & pixels) {
		img.setFromPixels(pixels);
		fbo.begin();
		ofClear(30,255);
		ofSetColor(255);
		img.draw(0,0);
		fbo.end();
		needsRedraw();
		if (_parent != NULL) {
			_parent->needsRedraw();
		}
	}

	void updateImage() {
		imageOrVoid();

		needsRedraw();
		if (_parent != NULL) {
			_parent->needsRedraw();
		}
	}

	// preset
	void drawSpecific() {
		ofSetColor(255);
		fbo.draw(rect.x, rect.y);
		if (val) {
			ofSetColor(settings->activeColor);
			ofDrawRectangle(activeRect);

			ofSetColor(255,0,70);
			ofDrawRectangle(activeRect.x + 10, activeRect.y + 10, 20, 20);
		}
	}
};



class presets : public mult {
public:
	presets(string n, uiConfig & u) {
		showLabel = false;
		eventWhenSameSelectedIndex = true;
		kind = PRESETS;
		varType = DMTRUI_STRING;
		settings = &u;
		name = n;

		getProperties();

		vector <string> items;
		for (auto a=0; a< settings->nPresets; a++) {
			items.push_back(ofToString(a));
			//cout << a << endl;
		}

		startChildren();
		for (auto & i : items) {
			//cout << settings->flow << endl;
			elements.push_back(new preset(i, u));
			elements.back()->_parent = this;
			boundsRect.growToInclude(elements.back()->boundsRect.getBottomRight());
		}
		endChildren();
		updateFlow();
	}

};

class fps : public element {
public:
	fps(string n, uiConfig & u) {
		kind = LABEL;
		settings = &u;
		name = n;
		getProperties();
		alwaysRedraw = true;
		saveXml = false;
	}

	void drawSpecific() {
		ofSetColor(255);
		if (settings->software->customFont) {
			settings->software->font.drawString(ofToString(ofGetFrameRate()), labelPos.x, labelPos.y);
		}
		else {
			ofDrawBitmapString(ofToString(ofGetFrameRate()), labelPos.x, labelPos.y);
		}
	}
};




// fazer derivado do mult tb, colorizar quadros assim posso usar eles tb pra moving heads por ex.
class color : public mult {
public:
	color (string n, uiConfig & u, vector <string> its) {
		kind = COLOR;
		varType = DMTRUI_STRING;
		settings = &u;
		name = n;
		items = its;
		getProperties();

		startChildren();
		for (auto & i : items) {
			elements.push_back(new coloritem(i, u));
			elements.back()->_parent = this;
			boundsRect.growToInclude(elements.back()->boundsRect.getBottomRight());
		}
		endChildren();
		updateFlow();

		for (auto & e : elements) {
			e->_parent = this;
			boundsRect.growToInclude(e->boundsRect.getBottomRight());
		}
	}
};



class image : public element {
public:
	ofImage img;
	
	void drawSpecific() {
		if (img.isAllocated()) {
			ofSetColor(255);
			img.draw(rect.x, rect.y);
		}
	}
	
	image(string i, uiConfig & u) {
		kind = IMAGE;
		settings = &u;
		getProperties();
		saveXml = false;
		img.load(i);
		boundsRect.width = img.getWidth();
		boundsRect.height = img.getHeight();
		rect.height = boundsRect.height;
		rect.width = boundsRect.height;
		activeRect.width = 0;
		activeRect.height = 0;
	}
};
