/*

ofxDmtrUI.h
Created by Dimitre Lima on 04/06/2016.
Proof of concept

*/

#include "ofxDmtrUI.h"
//ofRectangle coluna = ofRectangle(30,30,500,700);

// in the future measure events considering column offset

//--------------------------------------------------------------
void ofxDmtrUI::setup() {

	fbo.allocate(coluna.width, coluna.height, GL_RGBA);
	fbo.begin();
	ofClear(0);
	fbo.end();

	ofAddListener(ofEvents().draw, this, &ofxDmtrUI::onDraw);
	ofAddListener(ofEvents().update, this, &ofxDmtrUI::onUpdate);
	ofAddListener(ofEvents().keyPressed, this, &ofxDmtrUI::onKeyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxDmtrUI::onKeyReleased);
	ofAddListener(ofEvents().mousePressed, this, &ofxDmtrUI::onMousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofxDmtrUI::onMouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxDmtrUI::onMouseReleased);

	ofAddListener(ofEvents().mouseMoved, this, &ofxDmtrUI::onMouseMoved);
	ofAddListener(ofEvents().exit, this, &ofxDmtrUI::onExit);

}
// END SETUP

//--------------------------------------------------------------
void ofxDmtrUI::update() {
	for (auto & p : pFloat) {
		if (easing > 0) {
			pEasy[p.first] += (pFloat[p.first] - pEasy[p.first])/easing;
		}
		else {
			pEasy[p.first] = pFloat[p.first];
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::draw() {
	if (redraw) {
		fbo.begin();
		ofClear(0,100);
		for (auto & t : toggles) {
			ofSetColor(t.cor);
			ofDrawRectangle(t.rect);

			// toggle Label
//			ofSetColor(255);
//			ofDrawBitmapString(t.nome, t.rect.x+30, t.rect.y+14);

			if (t.valor) {
				ofSetColor(0);
				ofNoFill();
				int off = 3;
				ofDrawLine(t.rect.x + off, t.rect.y + off, 				   t.rect.x + t.rect.width -off, t.rect.y + t.rect.height -off);
				ofDrawLine(t.rect.x + off, t.rect.y + t.rect.height - off, t.rect.x + t.rect.width -off, t.rect.y + off);
				ofFill();

				// just to check if t.inside is working OK
//					if (t.inside) {
//						float raio = 5;
//						ofDrawEllipse(t.rect.x + 5, t.rect.y + 5, raio, raio);
//					}
			}
		}

		for (auto & s : sliders) {
			ofSetColor(s.cor);
			ofDrawRectangle(s.rect);
			ofSetColor(0,128);
			float w = s.rect.width * s.valor;
			ofDrawRectangle(s.rect.x, s.rect.y, w, s.rect.height);
			string label = s.nome + " "+ofToString(s.valor);
			ofSetColor(0,128);
			ofDrawBitmapString(label, s.rect.x+11, s.rect.y+15);
			ofSetColor(255);
			ofDrawBitmapString(label, s.rect.x+10, s.rect.y+14);
		}

//			float y = 0;
//			for (auto & p : pFloat) {
//				string frase = p.first + " :: " + ofToString(p.second);
//				ofDrawBitmapString(frase, 300, y+=20);
//			}

		fbo.end();
	}



	if (showGui) {
		//ofSetColor(255, columnOver ? 255 : 30);
		ofSetColor(255);
		fbo.draw(coluna.x, coluna.y);
	}
}


//--------------------------------------------------------------
void ofxDmtrUI::save(string xml){
	cout << "save: " + xml << endl;
	ofxXmlSettings settings;
	for (auto & s : sliders) {
		settings.setValue(s.nome, s.valor);
	}
	for (auto & t : toggles) {
		settings.setValue(t.nome, t.valor);
	}
	settings.save(xml);
}

//--------------------------------------------------------------
void ofxDmtrUI::load(string xml){
	cout << "load: " + xml << endl;

	ofxXmlSettings settings;
	settings.loadFile(xml);
	for (auto & s : sliders) {
		s.valor = settings.getValue(s.nome, 0.0);
		pFloat[s.nome] = s.valor;
	}
	for (auto & t : toggles) {
		t.valor = settings.getValue(t.nome, 0);
		pBool[t.nome] = t.valor;
	}
	redraw = true;
}

//--------------------------------------------------------------
void ofxDmtrUI::keyPressed(int key){
	if (key == '=') {
		showGui = !showGui;
	}

	// Temporary Shortcuts
	if (key == '1') {
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			save("1.xml");
		} else {
			load("1.xml");
		}
	}
	else if (key == '2') {
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			save("2.xml");
		} else {
			load("2.xml");
		}
	}
	else if (key == '3') {
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			save("3.xml");
		} else {
			load("3.xml");
		}
	}
	else if (key == '4') {
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			save("4.xml");
		} else {
			load("4.xml");
		}
	}

	else if (key == '5') {
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			save("5.xml");
		} else {
			load("5.xml");
		}
	}

	else if (key == '6') {
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			save("6.xml");
		} else {
			load("6.xml");
		}
	}

	else if (key == '7') {
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			save("7.xml");
		} else {
			load("7.xml");
		}
	}

	else if (key == '8') {
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			save("8.xml");
		} else {
			load("8.xml");
		}
	}
}


//--------------------------------------------------------------
void ofxDmtrUI::keyReleased(int key){
}


//--------------------------------------------------------------
void ofxDmtrUI::mouseDragged(int x, int y, int button){
	redraw = true;


	for (auto & t : toggles) {
		if (t.rect.inside(x,y) && !t.inside) {
			t.valor = !t.valor;
			t.inside = true;
			pBool[t.nome] = t.valor;
		}
	}

	for (auto & s : sliders) {
		if (s.rect.inside(x,y)) {
			s.update(x);
			s.inside = true;
			//s.valor = (x - s.rect.x)/(double)s.rect.width;
			pFloat[s.nome] = s.valor;
		} else {
			if (s.inside) {
				s.update(x);
				s.inside = false;
			}
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::mousePressed(int x, int y, int button){
	redraw = true;

	for (auto & t : toggles) {
		if (t.rect.inside(x,y)&& !t.inside) {
			t.valor = !t.valor;
			t.inside = true;
			pBool[t.nome] = t.valor;
		}
	}

	for (auto & s : sliders) {
		if (s.rect.inside(x,y)) {
			s.update(x);
			s.inside = true;
			pFloat[s.nome] = s.valor;
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::mouseReleased(int x, int y, int button){
	redraw = false;
	for (auto & t : toggles) {
		t.inside = false;
	}
	for (auto & s : sliders) {
		s.inside = false;
	}
}


//--------------------------------------------------------------
void ofxDmtrUI::mouseAll(int x, int y, int button){
	columnOver = coluna.inside(x,y);
}


//--------------------------------------------------------------
void ofxDmtrUI::exit() {
}


//--------------------------------------------------------------
void ofxDmtrUI::onDraw(ofEventArgs &data) {
	draw();
}

//--------------------------------------------------------------
void ofxDmtrUI::onUpdate(ofEventArgs &data) {
	update();
}

//--------------------------------------------------------------
void ofxDmtrUI::onKeyPressed(ofKeyEventArgs& data)
{
	keyPressed(data.key);
}

//--------------------------------------------------------------
void ofxDmtrUI::onKeyReleased(ofKeyEventArgs& data)
{
	keyReleased(data.key);
}

//--------------------------------------------------------------
void ofxDmtrUI::onMousePressed(ofMouseEventArgs& data)
{
	mousePressed(data.x, data.y, data.button);
	mouseAll(data.x, data.y, data.button);
}

//--------------------------------------------------------------
void ofxDmtrUI::onMouseDragged(ofMouseEventArgs& data)
{
	mouseDragged(data.x, data.y, data.button);
	mouseAll(data.x, data.y, data.button);
}

//--------------------------------------------------------------
void ofxDmtrUI::onMouseReleased(ofMouseEventArgs& data)
{
	mouseReleased(data.x, data.y, data.button);
	mouseAll(data.x, data.y, data.button);
}

//--------------------------------------------------------------
void ofxDmtrUI::onMouseMoved(ofMouseEventArgs& data)
{
	mouseAll(data.x, data.y, data.button);
}

//--------------------------------------------------------------
void ofxDmtrUI::onExit(ofEventArgs &data)
{
	exit();
}






// function only used to generate UI Elements
//--------------------------------------------------------------
void ofxDmtrUI::generate(string text) {
	int y = 20;
	for (int b=0; b<4; b++) {
		for (int a=0; a<10; a++) {
			toggle tt;
			tt.nome = "Toggle" + ofToString(a) + ofToString(b);
			float altura = 20;
			float altura2 = altura*1.25;
			float w = 20;
			tt.rect = ofRectangle(10 + b *(altura*1.25), 40 + a*(altura*1.25), w,altura); //(y+=altura2)
			tt.cor = ofColor::fromHsb(a*4+b*5,255,255);
			toggles.push_back(tt);
		}
	}

	for (int a=0; a<20; a++) {
		// temporary slider to push
		slider ts;
		ts.nome = "Slider" + ofToString(a);
		float altura = 20;
		float w = 150;
		ts.rect = ofRectangle(150, 40 + a*(altura*1.25),w,altura);
		ts.cor = ofColor::fromHsb(ofMap(a, 0,30, 0,200),255,255);
		ts.val = &pFloat[ts.nome];
		sliders.push_back(ts);
	}
}



//--------------------------------------------------------------
void ofxDmtrUI::create(string nome, string tipo) {
	if (tipo == "slider") {
		slider ts;
		ts.nome = nome;
		ts.rect = ofRectangle(flow.x, flow.y,150,20);
		ts.cor = ofColor::fromHsb(ofRandom(200),255,255);
		ts.val = &pFloat[ts.nome];
		sliders.push_back(ts);
	}

	else if (tipo == "toggle") {
		toggle tt;
		tt.nome = nome;
		tt.rect = ofRectangle(flow.x, flow.y, 20, 20);
		tt.cor = ofColor::fromHsb(ofRandom(200),255,255);
		toggles.push_back(tt);
	}

	flow.y += 30;
}