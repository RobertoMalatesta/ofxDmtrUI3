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
	
	ofAddListener(ofEvents().mouseMoved, this, &ofxDmtrUI3::onMouseMoved);
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
	// remove listeners here

	ofRemoveListener(ofEvents().draw, this, &ofxDmtrUI3::onDraw);
	ofRemoveListener(ofEvents().update, this, &ofxDmtrUI3::onUpdate);
	ofRemoveListener(ofEvents().mouseMoved, this, &ofxDmtrUI3::onMouseMoved);
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
		
		ofTranslate(software.offset);

		if (scale != 1) {
			fboUI.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
			fboUI.draw(settings.rect.x * scale, settings.rect.y * scale, fboUI.getWidth() * scale, fboUI.getHeight() * scale);
		} else {
			fboUI.draw(settings.rect.x, settings.rect.y);
		}
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
	if (UINAME == "master") {
		if (key == '=') {
			settings.software->visible ^= 1;
		}
		
		if ((key == 'f' || key == 'F')) {
			if (ofGetKeyPressed(OF_KEY_COMMAND)) {
				ofToggleFullscreen();
			}
		}
		
		
		if (key == 'a' || key == 'A') {
			loadPresetAll(0, true);
		}
		else if (key == 's' || key == 'S') {
			if (ofGetKeyPressed(OF_KEY_COMMAND)) {
				int slot = ofToInt(getElement("allPresets")->getValString());
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
	settings.uiname = UINAME;
	vector <string> linhas;
	
	
	//06 agosto 2017 tentando UIREMOTE3
	createdFromTextFile = "";

	if (ofFile::doesFileExist("uiAll.txt")) {

		// otimizar, carrega duas vezes cada arquivo.
		//createdFromTextFile += ofBufferFromFile("uiAll.txt").getText() + "\r\r";

		vector <string> linhas = textToVector("uiAll.txt");
		for (auto & l : linhas) {
			createFromLine(l);
		}
	}


	if (ofFile::doesFileExist(file)) {
		
		loadedTextFile = file;
		
		createdFromTextFile += ofBufferFromFile(file).getText() + "\r\r";
		vector <string> linhas = textToVector(file);
		for (auto & l : linhas) {
			if (buildingTemplate == "") {
				createFromLine(l);
			} else {
				if (ofIsStringInString(l, "endTemplate")) {
					buildingTemplate = "";
				} else {
					templateUI[buildingTemplate].push_back(l);
				}

			}
		}
		
		notify("createFromText");

	} else {
		cout << "ofxDmtrUI3 createFromText ::: File not found " + file << endl;
	}
	
	if (keepSettings) {
		//cout << "LOAD PRESET KEEPSETTINGS " + UINAME << endl;
		
		
		// xaxa _presets?
		string file = "_presets/" + UINAME + ".xml";
		load(file);
	}

	autoFit();
}

//--------------------------------------------------------------
void ofxDmtrUI3::createFromLines(vector<string> lines) {
	for (auto & l : lines) {
		if (buildingTemplate == "") {
			createFromLine(l);
		} else {
			if (ofIsStringInString(l, "endTemplate")) {
				buildingTemplate = "";
			} else {
				templateUI[buildingTemplate].push_back(l);
			}
			
		}
	}}

//--------------------------------------------------------------
void ofxDmtrUI3::createFromLine(string l) {
	
	
	//std::string text = l;
	//cout <<  std::regex_replace(l, "^${(.*)}$/g", "$1") << endl;

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
				hasPresets = true;
				elements.push_back(new presets("allPresets", settings));
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

			else if (tipo == "saveX") {
				settings.flowBak.x = settings.flow.x;
			}

			else if (tipo == "restoreX") {
				settings.flow.x = settings.flowBak.x;
			}

			// 19 agosto 2017 serie 3%
			else if (tipo == "spacer") {
				settings.flow.x += settings.sliderDimensions.y;
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

			else if (tipo == "template") {
				for (auto s : templateUI[nome]) {
					string str = ofJoinString(templateVectorString[nome], " ");
					ofStringReplace(s, "{$vectorString}", str);
					ofStringReplace(s, "$", valores);
					createFromLine(s);
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
				if (nome == "") {
					elements.push_back(new toggle(nome, settings, true, false));
				} else {
					elements.push_back(new toggle(nome, settings, true));
				}
				elements.back()->saveXml = false;
				using namespace std::placeholders;
				elements.back()->invokeBool = std::bind(&ofxDmtrUI3::showUI, this, _1);
			}

			else if (tipo == "bang") {
				elements.push_back(new bang(nome, settings));
			}

			else if (tipo == "toggleNoLabel" || tipo == "boolNoLabel") {
				bool val = valores == "1";
				elements.push_back(new toggle(nome, settings, val, false));
			}

			else if (tipo == "bangNoLabel") {
				elements.push_back(new bang(nome, settings, false));
			}
			
			else if (tipo == "radio") {
				vector <string> opcoes = ofSplitString(valores, " ");
				
//				if (ofIsStringInString(nome, "_shortcut")) {
//					radio r;
//					r.showLabel = false;
//					r.saveXml = false;
//				}
//				newElements.push_back(r);
//				elements.push_back(&newElements.back());
				elements.push_back(new radio(nome, settings, opcoes));
				if (ofIsStringInString(nome, "_shortcut")) {
					((radio*)elements.back())->showLabel = false;
					((radio*)elements.back())->saveXml = false;
				}

				if (cols.size() == 4) {
					((radio*)elements.back())->set(cols[3]);
				}
			}

			else if (tipo == "slider2d") {
				elements.push_back(new slider2d(nome, settings));
			}

			else if (tipo == "color") {
				vector <string> opcoes = ofSplitString(valores, " ");
				elements.push_back(new color(nome, settings, opcoes));
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

				for (auto & e : ((mult*)getElement(nome))->elements) {
					if ( colorMap.find(e->name) != colorMap.end() ) {
						e->color = colorMap[e->name];
						e->needsRedraw();
					}
				}
			}




			else if (tipo == "dirList" || tipo == "dirlist" || tipo == "dirListNoExt" || tipo == "scene") {
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

				//colors.push_back(nome);
				//lastHeight = 0;
				//createFromLine("");
			}


			// configurations
			// todo : margin



			else if (tipo == "beginTemplate") {
				buildingTemplate = nome;
			}

			else if (tipo == "tag") {
				settings.tag = nome;
			}
			
			else if (tipo == "bw") {
				settings.bw = ofToInt(nome);
			}
			else if (tipo == "pos") {
				vector<string> xy = ofSplitString(nome, " ");
				settings.rect.x = ofToInt(xy[0]);
				settings.rect.y = ofToInt(xy[1]);
			}
			
			// temporario
			else if (tipo == "addX") {
				settings.flow.x += ofToInt(nome);
				settings.colx = settings.flow.x;
			}
			
			else if (tipo == "presetDimensions") {
				vector<string> xy = ofSplitString(nome, " ");
				software.presetDimensions.x = ofToInt(xy[0]);
				software.presetDimensions.y = ofToInt(xy[1]);
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
				addUI(nome, tipo == "addUIDown", valores);
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
				for (auto & n : nomes) {
					createFromLine("bool	" + n + "	0");
				}
			}
			
			else if (tipo == "intsList" || tipo == "floatsList") {
				vector <string> nomes = ofSplitString(nome, " ");
				for (auto & n : nomes) {
					createFromLine("int	" + n + "	"+valores);
				}
			}
			
			else if (tipo == "togglesListHoriz" || tipo == "togglesListNoLabel") {
				vector <string> nomes = ofSplitString(nome, " ");
				int lineBreak = 999;
//				if (valores != "") {
//					lineBreak = ofToInt(valores);
//				}
				
				if (cols.size()>3) {
					lineBreak = ofToInt(cols[3]);
				}
				createFromLine("flowHoriz");
				string e = (tipo == "togglesListNoLabel") ? "boolNoLabel" : "bool";

				int i = 0;
				for (auto & n : nomes) {
					if (i%lineBreak == 0 && i>0) {
						createFromLine("flowVert");
						createFromLine("");
						createFromLine("flowHoriz");
						//newLine();
					}
					createFromLine(e+"	" + n + "	"+valores);
					i++;
				}
				createFromLine("flowVert");
			}

			else if (tipo == "toggleMatrix" || tipo == "bangMatrix") {
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
							
							if (tipo == "toggleMatrix") {
								createFromLine("toggleNoLabel	" + n + "	0");
							} else {
								createFromLine("bangNoLabel	" + n + "	0");
							}
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
void ofxDmtrUI3::onMouseMoved(ofMouseEventArgs& data) {
	if (software.autoScrolling && UINAME == "master") {
		int difX = software.rect.width - ofGetWindowWidth();
		int difY = software.rect.height - ofGetWindowHeight();
		
		//ofPoint scroll = ofPoint(0,0);
		int scrollX = difX > 0 ? - data.x * difX / ofGetWindowWidth() : 0;
		int scrollY = difY > 0 ? - data.y * difY / ofGetWindowHeight() : 0;

		software.offset = ofPoint(scrollX, scrollY);
		
		//cout << software.offset << endl;
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::onMousePressed(ofMouseEventArgs& data) {
	mouseUI(data.x, data.y, true);
}

//--------------------------------------------------------------
void ofxDmtrUI3::onMouseDragged(ofMouseEventArgs& data) {
	mouseUI(data.x, data.y);
}

//--------------------------------------------------------------
void ofxDmtrUI3::mouseUI(int x, int y, bool pressed) {
	
	//data.x += settings.software->offset.x;
	
//	if (settings.software->visible && settings.rect.inside(x, y)) {
	if (settings.software->visible &&
		
		settings.rect.inside(x - settings.software->offset.x, y - settings.software->offset.y)) {

		int mx = x - settings.rect.x - settings.software->offset.x;
		int my = y - settings.rect.y - settings.software->offset.y;
		
		//		if (scale != 1) {
		//			mx /= scale;
		//			my /= scale;
		//		}
		
		for (auto & e : elements) {
			e->checkMouseNeu(mx, my, pressed);
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
		save("_presets/" + UINAME + ".xml");
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::onWindowResized(ofResizeEventArgs &data) {
//	if (!ofGetMousePressed()) {
//		reFlow();
//	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::save(string xml) {
	ofxXmlSettings xmlSettings;
	xmlSettings.setValue("ofxDmtrUIVersion", 4.0);
	for (auto & e : elements) {
		if (e->saveXml) {
			if (e->kind == TOGGLE || e->kind == RADIOITEM) {
				xmlSettings.setValue("element:bool:" + e->name, (bool)e->getVal());
			}
			else if (e->kind == RADIO || e->kind == PRESETS) {
				xmlSettings.setValue("element:string:" + e->name, (string)e->getValString());
			}
			else if (e->kind == SLIDER2D) {
				xmlSettings.setValue("element:point:" + e->name + ":x", e->getValPoint().x);
				xmlSettings.setValue("element:point:" + e->name + ":y", e->getValPoint().y);
			}

			else if (e->kind != LABEL) {
				string tipo = typeid(e->getVal()).name();
				xmlSettings.setValue("element:" + tipo + ":" + e->name, e->getVal());
			}
		}
	}

	xmlSettings.save(xml);
}



//--------------------------------------------------------------
//void ofxDmtrUI3::save(string xml) {
//	ofxXmlSettings xmlSettings;
//	xmlSettings.setValue("ofxDmtrUIVersion", 5.0);
//	for (auto & e : elements) {
//		if (e->saveXml) {
//			string tipo = typeid(e->getVal()).name();
//			
//			if (e->kind == TOGGLE || e->kind == RADIOITEM) {
//				xmlSettings.setValue("element:bool:" , (bool)e->getVal());
//				xmlSettings.addAttribute("element:bool", "name", e->name, 0);
//				xmlSettings.addAttribute("element:bool", "kind", tipo, 0);
//			}
//			else if (e->kind == RADIO || e->kind == PRESETS) {
//				xmlSettings.setValue("element:string:" , (string)e->getValString());
//				xmlSettings.addAttribute("element:string", "name", e->name, 0);
//				xmlSettings.addAttribute("element:string", "kind", tipo, 0);
//			}
//			else if (e->kind == SLIDER2D) {
//				xmlSettings.setValue("element:point:x", e->getValPoint().x);
//				xmlSettings.addAttribute("element:point:x", "name", e->name, 0);
//				xmlSettings.setValue("element:point:y", e->getValPoint().y);
//				xmlSettings.addAttribute("element:point:y", "name", e->name, 0);
//				xmlSettings.addAttribute("element:point:x", "kind", tipo, 0);
//			}
//			
//			else if (e->kind != LABEL) {
//				string tipo = typeid(e->getVal()).name();
//				xmlSettings.setValue("element:" + tipo, e->getVal());
//				xmlSettings.addAttribute("element:" + tipo, "name", e->name, 0);
//				xmlSettings.addAttribute("element:" + tipo, "kind", tipo, 0);
//				
//			}
//		}
//	}
//	
//	xmlSettings.save(xml);
//}

//--------------------------------------------------------------
void ofxDmtrUI3::load(string xml) {
	ofxXmlSettings xmlSettings;
	if (ofFile::doesFileExist(xml)) {
		xmlSettings.loadFile(xml);
		loadedXmlFile = xml;
		int UIVersion = xmlSettings.getValue("ofxDmtrUIVersion", 0);
		
		if (UIVersion == 5) {
			// not good
//			for (int a=0; a<xmlSettings.getNumTags("element:string"); a++) {
//				string name = xmlSettings.getAttribute("element:string", "name", "");
//				string val = xmlSettings.getValue("element:string",  "");
//			}
//			
//			for (auto & e : elements) {
//				if (e->saveXml) {
//					if (e->kind == TOGGLE || e->kind == RADIOITEM) {
//						bool valor = xmlSettings.getValue("element:bool:" +e->name, e->getValBool());
//						e->set(valor);
//					}
//					else if (e->kind == RADIO || e->kind == PRESETS) {
//						string valor = xmlSettings.getValue("element:string:" +e->name, "");
//						e->set(valor);
//					}
//					
//					else if (e->kind == SLIDER2D) {
//						float x = xmlSettings.getValue("element:point:" +e->name + ":x", 0.0);
//						float y = xmlSettings.getValue("element:point:" +e->name + ":y", 0.0);
//						e->set(ofPoint(x,y));
//					}
//					
//					else if (e->kind == SLIDER) {
//						string tipo = typeid(e->getVal()).name();
//						
//						float valor = xmlSettings.getValue("element:" + tipo + ":" + e->name, e->getVal());
//						e->set(valor);
//					}
//				}
//			}
		}
		
		else if (UIVersion == 4) {
			for (auto & e : elements) {
				if (e->saveXml) {
					if (e->kind == TOGGLE || e->kind == RADIOITEM) {
						bool valor = xmlSettings.getValue("element:bool:" +e->name, e->getValBool());
						e->set(valor);
					}
					else if (e->kind == RADIO || e->kind == PRESETS) {
						string valor = xmlSettings.getValue("element:string:" +e->name, "");
						e->set(valor);
					}

					else if (e->kind == SLIDER2D) {
						float x = xmlSettings.getValue("element:point:" +e->name + ":x", 0.0);
						float y = xmlSettings.getValue("element:point:" +e->name + ":y", 0.0);
						e->set(ofPoint(x,y));
					}

					else if (e->kind == SLIDER) {
						string tipo = typeid(e->getVal()).name();

						float valor = xmlSettings.getValue("element:" + tipo + ":" + e->name, e->getVal());
						e->set(valor);
					}
				}
			}
		}

		else if (UIVersion == 3) {
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
				}
			}
		}
		notify("load");
		// XAXA Notify
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
		if (dimensoes.size() > 2) { 
			software.multiSampling = ofToInt(dimensoes[2]);
		}
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

	setFbo(*fbo);

	allUIs.push_back(this);

	if (keepSettings) {
	//	loadMaster();
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::loadMaster() {
	string file = "_presets/" + UINAME + ".xml";
	load(file);
}

//--------------------------------------------------------------
void ofxDmtrUI3::setFbo(ofFbo &fbo) {
	_fbo = &fbo;
}

//--------------------------------------------------------------
void ofxDmtrUI3::addUI(string nome, bool down, string valores) {
	// aqui tenho tres ponteiros. o _uiLast o uiNext e o uiRight. nao sei se precisa tantos.
	// de repente fazer um vector de ponteiros?
	
	
	
//	string uiname = nome;
//	string
	
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
	//cout << valores << endl;
	if (ofIsStringInString(valores, "text:")) {
		fileName = ofSplitString(valores, "text:")[1] + ".txt";
	}
	
	//cout << "addUI::" + nome + "\t\t" + fileName << endl;
	
	if (ofFile::doesFileExist(fileName)) {
		if (valores == "keepSettings") {
			uis[nome].keepSettings = true;
		}
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
	
	if (_uiUnder == NULL && _uiRight == NULL && _uiFather != NULL) {
		_uiFather->updateSoftwareRect();
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::autoFit() {
	
	//cout << "autoFit :: " + UINAME << endl;

	if (showInterface) {
		int maxw = 0;
		int maxh = 0;
		for (auto & e : elements) {
			//e->getProperties();
			maxw = MAX(maxw, e->boundsRect.x + e->boundsRect.width);
			maxh = MAX(maxh, e->boundsRect.y + e->boundsRect.height);
		}

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
		ofSaveImage(pixels, imgPath);
		((mult*)getElement("allPresets"))->elements[n]->updateFromPixels(pixels);
		getElement("allPresets")->needsRedraw();
	}
}


//--------------------------------------------------------------
void ofxDmtrUI3::loadPresetAll(int n, bool fromKey) {

	// temporary, just to fire event from master
	if (UINAME == "master") {

		// TODO, averiguar firing event twice here
		if (fromKey) {
			if (hasPresets) {
				getElement("allPresets")->set(ofToString(n));
			}
		}
		else {
			for (auto & u : uis) {
				if (u.first != "master") {
					if (!u.second.keepSettings) {
						string nome = getPresetsPath(ofToString(n) + u.first + ".xml");
						u.second.load(nome);
					}
					// feito pra ofxDmtrUI3Remote
					//u.second.notify("loadPreset");
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

	//&& e.tipo != LOAD
	if (ofIsStringInString(e.name, "_shortcut") ) {
//	if (ofIsStringInString(e.name, "_shortcutInt") ) {
		vector <string> split = ofSplitString(e.name, "_shortcut");
		string nome = split[0];
		((slider*)getElement(nome))->set(ofToFloat(pString[e.name]));
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
		for (auto & e : ((mult*)getElement("allPresets"))->elements) {
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
	string f = getRadio(n)->getFullFileName();
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
	for (auto & e : elements) {
		if (e->name == "e" && e->kind == SLIDER) {
			e->set(float(v));
		}
	}
};

void updateLookup() {

}

//slider * ofxDmtrUI3::getSlider("") {}
