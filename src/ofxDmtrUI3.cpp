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

	// REVER AQUI
	settings.software = &software;
	//settings.updateUI = &ofxDmtrUI3::updateUI;
	fboSettings.width = 10; // will resize on autofit
	fboSettings.height = 10;
	fboSettings.internalformat = GL_RGBA;

//	fboSettings.numSamples = 0;
//	fboSettings.useDepth = false;
//	fboSettings.useStencil = false;

	settings.pFloat	 	= &pFloat;
	settings.pInt	 	= &pInt;
	settings.pBool 		= &pBool;
	settings.pString	 	= &pString;
	settings.pPoint 		= &pPoint;

	fboUI.allocate(fboSettings);

	fboClear();

	ofAddListener(ofEvents().draw, this, &ofxDmtrUI3::onDraw);
	ofAddListener(ofEvents().update, this, &ofxDmtrUI3::onUpdate);
	ofAddListener(ofEvents().mousePressed, this, &ofxDmtrUI3::onMousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofxDmtrUI3::onMouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxDmtrUI3::onMouseReleased);
	ofAddListener(ofEvents().keyPressed, this, &ofxDmtrUI3::onKeyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxDmtrUI3::onKeyReleased);
	ofAddListener(ofEvents().exit, this, &ofxDmtrUI3::onExit);
	ofAddListener(ofEvents().windowResized, this, &ofxDmtrUI3::onWindowResized);

	ofAddListener(settings.uiEvent,this, &ofxDmtrUI3::uiEvents);
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
	//ofRemoveListener(settings.uiEventString,this, &ofxDmtrUI3::uiEventsString);
}

//--------------------------------------------------------------
void ofxDmtrUI3::update() {
	for (auto & p : pFloat) {
		if (easing > 0) {
			pEasy[p.first] += (pFloat[p.first] - pEasy[p.first])/easing;
		}
		else {
			pEasy[p.first] = pFloat[p.first];
		}
	}

	if (futureCommands.size()) {
		for (auto & f : futureCommands) {
			if (f.action == "loadAllPresets") {
				loadPresetAll(f.valInt);
			}
			if (f.action == "setFromIndex") {
				((radio*)getElement(f.name))->setFromIndex(f.valInt);
			}
		}
		futureCommands.clear();
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::redraw() {
	for (auto & e : elements) {
		e->needsRedraw();
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::draw() {
	if (settings.needsRedraw) {
		fboUI.begin();
		for (auto & e : elements) {
			if (e->redraw && !e->alwaysRedraw) {
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
		//cout << "SETTINGS REDRAW  :: " + UINAME << endl;
		fboClear();

		fboUI.begin();
		ofSetColor(255);
		for (auto & e : elements) {
			if (!e->alwaysRedraw) {
				e->draw();
			}
		}
		fboUI.end();
		settings.redraw = false;
	}

	if (settings.software->visible) {
		ofPushStyle();
//		ofSetColor(255, settings.opacity);
		ofSetColor(255, settings.software->opacity);
		fboUI.draw(settings.rect.x, settings.rect.y);
		// XAXA todo performance, vector or something
		ofPushMatrix();
		ofTranslate(settings.rect.x, settings.rect.y);
		for (auto & e : elements) {
			if (e->alwaysRedraw) {
				e->draw();
			}
		}
		ofPopMatrix();
		ofPopStyle();
	}

}


//--------------------------------------------------------------
void ofxDmtrUI3::keyPressed(int key){

	// Only on master UI
	//if (_uiFather == NULL) {
	if (UINAME == "master") {
		//cout << "uifather null, my uiname = " + UINAME << endl;
		if (key == '=') {
			settings.software->visible ^= 1;
		}

		if ((key == 'f' || key == 'F')) {
			if (ofGetKeyPressed(OF_KEY_COMMAND)) {
				//cout << "toggle fullscreen" << endl;
				ofToggleFullscreen();
				// falta um needsredraw por aqui
			}
		}
	}

//	if (key == '1' || key == '2' || key == '3' || key == '4' ) {
//		string nome = ofToString(char(key)) + ".xml";
//		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
//			save(nome);
//		} else {
//			load(nome);
//		}
//	}



	//if(2==3)
	if (UINAME == "master")
	{
		if (key == 'a' || key == 'A') {
			loadPresetAll(0, true);
		}
		else if (key == 's' || key == 'S') {
			if (ofGetKeyPressed(OF_KEY_COMMAND)) {
				int slot = ofToInt(getElement("allPresets")->getValString());
				cout << slot << endl;
				// xaxa
				savePresetAll(slot);
			} else {
				loadPresetAll(1, true);
			}
		}
		else if (key == 'd' || key == 'D') {
			loadPresetAll(2, true);
		}
		else if (key == 'f' || key == 'F') {
			loadPresetAll(3, true);
		}
		else if (key == 'g' || key == 'G') {
			loadPresetAll(4, true);
		}
		else if (key == 'h' || key == 'H') {
			loadPresetAll(5, true);
		}
		else if (key == 'j' || key == 'J') {
			loadPresetAll(6, true);
		}
		else if (key == 'k' || key == 'K') {
			loadPresetAll(7, true);
		}
		else if (key == 'l' || key == 'L') {
			loadPresetAll(8, true);
		}
		else if (key == ';') {
			loadPresetAll(9, true);
		}
		else if (key == 39) { //single quote
			loadPresetAll(10, true);
		}
		if (key == 'z' || key == 'Z') {
			loadPresetAll(11, true);
		}
		else if (key == 'x' || key == 'X') {
			loadPresetAll(12, true);
		}
		else if (key == 'c' || key == 'C') {
			loadPresetAll(13, true);
		}
		else if (key == 'v' || key == 'V') {
			loadPresetAll(14, true);
		}
		else if (key == 'b' || key == 'B') {
			loadPresetAll(15, true);
		}
		else if (key == 'n' || key == 'N') {
			loadPresetAll(16, true);
		}
		else if (key == 'm' || key == 'M') {
			loadPresetAll(17, true);
		}
		else if (key == ',') {
			loadPresetAll(18, true);
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::keyReleased(int key){
}


//--------------------------------------------------------------
void ofxDmtrUI3::notify(string e) {
	uiEv ev = uiEv(e);
	ofNotifyEvent(settings.uiEvent, ev);
}

//--------------------------------------------------------------
void ofxDmtrUI3::createFromText(string file, bool n) {

	vector <string> linhas;

	if (ofFile::doesFileExist("uiAll.txt")) {

		// otimizar, carrega duas vezes cada arquivo.
		createdFromTextFile += ofBufferFromFile("uiAll.txt").getText() + "\r\r";
		vector <string> linhas = textToVector("uiAll.txt");
		for (auto & l : linhas) {
			createFromLine(l);
		}
	}


	if (ofFile::doesFileExist(file)) {
		createdFromTextFile += ofBufferFromFile(file).getText() + "\r\r";
		vector <string> linhas = textToVector(file);
		for (auto & l : linhas) {
			createFromLine(l);
		}
	} else {
		cout << "ofxDmtrUI3 createFromText ::: File not found " + file << endl;
	}

	notify("createFromText");
	autoFit();
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

			else if (tipo == "fps") {
				// XAXA TEMP
				elements.push_back(new fps("", settings));
			}

			else if (tipo == "presets") {
				elements.push_back(new presets("allPresets", settings));
				//cout << elements.back()->boundsRect << endl;
			}

			else if (tipo == "flowVert") {
				settings.setFlowVert(true);
			}

			else if (tipo == "flowHoriz") {
				settings.setFlowVert(false);
			}

			else if (tipo == "saveY") {
				settings.flowBak.y = settings.flow.y;
			}

			else if (tipo == "restoreY") {
				settings.flow.y = settings.flowBak.y;
			}

			else if (tipo == "audioControls") {
				string s =
R"(slider2d	freq
float	audioGanho	0.0 .5 0.25
float	audioOffset	-1 0 -.2
float	peakhold	0 20 2
float	decay	0 .98 .85
bool	invertAudio	0)";

				for (auto & l : ofSplitString(s, "\n")) {
					//cout << l << endl;
					createFromLine(l);
				}
			}

			else if (tipo == "audioBpmControls") {
				string s =
R"(bool	audioOuBpm	0
int	BPM	1 200 120
radio	ondaBeats	1 2 4 8
radio	onda	s w ww r
slider2d	freq
float	audioGanho	0 .5 0.25
float	audioOffset	-1 0 -.2
float	peakhold	0 20 2
float	decay	0 .98 .85
bool	invertAudio	0)";
				for (auto & l : ofSplitString(s, "\n")) {
					createFromLine(l);
				}
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

				// TODO - Slidervert
				elements.push_back(new slider(nome, settings, min, max, val, tipo=="int"));

				if (cols.size() == 4) {
					if (cols[3] == "audio") {
						min = val = 0;
						elements.push_back(new slider(nome + "Audio", settings, min, max, val, tipo=="int"));
					}
				}
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

			else if (tipo == "hide") {
				//bool val = valores == "1";
				elements.push_back(new toggle(nome, settings, true));
				using namespace std::placeholders;
				elements.back()->invokeBool = std::bind(&ofxDmtrUI3::showUI, this, _1);
			}

			else if (tipo == "bang") {
				elements.push_back(new bang(nome, settings));
			}

			else if (tipo == "toggleNoLabel") {
				bool val = valores == "1";
				elements.push_back(new toggle(nome, settings, val, false));
			}

			else if (tipo == "radio") {
				vector <string> opcoes = ofSplitString(valores, " ");
				elements.push_back(new radio(nome, settings, opcoes));

				if (cols.size() == 4) {
					((radio*)elements.back())->set(cols[3]);
				}
			}


			else if (tipo == "radioColors") {
				vector <string> cadaPar = ofSplitString(valores, " ");

				map<string, ofColor> colorMap;
				for (auto & c : cadaPar) {
					vector <string> colors = ofSplitString(c, ":");
					string n = colors[0];
					ofColor cor = stringHexToColor(colors[1]);
					colorMap[n] = cor;
				}

				for (auto & e : getElement(nome)->elements) {
					if ( colorMap.find(e->name) != colorMap.end() ) {
						e->color = colorMap[e->name];
						e->needsRedraw();
					}
				}
			}

			else if (tipo == "color") {
				vector <string> opcoes = ofSplitString(valores, " ");
				elements.push_back(new color(nome, settings, opcoes));
			}

			else if (tipo == "slider2d") {
				elements.push_back(new slider2d(nome, settings));
			}

			else if (tipo == "dirList" || tipo == "dirListNoExt" || tipo == "scene") {
				ofDirectory dir;
				if (tipo == "scene") {
					dir.allowExt("txt");
				}
				dir.listDir(valores);
				vector <string> opcoes;
				for (auto & d : dir) {
					if (tipo == "dirListNoExt" || tipo == "scene") {
						opcoes.push_back(d.getBaseName());
					} else {
						opcoes.push_back(d.getFileName());
					}
				}
				elements.push_back(new radio(nome, settings, opcoes));
				if (tipo == "scene") {
					using namespace std::placeholders;
					elements.back()->changeUI = std::bind(
					&ofxDmtrUI3::changeUI, this, _1, _2);
				}
				// BEAUTIFUL
				((radio*)elements.back())->setFolder(valores);
				elements.back()->isDir = true;
			}

			else if (tipo == "colorLicht") {
				createFromLine("bool	"+nome+"UsaPaleta	0");
				createFromLine("slider2d	"+nome+"Paleta	.5 .5");
				createFromLine("slider2d	"+nome+"Hsv	.5 .5");
				createFromLine("fbo	"+nome+"PaletaAtual	200 10");
				createFromLine("float	"+nome+"S	0 255 255");
				createFromLine("float	"+nome+"HRange	0 720 100");
				createFromLine("float	"+nome+"HRangeAudio	0 360 0");
				//		createFromLine("float	"+nome+"BRange	0 255 0");
				createFromLine("float	"+nome+"BRange	0 512 0");
				createFromLine("float	"+nome+"BStop	0 1 1");
				createFromLine("int	"+nome+"HStep	0 6 0");
				createFromLine("float	"+nome+"Alpha	0 255 255");
				createFromLine("float	"+nome+"AlphaAudio	0 255 0");
				createFromLine("float	"+nome+"AlphaRange	0 255 0");

				//cout << "colorLicht ::: " + nome << endl;
				//colors.push_back(nome);
				//lastHeight = 0;
				//createFromLine("");
			}

			else if (tipo == "colorLichtSmall") {
				createFromLine("_bool	"+nome+"UsaPaleta	0");
				createFromLine("slider2d	"+nome+"Paleta	.5 .5");
				createFromLine("_slider2d	"+nome+"Hsv	.5 .5");
				createFromLine("fbo	"+nome+"PaletaAtual	200 10");
				createFromLine("float	"+nome+"S	0 255 255");
//				createFromLine("float	"+nome+"HRange	0 720 100");
				createFromLine("float	"+nome+"HRange	0 320 100");
				createFromLine("_float	"+nome+"HRangeAudio	0 360 0");
				//		createFromLine("float	"+nome+"BRange	0 255 0");
				createFromLine("_float	"+nome+"BRange	0 512 0");
				createFromLine("_float	"+nome+"BStop	0 1 1");
				createFromLine("_int	"+nome+"HStep	0 6 0");
				createFromLine("_float	"+nome+"Alpha	0 255 255");
				createFromLine("_float	"+nome+"AlphaAudio	0 255 0");
				createFromLine("_float	"+nome+"AlphaRange	0 255 0");

				//cout << "colorLicht ::: " + nome << endl;
				//colors.push_back(nome);
				//lastHeight = 0;
				//createFromLine("");
			}


			// configurations
			// todo : margin
			else if (tipo == "bw") {
				settings.bw = ofToInt(nome);
			}
			else if (tipo == "sliderWidth") {
				settings.setSliderWidth(ofToInt(nome));
				//settings.sliderDimensions.x = ofToInt(nome);
			}
			else if (tipo == "sliderHeight") {
				settings.sliderDimensions.y = ofToInt(nome);
			}
			else if (tipo == "sliderMargin") {
				settings.spacing = ofToInt(nome);
			}
			else if (tipo == "nPresets") {
				settings.nPresets = ofToInt(nome);
			}
			else if (tipo == "margin") {
				settings.margin = ofPoint(ofToInt(nome), ofToInt(nome));
			}

			else if (tipo == "addUI" || tipo == "addUIDown") {
				addUI(nome, tipo == "addUIDown");
			}



			else if (tipo == "ints" || tipo == "floats" || tipo == "bools" || tipo == "bangs" ||
					 tipo == "holds" || tipo == "colors" || tipo == "slider2ds" ||
					 tipo == "boolsNoLabel") {
				vector <string> nomes = ofSplitString(nome, "[");
				string n = nomes[0];
				string intervalo = ofSplitString(nomes[1], "]")[0];
				int start = ofToInt(ofSplitString(intervalo, "-")[0]);
				int end = ofToInt(ofSplitString(intervalo, "-")[1]);
				string newTipo = tipo.substr(0, tipo.size()-1);
				if (tipo == "boolsNoLabel") {
					createFromLine("flowHoriz");
					newTipo = "toggleNoLabel";
				}
				for (int a=start; a<=end; a++) {
					createFromLine(newTipo + "	"+n + "_" + ofToString(a)+"	"+valores);
				}
				if (tipo == "boolsNoLabel") {
					createFromLine("flowVert");
				}
			}

			else if (tipo == "pantilts") {
				for (int a=0; a<ofToInt(valores); a++) {
					string n = ofToString(a);
					createFromLine("int	" + nome + "_pan" +n+"	0 255 127");
					createFromLine("int	" + nome + "_tilt" +n+"	0 255 127");
				}
			}

			else if (tipo == "togglesList") {
				vector <string> nomes = ofSplitString(nome, " ");
				createFromLine("flowHoriz");
				for (auto & n : nomes) {
					createFromLine("bool	" + n + "	0");
//					createFromLine("radioitem	" + n + "	0");
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
	if (settings.software->visible && settings.rect.inside(data.x, data.y)) {
		for (auto & e : elements) {
			e->checkMouseNeu(data.x - settings.rect.x, data.y - settings.rect.y, true);
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::onMouseDragged(ofMouseEventArgs& data) {
	if (settings.software->visible && settings.rect.inside(data.x, data.y)) {
		for (auto & e : elements) {
			e->checkMouseNeu(data.x - settings.rect.x, data.y - settings.rect.y);
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::onMouseReleased(ofMouseEventArgs& data) {
	for (auto & e : elements) {
		e->isPressed = false;
		e->firstClicked = false;
		e->dragging = false;

		if (e->kind == BANG) {
			e->set(false);
		}
	}
}



//--------------------------------------------------------------
void ofxDmtrUI3::onExit(ofEventArgs &data) {
	if (keepSettings) {
		string file = "_presets/" + UINAME + ".xml";
		cout << "ofxDmtrUI3 :: save on exit :: " + file << endl;
		save("_presets/" + UINAME + ".xml");
	}
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
		if (e->saveXml) {

			// mudar tudo aqui pra valType.
			//cout << typeid(e->getVal()).name() << endl;
			if (e->kind == TOGGLE || e->kind == RADIOITEM) {
				xmlSettings.setValue("element:" + e->name, (bool)e->getVal());
			}
			else if (e->kind == RADIO || e->kind == PRESETS) {
				xmlSettings.setValue("element:" + e->name, (string)e->getValString());
			}
			else if (e->kind == SLIDER2D) {
				xmlSettings.setValue("element:" + e->name + ":x", e->getValPoint().x);
				xmlSettings.setValue("element:" + e->name + ":y", e->getValPoint().y);
			}

			else if (e->kind != LABEL) {
				xmlSettings.setValue("element:" + e->name, e->getVal());
			}
		}
	}
	xmlSettings.save(xml);
}

//--------------------------------------------------------------
void ofxDmtrUI3::load(string xml) {
	ofxXmlSettings xmlSettings;
	if (ofFile::doesFileExist(xml)) {
		xmlSettings.loadFile(xml);

		int UIVersion = xmlSettings.getValue("ofxDmtrUIVersion", 0);
		for (auto & e : elements) {
			if (e->saveXml) {
				if (e->kind == TOGGLE || e->kind == RADIOITEM) {
					bool valor = xmlSettings.getValue("element:" +e->name, e->getValBool());
					e->set(valor);
				}
				else if (e->kind == RADIO || e->kind == PRESETS) {
					string valor = xmlSettings.getValue("element:" +e->name, "");
					e->set(valor);
				}

				else if (e->kind == SLIDER2D) {
					float x = xmlSettings.getValue("element:" +e->name + ":x", 0.0);
					float y = xmlSettings.getValue("element:" +e->name + ":y", 0.0);
					e->set(ofPoint(x,y));
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
}

//--------------------------------------------------------------
void ofxDmtrUI3::reFlow() {
	cout << "==== reflow == " + UINAME << endl;
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
	settings.software = &software;
	UINAME = "master";
	keepSettings = true;

	createFromText(file);

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

	if (keepSettings) {
	//	loadMaster();
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::loadMaster() {
	//cout << "ofxDmtrUI :: loadMaster :: " + UINAME << endl;
	string file = "_presets/" + UINAME + ".xml";
	load(file);
}

//--------------------------------------------------------------
void ofxDmtrUI3::setFbo(ofFbo &fbo) {
	_fbo = &fbo;
}

//--------------------------------------------------------------
void ofxDmtrUI3::addUI(string nome, bool down) {
	// aqui tenho tres ponteiros. o _uiLast o uiNext e o uiRight. nao sei se precisa tantos.
	// de repente fazer um vector de ponteiros?
	uis[nome].UINAME = nome;

	// todos no mesmo sofwtare que o master.
	uis[nome].settings.software = &software;

	if (down) {
		autoFit();
	}

	if (_uiLast == NULL) {
		uis[nome].settings.hue = settings.hue;

		if (!down) {
			uis[nome].nextTo(*this);
		} else {
			//uis[nome].minimumWidth = settings.rect.width;
			uis[nome].settings.minimumWidth = settings.rect.width;
			uis[nome].downTo(*this);
		}
	} else {

		uis[nome].settings.hue = _uiLast->settings.hue;
		if (!down) {
			uis[nome].nextTo(*_uiLast);
		} else {
//			uis[nome].minimumWidth = _uiLast->settings.rect.width;
			uis[nome].settings.minimumWidth = _uiLast->settings.rect.width;
			uis[nome].downTo(*_uiLast);
		}
	}

	string fileName = nome+".txt";
	if (ofFile::doesFileExist(fileName)) {
		uis[nome].createFromText(fileName);
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
	if (_uiRight != NULL) {
		_uiRight->nextTo(*this);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::downTo(ofxDmtrUI3 & u) {
	u.autoFit();
	settings.rect.x  = u.settings.rect.x ;
	settings.rect.y  = u.settings.rect.y + u.settings.rect.height + u.settings.margin.y ;

	u._uiUnder = this;

	if (_uiUnder != NULL) {
		_uiUnder->downTo(*this);
	}

	if (_uiRight != NULL) {
		_uiRight->nextTo(*this);
	}
}

//--------------------------------------------------------------
// software - recursive repositioning until the last UI
void ofxDmtrUI3::reFlowUis() {
	if (_uiUnder != NULL) {
		_uiUnder->settings.rect.x = settings.rect.x;
		_uiUnder->settings.rect.y = settings.rect.y + settings.rect.height + settings.margin.y;
		_uiUnder->reFlowUis();
	}
	if (_uiRight != NULL) {
		_uiRight->settings.rect.x = settings.rect.x + settings.rect.width + settings.margin.x;
		_uiRight->reFlowUis();
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::autoFit() {

	if (showInterface) {
		int maxw = 0;
		int maxh = 0;
		for (auto & e : elements) {
			//e->getProperties();
			maxw = MAX(maxw, e->boundsRect.x + e->boundsRect.width);
			maxh = MAX(maxh, e->boundsRect.y + e->boundsRect.height);
		}

//		cout << "autofit ui :: " + UINAME << endl;
//		cout << settings.rect.width << endl;
//		cout << minimumWidth << endl;
//		cout << "------" << endl;

		settings.rect.width  = maxw + settings.margin.x;
		settings.rect.height = maxh + settings.margin.y;
		settings.rect.width = MAX(settings.rect.width, settings.minimumWidth);

		fboSettings.width = settings.rect.width;
		fboSettings.height = settings.rect.height;

		fboUI.allocate(fboSettings);
		settings.redraw = true;
		settings.needsRedraw = true;

		if (UINAME != "master") {
			reFlowUis();
		}
	}
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
string ofxDmtrUI3::getPresetsPath(string ext) {
	return software.presetsFolder + ext;
}

//--------------------------------------------------------------
void ofxDmtrUI3::savePresetAll(int n) {
	for (auto & u : uis) {
		string nome = getPresetsPath(ofToString(n) + u.first +  ".xml");
		u.second.save(nome);
	}
	if (_fbo != NULL) {
		ofFbo fboThumb;
		float w = software.presetDimensions.x;
		float h = software.presetDimensions.y;
		float aspectThumb = w / (float)h;
		float aspectFbo = _fbo->getWidth() / (float)_fbo->getHeight();
		float neww = w;
		//float newh = h;
		float offx = 0;
		float offy = 0;

		//wider fbo than thumbnail
		if (aspectFbo > aspectThumb) {
			float proporcao = _fbo->getHeight() / (float)h;
			neww = _fbo->getWidth() / proporcao;
			offx = (w - neww) / 2.0;
		}

		fboThumb.allocate(w,h,GL_RGBA);
		fboThumb.begin();
		ofClear(0,255);
		ofSetColor(255);
		_fbo->draw(offx, offy, neww,h);
		fboThumb.end();

		ofPixels pixels;
		pixels.allocate( w, h, OF_IMAGE_COLOR_ALPHA);
		fboThumb.readToPixels(pixels);
		string imgPath = getPresetsPath(ofToString(n) +".tif");
		//cout << imgPath << endl;
		ofSaveImage(pixels, imgPath);
		getElement("allPresets")->elements[n]->updateFromPixels(pixels);
		getElement("allPresets")->needsRedraw();
	}
}


//--------------------------------------------------------------
void ofxDmtrUI3::loadPresetAll(int n, bool fromKey) {

	// temporary, just to fire event from master
	if (UINAME == "master") {

		// TODO, averiguar firing event twice here
		//cout << "loadPresetAll :: " + UINAME << endl;
		if (fromKey) {
			getElement("allPresets")->set(ofToString(n));
		}
		else {
			for (auto & u : uis) {
				if (u.first != "master") {
					string nome = getPresetsPath(ofToString(n) + u.first + ".xml");
					u.second.load(nome);
				}
			}
		}
	}
}


//--------------------------------------------------------------
void ofxDmtrUI3::clear(bool keepVars) {

	createdFromTextFile = "";

	elements.clear();
	settings.reset();

	if (!keepVars) {
		pFloat.clear();
		pInt.clear();
		pBool.clear();
		pString.clear();
		pPoint.clear();
		pEasy.clear();
		pColor.clear();
	}

	settings.needsRedraw = true;
}


//--------------------------------------------------------------
void ofxDmtrUI3::uiEvents(uiEv & e) {
	//cout << "element fired inside UI code" << endl;

	//&& e.tipo != LOAD
	if (ofIsStringInString(e.name, "_shortcut") ) {
//	if (ofIsStringInString(e.name, "_shortcutInt") ) {
		vector <string> split = ofSplitString(e.name, "_");
		string nome = split[0];
		getElement(nome)->set(ofToFloat(pString[e.name]));
	}


	if (e.name == "presetsFolder") {
		string folder = pString["presetsFolder"];
		software.presetsFolder = "_presets/" + folder + "/";

		// reload preset images here
		if (!ofFile::doesFileExist(software.presetsFolder)) {
			if (!ofFile::doesFileExist("_presets")) {
				ofDirectory::createDirectory("_presets");
			}
			ofDirectory::createDirectory(software.presetsFolder);
		}
		for (auto & e : getElement("allPresets")->elements) {
			e->updateImage();
		}
	}

	else if (e.name == "easing") {
		easing = pFloat["easing"];
		for (auto & p : uis) {
			p.second.easing = pFloat["easing"];
		}
	}

	// SAVE / LOAD Presets
	else if (e.name == "allPresets") {
		string p = getElement("allPresets")->getValString();
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			savePresetAll(ofToInt(p));
		}
		else {
			loadPresetAll(ofToInt(p));
		}
	}
}

void ofxDmtrUI3::createRadio(string name, vector<string> options, string sel) {
	elements.push_back(new radio(name, settings, options));
	if (sel != "") {
		((radio*)elements.back())->set(sel);
	}
}

string ofxDmtrUI3::getFileFullPath(string n) {
	cout << "getfilefullpath :: " + n << endl;
	string f = ((radio*)getElement(n))->folder;
	// + "/" + getElement(n)->getValString();
	return f;
}

void ofxDmtrUI3::showUI(bool show) {
	showInterface = show;
	if (show) {
		autoFit();
	} else {
		settings.rect.width = 40;
		fboSettings.width = settings.rect.width;
		fboSettings.height = settings.rect.height;

		fboUI.allocate(fboSettings);
		settings.redraw = true;
		settings.needsRedraw = true;
		//if (UINAME != "master")

	}
	if (_uiUnder != NULL) {
		_uiUnder->showUI(show);
	}
	reFlowUis();
}

void ofxDmtrUI3::set(string e, bool v) {
	for (auto & e : elements) {
		if (e->name == "e" && e->kind == TOGGLE) {
			cout << "SET TOGGLE" + e->name << endl;
			e->set(v);
		}
	}
};

void ofxDmtrUI3::set(string e, string v) {
	for (auto & e : elements) {
		if (e->name == "e" && e->kind == RADIO) {
			e->set(v);
		}
	}

};

void ofxDmtrUI3::set(string e, float v) {
	for (auto & e : elements) {
		if (e->name == "e" && e->kind == SLIDER) {
			e->set(v);
		}
	}
};

void ofxDmtrUI3::set(string e, int v) {
	cout << "setInt" + e << endl;
	for (auto & e : elements) {
		if (e->name == "e" && e->kind == SLIDER) {
			e->set(float(v));
		}
	}
};

void updateLookup() {

}

//slider * ofxDmtrUI3::getSlider("") {}
