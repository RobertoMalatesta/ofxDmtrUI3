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

	ofPoint flow = ofPoint(marginx, marginy);

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

	if (keepSettings) {
		load(presetsFolder + UINAME + ".xml");
	}
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
		if (redraw) {
			fbo.begin();
			ofClear(0,100);
			for (auto & t : toggles) { t.draw(); }
			for (auto & s : sliders) { s.draw(); }
			for (auto & l : labels)  { l.draw(); }
			for (auto & r : radios)  { r.draw(); }
			fbo.end();
			redraw = false;
		}
		//ofSetColor(255, columnOver ? 255 : 128);
		ofSetColor(255);
		fbo.draw(coluna.x, coluna.y);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::save(string xml){
	cout << "save: " + xml << endl;
	ofxXmlSettings settings;
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
		// remover o selecionado totalmente?
		//settings.setValue(r.nome, r.selecionado);
		settings.setValue(r.nome, *r._val);
	}
	settings.save(xml);
}

//--------------------------------------------------------------
void ofxDmtrUI::load(string xml){
	cout << "load: " + xml << endl;
	ofxXmlSettings settings;
	settings.loadFile(xml);
	for (auto & e : sliders) {
		e.setValue(settings.getValue(e.nome, e.def));
	}
	for (auto & e : toggles) {
		e.setValue(settings.getValue(e.nome, e.def));
	}

	for (auto & e : radios) {
		// default? algo como asterisco no txt?
		//if (*e._val != settings.getValue(e.nome, ""))
		{
			e.setValue(settings.getValue(e.nome, ""), 2);
		}
	}
	redraw = true;

	string e = "load";
	ofNotifyEvent(uiEvent, e);
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
			string nome = presetsFolder + UINAME + ofToString(char(key)) + ".xml";
			if (ofGetKeyPressed(OF_KEY_COMMAND)) {
				save(nome);
			} else {
				load(nome);
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
	for (auto & e : radios) {
		if (e.rect.inside(x - coluna.x,y - coluna.y)) {
			e.checkMouse(x - coluna.x,y - coluna.y);
		}
	}

	for (auto & t : toggles) {
		if (t.rect.inside(x - coluna.x,y - coluna.y) && !t.inside) {
			t.inside = true;
			t.flip();
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::mouseDragged(int x, int y, int button){
	// simplificar, fazer o x - coluna.x antes
	for (auto & s : sliders) {
		if (s.rect.inside(x - coluna.x, y - coluna.y)) {
			s.update(x - coluna.x, y - coluna.y);
			s.inside = true;
		} else {
			if (s.inside) {
				s.update(x - coluna.x, y - coluna.y);
				s.inside = false;
			}
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::mousePressed(int x, int y, int button){
	for (auto & s : sliders) {
		if (s.rect.inside(x - coluna.x,y - coluna.y)) {
			s.update(x - coluna.x, y - coluna.y);
			s.inside = true;
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::mouseReleased(int x, int y, int button){
	redraw = false;
	for (auto & t : toggles) {
		t.inside = false;
		if (t.bang) {
			*t._val = false;
			redraw = true;
		}
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
	if (keepSettings) {
		save(presetsFolder + UINAME + ".xml");
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
	if (showGui) {
		mousePressed				(data.x, data.y, data.button);
		mouseAll					(data.x, data.y, data.button);
		mousePressedDragged		(data.x, data.y, data.button);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::onMouseDragged(ofMouseEventArgs& data)
{
	if (showGui) {
		mouseDragged			(data.x, data.y, data.button);
		mouseAll				(data.x, data.y, data.button);
		mousePressedDragged (data.x, data.y, data.button);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::onMouseReleased(ofMouseEventArgs& data)
{
	if (showGui) {
		mouseReleased		(data.x, data.y, data.button);
		mouseAll				(data.x, data.y, data.button);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::onMouseMoved(ofMouseEventArgs& data)
{
	if (showGui) {
		mouseAll				(data.x, data.y, data.button);
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::onExit(ofEventArgs &data)
{
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
	vector <string> linhas = textToVector(file);
	for (auto & l : linhas) {
		createFromLine(l);
	}
	// end reading from text files
	for (auto & e : radios) {
		ofAddListener(e.uiEvent,this, &ofxDmtrUI::uiEvents);
	}
	for (auto & e : sliders) {
		ofAddListener(e.uiEvent,this, &ofxDmtrUI::uiEvents);
	}
	for (auto & e : toggles) {
		ofAddListener(e.uiEvent,this, &ofxDmtrUI::uiEvents);
	}
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

		if (tipo == "newcol") {
			flow.x += sliderWidth + marginx * 1;
			flow.y = marginy;
		}
		else if (tipo == "marginy") {
			flow.y = marginy = ofToFloat(cols[1]);
		}
		else if (tipo == "marginx") {
			flow.x = marginx = ofToFloat(cols[1]);
		}
		else if (tipo == "autoFit") {
			autoFit();
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
					create(nome, tipo, cols[2]);
				} else {
					create(nome, tipo);
				}
			}
		}
	}
}


//--------------------------------------------------------------
void ofxDmtrUI::create(string nome, string tipo, string valores) {
	int hue = int(flow.x/8.0 + flow.y/6.0)%255;
	int saturation = bw ? 0 : 255;
	int brightness = bw ? 127 : 255;
	ofColor cor = ofColor::fromHsb(hue,saturation,brightness);

	lastHeight = sliderHeight;
	lastWidth = sliderWidth;


	if (tipo == "slider" || tipo == "int" || tipo == "sliderVert") {
		slider ts;
		ts.nome = nome;
		ts.vert = (tipo == "sliderVert");

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

		if (valores != "") {
//			cout << valores << endl;
//			cout << tipo << endl;
//			cout << nome << endl;
//			cout << "----" << endl;

			vector <string> vals = ofSplitString(valores, " ");
			ofVec3f val = ofVec3f(ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2]));
			ts.min = val.x;
			ts.max = val.y;
			ts.def = val.z;
			// for initialization without mouse.
			ts.setValue(ts.def);
		}
		pFloat[nome] = ts.def;
		if (ts.isInt) {
			pInt[nome] = ts.def;
		}
		indexElement[nome] = sliders.size();

		lastHeight = ts.rect.height;
		lastWidth  = ts.rect.width;
		sliders.push_back(ts);
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

		if (tipo == "toggleNolabel") {
			tt.showLabel = false;
			lastWidth  = tt.rect.width;
		}
		toggles.push_back(tt);
	}

	else if (tipo == "label") {
		label tl;
		tl.nome = nome;
		tl.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		tl.cor = cor;
		labels.push_back(tl);
	}

	else if (tipo == "radio") {
		radio temp;
		temp.nome = nome;
		temp.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		temp.cor = cor;
		temp.opcoes = ofSplitString(valores, " ");
		temp._val = &pString[nome];
		temp.init();

		indexElement[nome] = radios.size();

		radios.push_back(temp);
		lastHeight = temp.rect.height;
	}

	else if (tipo == "dirlist") {
		ofDirectory dir;
		// no futuro colocar o allowext por algum lado
		//		dir.allowExt("wav");
		dir.listDir(valores);

		vector <string> opcoes;
		for (auto & d : dir) {
			opcoes.push_back(d.getFileName());
			dirListMap[valores][d.getFileName()] = d.getAbsolutePath();
		}

		radio temp;
		temp.nome = nome;
		temp.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		temp.cor = cor;
		temp.opcoes = opcoes;
		pString[nome] = "";
		temp._val = &pString[nome];
		temp.init();
		lastHeight = temp.rect.height;

		radios.push_back(temp);
		flow.y += temp.rect.height - 25 + 5;
	} else if (tipo == "togglematrix") {
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

	if (flowDirection == VERT) {
		flow.y += lastHeight + sliderMargin;
	}
	if (flowDirection == HORIZ) {
		flow.x += lastWidth + sliderMargin;
	}
}

//--------------------------------------------------------------
void	 ofxDmtrUI::expires(int dataInicial, int dias) {
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
void	 ofxDmtrUI::uiEvents(string & e) {
//	ofNotifyEvent(evento, e);
//	cout << e << endl;

	// Marvellous
	if (ofIsStringInString(e, "shortcut") && !ofIsStringInString(e, "load")) {
		vector <string> split = ofSplitString(e, "_");
		string nome = split[1];
		pFloat[nome] = ofToFloat(pString[nome + "_shortcut"]);
	}
	if (ofIsStringInString(e, "shortcutInt") && !ofIsStringInString(e, "load")) {
		vector <string> split = ofSplitString(e, "_");
		string nome = split[1];
		pInt[nome] = ofToInt(pString[nome + "_shortcutInt"]);
	}

	ofNotifyEvent(uiEvent, e);
}

//--------------------------------------------------------------
void	 ofxDmtrUI::autoFit() {
	float minX = 6666;
	float minY = 6666;
	float maxW = 0;
	float maxH = 0;

	for (auto & e : toggles) { minX = MIN(e.rect.x, minX); minY = MIN(e.rect.y, minY); maxW = MAX(e.rect.x + e.rect.width, maxW); maxH = MAX(e.rect.y + e.rect.height, maxH); }
	for (auto & e : sliders) { minX = MIN(e.rect.x, minX); minY = MIN(e.rect.y, minY); maxW = MAX(e.rect.x + e.rect.width, maxW); maxH = MAX(e.rect.y + e.rect.height, maxH); }
	for (auto & e : labels)  { minX = MIN(e.rect.x, minX); minY = MIN(e.rect.y, minY); maxW = MAX(e.rect.x + e.rect.width, maxW); maxH = MAX(e.rect.y + e.rect.height, maxH); }
	for (auto & e : radios)  { minX = MIN(e.rect.x, minX); minY = MIN(e.rect.y, minY); maxW = MAX(e.rect.x + e.rect.width, maxW); maxH = MAX(e.rect.y + e.rect.height, maxH); }

	coluna.width = maxW + marginx;
	coluna.height = maxH + marginy;
	fbo.allocate(coluna.width, coluna.height, GL_RGBA);
	redraw = true;
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
	cout << nome << endl;
	cout << indexElement[nome] << endl;
	cout << radios[indexElement[nome]].nome << endl;
	cout << "------" << endl;
	radios[indexElement[nome]].setValue(val, true);
	redraw = true;
}

//--------------------------------------------------------------
void ofxDmtrUI::loadPreset(int n) {
	string nome = presetsFolder + UINAME + ofToString(n) + ".xml";
	load(nome);
}
