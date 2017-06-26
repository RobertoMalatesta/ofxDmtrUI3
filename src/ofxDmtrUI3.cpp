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

/*

ofxDmtrUI3.h
Created by Dimitre Lima on 17/06/2017.
http://dmtr.org/

*/

#include "ofxDmtrUI3.h"

ofFbo::Settings fboSettings;

//--------------------------------------------------------------
void ofxDmtrUI3::setup() {

//	fboSettings.width = ofGetWindowWidth();
//	fboSettings.height = ofGetWindowHeight();
	fboSettings.width = 10;
	fboSettings.height = 10;
	fboSettings.internalformat = GL_RGBA;
//	fboSettings.numSamples = 0;
//	fboSettings.useDepth = false;
//	fboSettings.useStencil = false;

	string vertShader =
	R"(
	varying vec2 texcoord0;
	void main() {
		gl_Position = ftransform();
		texcoord0 = vec2(gl_TextureMatrix[0] * gl_MultiTexCoord0);
	}
	)";

	string fragShader =
	R"(
	uniform int x, y, w, h;
	uniform sampler2DRect tex0;
	uniform sampler2DRect mask0;
	varying vec2 texcoord0;

	void main (void)
	{
		vec4 cor = texture2DRect(tex0, texcoord0);
//		vec4 cor = texture2DRect(tex0, texcoord0).rgb;
//		cor.a = texture2DRect(mask0, texcoord0).r;

		vec2 xy = gl_FragCoord.xy;
		int xx = int(xy.x);
		int yy = int(xy.y);
		bool limpa = false;
		if (xx > x && yy > y) {
			if (xx < (x+w) && yy < (y+h)) {
				limpa = true;
			}
		}
		gl_FragColor = limpa ? vec4(0.0, 0.0, 0.0, 0.0) : cor;
	}
	)";

	shader.setupShaderFromSource( GL_VERTEX_SHADER, vertShader );
	shader.setupShaderFromSource( GL_FRAGMENT_SHADER, fragShader );
	shader.bindDefaults();
	shader.linkProgram();


	//cout << "setup" << endl;
	settings.pFloat	 	= &pFloat;
	settings.pInt	 	= &pInt;
	settings.pBool 		= &pBool;
	settings.pString	 	= &pString;
	settings.pPoint 		= &pPoint;

//	fboUI.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);
	fboUI.allocate(fboSettings);

	fboClear();

//	fboUI2.allocate(fboSettings);
//	fboUI2.begin();
//	ofClear(0,0);
//	fboUI2.end();

	ofAddListener(ofEvents().draw, this, &ofxDmtrUI3::onDraw);
	//ofAddListener(ofEvents().update, this, &ofxDmtrUI3::onUpdate);
	ofAddListener(ofEvents().mousePressed, this, &ofxDmtrUI3::onMousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofxDmtrUI3::onMouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxDmtrUI3::onMouseReleased);
	ofAddListener(ofEvents().keyPressed, this, &ofxDmtrUI3::onKeyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxDmtrUI3::onKeyReleased);
	ofAddListener(ofEvents().exit, this, &ofxDmtrUI3::onExit);
	ofAddListener(ofEvents().windowResized, this, &ofxDmtrUI3::onWindowResized);

	ofAddListener(settings.uiEvent,this, &ofxDmtrUI3::uiEvents);
	ofAddListener(settings.uiEventString,this, &ofxDmtrUI3::uiEventsString);


}
// END SETUP

ofxDmtrUI3::ofxDmtrUI3() {
	setup();
}

//--------------------------------------------------------------
ofxDmtrUI3::~ofxDmtrUI3() {
	//cout << "destruct" << endl;
	// remove listeners here

	ofRemoveListener(ofEvents().draw, this, &ofxDmtrUI3::onDraw);
	ofRemoveListener(ofEvents().update, this, &ofxDmtrUI3::onUpdate);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxDmtrUI3::onMousePressed);
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxDmtrUI3::onMouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxDmtrUI3::onMouseReleased);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxDmtrUI3::onKeyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxDmtrUI3::onKeyReleased);
	ofRemoveListener(ofEvents().exit, this, &ofxDmtrUI3::onExit);
	ofRemoveListener(ofEvents().windowResized, this, &ofxDmtrUI3::onWindowResized);

	ofRemoveListener(settings.uiEvent,this, &ofxDmtrUI3::uiEvents);
	ofRemoveListener(settings.uiEventString,this, &ofxDmtrUI3::uiEventsString);
}

//--------------------------------------------------------------
void ofxDmtrUI3::update() {
}

//--------------------------------------------------------------
void ofxDmtrUI3::draw() {
	if (settings.needsRedraw) {
		fboUI.begin();
		for (auto & e : elements) {
			if (e->redraw) {
				//cout << "redraw :: " + e->name << endl;
				ofSetColor(255, 1);
				ofDrawRectangle(e->boundsRect);
				e->draw();
				e->redraw = false;
			}
		}
//		ofSetColor(255,0,0,40);
//		ofDrawRectangle(settings.rect);
		fboUI.end();
		settings.needsRedraw = false;
	}


	// lately we never redraw fully, only on allocate (autofit?)
	if (settings.redraw) {
		cout << "SETTINGS redraw :: " + UINAME << endl;

		fboClear();

		fboUI.begin();
		ofSetColor(255);
		for (auto & e : elements) {
			e->draw();
		}
		fboUI.end();
		settings.redraw = false;
	}
	ofSetColor(255, settings.opacity);

	fboUI.draw(settings.rect.x, settings.rect.y);
}


//--------------------------------------------------------------
void ofxDmtrUI3::keyPressed(int key){

	// bad access
	if (key == 'a') {
		if (UINAME == "master") {
			getElement("allPresets")->set("3");
		}
	}

	if ((key == 'f' || key == 'F')) {
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			ofToggleFullscreen();
			// falta um needsredraw por aqui
		}
	}

	if (key == '1' || key == '2' || key == '3' || key == '4' ) {
		string nome = ofToString(char(key)) + ".xml";
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			save(nome);
		} else {
			load(nome);
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::keyReleased(int key){
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
	} else {
		vector <string> cols = ofSplitString(l, "	");
		string tipo = cols[0];
		if (cols.size() == 1) {
			if (tipo == "newcol" || tipo == "newCol") {
				settings.newCol();
			}

			else if (tipo == "autoFit") {
				autoFit();
			}

			else if (tipo == "presets") {
				elements.push_back(new presets("allPresets", settings));
				cout << elements.back()->boundsRect << endl;
			}

			else if (tipo == "flowVert") {
				settings.setFlowVert(true);
//				settings.flowVert = true;
//				settings.newLine();
			}

			else if (tipo == "flowHoriz") {
				settings.setFlowVert(false);
				//settings.flowVert = false;
			}
		}
		else {
			string nome = cols[1];
			string valores;
			if (cols.size()>2) {
				valores = cols[2];
			}

			if (tipo == "slider") tipo = "float";
			if (tipo == "sliderInt") tipo = "int";
			if (tipo == "bool") tipo = "toggle";

			if (tipo == "float" || tipo == "int" || tipo == "sliderVert") {
				vector <string> v = ofSplitString(valores, " ");
				float min = ofToFloat(v[0]);
				float max = ofToFloat(v[1]);
				float val = ofToFloat(v[2]);
//				elements.push_back(new slider(nome, settings, val));
				elements.push_back(new slider(nome, settings, min, max, val, tipo=="int"));
			}

			else if (tipo == "label") {
				elements.push_back(new label(nome, settings));
			}

			else if (tipo == "radioitem") {
				elements.push_back(new radioitem(nome, settings));
			}

			else if (tipo == "toggle") {
				bool val = valores == "1";
				elements.push_back(new toggle(nome, settings, val));
			}

			else if (tipo == "toggleNoLabel") {
				bool val = valores == "1";
				elements.push_back(new toggle(nome, settings, val, false));
			}

			else if (tipo == "radio") {
				vector <string> opcoes = ofSplitString(valores, " ");
				elements.push_back(new radio(nome, settings, opcoes));
			}

			else if (tipo == "slider2d") {
				elements.push_back(new slider2d(nome, settings));
			}

			else if (tipo == "dirList" || tipo == "dirListNoExt") {
				ofDirectory dir;
				dir.listDir(valores);
				vector <string> opcoes;
				for (auto & d : dir) {
					if (tipo == "dirListNoExt") {
						opcoes.push_back(d.getBaseName());
					} else {
						opcoes.push_back(d.getFileName());
					}
				}
				elements.push_back(new radio(nome, settings, opcoes));
			}


			// configurations
			// todo : margin
			else if (tipo == "sliderWidth") {
				settings.sliderDimensions.x = ofToInt(nome);
			}
			else if (tipo == "sliderHeight") {
				settings.sliderDimensions.y = ofToInt(nome);
			}

			else if (tipo == "addUI" || tipo == "addUIDown") {
				addUI(nome, tipo == "addUIDown");
			}



			else if (tipo == "ints" || tipo == "floats" || tipo == "bools" || tipo == "bangs" || tipo == "holds" || tipo == "colors" || tipo == "slider2ds") {
				vector <string> nomes = ofSplitString(nome, "[");
				string n = nomes[0];
				string intervalo = ofSplitString(nomes[1], "]")[0];
				int start = ofToInt(ofSplitString(intervalo, "-")[0]);
				int end = ofToInt(ofSplitString(intervalo, "-")[1]);
				for (int a=start; a<=end; a++) {
					string newTipo = tipo.substr(0, tipo.size()-1);
					createFromLine(newTipo + "	"+n + ofToString(a)+"	"+valores);
				}
			}

			else if (tipo == "togglesList") {
				vector <string> nomes = ofSplitString(nome, " ");
				createFromLine("flowHoriz");
				for (auto & n : nomes) {
//					createFromLine("toggle	" + n + "	0");
					createFromLine("radioitem	" + n + "	0");
				}
				createFromLine("flowVert");

			}

			else if (tipo == "toggleMatrix") {
				if (valores != "") {
					vector <string> vals = ofSplitString(valores, " ");
					int maxx = ofToInt(vals[0]);
					int maxy = ofToInt(vals[1]);

					int contagem = 0;
					for (int y=0; y<maxy; y++) {
						createFromLine("flowHoriz");
						for (int x=0; x<maxx; x++) {
							string nomeElement = nome + ofToString(x) + ofToString(y);

							if (tipo == "toggleMatrixLinear") {
								nomeElement = nome + ofToString(contagem);
							}
							string n = nome + "_" + ofToString(x) + "_" +ofToString(y);
							createFromLine("toggleNoLabel	" + n + "	0");
							contagem++;
						}
						createFromLine("flowVert");
						createFromLine("");
					}
				}
			}
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
void ofxDmtrUI3::onKeyPressed(ofKeyEventArgs& data) {
	keyPressed(data.key);
}

//--------------------------------------------------------------
void ofxDmtrUI3::onKeyReleased(ofKeyEventArgs& data) {
	keyReleased(data.key);
}

//--------------------------------------------------------------
void ofxDmtrUI3::onMousePressed(ofMouseEventArgs& data) {
	for (auto & e : elements) {
//		e->checkMouse(data.x, data.y);
//		e->checkMousePress(data.x, data.y);
		e->checkMousePress(data.x - settings.rect.x, data.y - settings.rect.y);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::onMouseDragged(ofMouseEventArgs& data) {
	for (auto & e : elements) {
//		e->checkMouse(data.x, data.y);
		e->checkMouse(data.x - settings.rect.x, data.y - settings.rect.y);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::onMouseReleased(ofMouseEventArgs& data) {
	for (auto & e : elements) {
		e->isPressed = false;
		e->firstClicked = false;
	}
}

////--------------------------------------------------------------
//void ofxDmtrUI3::onMouseMoved(ofMouseEventArgs& data) {
//}

//--------------------------------------------------------------
void ofxDmtrUI3::uiEventsString(string & e) {
	//cout << e << endl;
}

//--------------------------------------------------------------
void ofxDmtrUI3::onExit(ofEventArgs &data) {
	//cout << "onexit dmtrui3" << endl;
	save("default.xml");
}

//--------------------------------------------------------------
void ofxDmtrUI3::onWindowResized(ofResizeEventArgs &data) {
	if (!ofGetMousePressed()) {
		reFlow();
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::save(string xml) {
	ofxXmlSettings xmlSettings;
	xmlSettings.setValue("ofxDmtrUIVersion", 3.0);
	for (auto & e : elements) {
		//cout << typeid(e->getVal()).name() << endl;
		if (e->kind == TOGGLE || e->kind == RADIOITEM) {
			xmlSettings.setValue("element:" + e->name, (bool)e->getVal());
		}
		else if (e->kind == RADIO) {
			//cout << "saving radio " + e->getValString() << endl;
			xmlSettings.setValue("element:" + e->name, (string)e->getValString());
		}
		else if (e->kind == SLIDER2D) {
			//cout << "saving radio " + e->getValString() << endl;
			xmlSettings.setValue("element:" + e->name + ":x", e->getValPoint().x);
			xmlSettings.setValue("element:" + e->name + ":y", e->getValPoint().y);
		}

		else if (e->kind != LABEL) {
			xmlSettings.setValue("element:" + e->name, e->getVal());
		}
	}
	xmlSettings.save(xml);
}

//--------------------------------------------------------------
void ofxDmtrUI3::load(string xml) {
	ofxXmlSettings xmlSettings;
	xmlSettings.loadFile(xml);

	int UIVersion = xmlSettings.getValue("ofxDmtrUIVersion", 0);
	for (auto & e : elements) {
		//string tagName = "element:" + e->name;
		//if (xmlSettings.tagExists(tagName))
		{
			if (e->kind == TOGGLE || e->kind == RADIOITEM) {
				bool valor = xmlSettings.getValue("element:" +e->name, false); //(bool)e->getVal()
				e->set(valor);
				//cout << "getting value for bool :: " +e->name + " :: "+ ofToString(valor) << endl;

			}
			else if (e->kind == RADIO) {
				string valor = xmlSettings.getValue("element:" +e->name, "");
				e->set(valor);
				//cout << "setting value for radio :: " +e->name + " :: "+ valor << endl;
			}

			else if (e->kind == SLIDER2D) {
				float x = xmlSettings.getValue("element:" +e->name + ":x", 0.0);
				float y = xmlSettings.getValue("element:" +e->name + ":y", 0.0);
				e->set(ofPoint(x,y));
				//cout << "setting value for radio :: " +e->name + " :: "+ valor << endl;
			}

			else if (e->kind == SLIDER) {
				float valor = xmlSettings.getValue("element:" +e->name, e->getVal());
				e->set(valor);
			}

			// nao usar nada pra label aqui

//			cout << e->name + " :: " + ofToString( valor) << endl;
//			cout << typeid(e).name() << endl;
		}
	}
	//settings.redraw = true;
}

//--------------------------------------------------------------
void ofxDmtrUI3::reFlow() {
	cout << "==== reflow" << endl;
	settings.flow = settings.margin;
//	for (auto & e : elements) {
//		e->getProperties();
//	}

	// xaxa
	// growtoinclude not working here
//	settings.rect.width = 0;
//	settings.rect.height = 0;
//	for (auto & e : elements) {
//		settings.rect.growToInclude(e->boundsRect);
//	}
//	fboSettings.width = settings.rect.width;
//	fboSettings.height = settings.rect.height;
//	cout << settings.rect.width << endl;
//	cout << settings.rect.height << endl;


	autoFit();
}


/*
 c++14 only
//--------------------------------------------------------------
auto ofxDmtrUI3::getVal(string n) {
	for (auto & e : elements) {
		if (e->name == n) {
			return e->getVal();
		}
	}
}
*/


// Dividir em outra pagina, legacy algo assim
//--------------------------------------------------------------
void ofxDmtrUI3::createSoftwareFromText(string file) {
	UINAME = "master";
//	keepSettings = true;
//	useShortcut = true;

	if (ofFile::doesFileExist("uiAll.txt")) {
		createFromText("uiAll.txt");
	}
	createFromText(file);
	//software.setup();

	if (ofFile::doesFileExist("output.txt")) {
		vector <string> output = textToVector("output.txt");
		vector <string> dimensoes = ofSplitString(output[0], " ");
		software.w = ofToInt(dimensoes[0]);
		software.h = ofToInt(dimensoes[1]);
	}

#ifdef DMTRUI_TARGET_TOUCH
	int format = GL_RGBA; //GL_RGBA32F_ARB  //GL_RGBA32F
#else
	int format = GL_RGBA32F_ARB; //GL_RGBA32F_ARB  //GL_RGBA32F
#endif
	ofFbo * fbo = &mapFbos["fbo"];
	if (software.multiSampling == 0) {
		fbo->allocate			(software.w, software.h, format);
	} else {
		fbo->allocate			(software.w, software.h, format, software.multiSampling);
	}

	fbo->begin();
	ofClear(0,255);
	fbo->end();

	allUIs.push_back(this);

	//mudar tudo pra fbo map aqui
	//setFbo(fbo);
	
}

//--------------------------------------------------------------
void ofxDmtrUI3::setFbo(ofFbo &fbo) {
	_fbo = &fbo;
}


//--------------------------------------------------------------
void ofxDmtrUI3::loadPresetAll(int n) {
	for (auto & u : uis) {
		string nome = software.presetsFolder + ofToString(n) + u.first +  ".xml";
		u.second.load(nome);
		//u.second.redraw = true;
	}

	software.presetLoaded = n;
	//allPresets.set(n);

	//redraw = true;

	// fazer aqui um tipo de evento especial

	// notify event
//	dmtrUIEvent te;
//	te.nome = "loadPresetAll";
//	ofNotifyEvent(evento, te);
}

//--------------------------------------------------------------
void ofxDmtrUI3::addUI(string nome, bool down) {
	// aqui tenho tres ponteiros. o _uiLast o uiNext e o uiRight. nao sei se precisa tantos.
	// de repente fazer um vector de ponteiros?
	uis[nome].UINAME = nome;
	//uis[nome].settings.bgColor = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
	if (ofFile::doesFileExist("uiAll.txt")) {
		uis[nome].createFromText("uiAll.txt");
	}

	if (down)
	{
		if (_uiLast != NULL) {
			uis[nome].minimumWidth = _uiLast->settings.rect.width;
		} else {
			uis[nome].minimumWidth = settings.rect.width;
		}
	}

	string fileName = nome+".txt";
	if (ofFile::doesFileExist(fileName)) {
		uis[nome].createFromText(fileName);
	}

	// uilast diz a ultima ui que foi adicionada. se nao houver Ž a master
	if (_uiLast == NULL) {
		uis[nome].settings.hue = settings.hue;

		if (!down) {
			uis[nome].nextTo(*this);
		} else {
			uis[nome].downTo(*this);
		}
	} else {
		uis[nome].settings.hue = _uiLast->settings.hue;
		if (!down) {
			uis[nome].nextTo(*_uiLast);
		} else {
			uis[nome].downTo(*_uiLast);
		}
	}
	uis[nome]._uiFather = this;

	_uiLast = &uis[nome];
	uis[nome].autoFit();

	allUIs.push_back(&uis[nome]);

}

//--------------------------------------------------------------
void ofxDmtrUI3::nextTo(ofxDmtrUI3 & u) {
	u.autoFit();
	settings.rect.x = u.settings.rect.x + u.settings.rect.width + u.settings.margin.x;
	settings.rect.y = 0;
	u._uiRight = this;

	if (_uiUnder != NULL) {
		_uiUnder->downTo(*this);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::downTo(ofxDmtrUI3 & u) {
	u.autoFit();
	settings.rect.x  = u.settings.rect.x ;
	settings.rect.y  = u.settings.rect.y + u.settings.rect.height + u.settings.margin.y ;

//	cout << settings.rect.x << endl;
//	cout << settings.rect.y << endl;
//	cout << "------" << endl;
	u._uiUnder = this;

	if (_uiUnder != NULL) {
		_uiUnder->downTo(*this);
	}

	if (_uiRight != NULL) {
		_uiRight->nextTo(*this);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::autoFit() {
	//cout << "autoFit, still empty" << endl;

//	if (_uiFather != NULL) {
//		_uiFather->autoFit();
//	}

	int maxw = 0;
	int maxh = 0;
	for (auto & e : elements) {
		//e->getProperties();
		maxw = MAX(maxw, e->boundsRect.x + e->boundsRect.width);
		maxh = MAX(maxh, e->boundsRect.y + e->boundsRect.height);
	}

//	settings.rect.width = maxw + settings.spacing;
//	settings.rect.height = maxh + settings.spacing;

	settings.rect.width  = maxw + settings.margin.x;
	settings.rect.height = maxh + settings.margin.y;

	fboSettings.width = settings.rect.width;
	fboSettings.height = settings.rect.height;
	cout << UINAME << endl;
	cout << settings.rect.width << endl;
	cout << settings.rect.height << endl;
	cout << "------" << endl;

	fboUI.allocate(fboSettings);
	settings.redraw = true;


}

//--------------------------------------------------------------
void ofxDmtrUI3::fboClear() {
	fboUI.begin();
	ofClear(0,0);
	ofSetColor(settings.bgColor);
	ofDrawRectangle(0,0,fboUI.getWidth(), fboUI.getHeight());
	fboUI.end();
}


//--------------------------------------------------------------
void ofxDmtrUI3::uiEvents(uiEv & e) {
	//cout << "uiEvents from inside addon " + e.name + ":: " + e.s << endl;

	//getElement("allPresets")->invokeInt = &ofxDmtrUI3::loadPresetAll;

	// fazer somente se tiver o software invocado.
	if (e.name == "allPresets") {
		string p = getElement("allPresets")->getValString();
		//cout << "allPresets val:: " << p << endl;
		loadPresetAll(ofToInt(p));
		// xaxa claramente com defeito aqui
		//cout << "pString allPresets val:: " << pString["allPresets"] << endl;
	}
}
