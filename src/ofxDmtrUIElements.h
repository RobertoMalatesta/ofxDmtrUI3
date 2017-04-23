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
	ofRectangle rect, activeRect;
	ofPoint labelPos;
	uiConfig * settings = NULL;
public:
	elementType kind = SLIDER;
	bool isPressed = false;
	bool redraw = true;
	string name;
	//ofEvent*<string> uiEvent;

	virtual void updateToggle() {}

	void getProperties() {
		int x = settings->flow.x;
		int y = settings->flow.y;
		setActiveRect(x,y,settings->sliderDimensions.x, settings->sliderDimensions.y);
		color = settings->color;
		rect.x = settings->flow.x;
		rect.y = settings->flow.y;
		rect.width = kind == TOGGLE ? settings->sliderDimensions.y : settings->sliderDimensions.x;
		rect.height = settings->sliderDimensions.y;

		// AUTOFLOW
		int altura = ofGetWindowHeight() - settings->margin.y*2;
		if (rect.y + rect.height > altura) {
			settings->newCol();
			rect.x = settings->flow.x;
			rect.y = settings->flow.y;
		}

		if (kind == SLIDER) {
			labelPos.x = rect.x + 5;
			labelPos.y = rect.y + 16;
			labelColor = ofColor(0);
			activeRect = rect;
		}

		else if (kind == TOGGLE) {
			labelPos.x = rect.x + 25;
			labelPos.y = rect.y + 16;
			updateToggle();
			//int w = getBitmapStringBoundingBox(name) + labelPos.x + 4*2;
			//activeRect.width = w;
		}

		settings->update(rect.height);
	}

	void addSettings (uiConfig & u) {
		settings = &u;
	}

	element() {}
	~element() {}

	virtual void clear() {
		ofSetColor(0,0);
		ofDrawRectangle(activeRect);
	}

	void setActiveRect(int x, int y, int w, int h) {
		activeRect.x = x; activeRect.y = y;
		activeRect.width = w; activeRect.height = h;
	}

	void setColor(ofColor c) {
		color = c;
	}

	virtual void drawLabel() {
		ofSetColor(labelColor);
		ofDrawBitmapString(name, labelPos.x, labelPos.y);
	}

	virtual void set(float i) {
		cout << "set function on primitive element " + name << endl;
	}

//	virtual void set(bool i) {
//		cout << "set function on primitive element" << endl;
//	}

	virtual float getVal() {	 return 1; }

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


class slider : public element {
public:
	string label;
	float min = 0;
	float max = 1;
	float val = .5;

//	slider(string n, uiConfig & u, float v = .5) {
//		settings = &u;
//		name = n;
//		getProperties();
//		set(v);
//	}

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
		redraw = true;
		(*settings->pFloat)[name] = val;
		// ofEvent here, criar um evento na classe pai.
	}

	float getVal() {
		return val;
	}

	void drawLabel() {
		ofSetColor(labelColor);
		ofDrawBitmapString(label, labelPos.x, labelPos.y);
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

	void checkMouse(int x,int y) {
		if (rect.inside(x,y)) {
			setValFromMouse(x,y);
			isPressed = true;
		} else {
			if (isPressed) {
				setValFromMouse(x,y);
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
	// esse vai pro pai e vira outro nome
	ofRectangle rectChecked;

	toggle(string n, uiConfig & u, bool v) {
		kind = TOGGLE;
		settings = &u;
		int x = settings->flow.x;
		int y = settings->flow.y;
		name = n;
		getProperties();
		updateToggle();
		set(v);
	}

	void updateToggle() {
		float margem = 4;
		rectChecked = ofRectangle(
		  rect.x + margem, rect.y + margem,
		  rect.width-margem*2, rect.height-margem*2
		  );
	}

	//void set(int v) {
	void set(bool v) {
		cout << "void set in bool " + name + "::" + ofToString(v) << endl;
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

	float getVal() {
		return val;
	}

	void draw() {
		ofSetColor(color);
		ofDrawRectangle(rect);
		if (val) {
			// transformar em color active, algo em settings talvez.
			ofSetColor(0, 127);
			ofDrawRectangle(rectChecked);
		}
		ofSetColor(255,30);
		ofDrawRectangle(activeRect);
		drawLabel();
	}
};



class label : public element {
public:
	label (string n, uiConfig & u) {
		kind = LABEL;
		settings = &u;
		name = n;
		int x = settings->flow.x;
		int y = settings->flow.y;
		labelPos.x = x + 5;
		labelPos.y = y + 16;
		getProperties();
	}
};

