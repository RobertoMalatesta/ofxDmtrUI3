/**********************************************************************************

 Copyright (C) 2016-2019 Dimitre Lima (dmtr.org)

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


//--------------------------------------------------------------
void ofxDmtrUI3::setup() {
	// REVER AQUI
	settings.setSoftware(software);
	// antigo. ear pra por os audiobpm na inicializacao de tudo.
	//createFromLines(templatesString);

	//settings.updateUI = &ofxDmtrUI3::updateUI;
//	fboSettings.width = 10; // will resize on autofit
//	fboSettings.height = 10;
	fboSettings.internalformat = GL_RGBA;

	settings.pFloat	 	= &pFloat;
	settings.pInt	 	= &pInt;
	settings.pBool 		= &pBool;
	settings.pString	 	= &pString;
	settings.pPoint 		= &pPoint;
	// 24 june 2019
	settings.pColor 		= &pColor;
	
	ofAddListener(ofEvents().draw, this, &ofxDmtrUI3::onDraw);
	ofAddListener(ofEvents().update, this, &ofxDmtrUI3::onUpdate);
	
	
#ifdef DMTRUI_TARGET_TOUCH
	ofAddListener(ofEvents().touchUp, this, &ofxDmtrUI3::onTouchUp);
	ofAddListener(ofEvents().touchDown, this, &ofxDmtrUI3::onTouchDown);
	ofAddListener(ofEvents().touchMoved, this, &ofxDmtrUI3::onTouchMoved);
	//ofAddListener(ofEvents().touch, this, &ofxDmtrUI3::onMouseDragged);

#else
	ofAddListener(ofEvents().mouseMoved, this, &ofxDmtrUI3::onMouseMoved);
	ofAddListener(ofEvents().mousePressed, this, &ofxDmtrUI3::onMousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofxDmtrUI3::onMouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxDmtrUI3::onMouseReleased);
#endif

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
	
	for (auto & f : updateFunctions) {
		f();
	}


	for (auto & p : pEasy) {
		if (easing > 0) {
			if (ABS(pEasy[p.first] - pFloat[p.first]) > 0.00007) {  //0.00007
				pEasy[p.first] += (pFloat[p.first] - pEasy[p.first])/easing;
			} else {
				pEasy[p.first] = pFloat[p.first];
			}
		}
		else {
			pEasy[p.first] = pFloat[p.first];
		}
	}
	
	for (auto & p : pPointEasy) {
		if (easing > 0) {
			if (
				(ABS(pPointEasy[p.first].x - pPoint[p.first].x) > 0.00007) ||
				(ABS(pPointEasy[p.first].y - pPoint[p.first].y) > 0.00007)
			)
			{  //0.00007
				pPointEasy[p.first] += (pPoint[p.first] - pPointEasy[p.first])/easing;
			} else {
				pPointEasy[p.first] = pPoint[p.first];
			}
		}
		else {
			pPointEasy[p.first] = pPoint[p.first];
		}
	}
	
	for (auto & p : pColorEasy) {
		if (easing > 0) {
			if (
				(ABS(pColorEasy[p.first].r - pColor[p.first].r) > 0.00007) ||
				(ABS(pColorEasy[p.first].g - pColor[p.first].g) > 0.00007) ||
				(ABS(pColorEasy[p.first].b - pColor[p.first].b) > 0.00007)
			)
			{  //0.00007
				pColorEasy[p.first].r += (pColor[p.first].r - pColorEasy[p.first].r)/easing;
				pColorEasy[p.first].g += (pColor[p.first].g - pColorEasy[p.first].g)/easing;
				pColorEasy[p.first].b += (pColor[p.first].b - pColorEasy[p.first].b)/easing;
			} else {
				pColorEasy[p.first] = pColor[p.first];
			}
		}
		else {
			pColorEasy[p.first] = pColor[p.first];
		}
	}

	if (futureCommands.size()) {
		for (auto & f : futureCommands) {
			if (f.action == "loadAllPresets") {
				loadPresetAll(f.valInt, true);
			}
			else if (f.action == "loadPresetHold") {
				lastPreset = ((presets*)getElement("allPresets"))->selectedId;
				loadPresetAll(f.valInt, true);
			}
			else if (f.action == "loadPresetRelease") {
				if (lastPreset>-1) {
					loadPresetAll(lastPreset, true);
				}
			}
			else if (f.action == "setFromIndex") {
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
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	if (UINAME == "master" && settings.software->needsRedraw) {
		settings.redraw = true;
		for (auto & u : uis) {
			u.second.settings.redraw = true;
		}
		settings.software->needsRedraw = false;
	}
	
	if (settings.needsRedraw) {
		fboUI.begin();
		for (auto & e : elements) {
			if (e->redraw && !e->alwaysRedraw) {
				ofSetColor(255, 0);
				ofDrawRectangle(e->boundsRect);
				ofSetColor(255);
				e->draw();
				e->redraw = false;
			}
		}
		fboUI.end();
		settings.needsRedraw = false;
	}

	// lately we never redraw fully, only on allocate
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

	if (settings.software->visible && visible) {
		ofPushStyle();
		ofSetColor(255, settings.software->opacity);
		ofTranslate(software.offset);

		if (scale != 1) {
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
        else if (key == '-') {
            ofToggleFullscreen();
        }
		
		else if (key == 'a' || key == 'A') {
			loadPresetAll(0, true);
		}
		else if (key == 's' || key == 'S') {
			if (ofGetKeyPressed(OF_KEY_COMMAND)) {
				saveActualPreset();
			} else {
				loadPresetAll(1, true);
			}
		}
		else if (key == 'd' || key == 'D') {
			loadPresetAll(2, true);
		}
		else if (key == 'f' || key == 'F') {
			if (ofGetKeyPressed(OF_KEY_COMMAND)) {
				ofToggleFullscreen();
			}
			else {

				loadPresetAll(3, true);
			}
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
	ev.uiname = UINAME;
	ofNotifyEvent(settings.uiEvent, ev);
}


void ofxDmtrUI3::reload() {
	if (loadedTextFile != "") {
		// keepvars not working. make new example
		clear(true);
		createFromText(loadedTextFile);
	}
}


////--------------------------------------------------------------
//void ofxDmtrUI3::createFromText(string file, bool n) {
//	ofBuffer lines;
//	if (ofFile::doesFileExist("uiAll.txt")) {
//	}
//}




//--------------------------------------------------------------
void ofxDmtrUI3::createFromText(string file, bool n) {
	//cout << "createFromText:" << file << ":" << (n ? "yes" : "no") << endl;
	
	
	/*
	 Aqui mudar toda a maneira com que carregamos interfaces.
	 primeiro somar em um unico arquivo txt o uiAll e o texto em si
	 depois parsear os includes dos dois.
	 */
	
	
	settings.uiname = UINAME;
	vector <string> linhas;
	createdFromTextFile = "";
	

	if (ofFile::doesFileExist("uiAll.txt")) {
		// aqui o mesmo arquivo é carregado duas vezes o que nao faz sentido.
        createdFromTextFile += ofBufferFromFile("uiAll.txt").getText() + "\r";
		vector <string> linhas = textToVector("uiAll.txt");
		if (fixedLabel != "") {
			linhas.insert(linhas.end(), "labelmain	" + fixedLabel);
		}
		createFromLines(linhas);
	}

	if (ofFile::doesFileExist(file)) {
		loadedTextFile = file;
        createdFromTextFile += ofBufferFromFile(file).getText();
		vector <string> linhas = textToVector(file);
		createFromLines(linhas);
		notify("createFromText");
	} else {
		cout << "ofxDmtrUI3 createFromText ::: File not found " + file << endl;
	}
	
	
	
	
	
	
	updateLookup();
	autoFit();
	if (settings.software->masterIsSetup) {
		reFlowUis();
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::createFromLines(string lines) {
	//cout << lines << endl;
	createFromLines(ofSplitString(lines, "\n"));
}


//--------------------------------------------------------------
void ofxDmtrUI3::createFromLines(vector<string> lines) {
	for (auto & l : lines) {
		if (buildingTemplate == "") {
			//cout << l << endl;
			
			// testing includes.
			vector <string> cols = ofSplitString(l, "\t");
			if (cols[0] == "include") {
				vector <string> linhas2 = textToVector(cols[1]);
				for (auto & l2 : linhas2) {
					cout << l2 << endl;
					createFromLine(l2);
				}
			}
			createFromLine(l);
		} else {
			if (ofIsStringInString(l, "endTemplate")) {
				buildingTemplate = "";
			} else {
				templateUI[buildingTemplate].push_back(l);
			}
		}
	}
}

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
			else if (tipo == "clear") {
				clear();
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
				settings.flow.x += settings.sliderDimensions.y + settings.spacing;
			}
			
			else if (tipo == "audioControls" || tipo == "audioBpmControls") {
				createFromLine("template	" + l);
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
				
				if (v.size() < 2) {
					cout << "WILL ERROR :: " + l;
				}
				//cout << l << endl;
				float val = ofToFloat(v[2]);
				
//				cout << nome << endl;
//				cout << "building float slider, value = " << val << endl;

				
				if (tipo == "int") {
					if (pInt.find(nome) != pInt.end() ) {
						val = pInt[nome];
					}
				} else {
					if (pFloat.find(nome) != pFloat.end() ) {
						val = pFloat[nome];
					}
				}

				// TODO - Slidervert
				
				bool isVert = tipo == "sliderVert";
				
				elements.push_back(new slider(nome, settings, min, max, val, tipo=="int", isVert));

				
				if (cols.size() == 4) {
					if (cols[3] == "audio") {
						min = val = 0;
						elements.push_back(new slider(nome + "Audio", settings, min, max, val, tipo=="int"));
					}
					if (cols[3] == "audioBeat") {
						min = val = 0;
						elements.push_back(new slider(nome + "Audio", settings, min, max, val, tipo=="int"));
						elements.push_back(new slider(nome + "Beat", settings, min, max, val, tipo=="int"));
					}
					if (cols[3] == "beat") {
						min = val = 0;
//						elements.push_back(new slider(nome + "Audio", settings, min, max, val, tipo=="int"));
						elements.push_back(new slider(nome + "Beat", settings, min, max, val, tipo=="int"));
					}

				}
			}
			
			
			if (tipo == "_float") {
				vector<string> vals = ofSplitString(valores," ");
				pFloat[nome] = stof(vals[2]);
			}
			
			else if (tipo == "_int") {
				vector<string> vals = ofSplitString(valores," ");
				pInt[nome] = stof(vals[2]);
			}
			
			else if (tipo == "_bool") {
				pBool[nome] = stoi(valores);
			}
			

			else if (tipo == "template") {
				for (auto s : templateUI[nome]) {
					string str = ofJoinString(templateVectorString[nome], " ");
					ofStringReplace(s, "{$vectorString}", str);
					if (s == "{$lineString}") {
						createFromLines(templateVectorString[nome]);
					}
					ofStringReplace(s, "$", valores);
					createFromLine(s);
				}
			}

			else if (tipo == "label" || tipo == "largelabel") {
				elements.push_back(new label(nome, settings));
			}
			
			else if (tipo == "fixedLabel") {
				fixedLabel = nome;
				createFromLine("labelmain	"+nome);
				//elements.push_back(new label(nome, settings));
			}
			
			else if (tipo == "labelmain") {
				elements.push_back(new label(nome, settings));
				((label*) elements.back())->labelColor = ofColor(255,0,160);
			}

			else if (tipo == "radioitem") {
				elements.push_back(new radioitem(nome, settings));
				elements.back()->varType = DMTRUI_BOOLEAN;
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
			
			else if (tipo == "saveMaster") {
				//bool val = valores == "1";
				if (nome == "") {
					elements.push_back(new bang(nome, settings));
				} else {
					elements.push_back(new bang(nome, settings));
				}
				elements.back()->saveXml = false;
				using namespace std::placeholders;
				elements.back()->invokeBool = std::bind(&ofxDmtrUI3::saveMaster, this, _1);
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
			
			else if (tipo == "radio" || tipo == "radioNoLabel" || tipo == "radioFloat") {
				vector <string> opcoes = ofSplitString(valores, " ");
				bool shortcut = ofIsStringInString(nome, "_shortcut");
				bool label = tipo == "radio" && !shortcut;
				elements.push_back(new radio(nome, settings, opcoes, label));
				if (shortcut) {
					//((radio*)elements.back())->showLabel = false;
					((radio*)elements.back())->saveXml = false;
				}

				if (cols.size() == 4) {
					((radio*)elements.back())->set(cols[3]);
				}
				
				if (tipo == "radioFloat") {
					((radio*)elements.back())->isFloat = true;
				}
			}
			
			else if (tipo == "radioPipe") {
				vector <string> opcoes = ofSplitString(valores, "|");
				
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
			
			else if (tipo == "plotter") {
				elements.push_back(new plotter(nome, settings));
			}
            
            else if (tipo == "inspector") {
                elements.push_back(new inspector(nome, settings));
            }
            
            else if (tipo == "bar") {
                elements.push_back(new bar(nome, settings));
            }

            
			else if (tipo == "colorHsv") {
				createFromLine("tag	color");
				elements.push_back(new slider2d(nome, settings));
				
				ofFbo * f = &mapFbos["fboColor"];
				int sliderWidth = settings.sliderDimensions.x;
				int sliderHeight = settings.sliderDimensions.y * 2 + settings.spacing;
				f->allocate(sliderWidth, sliderHeight, GL_RGBA);
				f->begin();
				ofClear(0);
				ofColor cor;
				for (int b=0; b<sliderHeight; b++) {
					for (int a=0; a<sliderWidth; a++) {
						int este = b*sliderWidth + a;
						float hue = (255 * a / (float) sliderWidth);
						cor = ofColor::fromHsb(hue, 255, b*255/sliderHeight, 255);
						ofFill();
						ofSetColor(cor);
						ofDrawRectangle(a,b,1,1);
					}
				}
				f->end();
				elements.back()->setFbo(*f);
				((slider2d*)elements.back())->showPointer = true;
				
				//float sat = 0;
				string sat = "0";
				
				if (cols.size() == 3) {
					//cout << cols[2] << endl;
					vector <string> vals = ofSplitString(cols[2], " ");
					float hue = ofToFloat(vals[0]);
					sat = vals[1];
					//cout << "sat = " << sat << endl;
					
					float brightness = ofToFloat(vals[2]);
					((slider2d*)elements.back())->set(ofPoint(hue/255.0, brightness/255.0));
				}
				
				// for some reason it doesn't work this way.
                // string line = "float	" + nome + "_S	0 255 " + sat;
				// createFromLine(line);
				elements.push_back(new slider(nome + "_S", settings, 0, 255, ofToInt(sat), tipo=="int"));
				createFromLine("tag	");
			}
			
			else if (tipo == "colorHsvA") {
				createFromLine("tag	colorHsvA");
				elements.push_back(new slider2d(nome, settings));
				
				ofFbo * f = &mapFbos["fboColor"];
				int sliderWidth = settings.sliderDimensions.x;
				int sliderHeight = settings.sliderDimensions.y * 2 + settings.spacing;
				f->allocate(sliderWidth, sliderHeight, GL_RGBA);
				f->begin();
				ofClear(0);
				ofColor cor;
				for (int b=0; b<sliderHeight; b++) {
					for (int a=0; a<sliderWidth; a++) {
						int este = b*sliderWidth + a;
						float hue = (255 * a / (float) sliderWidth);
						cor = ofColor::fromHsb(hue, 255, b*255/sliderHeight, 255);
						ofFill();
						ofSetColor(cor);
						ofDrawRectangle(a,b,1,1);
					}
				}
				f->end();
				elements.back()->setFbo(*f);
				((slider2d*)elements.back())->showPointer = true;
				elements.push_back(new slider(nome + "_S", settings, 0, 255, 255, tipo=="float"));
				elements.push_back(new slider(nome + "_Alpha", settings, 0, 255, 255, tipo=="float"));
				createFromLine("tag	");
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


			else if (tipo == "dirList" || tipo == "dirlist" || tipo == "dirListNoExt"
					 || tipo == "scene" || tipo == "sceneNoLabel" || tipo == "imageList"
					 ) {
				ofDirectory dir;
				if (tipo == "scene" || tipo == "sceneNoLabel") {
					dir.allowExt("txt");
				}

				dir.listDir(valores);
				dir.sort();
				vector <string> opcoes;
				for (auto & d : dir) {
					if (tipo == "dirListNoExt" || tipo == "scene" || tipo == "sceneNoLabel") {
						opcoes.push_back(d.getBaseName());
					} else {
						opcoes.push_back(d.getFileName());
					}

				}
				
				bool label = tipo != "sceneNoLabel";
				elements.push_back(new radio(nome, settings, opcoes, label));
				if (tipo == "scene" || tipo == "sceneNoLabel") {
					using namespace std::placeholders;
					((radio*)elements.back())->changeUI =
					std::bind(&ofxDmtrUI3::changeUI, this, _1, _2);
				}
				
				if (tipo == "imageList") {
					//cout << "imageList function bind" << endl;
					using namespace std::placeholders;
					((radio*)elements.back())->loadImageList =
					std::bind(&ofxDmtrUI3::loadImageList, this, _1, _2);
					//void loadImageList(string name, string file) {
					// xaxaxa
				}
				// BEAUTIFUL
				((radio*)elements.back())->setFolder(valores);
				((radio*)elements.back())->isDir = true;
				//elements.back()->isDir = true;
			}

			
			
			else if (tipo == "colorLicht") {
				// TEMPLATES
				createFromLine("template	"+tipo+"	"+nome);
			}
			
			else if (tipo == "image") {
				if (ofFile::doesFileExist(nome)) {
					elements.push_back(new image(nome, settings));
				}
			}

			else if (tipo == "beginTemplate") {
				buildingTemplate = nome;
				templateUI[buildingTemplate].clear();
			}

			else if (tipo == "tag") {
				settings.tag = nome;
			}
			
			else if (tipo == "uiTag") {
				software.uiTag = nome;
			}


			else if (tipo == "obeyColumn") {
				settings.obeyColumn = ofToInt(nome);
			}
			
			else if (tipo == "bw") {
				settings.bw = ofToInt(nome);
			}
			else if (tipo == "pos") {
				vector<string> xy = ofSplitString(nome, " ");
				settings.rect.x = ofToInt(xy[0]);
				settings.rect.y = ofToInt(xy[1]);
			}
			
			else if (tipo == "font") {
				software.customFont = software.font.load(nome, ofToInt(valores));;
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
				//if (!isDown) {
					settings.setSliderWidth(ofToInt(nome));
				//}
			}
			else if (tipo == "sliderHeight") {
				settings.sliderDimensions.y = ofToInt(nome);
			}
			
			else if (tipo == "keepSettings") {
				//keepSettings = ofToInt(nome);
				loadMode = MASTER;
				saveMode = MASTER;
			}
			
			// XAXA REVISAR TUDO
//			else if (tipo == "loadSave") {
//				loadSave = ofToInt(nome);
//			}
//			else if (tipo == "savePreset") {
//				savePreset = ofToInt(nome);
//			}
//			else if (tipo == "loadPreset") {
//				loadPreset = ofToInt(nome);
//			}

			
//			else if (tipo == "softwareSliderWidth") {
//				cout << tipo << endl;
//				settings.setSliderWidth(ofToInt(nome));
//				software.sliderDimensions.x = ofToInt(nome);
//				//settings.sliderDimensions.x = ofToInt(nome);
//			}
//			else if (tipo == "softwareSliderHeight") {
//				cout << tipo << endl;
//				settings.sliderDimensions.y = ofToInt(nome);
//				software.sliderDimensions.y = ofToInt(nome);
//			}

			
			else if (tipo == "sliderMargin") {
				// tava errado aqui XAXA
				settings.spacing = ofToInt(nome);
			}
			else if (tipo == "nPresets") {
				//cout << tipo << endl;
				settings.nPresets = ofToInt(nome);
			}
			else if (tipo == "colPadding") {
				settings.software->colPadding = ofToInt(nome);
				settings.colx = ofToInt(nome);
				settings.flow = ofPoint(ofToInt(nome), ofToInt(nome));
				// XAXA RESOLVER - total rewrite of the heart
				//settings.software->margin = ofPoint(ofToInt(nome), ofToInt(nome));
				//settings.margin = ofPoint(ofToInt(nome), ofToInt(nome));
			}
			else if (tipo == "colSpacing") {
				settings.software->colSpacing = ofToInt(nome);
			}

			else if (tipo == "addUI" || tipo == "addUIDown") {
				addUI(nome, tipo == "addUIDown", valores);
			}
			

			else if (tipo == "ints" || tipo == "floats" || tipo == "bools" || tipo == "bangs" ||
					 tipo == "holds" || tipo == "colors" || tipo == "slider2ds" ||
					 tipo == "boolsNoLabel" || tipo == "sliderVerts") {
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

			// usado pro MECA. remover?
			else if (tipo == "pantilts") {
				for (int a=0; a<ofToInt(valores); a++) {
					string n = ofToString(a);
					createFromLine("int	" + nome + "_pan" +n+"	0 255 127");
					createFromLine("int	" + nome + "_tilt" +n+"	0 255 127");
				}
			}

			else if (tipo == "togglesList" || tipo == "boolsList") {
				vector <string> nomes = ofSplitString(nome, " ");
				for (auto & n : nomes) {
					createFromLine("bool	" + n + "	0");
				}
			}
			
			else if (tipo == "radioitemList") {
				if (nome != "") {
				cout << tipo << endl;
				cout << nome << "x" << endl;
				vector <string> nomes = ofSplitString(nome, " ");
				for (auto & n : nomes) {
					createFromLine("radioitem	" + n + "	0");
				}
				}
			}

			else if (tipo == "intsList" || tipo == "floatsList") {
				vector <string> nomes = ofSplitString(nome, " ");
				for (auto & n : nomes) {
					createFromLine("int	" + n + "	"+valores);
				}
			}
			
			else if (tipo == "togglesListHoriz" || tipo == "togglesListNoLabel" || tipo == "boolsListHoriz") {
				vector <string> nomes = ofSplitString(nome, " ");
				int lineBreak = 999;
				
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
				createFromLine("");
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
//	if (software.autoScrolling && UINAME == "master") {
//		int difX = software.rect.width - ofGetWindowWidth();
//		int difY = software.rect.height - ofGetWindowHeight();
//		int scrollX = difX > 0 ? - data.x * difX / ofGetWindowWidth() : 0;
//		int scrollY = difY > 0 ? - data.y * difY / ofGetWindowHeight() : 0;
//		software.offset = ofPoint(scrollX, scrollY);
//	}
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

	if (settings.software->visible &&
		settings.rect.inside(x - settings.software->offset.x, y - settings.software->offset.y)) {

		int mx = x - settings.rect.x - settings.software->offset.x;
		int my = y - settings.rect.y - settings.software->offset.y;
		
		for (auto & e : elements) {
			e->checkMouseNeu(mx, my, pressed);
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::onMouseReleased(ofMouseEventArgs& data) {
	mouseRelease();
}

//--------------------------------------------------------------
void ofxDmtrUI3::mouseRelease() {
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
void ofxDmtrUI3::onTouchUp(ofTouchEventArgs & touch) {
	mouseRelease();
}

//--------------------------------------------------------------
void ofxDmtrUI3::onTouchDown(ofTouchEventArgs & data) {
	cout << "touch event " << data.x << ":" << data.y << ":" << data.id << endl;
	mouseUI(data.x, data.y, true);
}

//--------------------------------------------------------------
void ofxDmtrUI3::onTouchMoved(ofTouchEventArgs & data) {
	mouseUI(data.x, data.y);
}


//--------------------------------------------------------------
void ofxDmtrUI3::onExit(ofEventArgs &data) {
	if (saveMode == MASTER) {
		saveMaster();
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
	int version = 5;
	
	ofXml xmlSettings;
	if (version == 5) {
		xmlSettings.appendChild("ofxDmtrUIVersion").set(version);
		auto xmlElements = 	xmlSettings.appendChild("element");
		auto bools =		xmlElements.appendChild("bool");
		auto strings =		xmlElements.appendChild("string");
		auto points = 		xmlElements.appendChild("point");
		auto ints = 		xmlElements.appendChild("int");
		auto floats = 		xmlElements.appendChild("float");

		for (auto & e : elements) {
			
			if (e->saveXml) {
				if (e->kind == TOGGLE || e->kind == RADIOITEM) {
					//xmlSettings.appendChild("element:bool:" + e->name).set((bool)e->getVal());
					bools.appendChild(e->name).set((bool)e->getVal());
				}
				
				else if (e->kind == RADIO || e->kind == PRESETS || e->kind == COLOR) {
					//xmlSettings.appendChild("element:string:" + e->name).set((string)e->getValString());
					strings.appendChild(e->name).set((string)e->getValString());
				}
				
				else if (e->kind == SLIDER2D) {
					auto slider2ds = points.appendChild(e->name);
					slider2ds.appendChild("x").set(e->getValPoint().x);
					slider2ds.appendChild("y").set(e->getValPoint().y);
					
					//points.appendChild(e->name + ":x").set(e->getValPoint().x);
					//points.appendChild(e->name + ":x").set(e->getValPoint().x);
					//xmlSettings.appendChild("element:point:" + e->name + ":x").set(e->getValPoint().x);
					//xmlSettings.appendChild("element:point:" + e->name + ":y").set(e->getValPoint().y);
				}
				
				else if (e->kind == SLIDER) {
//					if (((slider*)e)->isInt) {
					if (e->varType == DMTRUI_INT) {
						ints.appendChild(e->name).set(e->getVal());
					} else {
						floats.appendChild(e->name).set(e->getVal());
					}
				}
				
				else if (e->kind != LABEL) {
					// generic. floats and ints // not anymore
					string tipo = typeid(e->getVal()).name();
					auto tipos = xmlElements.appendChild(tipo);
					tipos.appendChild(e->name).set(e->getVal());
				}
			}
		}
		xmlSettings.save(xml);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::load(string xml) {
	
//	cout << "load " << xml << endl;
//	if (loadMode == MASTER) {
//		cout << "loadmode master" << endl;
//	} else {
//		cout << "loadmode != master" << endl;
//	}
	
	
	ofXml xmlSettings;

	if (ofFile::doesFileExist(xml)) {
		xmlSettings.load(xml);
		loadedXmlFile = xml;
		int UIVersion = xmlSettings.getChild("ofxDmtrUIVersion").getIntValue();

		if (UIVersion == 4 || UIVersion == 5)
		{
			auto xmlElements = 	xmlSettings.getChild("element");
			auto bools =		xmlElements.findFirst("bool");
			auto strings =		xmlElements.findFirst("string");
			auto points = 		xmlElements.findFirst("point");
			auto ints = 		xmlElements.findFirst("int");
			auto floats = 		xmlElements.findFirst("float");
			auto fs = 			xmlElements.findFirst("f");

			for (auto & e : elements) {
				if (e->saveXml) {
					if (e->kind == TOGGLE || e->kind == RADIOITEM) {
                        if (bools.getChild(e->name)) {
                            bool valor = bools.getChild(e->name).getBoolValue();
                            e->set(valor, notifyEventOnLoad);
                        }
					}
					else if (e->kind == RADIO || e->kind == PRESETS  || e->kind == COLOR) {
                        //if (strings.getChild(e->name))
						{
                            string valor = strings.getChild(e->name).getValue();							
//							if (e->name == "scene") {
//								cout << "Set Value :: " << endl;
//								cout << e->name << endl;
//								cout << valor << endl;
//							}
                            e->set(valor, notifyEventOnLoad);
                        }
					}
					else if (e->kind == SLIDER2D) {
                        if (points.getChild(e->name)) {
                            auto x = points.getChild(e->name).getChild("x").getFloatValue();
                            auto y = points.getChild(e->name).getChild("y").getFloatValue();
                            e->set(ofPoint(x,y), notifyEventOnLoad);
                        }
					}

					else if (e->kind == SLIDER) {
						if (UIVersion == 4) {
                            if (fs.getChild(e->name)) {
                                auto valor = fs.getChild(e->name).getFloatValue();
                                //auto valor = fs.getChild(e->name).getValue();
                                e->set(valor, notifyEventOnLoad);
                            }
							
						}

						else {
							if (e->varType == DMTRUI_INT) {
                                if (ints.getChild(e->name)) {
                                    auto valor = ints.getChild(e->name).getIntValue();
                                    e->set(valor, notifyEventOnLoad);
                                }
							} else {
                                if (floats.getChild(e->name)) {
                                    
//                                    cout << e->name << endl;
//                                    cout << UINAME << endl;
//                                    cout << "-------" << endl;
                                    
                                    auto valor = floats.getChild(e->name).getFloatValue();
                                    e->set(valor, notifyEventOnLoad);
                                }
							}
						}
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
	
	// xaxa not working
	//settings.flow = settings.margin;

	
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
	//autoFit();
}



// Dividir em outra pagina, legacy algo assim
//--------------------------------------------------------------
void ofxDmtrUI3::createSoftwareFromText(string file) {
	
	if (!ofFile::doesFileExist(file)) {
		string alertString =
R"(Dmtr.org Software
_ ._. ._ _. ... .._ ._. .._. .. _. __.
		
File not found
Does the folder has a data folder?
Did you try run.command?
)";
		ofSystemAlertDialog(alertString);
		std::exit(1);
	}
	
//	settings.software = &software;
	settings.setSoftware(software);

	UINAME = "master";
	
	loadMode = MASTER;
	saveMode = MASTER;
	
	for (auto & f : presetupFunctions) {
		f();
	}

	createFromText(file);

	if (ofFile::doesFileExist("output.txt") && software.w < 1) {
		vector <string> output = textToVector("output.txt");
		vector <string> dimensoes = ofSplitString(output[0], " ");
		software.w = ofToInt(dimensoes[0]);
		software.h = ofToInt(dimensoes[1]);
		if (dimensoes.size() > 2) { 
			software.multiSampling = ofToInt(dimensoes[2]);
		}
		
//		cout << "output.txt" << endl;
//		cout << software.w << endl;
//		cout << software.h << endl;
	} else {
		cout << "missing output.txt file" << endl;
		software.w = 1280;
		software.h = 720;
	}
	
	ofFbo * fbo = &mapFbos["fbo"];
	allocateAndClearFbo(*fbo);
	setFbo(*fbo);
	allUIs.push_back(this);
	for (auto & f : setupFunctions) {
		f();
	}
	
	software.masterIsSetup = true;
}

//--------------------------------------------------------------
void ofxDmtrUI3::loadMaster() {
	
	string file = "_presets/" + UINAME + ".xml";
	//cout << "loadMaster() : " << file << endl;
	load(file);
	
	for (auto & u : uis) {
		if (u.second.loadMode == MASTER) {
			//cout << "loadMaster UI " << u.second.UINAME << endl;
			u.second.loadMaster();
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::setFbo(ofFbo &fbo) {
	_fbo = &fbo;
}

//--------------------------------------------------------------
void ofxDmtrUI3::addUI(string nome, bool down, string valores) {

	uis[nome].UINAME = nome;
	uis[nome]._uiFather = this;
	if (software.uiTag != "") {
		uis[nome].uiTag = software.uiTag;
	}
	// every GUI has the same software object than the master (pointer to it)
	uis[nome].settings.software = &software;

	// master is added to allUIs vector just on finish, but it already has addUI before it, so if size==0, then add this.
	if (allUIs.size())
	{
		uis[nome]._uiLast = allUIs.back();
	} else {
		uis[nome]._uiLast = this;
	}
	uis[nome]._uiLast->_uiNext = &uis[nome];
	allUIs.push_back(&uis[nome]);


	// Any UI added with addUI has a _uiLast pointer set.
	uis[nome].settings.hue = uis[nome]._uiLast->settings.hue;
	if (uis[nome]._uiLast->UINAME == "master") {
		//cout << "this is master, this is autofit" << endl;
		autoFit();
	}

	if (down) {
		uis[nome].isDown = true;
		//cout << "this new ui is uidown :: " << nome << endl;
		uis[nome].settings.minimumWidth = uis[nome]._uiLast->fboUI.getWidth();
		
		// nao funciona pois o uiAll.txt override.
		
		
		uis[nome].settings.sliderDimensions = uis[nome]._uiLast->settings.sliderDimensions;
//		cout << "this ui is called " << nome << endl;
//		cout << "sliders dimensions = " << uis[nome].settings.sliderDimensions << endl;
		//uis[nome].settings.setSliderWidth(settings.sliderDimensions.x);
//		uis[nome].downTo(*uis[nome]._uiLast);
		uis[nome].settings.rect.width = uis[nome]._uiLast->settings.rect.width;
		uis[nome].settings.rect.x  = uis[nome]._uiLast->settings.rect.x ;
		
		uis[nome].settings.rect.y  = uis[nome]._uiLast->settings.rect.y + uis[nome]._uiLast->settings.rect.height + software.colSpacing;
		// precisa?
//		uis[nome].settings.minimumWidth = MAX(settings.minimumWidth, settings.sliderDimensions.x);
	}
	else {

		uis[nome].settings.rect.x = uis[nome]._uiLast->settings.rect.x + uis[nome]._uiLast->settings.rect.width + software.colSpacing;
//		uis[nome].settings.rect.x = uis[nome]._uiLast->settings.rect.x + uis[nome]._uiLast->fboUI.getWidth() + software.colSpacing;
		uis[nome].settings.rect.y = 0;
	}
	
	string fileName = nome+".txt";
	if (ofIsStringInString(valores, "text:")) {
		fileName = ofSplitString(valores, "text:")[1] + ".txt";
	}
	
	if (ofFile::doesFileExist(fileName)) {
		if (valores == "keepSettings") {
			uis[nome].loadMode = MASTER;
			uis[nome].saveMode = MASTER;
		}
		else if (valores == "disableLoadSave") {
			uis[nome].loadMode = NONE;
			uis[nome].saveMode = NONE;
		}
		else if (valores == "loadOnly") {
			uis[nome].loadMode = MASTER;
			uis[nome].saveMode = NONE;
		}
		uis[nome].createFromText(fileName);
	}
	
//	cout << uis[nome]._uiLast->UINAME << endl;
//	cout << uis[nome]._uiLast->settings.rect.x << endl;
//	cout << uis[nome]._uiLast->settings.rect.width << endl;
//	cout << "-------" << endl;

	updateSoftwareRect();
}

//--------------------------------------------------------------
// software - recursive repositioning until the last UI
void ofxDmtrUI3::reFlowUis() {
	if (_uiNext != NULL) {
		if (_uiNext->isDown) {
			int nextHeight = visible ? settings.rect.height + settings.software->colSpacing : 0;
			_uiNext->settings.rect.x = settings.rect.x;
			_uiNext->settings.rect.y = settings.rect.y + nextHeight;
		} else {
			int nextWidth = visible ? settings.rect.width + settings.software->colSpacing : 0;
			_uiNext->settings.rect.x = settings.rect.x + nextWidth;
		}
		_uiNext->reFlowUis();
	} else {
		_uiFather->updateSoftwareRect();
		//cout << "----- this is the end" << endl;
	}
}

//--------------------------------------------------------------
void ofxDmtrUI3::autoFit() {
	
	//cout << "autoFit :: " + UINAME << endl;
	if (showInterface) {
		
		
//		int maxw = settings.minimumWidth + settings.software->colPadding;
		int maxw = settings.minimumWidth;
		maxw = MAX(maxw, settings.sliderDimensions.x + settings.software->colPadding);
		
		int maxh = 0;
		for (auto & e : elements) {
			maxw = MAX(maxw, e->boundsRect.x + e->boundsRect.width);
			maxh = MAX(maxh, e->boundsRect.y + e->boundsRect.height);
		}
		
		if (elements.size() == 0) {
			// xaxa ta errado. seria *2 aqui...
			//maxw = settings.sliderDimensions.x + settings.software->colPadding*3;
			maxw = settings.sliderDimensions.x + settings.software->colPadding*2;
		}
		
		settings.rect.width  = maxw + settings.software->colPadding;
		settings.rect.height = maxh + settings.software->colPadding;

		
		fboSettings.width = settings.rect.width;
		fboSettings.height = settings.rect.height;

		// novidade : somente redimensiona se for necessario
		if (fboUI.getWidth() != fboSettings.width || fboUI.getHeight() != fboSettings.height) {
			if (isDown) {
				
				settings.rect.width = fboSettings.width = _uiLast->fboSettings.width;
				settings.sliderDimensions.x = _uiLast->settings.sliderDimensions.x;
			}

			fboUI.allocate(fboSettings);
			fboUI.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

			settings.redraw = true;
			settings.needsRedraw = true;
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
		// aqui salva em preset
		if (u.second.saveMode == PRESETSFOLDER) {
			//if (!u.second.keepSettings && (u.second.loadSave && u.second.savePreset)) {
			string nome = getPresetsPath(ofToString(n) + u.first +  ".xml");
			u.second.save(nome);
		}
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
			software.eventOnClick = false;

			for (auto & u : uis) {
				if (u.second.loadMode == PRESETSFOLDER) {
					string nome = getPresetsPath(ofToString(n) + u.first + ".xml");
					u.second.load(nome);
				}
			}
			software.eventOnClick = true;
			notify("loadPresetAll");

			// xaxa event here
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
		pEasy.clear();
		pInt.clear();
		pBool.clear();
		pString.clear();
		pPoint.clear();
		pColor.clear();
		pColorEasy.clear();
	}
	settings.needsRedraw = true;
}


//--------------------------------------------------------------
void ofxDmtrUI3::uiEvents(uiEv & e) {
	

	for (auto & f : uiEventFunctions) {
		f(e);
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
		
		// da erro aqui se nao tiver um elemento chamado allPresets no master.
		// contornar?
		
		if (getElement("allPresets") != NULL) {
			for (auto & e : ((mult*)getElement("allPresets"))->elements) {
				e->updateImage();
			}
		}
	}
	
	// SAVE / LOAD Presets
	
	else if (e.name == "allPresets") {
		string p = getElement("allPresets")->getValString();
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			savePresetAll(ofToInt(p));
		}
		else {
			// aqui precisa entrar na fila pro futuro.
			if (enableLoadPresets) {
				loadPresetAll(ofToInt(p));
			}
		}
	}
	
	else if (e.name == "easing") {
		easing = pFloat["easing"];
		for (auto & p : uis) {
			// XAXA Passar isso aqui pra software master
			p.second.easing = pFloat["easing"];
		}
	}
	
	
	
	if (e.tag == "color" || e.tag == "colorHsvA") {
		string name = "";
		if (e.kind == SLIDER2D && e.onClick) {
			name = e.name;
		}

		if (e.kind == SLIDER) {
            name = ofSplitString(e.name, "_")[0];
		}

        // for some reason there is a empty name when I load preset
        if (name != "") {
            float h = pPoint[name].x * 255.0;
            float s = pFloat[name + "_S"];
            float b = pPoint[name].y * 255.0;
            float a = 255.0;
            if (e.tag == "colorHsvA") {
                a = pFloat[name + "_Alpha"];
            }
            pColor[name] = ofColor::fromHsb(h,s,b,a);
            pColor[name].a = a;
//            cout << UINAME << endl;
//            cout << "color " << name << " = " << pColor[name] << endl;
        }
	}

	if (ofIsStringInString(e.name, "_shortcut") ) {
		vector <string> split = ofSplitString(e.name, "_shortcut");
		string name = split[0];
		set(name, ofToFloat(pString[e.name]));
	}
}

void ofxDmtrUI3::createRadio(string name, vector<string> options, string sel) {
	elements.push_back(new radio(name, settings, options));
	if (sel != "") {
		((radio*)elements.back())->set(sel);
	}
}

string ofxDmtrUI3::getFileFullPath(string n) {
	radio * e = getRadio(n);
	return e != NULL ? e->getFullFileName() : "";
}

void ofxDmtrUI3::showUI(int show) {
	visible = show == 1;
	showInterface = show;
	if (show == 1) {
		autoFit();
	}
	else {
		settings.rect.width = 40;
		if (show == 2) {
			settings.rect.width = 1;
		}
		settings.redraw = true;
		settings.needsRedraw = true;
	}
	
	if (_uiNext != NULL) {
		if (_uiNext->isDown) {
			_uiNext->showUI(show);
		}
	}
	reFlowUis();
}


void ofxDmtrUI3::set(string el, bool v, bool notifyEvent) {
	toggle * e = getToggle(el);
	if (e != NULL) {
		e->set(v, notifyEvent);
		
	}
};

void ofxDmtrUI3::set(string el, string v, bool notifyEvent) {
	radio * e = getRadio(el);
	if (e != NULL) {
		e->set(v, notifyEvent);
	}
};

void ofxDmtrUI3::set(string el, float v, bool notifyEvent) {
	slider * e = getSlider(el);
	if (e != NULL) {
		e->set(v, notifyEvent);
	}
};

void ofxDmtrUI3::set(string el, int v, bool notifyEvent) {
	slider * e = getSlider(el);
	if (e != NULL) {
		e->set(v, notifyEvent);
	}
};

void ofxDmtrUI3::set(string el, ofPoint v, bool notifyEvent) {
	slider2d * e = getSlider2d(el);
	if (e != NULL) {
		e->set(v, notifyEvent);
	}
};


//--------------------------------------------------------------
void ofxDmtrUI3::allocateAndClearFbo(ofFbo &f, ofPoint dimensions) {
	//f.allocate(
	
	int w = software.w;
	int h = software.h;
	int m = software.multiSampling;
	if (dimensions != ofPoint(0,0)) {
		w = dimensions.x;
		h = dimensions.y;
		m = dimensions.z;
	}
	
	//cout << "allocate and clear fbo " << w << "x" << h << " - " << m << endl;
	
//#ifdef DMTRUI_TARGET_TOUCH || #ifdef DMTRUI_TARGET_NOARB
//#else
//	int format = GL_RGBA32F_ARB; //GL_RGBA32F_ARB  //GL_RGBA32F
//#endif
	if (m == 0) {
		//cout << "allocate without multisampling" << endl;
		f.allocate			(w, h, fboFormat);
	} else {
		f.allocate			(w, h, fboFormat, m);
	}
	
	f.begin();
	ofClear(0,255);
	f.end();	
}

void ofxDmtrUI3::saveMaster(bool s) {
	cout << "saveMaster :: " + UINAME << endl;
	//ofSystemAlertDialog("saveMaster");
	string file = "_presets/" + UINAME + ".xml";
	cout << file << endl;
	save("_presets/" + UINAME + ".xml");
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
