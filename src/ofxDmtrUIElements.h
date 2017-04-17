//#pragma once

struct uiConfig {
public:
	ofPoint flow;
	ofPoint sliderDimensions = ofPoint(200, 20);
	ofPoint margin = ofPoint(10,10);
	float hue = 60;
	ofEvent<string> uiEvent;
	bool redraw = true;

	map <string,float>			pFloat;
	map <string,bool>			pBool;
};

// acho que nao vai mais precisar disso...
//enum elementType {
//	SLIDER, LABEL, TOGGLE
//};

class element {
protected:
	ofColor labelColor = ofColor(255);
	ofColor color = ofColor(255);
	int min;
	int max;
	ofRectangle rect, activeRect;
	ofPoint labelPos;
	uiConfig * settings = NULL;
	// as vezes é igual, as vezes diferente.
	// fazer ou nao aqui uma string label?
public:
	bool isPressed = false;
	bool redraw = true;
	string name;
	//ofEvent*<string> uiEvent;



	void addSettings (uiConfig & u) {
		settings = &u;
	}

	element() {} // constructor
	~element() {} // destructor

	virtual void clear() {
		ofSetColor(0,0);
		ofDrawRectangle(activeRect);
	}

	void setRect(int x, int y, int w, int h) {
		rect.x = x; rect.y = y;
		rect.width = w; rect.height = h;
	}

	void setActiveRect(int x, int y, int w, int h) {
		activeRect.x = x; activeRect.y = y;
		activeRect.width = w; activeRect.height = h;
	}

	void setColor(ofColor c) {
		color = c;
	}
	// The word virtual means ‘‘may be redefined later in a class
	// derived from this one’’ in Simula and C++

	virtual void drawLabel() {
		ofSetColor(labelColor);
		ofDrawBitmapString(name, labelPos.x, labelPos.y);
	}


	virtual void set() {}
	virtual float getVal() {	return 1;}

	virtual void draw() {
		ofSetColor(color);
		ofDrawRectangle(rect);
		drawLabel();
	}

	virtual void checkMouse(int x, int y) {
		if (rect.inside(x,y)) {
			cout << "inside " + name << endl;
		}
	}
};


// virtual function table pra fazer o draw de cada um elemento
class slider : public element {
public:
	//string name;
	float val;

	slider(string n, int x, int y, float v) : val(v)  {
		name = n;
		setRect(x,y,200,20);
		labelPos.x = x + 5;
		labelPos.y = y + 16;
		labelColor = ofColor(0);
	}

	void set(int v) {
		val = v;
		redraw = true;
	}

	float getVal() {
		return val;
	}

	void drawLabel() {
		ofSetColor(labelColor);
		ofDrawBitmapString(name + " " + ofToString(val), labelPos.x, labelPos.y);
	}

	void draw() {
		clear();
		ofSetColor(color);
		ofDrawRectangle(rect);
		float w = rect.width * val;
		ofSetColor(0,90);
		ofDrawRectangle(rect.x, rect.y, w, rect.height);
		drawLabel();
	}

	void setValFromMouse(int x, int y) {
		int xx = ofClamp(x, rect.x, rect.x + rect.width);
		val = float(xx-rect.x)/(float) rect.width;
		redraw = true;
	}

	void checkMouse(int x,int y) {
		if (rect.inside(x,y)) {
			setValFromMouse(x,y);
			isPressed = true;
		} else {
			if (isPressed) {
				setValFromMouse(x,y);
				cout << "last captured value :: " + ofToString(val) << endl;
				// ispressed nao é redraw.. ?
				draw();

				settings->redraw = true;
				isPressed = false;
			}
		}
	}
};


class label : public element {
public:
	label(string n, int x, int y) {
		name = n;
		labelPos.x = x + 5;
		labelPos.y = y + 16;
	}
};


class toggle : public element {
public:
	bool val;
	ofRectangle checked;

	toggle(string n, int x, int y, bool v) : val(v) {
		name = n;
		setRect(x,y,20,20);
		float margem = 4;
		checked = ofRectangle(
			rect.x + margem, rect.y + margem,
			rect.width-margem*2, rect.height-margem*2
		);

//		ofRectangle r = getBitmapStringBoundingBox(name);
//		int w = 25 +
		setActiveRect(x,y,200,20);
		labelPos.x = x + 25;
		labelPos.y = y + 16;
	}

	void set(bool v) {
		val = v;
		redraw = true;
	}

	void checkMouse(int x,int y) {
		if (activeRect.inside(x,y)) {
			if (!isPressed) {
				val ^= 1;
				redraw = true;
				if (settings != NULL) {
					settings->pBool[name] = val;
					//cout << "not null" << endl;
					string s = name + " :: " + (val ? "true" : "false");
					ofNotifyEvent(settings->uiEvent, s);
				}
				isPressed = true;
			}
		} else {
			if (isPressed) { isPressed = false; }
		}

	}

	void draw() {
		ofSetColor(color);
		ofDrawRectangle(rect);
		if (val) {
			ofSetColor(0, 127);
			ofDrawRectangle(checked);
		}
		ofSetColor(255,30);
		ofDrawRectangle(activeRect);
		drawLabel();
	}
};

