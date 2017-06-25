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



struct uiConfig {
public:
	bool	 flowFree = true;
	bool flowVert = true;
	float hue = 60;

	float opacity = 200;

	ofPoint sliderDimensions = ofPoint(200, 20);
	ofPoint margin = ofPoint(20,20);
	ofPoint flow = margin;
	ofEvent<string> uiEvent;
	bool needsRedraw = false;
	bool redraw = false;

	map <string, float>		* pFloat;
	map <string, bool>		* pBool;
	map <string, string> 	pString;
	int spacing = 5;
	ofColor color;
	float flowxbak = -100;

	ofColor activeColor = ofColor(0,0,0,127);

	void updateColor() {
		hue = int(hue + 6)%255;
		color = ofColor::fromHsb(hue, 155, 210, 255);
	}

	uiConfig() {
		updateColor();
		//color = ofColor::fromHsb(hue, 155, 255, 255);
 	}

	void setMarginChildren(bool m) {
		spacing = m ? 1 : 5;
	}


	void update(ofRectangle & r) {
		if (flowVert) {
			flow.y += r.height + spacing;
		} else {
			if ((flow.x + r.width + spacing + margin.x) > (flowxbak + sliderDimensions.x)) {
				flow.x = flowxbak;
				flow.y += r.height + spacing;
			} else {
				flow.x += r.width + spacing;
			}
		}
		updateColor();
	}



	void newLine() {
		flow.y += sliderDimensions.y + spacing;
	}
	void newCol() {
		//cout << "newcol" << endl;
		flow.x += sliderDimensions.x + margin.x;
		flow.y =  margin.y;
	}

	void setFlowVert(bool f) {
		flowVert = f;
		if (f) {
			if (flowxbak > -100) {
				flow.x = flowxbak;
			}
			//newLine();
		} else {
			flowxbak = flow.x;
		}
	}
};

// acho que nao vai mais precisar disso...
enum elementType {
	SLIDER, LABEL, TOGGLE, RADIO, RADIOITEM
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

	// 19 june 2017
	bool firstClicked = false;
	//ofEvent*<string> uiEvent;

	void needsRedraw(bool need = true) {
		redraw = need;
		settings->needsRedraw = need;
	}

	void getProperties() {
		int x = settings->flow.x;
		int y = settings->flow.y;
		color = settings->color;

		// AUTOFLOW
		int altura = ofGetWindowHeight() - settings->margin.y*2;
		if (y + rect.height > altura) {
			settings->newCol();
			x = settings->flow.x;
			y = settings->flow.y;
		}

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



		if (kind == LABEL) {
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
		}

		settings->update(boundsRect);
	}

	void addSettings (uiConfig & u) {
		settings = &u;
	}

	element() {}
	~element() {
		//cout << "element distruktr" << endl;
	}

	virtual void clear() {
		// rever aqui como limpar o estado anterior
		//cout << "clear " + name + "::" + ofToString(ofRandom(0,100)) << endl;
//		ofSetColor(0, 0);
//		ofDrawRectangle(activeRect);
//		ofDrawRectangle(boundsRect);
	}


	void setColor(ofColor c) {
		color = c;
	}

	virtual void drawLabel() {
		ofSetColor(labelColor);
		ofDrawBitmapString(name, labelPos.x, labelPos.y);

		if (kind == SLIDER) {
			string n = name; //+ "::" + ofToString(_valFloat);
			ofSetColor(255);
			ofDrawBitmapString(n, labelPos.x + 1, labelPos.y - 1);
		}
	}

	// remover daqui a pouco
	//virtual void setBool(bool v) {}

	virtual void set(bool i) {
		cout << "set function on primitive element, using bool " + name << endl;
	}

	virtual void set(float i) {
		cout << "set function on primitive element, using float " + name << endl;
	}

	virtual void set(string i) {
		cout << "set function on primitive element, using string " + name << endl;
	}


	virtual float getVal() {
		cout << "getval function on primitive element" + name << endl;
	} //return 1;

	virtual string getValString() { return ""; } // never to be used

	virtual void draw() {
		//clear();
		ofSetColor(color);
		ofDrawRectangle(rect);

		drawSpecific();

		if (showLabel) {
			drawLabel();
		}
//		ofSetColor(255,0,0, 50);
//		ofDrawRectangle(boundsRect);
	}

	virtual void drawSpecific() {}

	virtual void setValFromMouse(int x, int y) {}

	virtual void setRadioVal(element * e) {}

	virtual void checkMousePress(int x, int y) {
		if (boundsRect.inside(x,y)) {
			cout << "checkMousePress :: " + name << endl;
			firstClicked = true;
			checkMouse(x, y);
		}
	}

	virtual void checkMouse(int x, int y) {
		// este check define se o click vai rodar livre ou nao
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
};


class slider : public element {
private:
	string label;
	float min = 0;
	float max = 1;
public:
	float val = .5;
	float lastVal;

	slider(string n, uiConfig & u, float mi, float ma, float v) : min(mi), max(ma) {
		settings = &u;
		name = n;
		getProperties();
		set(v);
	}

	void set(float v) {
		val = v;
		activeRect.width = rect.width * ((val-min) / (max-min));
		label = name + " " + ofToString(val);
		(*settings->pFloat)[name] = val;

		if (lastVal != val) {
			needsRedraw();
		}
		lastVal = val;

		string s = name + " :: SLIDER :: " + (val ? "true" : "false");
		ofNotifyEvent(settings->uiEvent, s);
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
		float valFloat = min + (max-min)*(float(xx-rect.x)/(float)rect.width);
		set(valFloat);
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



class booleano : public element {
public:
	bool val;

	void set(bool v) {
		val = v;
		(*settings->pBool)[name] = val;
		needsRedraw();

		string s = name + " :: TOGGLE :: " + (val ? "true" : "false");
		ofNotifyEvent(settings->uiEvent, s);
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



//class toggleOld : public element {
//public:
//	bool val;
//
//	toggleOld(string n, uiConfig & u, bool v, bool l = true)  {
//		kind = TOGGLE;
//		settings = &u;
//		name = n;
//		showLabel = l;
//		getProperties();
//		set(v);
//	}
//
//	void set(bool v) {
//		val = v;
//		(*settings->pBool)[name] = val;
//		needsRedraw();
//
//		string s = name + " :: TOGGLE :: " + (val ? "true" : "false");
//		ofNotifyEvent(settings->uiEvent, s);
//	}
//
//	void setValFromMouse(int x, int y) {
//		if (!isPressed) {
//			set(val ^ 1);
//			isPressed = true;
//		}
//	}
//
//	float getVal() {
//		return val;
//	}
//
//	// toggle
//	void drawSpecific() {
//		if (val) {
//			ofSetColor(settings->activeColor);
//			ofDrawRectangle(activeRect);
//		}
//	}
//};




// estes dois poderiam ser um subgrupo em comum, tipo class radioitem : public bool : public element
class radioitemOld : public element {
public:
	bool val = false;

	radioitemOld (string n, uiConfig & u) {
		kind = RADIOITEM;
		settings = &u;
		name = n;
		getProperties();
	}

	void setValFromMouse(int x, int y) {
		if (!isPressed) {
			set(val ^ 1);
			isPressed = true;
		}
	}

	void set(bool v) {
		val = v;
		needsRedraw();
	}

	float getVal() {
		return val;
		cout << "get val boolean for radioitem " + name << endl;
		cout << val << endl;
	}

	void drawSpecific() {
		if (val) {
			ofSetColor(settings->activeColor);
			ofDrawRectangle(activeRect);
		}
	}
};


class radio : public element {
public:

	vector <string> items;
	radio (string n, uiConfig & u, vector <string> its) {

		//(*settings).pString[name] = valString;

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
	}

	void drawSpecific() {
		for (auto & e : elements) {
			e->draw();
		}
	}

	void setValFromMouse(int x, int y) {
		for (auto & e : elements) {
			if (e->boundsRect.inside(x,y)) {
				setRadioVal(e);
			}
		}
	}

	string getValString() {
		return valString;
	}

//	void getVal() {
//		return valString;
//	}

	void set(string s) {
		cout << "set on radio, string :: "+s << endl;
		for (auto & e : elements) {
			if (e->name == s) {
				//if (valString != e->name)
				{
					valString = e->name;
					e->set(true);
					needsRedraw();
				}
			} else {
				e->set(false);
			}
		}
		settings->pString[name] = valString;

	}

	void setRadioVal(element * ee) {
		//cout << "setradioval" << endl;
		for (auto & e : elements) {
			if (e->name == ee->name) {
				if (valString != e->name) {
					valString = e->name;
					cout << e->name << endl;
					e->set(true);
					needsRedraw();
				}
			}
			else {
				// apenas clarear o radioitem que estava selecionado
				//if (!e->val)
				{
					e->set(false);
				}
			}
		}
		settings->pString[name] = valString;

		//drawSpecific();
	}
};