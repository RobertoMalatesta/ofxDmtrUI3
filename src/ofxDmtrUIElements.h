

class element {
protected:
	ofColor color = ofColor(255);
	ofRectangle activeRect;
	int min;
	int max;
	ofRectangle rect;
	string name;

	// as vezes é igual, as vezes diferente.
	// fazer ou nao aqui uma string label?

public:
	//int val;

	element() {} // constructor
	~element() {} // destructor
//	element(string n, int v, int x, int y) : name(n), val(v) {
//		rect.width = 200;
//		rect.height = 20;
//		rect.x = x;
//		rect.y = y;
//	}

	void setColor(ofColor c) {
		color = c;
	}
	// The word virtual means ‘‘may be redefined later in a class
	// derived from this one’’ in Simula and C++

	
	virtual void draw() {
		ofSetColor(color);
		ofDrawRectangle(rect);
	}
};


// virtual function table pra fazer o draw de cada um elemento
class slider : public element {
public:
	//string name;
	float val;
	slider(string n, float v, int x, int y)  {
		name = n;
		rect.width = 200;
		rect.height = 20;
		rect.x = x;
		rect.y = y;
	}
	
	void draw() {
		ofSetColor(color);
		ofDrawRectangle(rect);
	}

//	slider(string n, int v, int x, int y) : name(n), val(v) {
//	}
};

class toggle : public element {
public:

	toggle(string n, float v, int x, int y) {
		name = n;
		rect.width = 20;
		rect.height = 20;
		rect.x = x;
		rect.y = y;
	}

	void draw() {
		ofSetColor(color);
		ofDrawRectangle(rect);
		ofSetColor(255);
		ofDrawBitmapString(name, rect.x, rect.y + 10);
	}
};