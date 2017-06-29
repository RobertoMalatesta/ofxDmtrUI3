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


#pragma once


enum elementType {
	SLIDER, LABEL, TOGGLE, RADIO, RADIOITEM, SLIDER2D, PRESET, PRESETS
};


struct soft {
public:
	ofPoint presetDimensions = ofPoint(72,48);
	string presetsFolder = "_presets/";
	int presetLoaded = -1;
	int multiSampling = 0;
	int w = 1280;
	int h = 720;
	soft() {}
};


// rename to a more significant name?
struct uiEv {
public:
	string name;

	// STILL TO BE COMPLETED
	string uiname;

	elementType kind;
	bool b;
	int i;
	float f;
	ofPoint p;
	string s;
	//uiEv();
	uiEv(string n, elementType k) 				: name(n), kind(k) {}
	uiEv(string n, elementType k, float ff)		: name(n), kind(k), f(ff) {}
	uiEv(string n, elementType k, int ii) 		: name(n), kind(k), i(ii) {}
	uiEv(string n, elementType k, bool bb) 		: name(n), kind(k), b(bb) {}
	uiEv(string n, elementType k, ofPoint pp) 	: name(n), kind(k), p(pp) {}
	uiEv(string n, elementType k, string ss) 	: name(n), kind(k), s(ss) {}
};



struct uiConfig {
public:


	//senao fazer pointer to function aqui...
	//std::function<void(string,string)> updateUI = NULL;

	//void (*updateUI)(string,string) = NULL;
	//map <string, string> radioUIMap;
	ofRectangle rect;
	int nPresets = 21;
	// ponteiro pro addUI geral.
	soft * software = NULL;

	string getPresetsPath(string ext) {
		if (software != NULL) {
			return software->presetsFolder + ext;
		}
	}


	ofColor bgColor = ofColor(40, 180);

	// mouse flows free from one item to another.
	bool	 flowFree = true;
	bool flowVert = true;
	float hue;

	float opacity = 200;

	ofPoint sliderDimensions = ofPoint(200, 20);
	ofPoint margin = ofPoint(20,20);
	ofPoint flow = margin;
	float colx = margin.x;
	// legacy
	ofEvent<string> uiEventString;
	// complete
	ofEvent<uiEv> uiEvent;
	bool needsRedraw = false;
	bool redraw = false;

	map <string, float>		* pFloat;
	map <string, int>		* pInt;
	map <string, bool>		* pBool;
	map <string, string> 	* pString;
	map <string, ofPoint>	* pPoint;
	int spacing = 5;
	int hueStep = 6;
	ofColor color;
	float flowxbak = -100;

	ofColor activeColor = ofColor(0,0,0,140);

	void updateColor() {
		hue = int(hue + hueStep)%255;
		color = ofColor::fromHsb(hue, 155, 210, 255);
	}

	uiConfig() {
		updateColor();
 	}

	void reset() {
		flow = margin;
		// starthue no futuro
		hue = 60;
	}

	// only to make children items of radio.
	void setMarginChildren(bool m) {
		spacing = m ? 1 : 5;
		hueStep = m ? 3 : 6;
	}

	void update(ofRectangle & r) {
		if (flowVert) {
			flow.y += r.height + spacing;
		} else {
			if ((flow.x + r.width) > (colx + sliderDimensions.x)) {
				flow.x = colx;
				flow.y += r.height + spacing;
			} else {
				flow.x += r.width + spacing;
			}
		}
		updateColor();
		//rect.growToInclude(r.getBottomRight());
	}

	void newLine() {
		flow.y += sliderDimensions.y + spacing;
	}
	void newCol() {
		//cout << "newcol" << endl;
		colx +=  sliderDimensions.x + margin.x;
		//flow.x += sliderDimensions.x + margin.x;
		flow.x = colx;
		flow.y =  margin.y;
	}

	void setFlowVert(bool f) {
		flowVert = f;
		if (f) {
			flow.x = colx;
		}
	}
};




class element {
protected:
	ofColor labelColor = ofColor(255);
	ofColor color = ofColor(255);
	//ofColor activeRectColor = ofColor(0, 90);
	ofPoint labelPos;
	uiConfig * settings = NULL;
	bool showLabel = true;

public:
	std::function<void(string, string)> changeUI = NULL;

	bool alwaysRedraw = false;
	// 24 june 2017 - webcams
	int selectedId = -1;


	// teste para scene
	string uiScene = "";

	void (*invoke)(void) = NULL;
	void (*invokeBool)(bool) = NULL;
	void (*invokeFloat)(float) = NULL;
	void (*invokeInt)(int) = NULL;
	void (*invokeString)(string) = NULL;

	virtual void setFolder(string s) {};

	bool useLabelShadow = true;
	// medida provisoria ate resolver o radioitem
	string valString = "";

	vector <element*> elements;

	// passei pra publico pra melhorar pro radio
	ofRectangle rect, activeRect, boundsRect;

	// 21 de junho, nao tem serventia ainda, vou tentar fazer o radioitem
	element * _parent = NULL;
	elementType kind = SLIDER;
	bool isPressed = false;
	bool redraw = false;
	string name;
	string label = "";

	// 19 june 2017
	bool firstClicked = false;

	void notify() {
		// fires any kind of void on any change
		if ((*invoke) != NULL) {
			(*invoke)();
		}

		string tipo;
//		SLIDER, LABEL, TOGGLE, RADIO, RADIOITEM

		if (kind == SLIDER) { tipo = "SLIDER"; }
		if (kind == TOGGLE) { tipo = "TOGGLE"; }
		if (kind == RADIO) { tipo = "RADIO"; }
		if (kind == RADIOITEM) { tipo = "RADIOITEM"; }
		if (kind == SLIDER2D) { tipo = "SLIDER2D"; }
		if (kind == PRESETS) { tipo = "PRESETS"; }

		string valor;

		// irmaos
		if (kind == RADIO || kind == PRESETS) {
			valor = getValString();
//			cout << "inside notify()" << endl;
//			cout << valor << endl;
			uiEv e = uiEv(name, kind, (string)valor);
			//e.uiname = settings->UINAME;
			ofNotifyEvent(settings->uiEvent, e);
		}
		else if (kind == SLIDER2D) {
			valor = ofToString(getValPoint().x) + ":" + ofToString(getValPoint().y);
			uiEv e = uiEv(name, kind, getValPoint());
			//e.uiname = settings->UINAME;
			ofNotifyEvent(settings->uiEvent, e);
		}

		// avoid notification
		//else {
		else if (kind != PRESET && kind != RADIOITEM) {
			valor = ofToString(getVal());
			uiEv e = uiEv(name, kind, getVal());
			//e.uiname = settings->UINAME;
			ofNotifyEvent(settings->uiEvent, e);
		}

		// LEGACY TEST
		string s = name + " :: " + tipo + " :: " + valor;
		ofNotifyEvent(settings->uiEventString, s);
	}

	void needsRedraw(bool need = true) {
		redraw = need;
		settings->needsRedraw = need;
	}

	void getProperties() {
		int x = settings->flow.x;
		int y = settings->flow.y;
		color = settings->color;

		// AUTOFLOW // nao desejavem por agora
//		int altura = ofGetWindowHeight() - settings->margin.y*2;
//		if (y + rect.height > altura) {
//			settings->newCol();
//			x = settings->flow.x;
//			y = settings->flow.y;
//		}

		if (kind != LABEL) {
			rect.x = x;
			rect.y = y;
			rect.width = kind == TOGGLE ? settings->sliderDimensions.y : settings->sliderDimensions.x;
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

		// generic
		labelPos.x = x + 5;
		labelPos.y = y + 16;

		// temporario
		if (kind == PRESETS) {
			rect.width = activeRect.width = 0;
//			boundsRect.height = 400;
//			rect = boundsRect;
//			activeRect.width = 0;
//			activeRect.height = 0;
		}

		if (kind == PRESET) {
//			boundsRect.width = 72;
//			boundsRect.height = 48;
			boundsRect.width = settings->software->presetDimensions.x;
			boundsRect.height = settings->software->presetDimensions.y;
			activeRect = rect = boundsRect;
			color = ofColor(0,255);
		}

		if (kind == SLIDER2D) {
//			labelPos.x = x + 5;
//			labelPos.y = y + 16;
			boundsRect.height = settings->sliderDimensions.y * 2 + settings->spacing;
			rect.height = boundsRect.height;
			activeRect.width = 0;
			activeRect.height = 0;
		}

		else if (kind == LABEL) {
			labelPos.x = x;
			labelPos.y = y + 16;
		}

		else if (kind == RADIO) {
			labelPos.x = x;
			labelPos.y = y + 16;
			rect.width = 0;
			rect.height = 0;
			//boundsRect.height = settings->sliderDimensions.y * 2;
		}

		else if (kind == SLIDER) {
			labelPos.x = x + 5;
			labelPos.y = y + 16;
			labelColor = ofColor(0);
		}

		else if (kind == TOGGLE) {
			labelPos.x = x + 25;
			labelPos.y = y + 16;

			float margem = 4;
			activeRect = ofRectangle(
				rect.x + margem, rect.y + margem,
				rect.width-margem*2, rect.height-margem*2
			);

			if (!showLabel) {
				boundsRect.width = boundsRect.height;
			} else {
				int contaletras = 0;
				for(auto c: ofUTF8Iterator(name)){
					contaletras++;
				}
				boundsRect.width = boundsRect.height + margem*2 + contaletras * 8;
			}
		}

		else if (kind == RADIOITEM) {
			float margem = 4;

			labelPos.x = x + margem;
			labelPos.y = y + 16;

			int contaletras = 0;
			for(auto c: ofUTF8Iterator(name)){
				contaletras++;
			}
			boundsRect.width = margem*2 + contaletras * 8;
			rect.width = boundsRect.width;
			activeRect = rect;

			// ugly solution to overflow but working
			if ((settings->flow.x + boundsRect.width) >
				(settings->colx + settings->sliderDimensions.x)) {
				settings->update(boundsRect);
				getProperties();
				settings->flow.x -= boundsRect.width + settings->spacing;
			}
		}

		settings->update(boundsRect);
		// pra todos que precisam disso na inicializacao
		// posteriormente remover pro toggle laembaixo;
		needsRedraw();
	}

	void addSettings (uiConfig & u) {
		settings = &u;
	}

	element() {}
	~element() {
	}


	void setColor(ofColor c) {
		color = c;
	}

	virtual void updateFromPixels(ofPixels & p) {}
	virtual void updateImage() {}

	virtual void drawLabel() {
		string n = name + label;
		if (useLabelShadow) {
			ofSetColor(0,180);
			ofDrawBitmapString(n, labelPos.x - 1, labelPos.y + 1);
		}

		//if (kind == SLIDER)
		//if (useLabelShadow)
		{
			//decidir se remover o labelcolor totalmente
			//ofSetColor(labelColor);
			ofSetColor(255);
			ofDrawBitmapString(n, labelPos.x, labelPos.y);
		}
	}


	virtual void set(bool i) {
		cout << "set function on primitive element, using bool " + name << endl;
	}

	virtual void set(float i) {
		cout << "set function on primitive element, using float " + name << endl;
	}

	virtual void set(string i) {
		cout << "set function on primitive element, using string " + name << endl;
	}

	virtual void set(ofPoint i) {
		cout << "set function on primitive element, using ofPoint " + name << endl;
	}


	virtual float getVal() {
		cout << "getVal function on primitive element " + name << endl;
	} //return 1;

	virtual string getValString() { cout << "never to be used :: " + name << endl; }
	virtual ofPoint getValPoint() { cout << "never to be used :: " + name << endl; }

	virtual void draw() {
		ofSetColor(color);
		ofDrawRectangle(rect);
		drawSpecific();

		if (showLabel) {
			drawLabel();
		}
//		ofSetColor(255,0,0, 50);
//		ofDrawRectangle(boundsRect);
	}

	// temporario somente pra resolver
	bool dragging = false;

	virtual void drawSpecific() {}

	virtual void setValFromMouse(int x, int y) {}

	virtual void setRadioVal(element * e) {}

	virtual void checkMouseNeu(int x, int y, bool first = false) {
		//cout << "checkmouseneu" << ":" << x << ":" << y << endl;
		if (first) {
			dragging = false;
			if (boundsRect.inside(x,y)) {
				firstClicked = true;
				//checkMouseNeu(x, y);
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

	// remover?
//	virtual void checkMousePress(int x, int y) {
//		if (boundsRect.inside(x,y)) {
//			//cout << "checkMousePress :: " + name << endl;
//			firstClicked = true;
//			checkMouse(x, y);
//		}
//	}
//
//	virtual void checkMouse(int x, int y) {
//		// este check define se o click vai rodar livre ou nao
//		if (firstClicked || settings->flowFree)
//		{
//			if (boundsRect.inside(x,y)) {
//				setValFromMouse(x,y);
//				isPressed = true;
//			} else {
//				if (isPressed) {
//					setValFromMouse(x,y);
//					draw();
//					isPressed = false;
//				}
//			}
//		}
//	}
};



class slider2d : public element {
private:
	ofPoint min = ofPoint(0,0);
	ofPoint max = ofPoint(1,1);
public:
	ofPoint val = ofPoint(0.5, 0.5);
	//ofPoint lastVal = val;
	ofPoint lastVal;

	slider2d(string n, uiConfig & u, ofPoint mi = ofPoint(0,0), ofPoint ma = ofPoint(1,1), ofPoint v = ofPoint(.5,.5))
	: min(mi), max(ma) {
		kind = SLIDER2D;
		settings = &u;
		name = n;
		getProperties();
		set(v);
	}

	void set(ofPoint v) {
		val = v;
		if (lastVal != val) {
			(*settings->pPoint)[name] = val;
			lastVal = val;
			needsRedraw();
			label = " " + ofToString(val.x) + ":" + ofToString(val.y);
			notify();
		}
	}

	ofPoint getValPoint() {
		return val;
	}

	// slider
	void drawSpecific() {
//		ofSetColor(settings->activeColor);
//		ofDrawRectangle(activeRect);
		ofSetColor(0);
		float x = rect.x + val.x * rect.width;
		float y = rect.y + val.y * rect.height;
		ofDrawLine(x, rect.y, x, rect.y + rect.height);
		ofDrawLine(rect.x, y, rect.x + rect.width, y);
	}

	void setValFromMouse(int x, int y) {
		//cout << "setValFromMouse" << endl;
		int xx = ofClamp(x, rect.x, rect.x + rect.width);
		int yy = ofClamp(y, rect.y, rect.y + rect.height);
		ofPoint xy = ofPoint (xx,yy) - ofPoint(rect.x, rect.y);
		ofPoint wh = ofPoint (rect.width, rect.height);
		ofPoint val = min + (max-min)*(xy/wh);
		set(val);
	}

};


class slider : public element {
private:
	float min = 0;
	float max = 1;
	bool isInt;
public:
	float val = .5;
	float lastVal;

	slider(string n, uiConfig & u, float mi, float ma, float v, bool i=false) : min(mi), max(ma), isInt(i) {
		settings = &u;
		name = n;
		getProperties();
		set(v);
	}

	// slider
	void set(float v) {
		val = v;

		if (lastVal != val) {
			activeRect.width = rect.width * ((val-min) / (max-min));
			if (isInt) {
				val = int(val);
				(*settings->pInt)[name] = val;
			} else {
				(*settings->pFloat)[name] = val;
			}
			lastVal = val;
			needsRedraw();
			label = " " + ofToString(val);
			notify();

			if ((*invokeFloat) != NULL) {
				(*invokeFloat)(val);
			}
		}
		// ofEvent here, criar um evento na classe pai.
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
		int xx = ofClamp(x, rect.x, rect.x + rect.width);
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
};



class label : public element {
public:
	label (string n, uiConfig & u) {
		kind = LABEL;
		settings = &u;
		name = n;
		getProperties();
	}
};


// TYPE TO HANDLE BOTH RADIOITEM AND TOGGLE
class booleano : public element {
public:
	bool val = false;

	void set(bool v) {
		if (val != v) {
			val = v;
			(*settings->pBool)[name] = val;
			needsRedraw();
			notify();
			if ((*invokeBool) != NULL) {
				(*invokeBool)(val);
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


class radioitem : public booleano {
public:
	radioitem (string n, uiConfig & u) {
		kind = RADIOITEM;
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




// primitive class to radio and presets, classes that has children elements
class mult : public element {
public:

	string folder;
	vector <string> items;
	string lastVal;
	bool eventWhenSameSelectedIndex = false;

	void setFolder(string s) {
		folder = s;
	}

	void drawSpecific() {
		for (auto & e : elements) {
			e->draw();
		}
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

	// mult (radio and presets)
	void set(string s) {
		int index = 0;
		for (auto & e : elements) {
			if (e->name == s) {
				if (valString != e->name || (eventWhenSameSelectedIndex && !dragging))
				{
					//cout << "set on radio OR presets, string :: "+s << endl;
					valString = e->name;
					(*settings->pString)[name] = valString;
					e->set(true);
					needsRedraw();
					notify();

					if (changeUI != NULL) {
						//cout << "changeUI not null" << endl;
						string uiSceneFolder = "_scene/";
						string f = uiSceneFolder + valString + ".txt";
						//cout << f << endl;
						changeUI(name, f);
					}

					//							(*settings->updateUI)(uiScene, f);
					if (uiScene != "") {
						// pointer to function melhor?
						//uis[uiScene].clear();

						// temp
						//changeUI(uiScene, f);

						//uis[uiScene].createFromText(f);
					}

					// provisorio
					selectedId = index;

					if ((*invokeString) != NULL) {
						(*invokeString)(valString);
					}
					if ((*invokeInt) != NULL) {
						(*invokeInt)(selectedId);
					}
				}
			} else {
				if (e->getVal() == true) {
					e->set(false);
				}
			}
			index++;
		}
	}
};


class radio : public mult {
public:
	radio (string n, uiConfig & u, vector <string> its, string s) {
		uiScene = s;
		radio (n, u, its);
	}

	radio (string n, uiConfig & u, vector <string> its) {
		kind = RADIO;
		settings = &u;
		name = n;
		items = its;
		getProperties();

		settings->setFlowVert(false);
		u.setMarginChildren(true);
		for (auto & i : items) {
			elements.push_back(new radioitem(i, u));
		}
		u.setMarginChildren(false);

		for (auto & e : elements) {
			e->_parent = this;
			boundsRect.growToInclude(e->boundsRect.getBottomRight());
		}
		settings->setFlowVert(true);
		settings->newLine();
	}
};



class preset : public booleano {
public:
	ofImage img;
	ofFbo fbo;
	ofPoint dimensions; // = ofPoint(72, 48);

	preset (string n, uiConfig & u) {
		kind = PRESET;
		settings = &u;
		name = n;
		getProperties();

		dimensions = settings->software->presetDimensions;
		fbo.allocate(dimensions.x, dimensions.y, GL_RGBA);

		string file = settings->getPresetsPath(name + ".tif");
		fbo.begin();
		ofClear(30,255);
		if (ofFile::doesFileExist(file)) {
			img.load(file);
			ofSetColor(255);
			img.draw(0,0);
		} else {
			ofSetColor(255,0,40);
			ofTranslate(fbo.getWidth()/2, fbo.getHeight()/2);
			int n = 12;
			ofDrawLine(-n, n, n, -n);
			ofDrawLine(-n, -n, n, n);
		}
		fbo.end();
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
		string file = settings->getPresetsPath(name + ".tif");
		//cout << "update Image " + name + file << endl;
		fbo.begin();
		ofClear(30,255);
		if (ofFile::doesFileExist(file)) {
			//cout << "file exists " + file << endl;
			img.load(file);
			ofSetColor(255);
			img.draw(0,0);
		} else {
			ofSetColor(255,0,40);
			ofTranslate(fbo.getWidth()/2, fbo.getHeight()/2);
			int n = 12;
			ofDrawLine(-n, n, n, -n);
			ofDrawLine(-n, -n, n, n);
		}
		fbo.end();
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
		}
	}
};

class presets : public mult {
public:
	presets(string n, uiConfig & u) {
		eventWhenSameSelectedIndex = true;
		kind = PRESETS;
		settings = &u;
		name = n;
		getProperties();

		vector <string> items;
		for (auto a=0; a< settings->nPresets; a++) {
			items.push_back(ofToString(a));
		}

		settings->setFlowVert(false);
		u.setMarginChildren(true);
		for (auto & i : items) {
			elements.push_back(new preset(i, u));
		}
		u.setMarginChildren(false);

		for (auto & e : elements) {
			e->_parent = this;
			boundsRect.growToInclude(e->boundsRect.getBottomRight());
		}
		settings->setFlowVert(true);
		settings->newLine();

	}

};