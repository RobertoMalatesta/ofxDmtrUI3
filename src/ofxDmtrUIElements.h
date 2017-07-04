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

// naming conflicts? namespace?
enum elementType {
	SLIDER, LABEL, TOGGLE, RADIO, RADIOITEM, SLIDER2D, PRESET, PRESETS, COLOR, COLORITEM
};

// naming conflicts? namespace?
enum dmtrUIVarType {
	FLOAT, INT, STRING, BOOLEAN, POINT
};


struct soft {
public:
	bool visible = true;
	float opacity = 180;
	ofPoint presetDimensions = ofPoint(72,48);
	string presetsFolder = "_presets/";
	int presetLoaded = -1;
	int multiSampling = 0;
	int w = 1280;
	int h = 720;
	soft() {}
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

	uiEv(string n) : name(n) {}

	uiEv(string n, elementType k, dmtrUIVarType t) 				: name(n), kind(k), varType(t) {}
	uiEv(string n, elementType k, dmtrUIVarType t, float ff)		: name(n), kind(k), varType(t), f(ff) {}
	uiEv(string n, elementType k, dmtrUIVarType t, int ii) 		: name(n), kind(k), varType(t), i(ii) {}
	uiEv(string n, elementType k, dmtrUIVarType t, bool bb) 		: name(n), kind(k), varType(t), b(bb) {}
	uiEv(string n, elementType k, dmtrUIVarType t, ofPoint pp) 	: name(n), kind(k), varType(t), p(pp) {}
	uiEv(string n, elementType k, dmtrUIVarType t, string ss) 	: name(n), kind(k), varType(t), s(ss) {}
};


struct uiConfig {
public:
	//senao fazer pointer to function aqui...
	//std::function<void(string,string)> updateUI = NULL;

	//void (*updateUI)(string,string) = NULL;
	//map <string, string> radioUIMap;

	ofPoint sliderDimensions = ofPoint(200, 20);
	ofPoint margin = ofPoint(20,20);
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

	string getPresetsPath(string ext) {
		if (software != NULL) {
			return software->presetsFolder + ext;
		}
	}

	ofColor bgColor = ofColor(40, 180);

	// mouse flows free from one item to another.
	bool	 flowFree = true;
	bool flowVert = true;

	// legacy
	//ofEvent<string> uiEventString;

	// complete
	ofEvent<uiEv> uiEvent;
	bool needsRedraw = false;
	bool redraw = false;

	map <string, float>		* pFloat;
	map <string, int>		* pInt;
	map <string, bool>		* pBool;
	map <string, string> 	* pString;
	map <string, ofPoint>	* pPoint;

	float hueStart = 60;
	float hue;
	int hueStep = 4;
	ofColor color;
	bool bw = false;
	//float flowxbak = -100;

	ofColor activeColor = ofColor(0,0,0,140);

	void updateColor() {
		hue = int(hue + hueStep)%255;
		color = ofColor::fromHsb(hue, 155, 210, 255);

		if (bw) {
			color = ofColor(127);
		}
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
			if ((flow.x + r.width) > (colx + sliderDimensions.x)) {
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
	//ofColor activeRectColor = ofColor(0, 90);
	ofPoint labelPos;
	uiConfig * settings = NULL;

public:
	bool showLabel = true;
	ofColor color = ofColor(255);

	bool alwaysRedraw = false;
	dmtrUIVarType varType;

	// pode ser temporario
	bool isDir = false;
	std::function<void(string, string)> changeUI = NULL;

	// 24 june 2017 - webcams
	int selectedId = -1;


//	void (*invoke)(void) = NULL;
//	void (*invokeBool)(bool) = NULL;
//	void (*invokeFloat)(float) = NULL;
//	void (*invokeInt)(int) = NULL;
//	void (*invokeString)(string) = NULL;

	virtual void setFbo (ofFbo &fbo) {}
	virtual void setFolder(string s) {};

	bool useLabelShadow = true;
	// medida provisoria ate resolver o radioitem
	string valString = "";

	vector <element*> elements;

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



	void notify() {
		// fires any kind of void on any change
//		if ((*invoke) != NULL) {
//			(*invoke)();
//		}


		if (varType == STRING) {
			//cout << "notify event string" << endl;
			uiEv e = uiEv(name, kind, varType, getValString());
			e.isDir = isDir;
			ofNotifyEvent(settings->uiEvent, e);
		}
		else if (varType == POINT) {
			//cout << "notify event point" << endl;
			uiEv e = uiEv(name, kind, varType, getValPoint());
			ofNotifyEvent(settings->uiEvent, e);
		}
		else  {
			//cout << "notify event int or float" << endl;
//			uiEv e = uiEv(name, kind, varType, varType == INT ? int(getVal()) : getVal());
			uiEv e = uiEv(name, kind, varType, getVal());
			ofNotifyEvent(settings->uiEvent, e);
		}
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
			boundsRect.height = settings->sliderDimensions.y * 2 + settings->getSpacing();
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
			varType = FLOAT;

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

		else if (kind == RADIOITEM || kind == COLORITEM) {
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
				settings->flow.x -= boundsRect.width + settings->getSpacing();
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
		if (varType == FLOAT) {
			set((*settings->pFloat)[name]);
		}
		else if (varType == INT) {
			// ainda igual ao int. vamos ver o que acontece.
			set(float((*settings->pInt)[name]));
		}
		else if (varType == STRING) {
			set((*settings->pString)[name]);
		}
		else if (varType == POINT) {
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
		varType = POINT;

		settings = &u;
		name = n;
		getProperties();
		set(v);
	}

	void set(ofPoint v, bool notifyEvent = true) {
		val = v;
		if (lastVal != val) {
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
		if (_fbo != NULL) {
			//cout << "fbo is not null" << endl;
			ofSetColor(255);
//			_fbo->draw(rect.x + settings->rect.x , rect.y + settings->rect.y);
			_fbo->draw(rect.x, rect.y);
		}
		ofSetColor(0);
		float x = rect.x + val.x * rect.width;
		float y = rect.y + val.y * rect.height;
		ofDrawLine(x, rect.y, x, rect.y + rect.height);
		ofDrawLine(rect.x, y, rect.x + rect.width, y);
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


class slider : public element {
private:
	bool isInt;
public:
	float min = 0;
	float max = 1;
	float val = .5;
	float lastVal;

	slider(string n, uiConfig & u, float mi, float ma, float v, bool i=false) : min(mi), max(ma), isInt(i) {
		settings = &u;
		name = n;
		getProperties();
		set(v);
		varType = isInt ? INT : FLOAT;
	}

	// slider
	void set(float v, bool notifyEvent = true) {
		val = v;

		if (lastVal != val) {
			if (isInt) {
				val = int(val);
				(*settings->pInt)[name] = val;
			} else {
				(*settings->pFloat)[name] = val;
			}
			activeRect.width = rect.width * ((val-min) / (max-min));
			lastVal = val;
			needsRedraw();
			label = " " + ofToString(val);
			if (notifyEvent) {
				notify();
			}

//			if ((*invokeFloat) != NULL) {
//				(*invokeFloat)(val);
//			}
		}
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

	void set(bool v, bool notifyEvent = true) {
		if (val != v) {
			val = v;
			(*settings->pBool)[name] = val;
			needsRedraw();
			if (notifyEvent) {
				notify();
			}
//			if ((*invokeBool) != NULL) {
//				(*invokeBool)(val);
//			}
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
		varType = BOOLEAN;
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
			ofSetColor(color);
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
	int nElements = 0;

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

//	void getVal() {
//		cout << "getval from mult" + name << endl;
//		return (float)selectedIndex;
//	}

	// mult (radio and presets)


	void set(string s, bool notifyEvent = true) {
		int index = 0;
		for (auto & e : elements) {
			if (e->name == s) {
				if (valString != e->name || (eventWhenSameSelectedIndex && !dragging))
				{
					valString = e->name;
					(*settings->pString)[name] = valString;
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

					selectedId = index;
					// invoke pointer to functions (string and id)
				}
			} else {
				if (e->getVal() == true) {
					e->set(false);
				}
			}
			index++;
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
};


class radio : public mult {
public:
	radio (string n, uiConfig & u, vector <string> its) {
		kind = RADIO;
		varType = STRING;
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
		nElements = elements.size();
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
		varType = BOOLEAN;

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

			ofSetColor(255,0,70);
			ofDrawRectangle(activeRect.x + 10, activeRect.y + 10, 20, 20);
		}
	}
};

class presets : public mult {
public:
	presets(string n, uiConfig & u) {
		eventWhenSameSelectedIndex = true;
		kind = PRESETS;
		varType = STRING;

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
		nElements = elements.size();
	}
};

class fps : public element {
public:
	fps(string n, uiConfig & u) {
		kind = PRESETS;
		settings = &u;
		name = n;
		getProperties();
		alwaysRedraw = true;
	}

	void drawSpecific() {
		ofSetColor(255);
		ofDrawBitmapString(ofToString(ofGetFrameRate()), labelPos.x, labelPos.y);
	}
};



// fazer derivado do mult tb, colorizar quadros assim posso usar eles tb pra moving heads por ex.
class color : public mult {
public:
	color (string n, uiConfig & u, vector <string> its) {
		kind = COLOR;
		varType = STRING;
		settings = &u;
		name = n;
		items = its;
		getProperties();

		settings->setFlowVert(false);
		u.setMarginChildren(true);
		for (auto & i : items) {
			elements.push_back(new coloritem(i, u));
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