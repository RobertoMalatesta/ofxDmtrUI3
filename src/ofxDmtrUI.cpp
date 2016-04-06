/*

ofxDmtrUI.h
Created by Dimitre Lima on 04/06/2016.
Proof of concept

*/

#include "ofxDmtrUI.h"


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
		for (auto & t : toggles) { t.draw(); }
		for (auto & s : sliders) { s.draw(); }
		for (auto & l : labels)  { l.draw(); }
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
		// fazer uma funcao pra calcular o valorpixels atraves do valor.
		//s.valor = settings.getValue(s.nome, 0.0);
		s.setValue(settings.getValue(s.nome, 0.0));
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
	// fazer funcao loadorsave, substituir aqui
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
// remove in next versions
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

	flow.x = 150;
	flow.y = 40;
	for (int a=0; a<20; a++) {
		create("Slider" + ofToString(a));
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::create(string nome, string tipo, ofVec3f valores) {
	int hue = int(flow.x + flow.y/6.0)%255;
	int height = 20;
	int sliderWidth = 150;

	if (tipo == "slider") {
		slider ts;
		ts.nome = nome;
		ts.rect = ofRectangle(flow.x, flow.y, sliderWidth, height);
		ts.cor = ofColor::fromHsb(hue,255,255);
		ts.val = &pFloat[ts.nome];
		ts.min = valores.x;
		ts.max = valores.y;
		ts.def = ts.valor = valores.z;
		sliders.push_back(ts);
	}

	else if (tipo == "toggle") {
		toggle tt;
		tt.nome = nome;
		tt.rect = ofRectangle(flow.x, flow.y, height, height);
		tt.cor = ofColor::fromHsb(hue,255,255);
		toggles.push_back(tt);
	}

	else if (tipo == "label") {
		label tl;
		tl.nome = nome;
		tl.rect = ofRectangle(flow.x, flow.y, sliderWidth, height);
		tl.cor = ofColor::fromHsb(hue,255,255);
		labels.push_back(tl);
	}

	flow.y += 25;
}

//--------------------------------------------------------------
void ofxDmtrUI::createFromText(string file) {
	vector <string> linhas = textToVector(file);
	for (auto & l : linhas) {
		if (l == "") { // spacer
			flow.y += 25;
		} else {
			vector <string> cols = ofSplitString(l, "	");
			string tipo = cols[0];
			string nome = cols[1];

			ofVec3f valores = ofVec3f(0,1,0);

			if (cols.size()>2) {
				vector <string> vals = ofSplitString(cols[2], " ");
				// min max def.
				valores = ofVec3f(ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2]));
			}
			if (tipo == "float") tipo = "slider";
			if (tipo == "bool") tipo = "toggle";

			if (tipo == "newcol") {
				flow.x += 180;
				flow.y = marginy;
			}
			else if (tipo == "marginy") {
				marginy = ofToFloat(cols[1]);
				flow.y = marginy;
			}

			else {
				create(nome, tipo, valores);
			}
		}
	}
}


//--------------------------------------------------------------
vector <string> ofxDmtrUI::textToVector(string file) {
	vector <string> saida;
	ofBuffer buff2 = ofBufferFromFile(file);
	for(auto & line: buff2.getLines()) {
		saida.push_back(line);
	}
	return saida;
}