/**********************************************************************************

 Copyright (C) 2016 Dimitre Lima (www.dmtr.org)

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

/*

ofxDmtrUI.h
Created by Dimitre Lima on 04/06/2016.
http://dmtr.org/

*/

#include "ofxDmtrUI3.h"

//--------------------------------------------------------------
ofxDmtrUI3::~ofxDmtrUI3() {
	//cout << "destruct" << endl;
	// remove listeners here

	ofRemoveListener(ofEvents().draw, this, &ofxDmtrUI3::onDraw);
	ofRemoveListener(ofEvents().update, this, &ofxDmtrUI3::onUpdate);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxDmtrUI3::onMousePressed);
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxDmtrUI3::onMouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxDmtrUI3::onMouseReleased);
	ofRemoveListener(ofEvents().exit, this, &ofxDmtrUI3::onExit);
	ofRemoveListener(settings.uiEvent,this, &ofxDmtrUI3::uiEvents);
}

//--------------------------------------------------------------
void ofxDmtrUI3::setup() {

	settings.pFloat = &pFloat;
	settings.pBool = &pBool;

	fboUI.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);
	fboUI.begin();
	ofClear(0,0);
	fboUI.end();
	
	ofAddListener(ofEvents().draw, this, &ofxDmtrUI3::onDraw);
	//ofAddListener(ofEvents().update, this, &ofxDmtrUI3::onUpdate);
	ofAddListener(ofEvents().mousePressed, this, &ofxDmtrUI3::onMousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofxDmtrUI3::onMouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxDmtrUI3::onMouseReleased);
	ofAddListener(ofEvents().exit, this, &ofxDmtrUI3::onExit);
	ofAddListener(ofEvents().windowResized, this, &ofxDmtrUI3::onWindowResized);

	ofAddListener(settings.uiEvent,this, &ofxDmtrUI3::uiEvents);

}
// END SETUP

//--------------------------------------------------------------
void ofxDmtrUI3::update() {
}

//--------------------------------------------------------------
void ofxDmtrUI3::draw() {
	ofPushStyle();
	if (settings.redraw) {
		fboUI.begin();
		ofClear(0);
		for (auto & e : elements) {
			if (e->redraw) {
				e->draw();
			}
			//cout << typeid(e).name() << endl;
		}
		fboUI.end();
		//cout << "redraw" + ofToString(ofRandom(0,1)) << endl;
	}


	if (settings.redraw || !onlyDrawOnRedraw) {
		ofSetColor(255);
		fboUI.draw(0,0);
	}

	if (settings.redraw) {
		settings.redraw = false;
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxDmtrUI3::createFromText(string file) {
	if (ofFile::doesFileExist(file)) {
		vector <string> linhas = textToVector(file);
		for (auto & l : linhas) {
			createFromLine(l);
		}
	} else {
		cout << "ofxDmtrUI createFromText ::: File not found " + file << endl;
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::createFromLine(string l) {

	if (l == "") { // spacer
		settings.newLine();
		//flow.y += sliderHeight + sliderMargin;
	} else {
		vector <string> cols = ofSplitString(l, "	");
		string tipo = cols[0];
		if (cols.size() == 1) {
			if (tipo == "newcol" || tipo == "newCol") {
				settings.newCol();
//				flow.x += 220;
//				flow.y = 10;
			}
		}
		else {
			string nome = cols[1];
			string valores;
			if (cols.size()>2) {
				valores = cols[2];
			}

			if (tipo == "float") tipo = "slider";
			if (tipo == "bool") tipo = "toggle";

			if (tipo == "slider" || tipo == "int" || tipo == "sliderVert") {
				vector <string> v = ofSplitString(valores, " ");
				float val = ofToFloat(v[2]);
				// remover
				//settings.flow = flow;
				elements.push_back(new slider(nome, settings, val));
			}

			else if (tipo == "label") {
//				elements.push_back(new label(nome, flow.x, flow.y));
				elements.push_back(new label(nome, settings));
			}

			else if (tipo == "toggle") {
				bool val = valores == "1";
				// remover em breve
				//settings.flow = flow;
				elements.push_back(new toggle(nome, settings, val));
			}

			else if (tipo == "ints" || tipo == "floats" || tipo == "bools" || tipo == "bangs" || tipo == "holds" || tipo == "colors" || tipo == "slider2ds") {
				vector <string> nomes = ofSplitString(nome, "[");
				string n = nomes[0];
				string intervalo = ofSplitString(nomes[1], "]")[0];
				int start = ofToInt(ofSplitString(intervalo, "-")[0]);
				int end = ofToInt(ofSplitString(intervalo, "-")[1]);
				for (int a=start; a<=end; a++) {

					// lacks elegancy
					string newTipo = tipo == "ints" ? "int" : "float";
					if (tipo == "bools") newTipo = "bool";
					if (tipo == "bangs") newTipo = "bang";
					if (tipo == "holds") newTipo = "hold";
					if (tipo == "colors") newTipo = "color";
					if (tipo == "slider2ds") newTipo = "slider2d";
					createFromLine(newTipo + "	"+n + ofToString(a)+"	"+valores);
				}
			}

			//flow.y += sliderHeight + sliderMargin;
		}
	}
}

//--------------------------------------------------------------
vector <string> ofxDmtrUI3::textToVector(string file) {
	vector <string> saida;
	ofBuffer buff2 = ofBufferFromFile(file);
	for(auto & line: buff2.getLines()) {
		saida.push_back(line);
	}
	return saida;
}

//--------------------------------------------------------------
void ofxDmtrUI3::onDraw(ofEventArgs &data) {
	draw();
}

//--------------------------------------------------------------
void ofxDmtrUI3::onUpdate(ofEventArgs &data) {
	update();
}


//--------------------------------------------------------------
void ofxDmtrUI3::onMousePressed(ofMouseEventArgs& data) {
	for (auto & e : elements) {
		e->checkMouse(data.x, data.y);
	}
	settings.redraw = true;
}

//--------------------------------------------------------------
void ofxDmtrUI3::onMouseDragged(ofMouseEventArgs& data) {
	for (auto & e : elements) {
		e->checkMouse(data.x, data.y);
	}
	settings.redraw = true;
}

//--------------------------------------------------------------
void ofxDmtrUI3::onMouseReleased(ofMouseEventArgs& data) {
	for (auto & e : elements) {
		e->isPressed = false;
	}
	settings.redraw = true;
}

////--------------------------------------------------------------
//void ofxDmtrUI3::onMouseMoved(ofMouseEventArgs& data) {
//}

//--------------------------------------------------------------
void ofxDmtrUI3::uiEvents(string & e) {
	//cout << e << endl;
}

auto ofxDmtrUI3::getVal(string n) {
	for (auto & e : elements) {
		if (e->name == n) {
			return e->getVal();
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::onExit(ofEventArgs &data) {
	//cout << "onexit dmtrui3" << endl;
	save("default.xml");
}

//--------------------------------------------------------------
void ofxDmtrUI3::onWindowResized(ofResizeEventArgs &data) {
	reFlow();
}

//--------------------------------------------------------------
void ofxDmtrUI3::save(string xml) {
	//cout << "save file :: " + xml << endl;
	ofxXmlSettings xmlSettings;
	xmlSettings.setValue("ofxDmtrUIVersion", 3.0);
	for (auto & e : elements) {
		//cout << typeid(e->getVal()).name() << endl;
		xmlSettings.setValue("element:" + e->name, e->getVal());
	}
	xmlSettings.save(xml);
}

//--------------------------------------------------------------
void ofxDmtrUI3::load(string xml) {
	ofxXmlSettings xmlSettings;
	xmlSettings.loadFile(xml);
	//cout << "load file :: " + xml << endl;

	int UIVersion = xmlSettings.getValue("ofxDmtrUIVersion", 0);
	for (auto & e : elements) {
		string tagName = "element:" + e->name;
		if (xmlSettings.tagExists(tagName)) {
			auto valor = xmlSettings.getValue("element:" +e->name, e->getVal());
			e->set(valor);
//				cout << e->name + " :: " + ofToString( valor) << endl;
//				cout << typeid(e).name() << endl;
			//e->set(xmlSettings.getValue(tagName, e->getVal()));
		}
	}
	settings.redraw = true;
}

//--------------------------------------------------------------
void ofxDmtrUI3::reFlow() {
	cout << "==== reflow" << endl;
	settings.flow = settings.margin;
	for (auto & e : elements) {
		e->getProperties();
	}
	fboUI.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);
	fboUI.begin();
	ofClear(0,0);
	fboUI.end();
	settings.redraw = true;
}