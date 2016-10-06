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

#include "ofxDmtrUI.h"

//--------------------------------------------------------------
void ofxDmtrUI::setup() {
	ofSetEscapeQuitsApp(false);

	flow = ofPoint(marginx, marginy);

	if (!fboColumn.isAllocated()) {
		fboColumn.allocate(coluna.width, coluna.height, GL_RGBA);
		fboColumn.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		fboColumn.begin();
		ofClear(0);
		fboColumn.end();
	}
	ofAddListener(ofEvents().draw, this, &ofxDmtrUI::onDraw);
	ofAddListener(ofEvents().update, this, &ofxDmtrUI::onUpdate);
	ofAddListener(ofEvents().keyPressed, this, &ofxDmtrUI::onKeyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxDmtrUI::onKeyReleased);
	ofAddListener(ofEvents().mousePressed, this, &ofxDmtrUI::onMousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofxDmtrUI::onMouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxDmtrUI::onMouseReleased);
	ofAddListener(ofEvents().mouseMoved, this, &ofxDmtrUI::onMouseMoved);
	ofAddListener(ofEvents().exit, this, &ofxDmtrUI::onExit);

#ifdef DMTRUI_TARGET_TOUCH
	ofAddListener(ofEvents().touchDown, this, &ofxDmtrUI::onTouchDown);
	ofAddListener(ofEvents().touchMoved, this, &ofxDmtrUI::onTouchMoved);
	ofAddListener(ofEvents().touchUp, this, &ofxDmtrUI::onTouchUp);
#endif

	if (keepSettings) {
		string fileName = presetsFolder + UINAME + ".xml";
		if (ofFile::doesFileExist(fileName)) {
			load(fileName);
		}
	}
	dmtrUIEvent te;
	te.nome = "setup";
	ofNotifyEvent(evento, te);
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
	if (showGui) {
		ofEnableAlphaBlending();
		if (redraw) {
			fboColumn.begin();
			ofClear(colunaBackground);
//			for (auto & e : elements) 	{ e.draw(); }
			for (auto & e : sliders) 	{ e.draw(learnMode); }
			for (auto & e : toggles) 	{ e.draw(); }
			for (auto & e : labels)  	{ e.draw(); }
			for (auto & e : radios)  	{ e.draw(); }
			//for (auto & e : sliders2d)  { e.draw(); }
			if (allPresets.ok) {
				allPresets.draw();
			}
			fboColumn.end();
			redraw = false;
		}
		//ofSetColor(255, columnOver ? 255 : 128);
		ofSetColor(255, columnOver ? opacity : opacityRest);
		//ofEnableBlendMode(OF_BLENDMODE_DIFFERENCE);
		//ofEnableBlendMode(OF_BLENDMODE_ADD);
		//ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		if (blendMode) {
			ofEnableBlendMode(blendMode);
		}
		fboColumn.draw(coluna.x, coluna.y);
		ofPushMatrix();
		ofTranslate(coluna.x, coluna.y);
		for (auto & e : sliders2d)  { e.draw(); }
		for (auto & e : inspectors)  { e.draw(); }
		ofPopMatrix();
		if (blendMode) {
			ofEnableAlphaBlending();
		}
		ofSetColor(255);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::save(string xml){
	if (debug) {
		cout << "save xml in interface " +UINAME+ ": " + xml << endl;
	}
	ofxXmlSettings settings;

	bool newVersion = true;
	if (newVersion) {
		settings.setValue("ofxDmtrUIVersion", 2.0);
		for (auto & e : sliders) {
			if (e.isInt) {
				settings.setValue("slider:" + e.nome,   *e._valInt);
			} else {
				settings.setValue("slider:" + e.nome, *e._val);
			}
		}
		for (auto & e : toggles) {
			settings.setValue("toggle:" + e.nome, *e._val);
		}
		for (auto & e : radios) {
			if (e.multiple) {
				int i = 0;
				for (auto & o : e.opcoes) {
					settings.setValue("radio:"+e.nome+":"+o, *e._vals[i]);
					i++;
				}
			} else {
				settings.setValue("radio:"+e.nome, *e._val);
			}
		}
		for (auto & e : sliders2d) {
			ofPoint xy = *e._val;
			settings.setValue("slider2d:" + e.nome + ":x", xy.x);
			settings.setValue("slider2d:" + e.nome + ":y", xy.y);
		}
		settings.setValue("presets", allPresets.valor);
	}
	else {
		for (auto & s : sliders) {
			if (s.isInt) {
				settings.setValue(s.nome, *s._valInt);
			} else {
				settings.setValue(s.nome, *s._val);
			}
		}

		for (auto & t : toggles) {
			settings.setValue(t.nome, *t._val);
		}

		for (auto & r : radios) {
			if (!r.multiple) {
				settings.setValue(r.nome, *r._val);
			} else {
				int i = 0;
				for (auto & o : r.opcoes) {
					settings.setValue(o, *r._vals[i]);
					i++;
				}
			}
		}
		// ainda nao sei se funciona
		for (auto & e : sliders2d) {
			ofPoint xy = *e._val;
			settings.setValue(e.nome + "x", xy.x);
			settings.setValue(e.nome + "y", xy.y);
		}
		// salvar tb o presets.
	}
	settings.save(xml);
}

//--------------------------------------------------------------
void ofxDmtrUI::load(string xml){
	if (debug) {
		cout << "load: " + xml << endl;
		if (!ofFile::doesFileExist(xml)) {
			cout << " ----- file doesn't exist: ";
			cout << xml << endl;
		}
	}
	ofxXmlSettings settings;
	settings.loadFile(xml);

	int UIVersion = settings.getValue("ofxDmtrUIVersion", 0);

	if (UIVersion == 2) {
		for (auto & e : sliders) {
			e.setValue(settings.getValue("slider:" +e.nome, e.def));
		}
		for (auto & e : toggles) {
			e.setValue(settings.getValue("toggle:" +e.nome, e.def));
		}
		for (auto & e : radios) {
			if (e.multiple) {
				int i = 0;
				for (auto & o : e.opcoes) {
					*e._vals[i] = settings.getValue("radio:"+e.nome+":"+o, false);
					i++;
				}
				e.draw();
			} else {
				e.setValue(settings.getValue("radio:" + e.nome, ""), 2);
			}

		}
		for (auto & e : sliders2d) {
			float x = settings.getValue("slider2d:"+e.nome+":x", 0.0);
			float y = settings.getValue("slider2d:"+e.nome+":y", 0.0);
			e.setValue(ofPoint(x, y));
		}
	} else {
		for (auto & e : sliders) {
			e.setValue(settings.getValue(e.nome, e.def));
		}
		for (auto & e : toggles) {
			e.setValue(settings.getValue(e.nome, e.def));
		}
		for (auto & e : radios) {
			// default? algo como asterisco no txt? ou um parametro novo, um tab a mais.
			//if (*e._val != settings.getValue(e.nome, ""))
			if (!e.multiple) {
				e.setValue(settings.getValue(e.nome, ""), 2);
			}

			else {
				int i = 0;
				for (auto & o : e.opcoes) {
					*e._vals[i] = settings.getValue(o, false);
					i++;
				}
				e.draw();
			}
		}
		for (auto & e : sliders2d) {
			float x = settings.getValue(e.nome+"x", 0.0);
			float y = settings.getValue(e.nome+"y", 0.0);
			e.setValue(ofPoint(x, y));
		}
	}

	dmtrUIEvent te;
	te.nome = "loadPreset";
	ofNotifyEvent(evento, te);

}

//--------------------------------------------------------------
void ofxDmtrUI::keyPressed(int key){

	if (key == '=') {
		showGui = !showGui;
	}

	if (saveLoadShortcut) {
		if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' ||
			key == '8' || key == '9' || key == '0'
			) {
			string nome = getPresetsFolder() + UINAME + ofToString(char(key)) + ".xml";
			if (ofGetKeyPressed(OF_KEY_COMMAND)) {
				save(nome);
			} else {
				load(nome);
			}
		}
	}

	if (useShortcut) {
		if (ofGetKeyPressed(OF_KEY_COMMAND)) {
			if ((key == 'f' || key == 'F')) {
				ofToggleFullscreen();
			}
		} else {
			if (key == 'a' || key == 'A') {
				loadPresetAll(0 + pInt["atalhoOffset"]);
			}
			else if (key == 's' || key == 'S') {
				loadPresetAll(1 + pInt["atalhoOffset"]);
			}
			else if (key == 'd' || key == 'D') {
				loadPresetAll(2 + pInt["atalhoOffset"]);
			}
			else if (key == 'f' || key == 'F') {
				loadPresetAll(3 + pInt["atalhoOffset"]);
			}
			else if (key == 'g' || key == 'G') {
				loadPresetAll(4 + pInt["atalhoOffset"]);
			}
			else if (key == 'h' || key == 'H') {
				loadPresetAll(5 + pInt["atalhoOffset"]);
			}
			else if (key == 'j' || key == 'J') {
				loadPresetAll(6 + pInt["atalhoOffset"]);
			}
			else if (key == 'k' || key == 'K') {
				loadPresetAll(7 + pInt["atalhoOffset"]);
			}
			else if (key == 'l' || key == 'L') {
				loadPresetAll(8 + pInt["atalhoOffset"]);
			}
			else if (key == ';') {
				loadPresetAll(9 + pInt["atalhoOffset"]);
			}
			else if (key == 39) { //single quote
				loadPresetAll(10 + pInt["atalhoOffset"]);
			}
			if (key == 'z' || key == 'Z') {
				loadPresetAll(11 + pInt["atalhoOffset"]);
			}
			else if (key == 'x' || key == 'X') {
				loadPresetAll(12 + pInt["atalhoOffset"]);
			}
			else if (key == 'c' || key == 'C') {
				loadPresetAll(13 + pInt["atalhoOffset"]);
			}
			else if (key == 'v' || key == 'V') {
				loadPresetAll(14 + pInt["atalhoOffset"]);
			}
			else if (key == 'b' || key == 'B') {
				loadPresetAll(15 + pInt["atalhoOffset"]);
			}
			else if (key == 'n' || key == 'N') {
				loadPresetAll(16 + pInt["atalhoOffset"]);
			}
			else if (key == 'm' || key == 'M') {
				loadPresetAll(17 + pInt["atalhoOffset"]);
			}
			else if (key == ',') {
				loadPresetAll(18 + pInt["atalhoOffset"]);
			}
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::keyReleased(int key){
}

//--------------------------------------------------------------
void ofxDmtrUI::mousePressedDragged(int x, int y, int button){
	redraw = true;
//	if (slideFree)
	{
		for (auto & e : radios) {
			if (e.rect.inside(x - coluna.x,y - coluna.y)) {
				e.checkMouse(x - coluna.x,y - coluna.y);
				// 5 agosto de 2016, pro Nike
				mousePressedElement = e.nome;
			}
		}

		for (auto & e : toggles) {
			if (e.rect.inside(x - coluna.x,y - coluna.y) && !e.inside) {
				e.inside = true;
				e.flip();
				mousePressedElement = e.nome;
			}
		}

		if (allPresets.ok && allPresets.rect.inside(x - coluna.x,y - coluna.y)) {
			allPresets.checkMouse(x - coluna.x, y - coluna.y);
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::mouseDragged(int x, int y, int button){
	// simplificar, fazer o x - coluna.x antes

	for (auto & e : sliders2d) {
		bool check = slideFree || e.nome == mousePressedElement;
		if (e.rect.inside(x - coluna.x, y - coluna.y)) {
			if (check) {
				e.checkMouse(x - coluna.x, y - coluna.y);
			}
			e.inside = true;
		} else {
			if (e.inside) {
				if (check) {
					e.checkMouse(x - coluna.x, y - coluna.y);
				}
				if (slideFree) {
					e.inside = false;
				}
			}
		}
	}

	for (auto & e : sliders) {
		bool check = slideFree || e.nome == mousePressedElement;
		if (e.rect.inside(x - coluna.x, y - coluna.y)) {
			if (check) {
				e.update(x - coluna.x, y - coluna.y);
			}
			e.inside = true;
		} else {
			if (e.inside) {
				if (check) {
					e.update(x - coluna.x, y - coluna.y);
				}
				if (slideFree) {
					e.inside = false;
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::mousePressed(int x, int y, int button){
	for (auto & e : sliders) {
		if (e.rect.inside(x - coluna.x,y - coluna.y)) {
			e.update(x - coluna.x, y - coluna.y);
			e.inside = true;
			mousePressedElement = e.nome;
		}
	}

	for (auto & e : sliders2d) {
		if (e.rect.inside(x - coluna.x,y - coluna.y)) {
			e.checkMouse(x - coluna.x, y - coluna.y);
			e.inside = true;
			mousePressedElement = e.nome;
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::mouseReleased(int x, int y, int button){
	redraw = false;
	for (auto & e : toggles) {
		e.inside = false;
		if (e.bang) {
			*e._val = false;
			redraw = true;
		}
	}
	for (auto & e : sliders) {
		e.inside = false;
	}
	for (auto & e : sliders2d) {
		e.inside = false;
	}
	//for (auto & s : radios) {
		//s.clicked = false;
	//}
}

//--------------------------------------------------------------
void ofxDmtrUI::mouseAll(int x, int y, int button){
	columnOver = coluna.inside(x,y);
}

//--------------------------------------------------------------
void ofxDmtrUI::exit() {
	if (keepSettings) {
		cout << "ofxDmtrUI exit" << endl;
		string fileName = presetsFolder + UINAME + ".xml";
		cout << "Saving::"+fileName << endl;
		save(fileName);
	}
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
void ofxDmtrUI::onKeyPressed(ofKeyEventArgs& data) {
	keyPressed(data.key);
}

//--------------------------------------------------------------
void ofxDmtrUI::onKeyReleased(ofKeyEventArgs& data) {
	keyReleased(data.key);
}

//--------------------------------------------------------------
void ofxDmtrUI::onMousePressed(ofMouseEventArgs& data) {
	if (showGui) {
		mousePressed				(data.x, data.y, data.button);
		mouseAll					(data.x, data.y, data.button);
		mousePressedDragged		(data.x, data.y, data.button);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::onMouseDragged(ofMouseEventArgs& data) {
	if (showGui) {
		mouseDragged			(data.x, data.y, data.button);
		mouseAll				(data.x, data.y, data.button);
		mousePressedDragged (data.x, data.y, data.button);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::onMouseReleased(ofMouseEventArgs& data) {
	if (showGui) {
		mouseReleased		(data.x, data.y, data.button);
		mouseAll				(data.x, data.y, data.button);
	}
}


//--------------------------------------------------------------
void ofxDmtrUI::onMouseMoved(ofMouseEventArgs& data) {
	if (showGui) {
		mouseAll				(data.x, data.y, data.button);
	}
}

#ifdef DMTRUI_TARGET_TOUCH

//--------------------------------------------------------------
void ofxDmtrUI::onTouchDown(ofTouchEventArgs &data) {
	if (showGui) {
		mousePressed				(data.x, data.y, data.id);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::onTouchMoved(ofTouchEventArgs &data) {
	if (showGui) {
		mouseDragged	(data.x, data.y, data.id);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::onTouchUp(ofTouchEventArgs &data) {
	if (showGui) {
		mouseReleased	(data.x, data.y, data.id);
		mouseAll		(data.x, data.y, data.id);
	}

}
#endif

//--------------------------------------------------------------
void ofxDmtrUI::onExit(ofEventArgs &data) {
	exit();
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

//--------------------------------------------------------------
void ofxDmtrUI::createFromText(string file) {
	//cout << file << endl;
	// 27 de setembro isso pode quebrar muita coisa
	// se tiver multiplas pastas fazer o seguinte
	string f = ofSplitString(file, "/").back();
	//cout << f << endl;
	string onlyName = ofSplitString(f, ".")[0];
	//cout << onlyName << endl;
//	if (UINAME == "") {
//		UINAME = onlyName;
//	}
	if (ofFile::doesFileExist(file)) {
		createdFromTextFile = file;
		
		if (debug) {
			cout << "ofxDmtrUI createFromText ::: " + file << endl;
		}

		vector <string> linhas = textToVector(file);
		for (auto & l : linhas) {
			//cout << l << endl;
			createFromLine(l);
		}
		addAllListeners();
	} else {
		cout << "ofxDmtrUI createFromText ::: File not found " + file << endl;
	}

	dmtrUIEvent te;
	te.nome = "createFromText";
	ofNotifyEvent(evento, te);
}

//--------------------------------------------------------------
void ofxDmtrUI::addAllListeners() {
	// end reading from text files
	for (auto & e : sliders) {
		ofAddListener(e.uiEvent,this, &ofxDmtrUI::uiEvents);
		ofAddListener(e.evento ,this, &ofxDmtrUI::uiEventsNeu);
	}
	for (auto & e : sliders2d) {
		ofAddListener(e.uiEvent,this, &ofxDmtrUI::uiEvents);
		ofAddListener(e.evento,this, &ofxDmtrUI::uiEventsNeu);
	}

	for (auto & e : radios) {
		ofAddListener(e.uiEvent,this, &ofxDmtrUI::uiEvents);
		ofAddListener(e.evento ,this, &ofxDmtrUI::uiEventsNeu);
	}
	for (auto & e : toggles) {
		ofAddListener(e.uiEvent,this, &ofxDmtrUI::uiEvents);
		ofAddListener(e.evento ,this, &ofxDmtrUI::uiEventsNeu);
	}
	ofAddListener(allPresets.uiEvent,this, &ofxDmtrUI::uiEvents);
}

//--------------------------------------------------------------
void ofxDmtrUI::createFromLine(string l) {
	if (l == "") { // spacer
		//flow.y += 25;
		flow.y += sliderHeight + sliderMargin;
	} else {

		vector <string> cols = ofSplitString(l, "	");
		string tipo = cols[0];
		string nome = "";
		if (cols.size()>1) {
			nome = cols[1];
		}

		if (tipo == "float") tipo = "slider";
		if (tipo == "bool") tipo = "toggle";

		if (tipo == "newcol" || tipo == "newCol") {
			float sw = MAX(pFloat["maxWidthHorizontal"] - coluna.x, sliderWidth) ;
			flow.x += sw + marginx * 1;
			flow.y = marginy;
			pFloatBak["flowx"] = flow.x;
			pFloat["maxWidthHorizontal"] = 0;
		}

		// dava pra fazer um configbool pra todos, nao ficar inventando nomes exclusivos.
		//assim pula mais rapido pra fora do loop

		else if (tipo == "dirListExtension") {
			dirListEntireName = ofToBool(cols[1]);
		}

		else if (tipo == "uiClear" || tipo == "clear") {
			clear();
		}

		else if (tipo == "addUI" || tipo == "addUIDown") {
			uis[nome].UINAME = nome;
			uis[nome].hueStart = hue;

			if (ofFile::doesFileExist("uiAll.txt")) {
				uis[nome].createFromText("uiAll.txt");
			}

			string fileName = nome+".txt";
			if (ofFile::doesFileExist(fileName)) {
				uis[nome].createFromText(fileName);
			}

			if (_uiLast == NULL) {
				if (tipo == "addUI") {
					uis[nome].nextTo(*this);
				} else {
					uis[nome].downTo(*this);
				}
			} else {
				if (tipo == "addUI") {
					uis[nome].nextTo(*_uiLast);
				} else {
					uis[nome].downTo(*_uiLast);
				}
			}
			_uiLast = &uis[nome];
			uis[nome]._uiFather = this;
			uis[nome].setup();
			
			allUIs.push_back(&uis[nome]);
		}

		// talvez remover
		else if (tipo == "radioUI") {
			string nome = cols[1];
			string uiName = cols[2];
			radioUIMap[nome] = uiName;
		}

		else if (tipo == "hueStart") {
			hueStart = ofToFloat(cols[1]);
		}

		// em portugues?
		else if (tipo == "colunaBackground") {
			colunaBackground  = ofColor::fromHex(ofHexToInt(cols[1].substr(1,6)));
			// fazer aqui o lance pro alpha
			if (cols[1].length() > 7) {
				colunaBackground.a = ofHexToInt(cols[1].substr(7));
			} else {
				colunaBackground.a = 100;
			}
		}
		else if (tipo == "slideFree") {
			slideFree = ofToBool(cols[1]);
		}
		else if (tipo == "blendMode") {
			if (cols[1] == "ADD") {
				blendMode = OF_BLENDMODE_ADD;
			}
			else if (cols[1] == "SCREEN") {
				blendMode = OF_BLENDMODE_SCREEN;
			}
		}

		else if (tipo == "bw") {
			bw  = ofToInt(cols[1]);
		}

		else if (tipo == "keepSettings") {
			keepSettings  = ofToBool(cols[1]);
		}

		else if (tipo == "opacity") {
			opacity  	= ofToInt(cols[1]);
			opacityRest = ofToInt(cols[1]);
		}

		else if (tipo == "opacityRest") {
			opacityRest = ofToInt(cols[1]);
		}

		else if (tipo == "saveY") {
			pFloatBak["saveY"] = flow.y;
		}

		else if (tipo == "restoreY") {
			flow.y = pFloatBak["saveY"];
		}

		else if (tipo == "saveX") {
			pFloatBak["saveX"] = flow.x;
		}

		else if (tipo == "restoreX") {
			flow.x = pFloatBak["saveX"];
		}

		else if (tipo == "margin") {
			flow.x = flow.y = marginx = marginy = ofToFloat(cols[1]);
		}
		else if (tipo == "marginy") {
			flow.y = marginy = ofToFloat(cols[1]);
		}
		else if (tipo == "marginx") {
			flow.x = marginx = ofToFloat(cols[1]);
		}
		else if (tipo == "flowX") {
			flow.x = ofToFloat(cols[1]);
		}
		else if (tipo == "flowY") {
			flow.y = ofToFloat(cols[1]);
		}

		else if (tipo == "presetDimensions") {
			vector <string> dimensions = ofSplitString(cols[1], " ");
			presetDimensions = ofPoint(ofToInt(dimensions[0]), ofToInt(dimensions[1]));
		}
		else if (tipo == "autoFit") {
			autoFit();
		}
		else if (tipo == "autoFitW") {
			autoFit(1,0);
		}
		else if (tipo == "autoFitH") {
			autoFit(0,1);
		}
		else if (tipo == "sliderWidth") {
			sliderWidth = ofToFloat(cols[1]);
		}
		else if (tipo == "sliderHeight") {
			sliderHeight = ofToFloat(cols[1]);
		}
		else if (tipo == "sliderMargin") {
			sliderMargin = ofToFloat(cols[1]);
		}
		else if (tipo == "flowVert") {
			flowDirection = VERT;
			if (pFloatBak["flowx"]) {
				flow.x = pFloatBak["flowx"];
			}
			flow.y += lastHeight + sliderMargin;
		}
		else if (tipo == "flowHoriz") {
			flowDirection = HORIZ;
			pFloatBak["flowx"] = flow.x;
		}

		else if (tipo == "rect") {
			vector <string> v = ofSplitString(cols[1], " ");
			coluna = ofRectangle(ofToInt(v[0]),ofToInt(v[1]),ofToInt(v[2]),ofToInt(v[3]));
		}

		else {
			if (tipo.substr(0,1) != "#") { //comment
				if (cols.size()>2) {
					if (cols.size()==3)
						create(nome, tipo, cols[2]);
					if (cols.size()==4)
						create(nome, tipo, cols[2], cols[3]);
					if (cols.size()==5)
						create(nome, tipo, cols[2], cols[3], cols[4]);

				} else {
					create(nome, tipo);
				}
			}
		}
	}
}

////--------------------------------------------------------------
//void ofxDmtrUI::createRadio(string nome, vector <string> opcoes) {
//	radio temp;
//	temp.nome = nome;
//	temp.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
//	temp.opcoes = opcoes;
//	temp._val = &pString[nome];
//	temp.init();
//	indexElement[nome] = radios.size();
//	radios.push_back(temp);
//	element te;
//	te.set(radios.back());
//	elements.push_back(te);
//	lastHeight = temp.rect.height;
//}

//--------------------------------------------------------------
void ofxDmtrUI::create(string nome, string tipo, string valores, string valores2, string valores3) {
	// vamos tentar inventar uma variavel aqui nos parametros somente.
	ofStringReplace(valores, "sliderWidth", ofToString(sliderWidth));

	hue = int(flow.x/8.0 + flow.y/6.0 + hueStart)%255;
	int saturation = bw ? 0 : 255;
	int brightness = bw ? 50 : 200;

	ofColor cor = ofColor::fromHsb(hue,saturation,brightness);

	lastHeight = sliderHeight;
	lastWidth = sliderWidth;

	bool flowing = true;

	// feiooo. da pra fazer muito melhor depois
	if (tipo == "presets" || tipo == "presets2") {
		int cols = 3;
		int rows = 4;
		if (nome != "") {
			vector <string> vals = ofSplitString(nome, " ");
			cols = ofToInt(vals[0]);
			rows = ofToInt(vals[1]);
		}
		int w = presetDimensions.x;
		int h = presetDimensions.y;
		int x = flow.x;
		int y = flow.y;
		// temporario

		allPresets.ok = true;
		// temporario também
		allPresets.rect = ofRectangle(flow.x, flow.y, cols * w + (cols)*sliderMargin, rows * h + (rows)*sliderMargin);
		ofSetColor(255);
		for (int a=0; a<cols * rows; a++) {
			preset tp;
			tp.index = a;
			tp.nome = ofToString(a);
			tp.rect.x = x;
			tp.rect.y = y;
			tp.rect.width = w;
			tp.rect.height = h;
			tp.fbo.allocate(w, h, GL_RGBA);
			tp.fbo.begin();
			ofClear(0,255);

			// checks if the key exists in the map, in the case, not found
			if (indexElement.find("presetsFolder") == indexElement.end()) {
				string filename = getPresetsFolder() + ofToString(a) + ".tif";
				if (ofFile::doesFileExist(filename)) {
					tp.img.load(filename);
					tp.img.draw(0,0);
				} else {
					cout << "file doesnt exists: " + filename << endl;
				}
			}
			tp.fbo.end();
			allPresets.presets.push_back(tp);

			if (a%cols==(cols-1)) {
				x = flow.x;
				y += h + sliderMargin;
			} else {
				x += w + sliderMargin;
			}
		}

//		flow.y += allPresets.rect.height + sliderMargin;
		flow.y += allPresets.rect.height ;

		lastHeight = 0;

		element te;



		te.set(tipo == "presets" ? allPresets : allPresets2);
		elements.push_back(te);

//		te.tipo = PRESETS;
//		te._rect = &allPresets.rect;
//		te._presets = &allPresets;
//		elements.push_back(te);

	}

	else if (tipo == "slider2d" || tipo == "fbo" ) {
		slider2d ts;
		ts.nome = nome;
		ts.cor = cor;
		ts.rect = ofRectangle(flow.x, flow.y, sliderWidth, 50);
		if (tipo == "fbo") {
			ts.isSlider = false;
			if (valores != "") {
				vector<string> vals = ofSplitString(valores," ");
				if (vals[0] != "")
					ts.rect.width = ofToInt(vals[0]);
				if (vals[1] != "")
					ts.rect.height = ofToInt(vals[1]);
			}
		}

		pPoint[nome] = ofPoint(.5, .5);
		ts._val = &pPoint[nome];
		lastHeight = ts.rect.height;
		lastWidth  = ts.rect.width;
		indexElement[nome] = sliders2d.size();

		sliders2d.push_back(ts);

		element te;
		te.set(sliders2d.back());
		elements.push_back(te);
	}

	else if (tipo == "inspector" || tipo == "inspectorFloat" || tipo == "fps") {
		inspector te;
		te.nome = nome;
		te.cor = cor;
		te.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
//		te.tipo = tipo == "inspectorFloat" ? "float" : "string";
		te.tipo = tipo;
//		if (tipo == "fps") {
//			te.tipo = "fps";
//		}
		if (te.tipo == "string") {
			te._val = &pInspector[nome];
			pInspector[nome] = "";
		}
		else if (te.tipo == "inspectorFloat") {
			te._valFloat = &pInspectorFloat[nome];
			pInspectorFloat[nome] = 0;
		}
		lastHeight = te.rect.height;
		lastWidth  = te.rect.width;
		te.init();
		//indexElement[nome] = sliders2d.size();

		// Ainda falta todo o resto
		inspectors.push_back(te);
	}

	else if (tipo == "slider" || tipo == "int" || tipo == "sliderVert") {
		slider ts;
		ts.nome = nome;

		// 16 julho de 2016
		ts.tipo = "int" ? SLIDERINT : SLIDER;
		ts.vert = (tipo == "sliderVert");

		// inverte as coordenadas
		if (ts.vert) {
			lastHeight = sliderWidth;
			lastWidth = sliderHeight;
		}

		if (ts.vert) {
			ts.rect = ofRectangle(flow.x, flow.y, sliderHeight, sliderWidth);
		} else {
			ts.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		}
		ts.cor = cor;
		ts.isInt = tipo == "int";

		if (ts.isInt) {
			ts._valInt = &pInt[ts.nome];
		} else {
			ts._val = &pFloat[ts.nome];
		}

		//cout << "valores : " + valores << endl;
		if (valores != "") {
			vector <string> vals = ofSplitString(valores, " ");
			ofVec3f val = ofVec3f(ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2]));
			ts.min = val.x;
			ts.max = val.y;
			ts.def = val.z;
			// for initialization without mouse.
			// removed 18 june 2016 for shader reload keeping parameters intact initialization
			//ts.setValue(ts.def);
		}

		if (!pFloat[nome])
			pFloat[nome] = ts.def;

		if (ts.isInt) {
			pInt[nome] = ts.def;
		}
		indexElement[nome] = sliders.size();
		slidersIndex[nome] = sliders.size();

		lastHeight = ts.rect.height;
		lastWidth  = ts.rect.width;


		ts.init();
		sliders.push_back(ts);

		element te;
		te.set(sliders.back());
		elements.push_back(te);
	}

	else if (tipo == "toggle" || tipo == "bang" || tipo == "toggleNolabel") { // bool
		toggle tt;
		tt.nome = nome;
		tt.rect = ofRectangle(flow.x, flow.y, sliderHeight, sliderHeight);
		tt.cor = cor;
		if (valores == "1") {
			tt.def = true;
		}
		if (tipo == "bang") {
			tt.bang = true;
		}
		pBool[nome] = tt.def;
		tt._val = &pBool[nome];
		indexElement[nome] = toggles.size();
		togglesIndex[nome] = toggles.size();

		if (tipo == "toggleNolabel") {
			tt.showLabel = false;
			lastWidth  = tt.rect.width;
		}

		tt.init();
		toggles.push_back(tt);

		togglesMap[nome] = &toggles.back();
//		if (toggles.size()>0) {
//			if (nome == toggles[toggles.size()-1].nome) {
//				togglesMap[nome] = &toggles[toggles.size()-1];
//			}
//		}

//		cout << nome << endl;
//		cout << toggles.back().nome << endl;
//		cout << "-----" << endl;

		element te;
		//te._toggle = &toggles.back();
		te.set(toggles.back());
		elements.push_back(te);
	}

	else if (tipo == "label") {
		label tl;
		tl.nome = nome;
		tl._val = &pLabel[nome];
		tl.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		tl.cor = cor;
		if (bw) { tl.cor =  ofColor(255); }

//		element te;
//		te._rect = &tl.rect;
//		te.tipo = LABEL;
//		te._label = &tl;
//		elements.push_back(te);

		labels.push_back(tl);

		element te;
		te.set(labels.back());
		elements.push_back(te);

		lastHeight = 20;
	}

	else if (tipo == "radio" || tipo == "radioMult" || tipo == "radioText") {

		radio temp;
		temp.nome = nome;
		temp.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		temp.cor = cor;

		if (tipo == "radioText") {
			temp.opcoes = textToVector(valores);
		} else {
			temp.opcoes = ofSplitString(valores, " ");
		}

//		for (auto & t : temp.opcoes) {
//			cout << t << endl;
//		}

		temp._val = &pString[nome];
		// 26 de junho de 2016, teste de null pointer
		//pString[nome] = "";
		
		if (tipo == "radioMult") {
			temp.multiple = true;
			for (auto & o : temp.opcoes) {
				bool *tempBool = &pBool[o];
				temp._vals.push_back(tempBool);
				temp.cores.push_back(cor);
			}
		}

		temp.height = sliderHeight;
		temp.init();
		// remover o indexElement em breve
		indexElement[nome] = radios.size();
		radiosIndex[nome] = radios.size();

//		element te;
//		te._rect = &temp.rect;
//		te.tipo = RADIO;
//		te._radio = &temp;
//		elements.push_back(te);

		radios.push_back(temp);

		element te;
		te.set(radios.back());
		elements.push_back(te);

		lastHeight = temp.rect.height;
	}

	else if (tipo == "dirlist" || tipo == "dirList") {

		ofDirectory dir;
		// no futuro colocar o allowext por algum lado :: dir.allowExt("wav");
		if (valores2 != "") {
			vector <string> vals = ofSplitString(valores2, "	");
			for (auto & c : vals) {
				dir.allowExt(c);
			}
		}

		pFolder[nome] = valores;
		dir.listDir(valores);

		vector <string> opcoes;
		for (auto & d : dir) {
			if (dirListEntireName) {
				opcoes.push_back(d.getFileName());
			} else {
				opcoes.push_back(d.getBaseName());
			}
			dirListMap[valores][d.getFileName()] = d.getAbsolutePath();
		}

		radio temp;
		temp.dirList = true;
		temp.nome = nome;
		temp.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		temp.cor = cor;
		temp.opcoes = opcoes;
		pString[nome] = "";
		temp._val = &pString[nome];
		temp.height = sliderHeight;
		temp.init();

		lastHeight = temp.rect.height;

		radios.push_back(temp);

		element te;
		te.set(radios.back());
		elements.push_back(te);
		//flow.y += temp.rect.height - 25 + 5;
	}

	// Aqui começam os tipos compostos

	else if (tipo == "togglematrix") {
		if (valores != "") {
			vector <string> vals = ofSplitString(valores, " ");
			int maxx = ofToInt(vals[0]);
			int maxy = ofToInt(vals[1]);

			for (int y=0; y<maxy; y++) {
				createFromLine("flowVert");
				createFromLine("flowHoriz");
				for (int x=0; x<maxx; x++) {
					string nomeElement = nome + ofToString(x) + ofToString(y);
					create(nomeElement, "toggleNolabel");
				}
			}
		}
	}


	else if (tipo == "color") {
		//string n = nome + "Cor";
		vector <string> cores = ofSplitString(valores, " ");
		ofFbo corFbo;
		corFbo.allocate(sliderWidth, 20 ,GL_RGBA);
		corFbo.begin();
		ofClear(0,255);

		ofSetColor(255,255,0);
		ofDrawRectangle(0,0,corFbo.getWidth(), corFbo.getHeight());
		float largura = corFbo.getWidth() / cores.size();

		vector <ofColor> colors;
		for (int a=0; a<cores.size(); a++) {
			float x = a*largura;
			istringstream stream("0x" + cores[a].substr(1));
			stream.unsetf(ios_base::dec);
			int cor;
			stream >> cor;
			ofColor corzinha = ofColor::fromHex(cor);
			ofFill();
			ofSetColor(corzinha);
			ofDrawRectangle(x, 0, largura, corFbo.getHeight());
			colors.push_back(corzinha);
		}
		corFbo.end();
		mapFbos[nome] = corFbo;

		createFromLine("label	"+nome);

		slider2d ts;
		ts.cores = colors;
		//ts._fbo = &mapFbos[nome];
		ts.nome = nome;
		ts.cor = cor;
		ts.rect = ofRectangle(flow.x, flow.y, sliderWidth, 20);
		pPoint[nome] = ofPoint(.5, .5);
		ts._val = &pPoint[nome];
		ts.setFbo(mapFbos[nome]);
		ts.isColor = true;
		ts._valColor = &pColor[nome];

		lastHeight = ts.rect.height;
		lastWidth  = ts.rect.width;

		// descontinuar com o tempo
		indexElement[nome] = sliders2d.size();
		sliders2dIndex[nome] = sliders2d.size();

		sliders2d.push_back(ts);

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
		createFromLine("int	"+nome+"HStep	0 6 0");
		createFromLine("float	"+nome+"Alpha	0 255 255");
		createFromLine("float	"+nome+"AlphaAudio	0 255 0");
		createFromLine("float	"+nome+"AlphaRange	0 255 0");
		colors.push_back(nome);
		lastHeight = 0;
		//createFromLine("");
	}

	else if (tipo == "colorPaleta") {
		createFromLine("_bool	"+nome+"UsaPaleta	1");
		createFromLine("slider2d	"+nome+"Paleta	.5 .5");
		createFromLine("fbo	"+nome+"PaletaAtual	200 10");
		createFromLine("_float	"+nome+"HRange	0 720 0");
		createFromLine("_float	"+nome+"HRangeAudio	0 360 0");
//		createFromLine("float	"+nome+"BRange	0 255 0");
//		createFromLine("int	"+nome+"HStep	0 6 0");
		createFromLine("_float	"+nome+"Alpha	0 255 255");
//		createFromLine("float	"+nome+"AlphaAudio	0 255 0");
//		createFromLine("float	"+nome+"AlphaRange	0 255 0");
		colors.push_back(nome);
		//createFromLine("");
		lastHeight = 0;
	}

	else if (tipo == "preview3d") {
		string s =
		R"(bool	iluminaPreview	0
_int	pointSize	1 4 2
_float	bgPreview	0 255 60
_float	bgPiso	0 255 40
_float	lookX	-30 30 0
float	lookY	0 20 1.7
_float	lookZ	-30 30 0
_float	rotCamX	-360 360 0
_float	rotCamZ	-360 360 0
__float	rotCamY	-360 360 0
__float	rotCamYAuto	-1 1 0
float	cameraFov	30 120 36

_bool	cameraPolar	1
float	cameraDist	0 50 23
float	cameraAngle	-180 180 -45
_float	cameraX	-50 50 0
float	cameraY	-1 20 1.7
_float	cameraZ	-50 50 20)";
		for (auto & l : ofSplitString(s, "\n")) {
			createFromLine(l);
		}
		lastHeight = 0;
	}

	else if (tipo == "audioControls") {
		// 17 de julho de 2016, não sei se compila bem em outras plataformas.
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
		lastHeight = 0;
	}

	else if (tipo == "audioBpmControls") {
		// 17 de julho de 2016, não sei se compila bem em outras plataformas.
		string s =
		R"(bool	audioOuBpm	0
int	BPM	80 200 120
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
		lastHeight = 0;
	}



	else if (tipo == "noise") {
		if (valores == "") {
			valores = ".5 .1 .5 10";
		}
		vector <string> vals = ofSplitString(valores, " ");
		createFromLine("label	"+nome);
		createFromLine("float	"+nome+"NoiseSeed	0 1 " + vals[0]);
		createFromLine("float	"+nome+"NoiseMin	-.2 1 " + vals[1]);
		createFromLine("float	"+nome+"Noise	0 1.4 " + vals[2]);
		createFromLine("float	"+nome+"NoiseMult	.1 30 " + vals[3]);
		createFromLine("fbo	"+nome+"NoiseFbo	sliderWidth 30");
		lastHeight = 0;
	}

	else if (tipo == "sliderVertMatrix") {
		createFromLine("label	" + nome);
		createFromLine("flowHoriz");
		for (int a=0; a<ofToInt(valores); a++) {
			createFromLine("sliderVert	" + nome + "_" + ofToString(a) + "	0 1 0");
		}
		createFromLine("flowVert");
		flow.y -= sliderWidth;
	}

	else {
		flowing = false;
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
	}

	if (flowing) {
		if (flowDirection == VERT) {
			flow.y += lastHeight + sliderMargin;
		}
		if (flowDirection == HORIZ) {
			flow.x += lastWidth + sliderMargin;
			pFloat["maxWidthHorizontal"] = MAX(pFloat["maxWidthHorizontal"], flow.x);
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::expires(int dataInicial, int dias) {
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	cout << "-------- Dmtr Expires: " ;
	cout << rawtime << endl;
	int segundosPorDia = 86400;
	int segundosExpira = segundosPorDia * dias;
	float diasExpira = (segundosExpira - (difftime(rawtime,dataInicial))) / (float)segundosPorDia;

	cout << "expira em " + ofToString(diasExpira) + " dias" << endl;
	cout << "---------" << endl;
	if (diasExpira < 0 || diasExpira > dias) {
		ofSystemAlertDialog("Dmtr.org Software Expired ~ " + ofToString(dataInicial));
		std::exit(1);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::uiEventsNeu(dmtrUIEvent & e) {
	if (debug) {
		cout << "ofxDmtrUI::uiEventsNeu :: " + e.nome << endl;
	}

	if (e.element == RADIO) {
		if ( radioUIMap.find(e.nome) != radioUIMap.end() ) {
			ofxDmtrUI * _u = &_uiFather->uis[radioUIMap[e.nome]];

			string fileName = pFolder[e.nome] + "/" + pString[e.nome] + ".txt";
			if (_u->createdFromTextFile != fileName) {
				_u->clear();

				string fileDefault = radioUIMap[e.nome]+".txt";
				if (ofFile::doesFileExist(fileDefault)) {
					_u->createFromText(fileDefault);
				}

				if (ofFile::doesFileExist(fileName)) {
					_u->createFromText(fileName);
				}
				_u->createFromLine("autoFit");
			}

			// setup novamente adicionaria os listeners novamente
			// xaxa
			//_u->setup();
		}
	}


	// este novo serve tanto pro INT quanto pro Float. ta lindo isso
	if (ofIsStringInString(e.nome, "shortcut") && e.tipo != LOAD) {
		vector <string> split = ofSplitString(e.nome, "_");
		string nome = split[0];
//		getSlider(nome)->setValue(ofToFloat(pString[e.nome]));
		if (sliders[indexElement[nome]].nome == nome) {
			sliders[indexElement[nome]].setValue(ofToFloat(pString[e.nome]));
		}
	}

	if (learnMode) {
		lastLearn = e.nome;
		// propagar evento aqui...
		vector <string> serie;
		serie.push_back(e.nome);
		//serie.push_back(e.tipo);
		serie.push_back(UINAME);
		ofNotifyEvent(uiEventMidi, serie);

//		cout << "lastLearn" << endl;
//		cout << e.nome << endl;
	}

	if (e.nome == "presetsFolder") {
		string newPresetsFolder = getPresetsFolder();
		if (!ofFile::doesFileExist(newPresetsFolder)) {
			if (!ofFile::doesFileExist("_presets")) {
				ofDirectory::createDirectory("_presets");
			}

			ofDirectory::createDirectory(newPresetsFolder);
		}
		if (ofFile::doesFileExist(newPresetsFolder)) {
			for (auto & p : allPresets.presets) {
				p.fbo.begin();
				ofClear(0,255);
				int a = p.index;
				string filename = getPresetsFolder() + ofToString(a) + ".tif";
				ofSetColor(0);
				ofDrawRectangle(0,0,p.fbo.getWidth(), p.fbo.getHeight());
				if (ofFile::doesFileExist(filename)) {
					p.img.load(filename);
					p.img.draw(0,0);
				} else {
					p.img.clear();
				}
				p.fbo.end();
			}
			redraw = true;
		} else {
			cout << "Presets Folder Doesnt Exist : ";
			cout << newPresetsFolder << endl;
		}
	}

	else if (e.nome == "easing") {
		//cout << pFloat["easing"] << endl;
		easing = pFloat["easing"];
		for (auto & p : uis) {
			p.second.easing = pFloat["easing"];
		}
//		for (auto & p : _presetsUIs) {
//			p->easing = pFloat["easing"];
//		}
	}


	else if (e.nome == "scene") {
		if (uiC != NULL) {
			if (_uiBak != NULL) {
				//cout << "uibak not null" <<endl;
				// clonar todas as variáveis aqui...
				// dessa forma nao sei se funciona pq eles sao apenas ponteiros... ou nao?
				_uiBak->pFloat = uiC->pFloat;
				_uiBak->pInt = uiC->pInt;
				_uiBak->pString["scene"] = pString["sceneAnterior"]; // ui.pstring
			}
			if (pString["sceneAnterior"] != pString["scene"]) {
				uiC->clear();
				uiC->createFromText("uiC.txt");
				// 
				string fileName = "_scene/" + pString["scene"] + ".txt";
				if (ofFile::doesFileExist(fileName)) {
					uiC->createFromText(fileName);
				}
				uiC->createFromLine("autoFit");
				uiC->setup();
			}
			pString["sceneAnterior"] = pString["scene"];
		}
	}

	// vai precisar?
	else if (e.nome == "loadPreset") {
	}


	ofNotifyEvent(evento, e);
}

//--------------------------------------------------------------
void	 ofxDmtrUI::uiEvents(string & e) {
	//cout << e << endl;

	// passar isso tudo pro estilo novo de eventos?
	// pra isso preciso mudar a class preset. ali chama o loadPresetAll, que não é acessivel via .h ?

	if (ofIsStringInString(e, "erasePreset")) {
		vector <string> split = ofSplitString(e, "_");
		int slot = ofToInt(split[1]);
		// terminar isso aqui.
	}

	else if (ofIsStringInString(e, "loadPreset")) {
		vector <string> split = ofSplitString(e, "_");
		int slot = ofToInt(split[1]);
		loadPresetAll(slot);
	}

	else if (ofIsStringInString(e, "savePreset")) {
		vector <string> split = ofSplitString(e, "_");
		int slot = ofToInt(split[1]);
		savePresetAll(slot);
		// talvez salvar o thumbnail no folder da outra UI? nao sei.

		if (_fbo != NULL) {
			ofFbo fboThumb;
			float w = allPresets.presets[0].fbo.getWidth();
			float h = allPresets.presets[0].fbo.getHeight();
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
			// este fbo nao precisa.
			allPresets.presets[slot].fbo.begin();
			ofClear(0);
			ofSetColor(255);
			_fbo->draw(offx, offy, neww,h);
			allPresets.presets[slot].fbo.end();
			//fboThumb.end();

			ofPixels pixels;
			pixels.allocate( w, h, OF_IMAGE_COLOR_ALPHA);
			allPresets.presets[slot].fbo.readToPixels(pixels);
//			string imgPath = presetsFolder + ofToString(slot) + ".tif";
			string imgPath = getPresetsFolder() + ofToString(slot) + ".tif";

			ofSaveImage(pixels, imgPath);
			allPresets.draw();
			allPresets.presets[slot].img.setFromPixels(pixels);
		}
	}

//	if (ofIsStringInString(e, "shortcut") && !ofIsStringInString(e, "load")) {
//		vector <string> split = ofSplitString(e, "_");
//		string nome = split[1];
//
//		// XAXA forte
//		getSlider(nome).setValue(ofToFloat(pString[nome + "_shortcut"]));
//		//pFloat[nome] = ofToFloat(pString[nome + "_shortcut"]);
//		//cout << "shortcut!" << endl;
//	}
//	if (ofIsStringInString(e, "shortcutInt") && !ofIsStringInString(e, "load")) {
//		vector <string> split = ofSplitString(e, "_");
//		string nome = split[1];
//		pInt[nome] = ofToInt(pString[nome + "_shortcutInt"]);
//	}

	// XAXA Corrigir
	ofNotifyEvent(uiEvent, e);
}

//--------------------------------------------------------------
void	 ofxDmtrUI::autoFit(bool w, bool h) {
	float minX = 6666;
	float minY = 6666;
	float maxW = 0;
	float maxH = 0;

	//cout << "autofit" << endl;

	// antes era somente usado pro allPresets aqui... agora vamos ver.

	//auto &e = allPresets;
	minX = MIN(allPresets.rect.x, minX); minY = MIN(allPresets.rect.y, minY); maxW = MAX(allPresets.rect.x + allPresets.rect.width, maxW); maxH = MAX(allPresets.rect.y + allPresets.rect.height, maxH);

	// pro futuro, ainda nao consigo resolver
//	for (auto & e : elements) {
//		cout << e.nome << endl;
//		cout << e._rect << endl;
//		cout << *e._rect << endl;
//		cout << e._rect->x << endl;
//		minX = MIN(*e._rect->x, minX); minY = MIN(*e._rect->y, minY); maxW = MAX(*e._rect->x + *e._rect->width, maxW); maxH = MAX(*e._rect->y + *e._rect->height, maxH);
//	}

	for (auto & e : sliders) { minX = MIN(e.rect.x, minX); minY = MIN(e.rect.y, minY); maxW = MAX(e.rect.x + e.rect.width, maxW); maxH = MAX(e.rect.y + e.rect.height, maxH); }
	for (auto & e : toggles) { minX = MIN(e.rect.x, minX); minY = MIN(e.rect.y, minY); maxW = MAX(e.rect.x + e.rect.width, maxW); maxH = MAX(e.rect.y + e.rect.height, maxH); }
	for (auto & e : labels)  { minX = MIN(e.rect.x, minX); minY = MIN(e.rect.y, minY); maxW = MAX(e.rect.x + e.rect.width, maxW); maxH = MAX(e.rect.y + e.rect.height, maxH); }
	for (auto & e : radios)  { minX = MIN(e.rect.x, minX); minY = MIN(e.rect.y, minY); maxW = MAX(e.rect.x + e.rect.width, maxW); maxH = MAX(e.rect.y + e.rect.height, maxH); }
	for (auto & e : sliders2d)  { minX = MIN(e.rect.x, minX); minY = MIN(e.rect.y, minY); maxW = MAX(e.rect.x + e.rect.width, maxW); maxH = MAX(e.rect.y + e.rect.height, maxH); }

	for (auto & e : inspectors)  { minX = MIN(e.rect.x, minX); minY = MIN(e.rect.y, minY); maxW = MAX(e.rect.x + e.rect.width, maxW); maxH = MAX(e.rect.y + e.rect.height, maxH); }

	if (w)
		coluna.width = maxW + marginx;
	if (h)
		coluna.height = maxH + marginy;
	fboColumn.allocate(coluna.width, coluna.height, GL_RGBA);
	fboColumn.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	redraw = true;

	if (_uiUnder != NULL) {
		_uiUnder->downTo(*this);
	}
	if (_uiRight != NULL) {
		_uiRight->nextTo(*this);
	}
}


//--------------------------------------------------------------
void ofxDmtrUI::setFloat(string nome, float val) {
}

//--------------------------------------------------------------
void ofxDmtrUI::setBool(string nome, bool val) {
	toggles[indexElement[nome]].setValue(val);
	redraw = true;
}

//--------------------------------------------------------------
void ofxDmtrUI::setRadio(string nome, string val) {
	radios[indexElement[nome]].setValue(val, true);
	redraw = true;
}

//--------------------------------------------------------------
void ofxDmtrUI::loadPresetAll(int n) {
	if (debug) {
 		cout << "loadPresetAll:" + ofToString(n) << endl;
	}

	for (auto & u : uis) {
		string nome = getPresetsFolder() + ofToString(n) + u.first +  ".xml";
//		cout << u.first << endl;
//		cout << u.second.UINAME << endl;
//		cout << nome << endl;
//		cout << "----" << endl;
		u.second.load(nome);
		u.second.redraw = true;
	}

//	for (auto & p : _presetsUIs) {
//		string nome = getPresetsFolder() + ofToString(n) + p->UINAME +  ".xml";
//		p->load(nome);
//		p->redraw = true;
//	}

	presetLoaded = n;
	allPresets.set(n);
	
	redraw = true;

	dmtrUIEvent te;
	te.nome = "loadPresetAll";
	ofNotifyEvent(evento, te);
	// fazer um UIEvent aqui.
}

//--------------------------------------------------------------
void ofxDmtrUI::savePresetAll(int n) {

	for (auto & u : uis) {
		string nome = getPresetsFolder() + ofToString(n) + u.first +  ".xml";
		u.second.save(nome);
	}
	//cout << "savePresetAll" << endl;
//	for (auto & p : _presetsUIs) {
//		//cout << p->UINAME << endl;
//		string nome = getPresetsFolder() + ofToString(n) + p->UINAME +  ".xml";
//		p->save(nome);
//	}
	presetLoaded = n;
	allPresets.set(n);
}

//--------------------------------------------------------------
void ofxDmtrUI::loadNextPresetAll() {
	int n = (presetLoaded + 1) % allPresets.presets.size();
	cout << "loadNextPresetAll" << endl;
	cout << n << endl;
	loadPresetAll(n);
}

void ofxDmtrUI::erasePresetAll(int n) {
//	for (auto & p : _presetsUIs) {
//		string nome = getPresetsFolder() + ofToString(n) + p->UINAME +  ".xml";
//		ofFile::removeFile(nome);
//	}
	ofFile::removeFile(ofToString(n)+".tif");

	presetLoaded = n;
	allPresets.set(n);
}



//--------------------------------------------------------------
void ofxDmtrUI::loadPreset(int n) {
//	string nome = presetsFolder + UINAME + ofToString(n) + ".xml";
	string nome = presetsFolder + ofToString(n) + UINAME +  ".xml";
	load(nome);
	presetLoaded = n;

	allPresets.set(n);
//	redraw = true;
	//re();
	//cout << allPresets.valor << endl;
	//allPresets.presets[allPresets.valor].selecionado = false;
	//allPresets.presets[n].selecionado = true;
}

//--------------------------------------------------------------
void ofxDmtrUI::savePreset(int n) {
	string nome = presetsFolder + ofToString(n) + UINAME +  ".xml";
	save(nome);
}


//--------------------------------------------------------------
void ofxDmtrUI::setFbo(ofFbo &fbo) {
	_fbo = &fbo;
}

//--------------------------------------------------------------
void ofxDmtrUI::setFboElement(string nome, ofFbo &fbo) {
	if (indexElement.find(nome) != indexElement.end()) {
		if (sliders2d[indexElement[nome]].nome == nome) {
			sliders2d[indexElement[nome]].setFbo(fbo);
		}
	}
}


//--------------------------------------------------------------
void ofxDmtrUI::re() {
	redraw = true;
	draw();
}

//--------------------------------------------------------------
void ofxDmtrUI::clear(bool keepVars) {
	createdFromTextFile = "";
	if (debug) {
		cout << "ofxDmtrUI Clear!" << endl;
	}
	flow = ofPoint(marginx, marginy);
	sliders.clear();
	sliders2d.clear();
	toggles.clear();
	radios.clear();
	elements.clear();
	labels.clear();

	if (!keepVars) {
		pEasy.clear();
		pFloat.clear();
		pInt.clear();
		pBool.clear();
		pString.clear();
		pLabel.clear();
		pPoint.clear();
		pColor.clear();
		pFolder.clear();
	}
	
	redraw = true;
}

//--------------------------------------------------------------
float ofxDmtrUI::getNoise(string nome, float a) {
	return (pFloat[nome+"NoiseMin"] + ofNoise(pFloat[nome+"NoiseSeed"],
	a * pEasy[nome+"NoiseMult"]) * pFloat[nome+"Noise"]);
}

//--------------------------------------------------------------
string ofxDmtrUI::getPresetsFolder() {
	string out = presetsFolder;
	if (uiM != NULL) {
		if (uiM->pString["presetsFolder"] != "") {
			out += uiM->pString["presetsFolder"] + "/";
		}
	} else {
		if (pString["presetsFolder"] != "") {
			out += pString["presetsFolder"] + "/";
		}
	}
	return out;
}

// almost there... still not working 100% for unknown reason
//--------------------------------------------------------------
slider * ofxDmtrUI::getSlider(string nome) {
	if (sliders[slidersIndex[nome]].nome == nome) {
		return &sliders[slidersIndex[nome]];
	}
}

//--------------------------------------------------------------
radio * ofxDmtrUI::getRadio(string nome) {
	if (radios[radiosIndex[nome]].nome == nome) {
		return &radios[radiosIndex[nome]];
	} 
}

//--------------------------------------------------------------
toggle * ofxDmtrUI::getToggle(string nome) {
	if (toggles[togglesIndex[nome]].nome == nome) {
		return &toggles[togglesIndex[nome]];
	}
}

//--------------------------------------------------------------
slider2d * ofxDmtrUI::getSlider2d(string nome) {
	if (sliders2d[sliders2dIndex[nome]].nome == nome) {
		return &sliders2d[sliders2dIndex[nome]];
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::nextTo(ofxDmtrUI & uiNext) {
	coluna.x = uiNext.coluna.x + uiNext.coluna.width + uiNext.marginx;
//	coluna.y = uiNext.coluna.y;
	coluna.y = 0;
	uiNext._uiRight = this;

	if (debug) {
		cout << "interface //" + UINAME << endl;
		cout << "next to: " + uiNext.UINAME << endl;
	}
    
    if (_uiUnder != NULL) {
        _uiUnder->downTo(*this);
    }
    if (_uiRight != NULL) {
        _uiRight->nextTo(*this);
    }
}

//--------------------------------------------------------------
void ofxDmtrUI::downTo(ofxDmtrUI & uiNext) {
	coluna.y = uiNext.coluna.y + uiNext.coluna.height + uiNext.marginy;
	coluna.x = uiNext.coluna.x;
	uiNext._uiUnder = this;
    
    if (_uiUnder != NULL) {
        _uiUnder->downTo(*this);
    }
    if (_uiRight != NULL) {
        _uiRight->nextTo(*this);
    }
}

//--------------------------------------------------------------
string ofxDmtrUI::getFileFullPath(string & nome) {
	string saida = pFolder[nome] + "/" + pString[nome];
	return saida;
//	if (pString[nome] != "") {
//	} else {
//		return false;
//	}
}

//vector <ofxDmtrUI *> ofxDmtrUI::allUIs() {
//}


//--------------------------------------------------------------
void ofxDmtrUI::createSoftwareFromText(string file) {
	UINAME = "master";

	keepSettings = true;
	useShortcut = true;
	if (ofFile::doesFileExist("uiAll.txt")) {
		createFromText("uiAll.txt");
	}
	createFromText(file);
	setup();

	allUIs.push_back(this);

	int w,h;

	if (ofFile::doesFileExist("output.txt")) {
		vector <string> output = textToVector("output.txt");
		vector <string> dimensoes = ofSplitString(output[0], " ");
		w = ofToInt(dimensoes[0]);
		h = ofToInt(dimensoes[1]);
	} else {
		w = 1920;
		h = 1080;
	}

#ifdef DMTRUI_TARGET_TOUCH
	int format = GL_RGBA; //GL_RGBA32F_ARB  //GL_RGBA32F
#else
	int format = GL_RGBA32F_ARB; //GL_RGBA32F_ARB  //GL_RGBA32F
#endif
	int multiSampling = 2;
	if (multiSampling == 0) {
		fbo.allocate			(w,h, format);
	} else {
		fbo.allocate			(w,h, format, multiSampling);
	}

	if (multiSampling == 0) {
		fboRastros.allocate			(w,h, format);
	} else {
		fboRastros.allocate			(w,h, format, multiSampling);
	}

	if (multiSampling == 0) {
		fboFade.allocate			(w,h, format);
	} else {
		fboFade.allocate			(w,h, format, multiSampling);
	}

	fbo.begin();
	ofClear(0,255);
	fbo.end();

	fboRastros.begin();
	ofClear(0,255);
	fboRastros.end();

	setFbo(fboRastros);
}
