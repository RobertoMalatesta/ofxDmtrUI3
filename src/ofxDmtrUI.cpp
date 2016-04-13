/*

ofxDmtrUI.h
Created by Dimitre Lima on 04/06/2016.
Proof of concept

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
		// instead make a function called getValue() here
		if (s.isInt) {
			settings.setValue(s.nome, s.valorInt);
		} else {
			settings.setValue(s.nome, s.valor);
		}
	}

	for (auto & t : toggles) {
		settings.setValue(t.nome, t.valor);
	}

	for (auto & r : radios) {
		// mudar pra valor
		cout << r.nome << endl;
		cout << r.selecionado << endl;
		settings.setValue(r.nome, r.selecionado);
	}
	settings.save(xml);
}

//--------------------------------------------------------------
void ofxDmtrUI::load(string xml){
	cout << "load: " + xml << endl;
	ofxXmlSettings settings;
	settings.loadFile(xml);
	for (auto & s : sliders) {
		// load default slider value in the case it doesnt exist in xml
		s.setValue(settings.getValue(s.nome, s.def));
		if (s.isInt) {
			pInt[s.nome] = settings.getValue(s.nome, s.def);
		} else {
			pFloat[s.nome] = settings.getValue(s.nome, s.def);
		}

	}
	for (auto & t : toggles) {
		t.valor = settings.getValue(t.nome, 0); // mudar o zero pra t.def
		pBool[t.nome] = t.valor;
	}

	for (auto & r : radios) {
		r.selecionado = settings.getValue(r.nome, "");
		pString[r.nome] = r.selecionado;
	}

	redraw = true;
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
			string nome = ofToString(char(key)) + ".xml";
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
		pString[e.nome] = e.selecionado;
	}

	for (auto & t : toggles) {
		if (t.rect.inside(x - coluna.x,y - coluna.y) && !t.inside) {
			t.valor = !t.valor;
			t.inside = true;
			pBool[t.nome] = t.valor;
		}
	}

}

//--------------------------------------------------------------
void ofxDmtrUI::mouseDragged(int x, int y, int button){
	for (auto & s : sliders) {
		if (s.rect.inside(x - coluna.x,y - coluna.y)) {
			s.update(x - coluna.x);
			s.inside = true;
			//s.valor = (x - s.rect.x)/(double)s.rect.width;
			if (s.isInt) {
				pInt[s.nome] = s.valorInt;
			} else {
				pFloat[s.nome] = s.valor;
			}
		} else {
			if (s.inside) {
				s.update(x - coluna.x);
				s.inside = false;
			}
		}
	}
}

//--------------------------------------------------------------
void ofxDmtrUI::mousePressed(int x, int y, int button){
	for (auto & s : sliders) {
		if (s.rect.inside(x - coluna.x,y - coluna.y)) {
			s.update(x - coluna.x);
			s.inside = true;
			if (s.isInt) {
				pInt[s.nome] = s.valorInt;
			} else {
				pFloat[s.nome] = s.valor;
			}
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
		if (l == "") { // spacer
			flow.y += 25;
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
			else if (tipo == "sliderWidth") {
				sliderWidth = ofToFloat(cols[1]);
				cout << tipo << endl;
			}
			else if (tipo == "sliderHeight") {
				sliderHeight = ofToFloat(cols[1]);
				cout << tipo << endl;
			}

			else if (tipo == "rect") {
				vector <string> v = ofSplitString(cols[1], " ");
				coluna = ofRectangle(ofToInt(v[0]),ofToInt(v[1]),ofToInt(v[2]),ofToInt(v[3]));
				fbo.allocate(coluna.width, coluna.height, GL_RGBA);
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
}

//--------------------------------------------------------------
void ofxDmtrUI::create(string nome, string tipo, string valores) {
	int hue = int(flow.x + flow.y/6.0)%255;

	if (tipo == "slider" || tipo == "int") {
		slider ts;
		ts.nome = nome;
		ts.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		ts.cor = ofColor::fromHsb(hue,255,255);
		ts.isInt = tipo == "int";
		ts.val = &pFloat[ts.nome];

		if (valores != "") {
			vector <string> vals = ofSplitString(valores, " ");
			ofVec3f val = ofVec3f(ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2]));
			ts.min = val.x;
			ts.max = val.y;
			ts.def = val.z;
			// for initialization without mouse.
			ts.setValue(ts.def);
		}
		sliders.push_back(ts);
		pFloat[nome] = ts.def;
		if (ts.isInt) {
			pInt[nome] = ts.def;
		}
	}

	else if (tipo == "toggle") {
		toggle tt;
		tt.nome = nome;
		tt.rect = ofRectangle(flow.x, flow.y, sliderHeight, sliderHeight);
		tt.cor = ofColor::fromHsb(hue,255,255);
		if (valores == "1") {
			tt.valor = true;
		}
		toggles.push_back(tt);
	}

	else if (tipo == "label") {
		label tl;
		tl.nome = nome;
		tl.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		tl.cor = ofColor::fromHsb(hue,255,255);
		labels.push_back(tl);
	}

	else if (tipo == "radio") {
		radio temp;
		temp.nome = nome;
		temp.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		temp.cor = ofColor::fromHsb(hue,255,255);
		temp.opcoes = ofSplitString(valores, " ");
		temp.init();
		//vector <string> parametros = ofSplitString(valores, " ");
		radios.push_back(temp);
	}

	else if (tipo == "dirlist") {
		ofDirectory dir;

		// no futuro colocar o allowext por algum lado
		//		dir.allowExt("wav");
		dir.listDir(valores);

		vector <string> opcoes;
		for (auto & d : dir) {
			//cout << d.getFileName() << endl;
			opcoes.push_back(d.getFileName());
		}

		radio temp;
		temp.nome = nome;
		temp.rect = ofRectangle(flow.x, flow.y, sliderWidth, sliderHeight);
		temp.cor = ofColor::fromHsb(hue,255,255);
		temp.opcoes = opcoes;
		temp.init();
		//vector <string> parametros = ofSplitString(valores, " ");
		flow.y += temp.rect.height - 25 + 5;
		radios.push_back(temp);


	}

	if (flowDirection == VERT) {
		flow.y += 25;
	}
	if (flowDirection == HORIZ) {
		flow.x += 25;
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
		ofSystemAlertDialog("Software Expirado ~ " + ofToString(dataInicial) + "\rdimitre79@gmail.com\r(11) 99450 3821");
		std::exit(1);
	}
}