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
	float hue = 60;
	ofPoint sliderDimensions = ofPoint(200, 20);
	ofPoint margin = ofPoint(20,20);
	ofPoint flow = margin;
	ofEvent<string> uiEvent;
	bool redraw = true;

	map <string,float>	* pFloat;
	map <string,bool>	* pBool;
	int spacing = 5;
	ofColor color;

	uiConfig() {
		color = ofColor::fromHsb(hue, 155, 255, 220);
 	}

	void update(int height) {
		flow.y += height + spacing;
		hue = int(hue + 6)%255;
		color = ofColor::fromHsb(hue, 155, 255, 220);
	}

	void newLine() {
		flow.y += sliderDimensions.y + spacing;
	}
	void newCol() {
		//cout << "newcol" << endl;
		flow.x += sliderDimensions.x + margin.x;
		flow.y =  margin.y;
	}
};

// acho que nao vai mais precisar disso...
enum elementType {
	SLIDER, LABEL, TOGGLE
};

class element {
protected:
	ofColor labelColor = ofColor(255);
	ofColor color = ofColor(255);
	ofColor activeRectColor = ofColor(0, 90);
	ofRectangle rect, activeRect, boundsRect;
	ofPoint labelPos;
	uiConfig * settings = NULL;
public:
	elementType kind = SLIDER;
	bool isPressed = false;
	bool redraw = true;
	string name;

	// 19 june 2017
	bool firstClicked = false;
	//ofEvent*<string> uiEvent;

	virtual void updateToggle() {}
	virtual void setBool(bool v) {}

	void getProperties() {
		int x = settings->flow.x;
		int y = settings->flow.y;
		color = settings->color;

		rect.width = kind == TOGGLE ? settings->sliderDimensions.y : settings->sliderDimensions.x;
		rect.height = settings->sliderDimensions.y;

		// AUTOFLOW
		int altura = ofGetWindowHeight() - settings->margin.y*2;
		if (y + rect.height > altura) {
			settings->newCol();
			x = settings->flow.x;
			y = settings->flow.y;

		}
		rect.x = x;
		rect.y = y;

		// never draw.
		boundsRect.x = x;
		boundsRect.y = y;
		boundsRect.width = settings->sliderDimensions.x;
		boundsRect.height = settings->sliderDimensions.y;

		activeRect.x = x; activeRect.y = y;
		activeRect.height = settings->sliderDimensions.y;

		if (kind == SLIDER) {
			labelPos.x = x + 5;
			labelPos.y = y + 16;
			labelColor = ofColor(0);
			activeRect.x = x;
			activeRect.y = y;
		}

		else if (kind == TOGGLE) {
			labelPos.x = x + 25;
			labelPos.y = y + 16;
			updateToggle();
			//int w = getBitmapStringBoundingBox(name) + labelPos.x + 4*2;
			//activeRect.width = w;
		}

		else if (kind == LABEL) {
			labelPos.x = x;
			labelPos.y = y + 16;
		}

		settings->update(rect.height);
	}

	void addSettings (uiConfig & u) {
		settings = &u;
	}

	element() {}
	~element() {
		//cout << "element distruktr" << endl;
	}

	virtual void clear() {

		//cout << "clear " + name + "::" + ofToString(ofRandom(0,100)) << endl;
		ofSetColor(0, 0);
		ofDrawRectangle(rect);
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

	virtual void set(float i) {
		cout << "set function on primitive element " + name << endl;
	}

	virtual float getVal() {	 return 1; }

	virtual void draw() {
		ofSetColor(color);
		ofDrawRectangle(rect);
		drawLabel();
	}

	virtual void setValFromMouse(int x, int y) {
	}

	virtual void checkMousePress(int x, int y) {
		if (boundsRect.inside(x,y)) {
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
			redraw = true;
		}
		lastVal = val;

		string s = name + " :: SLIDER :: " + (val ? "true" : "false");
		ofNotifyEvent(settings->uiEvent, s);
		// ofEvent here, criar um evento na classe pai.
	}

	float getVal() {
		return val;
	}

	void draw() {
		clear();
		ofSetColor(color);
		ofDrawRectangle(rect);

		ofSetColor(activeRectColor);
		ofDrawRectangle(activeRect);
		drawLabel();
	}

	void setValFromMouse(int x, int y) {
		int xx = ofClamp(x, rect.x, rect.x + rect.width);
		//this one is not needed, repeat on function set
		activeRect.width = xx - rect.x;
		float valFloat = min + (max-min)*(float(xx-rect.x)/(float)rect.width);
		set(valFloat);
	}
};


class toggle : public element {
public:
	bool val;
	// mudar pra showlabel?
	bool label;

	toggle(string n, uiConfig & u, bool v, bool l = true) : label(l) {
		kind = TOGGLE;
		settings = &u;
		name = n;
		getProperties();
		set(v);
	}

	void updateToggle() {
		float margem = 4;
		activeRect = ofRectangle(
		  rect.x + margem, rect.y + margem,
		  rect.width-margem*2, rect.height-margem*2
		  );

		if (!label) {
			boundsRect.width = boundsRect.height;
		}
	}

	void setBool(bool v) {
		set(v);
	}

	void set(bool v) {
		val = v;
		(*settings->pBool)[name] = val;
		redraw = true;

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

	void draw() {
		ofSetColor(color);
		ofDrawRectangle(rect);
		if (val) {
			// transformar em color active, algo em settings talvez.
			ofSetColor(0, 127);
			ofDrawRectangle(activeRect);
		}
		ofSetColor(255,30);
		ofDrawRectangle(boundsRect);
		if (label) {
			drawLabel();
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

	void draw() {
		drawLabel();
	}
};
