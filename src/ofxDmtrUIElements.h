//#pragma once

struct uiConfig {
public:
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
		cout << "newcol" << endl;
		flow.x += sliderDimensions.x + margin.x;
		flow.y =  margin.y;
	}
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

	void getProperties() {
		color = settings->color;
		settings->update(rect.height);

		// AUTOFLOW
		int altura = ofGetWindowHeight() - settings->margin.y*2;
		if (rect.y + rect.height > altura) {
			settings->newCol();
			rect.x = settings->flow.x;
			rect.y = settings->flow.y;
		}
	}

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


	virtual void set(float i) {
		cout << "set function on primitive element" << endl;
	}
//	virtual void set(bool i) {
//		cout << "set function on primitive element" << endl;
//	}
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

	slider(string n, uiConfig & u, float v) {
		settings = &u;
		name = n;
		// fazer um rect la dentro dos settings e copiar. q lindo.
		setRect(settings->flow.x, settings->flow.y, 200,20);
		labelPos.x = rect.x + 5;
		labelPos.y = rect.y + 16;
		labelColor = ofColor(0);
		getProperties();
		set(v);
	}

	// slider
	void set(float v) {
		val = v;
		//cout << "set slider " + name + " to val : " + ofToString(val) << endl;
		redraw = true;
		//(*settings->pFloat)[name] = val;
		(*settings->pFloat)[name] = val;
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
		set(float(xx-rect.x)/(float) rect.width);
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


class toggle : public element {
public:
	bool val;
	ofRectangle checked;

	toggle(string n, uiConfig & u, bool v) {
		settings = &u;
		int x = settings->flow.x;
		int y = settings->flow.y;
		name = n;
		setRect(x,y,20,20);
		float margem = 4;
		checked = ofRectangle(
			rect.x + margem, rect.y + margem,
			rect.width-margem*2, rect.height-margem*2
		);
		setActiveRect(x,y,200,20);
		labelPos.x = x + 25;
		labelPos.y = y + 16;
		getProperties();
		set(v);
	}

	void set(bool v) {
		val = v;
		(*settings->pBool)[name] = val;
		redraw = true;
		string s = name + " :: " + (val ? "true" : "false");
		ofNotifyEvent(settings->uiEvent, s);
	}

	void checkMouse(int x,int y) {
		if (activeRect.inside(x,y)) {
			if (!isPressed) {
				set(val ^ 1);
				//val ^= 1;

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




class label : public element {
public:
	label (string n, uiConfig & u) {
		name = n;
		int x = settings->flow.x;
		int y = settings->flow.y;
		labelPos.x = x + 5;
		labelPos.y = y + 16;
	}

//	label(string n, int x, int y) {
//		name = n;
//		labelPos.x = x + 5;
//		labelPos.y = y + 16;
//	}
};

