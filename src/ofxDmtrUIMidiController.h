/*
ofxDmtrUIMidiController 
 Created by Dimitre Lima on 18/05/16.
 Copyright 2016 Dmtr.org. All rights reserved.
 Only for Dmtr.org projects use.
*/

#pragma once
#include "ofMain.h"
#include "ofEvents.h"
//#include "ofxDmtrUI3.h"
#include "ofxMidi.h"

class elementListMidiController {
public:
	string ui;
	string tipo;
	string nome;
	string valor = "";

	int channel;
	int pitch;
};


class ofxDmtrUIMidiController : public ofBaseApp, public ofxMidiListener, public ofxMidiConnectionListener {
public:
	
//	void onExit(ofEventArgs &data);
	//void		uiEvent(dmtrUIEvent & e);
//	void setup(string midiDevice);
//	void exit();
//	void newMidiMessage(ofxMidiMessage& eventArgs);
//	void uiEventMidi(vector <string> & strings);
//	void teste();
//	void testeApcMini();
//	void midiInputAdded(string name, bool isNetwork);
//	void midiInputRemoved(string name, bool isNetwork);
//	void uiEvent(uiEv & e);
//	void setUI(ofxDmtrUI3 &u);
	// Midicontroller
	ofxMidiIn 	midiControllerIn;
	ofxMidiOut	midiControllerOut;
	ofxMidiMessage midiMessage;
	bool midiKeys[4000];

	//map <int, map <int, map<int, map<int, elementListMidiController> > > > mapaMidiController;

	map <string, elementListMidiController> midiControllerMap;
	vector <elementListMidiController *> elements;

	string lastString;


	ofFbo testeFbo;
	ofPixels fboTrailsPixels;

	map <string,string>			pString;

	// midi device (dis)connection event callbacks


	ofxDmtrUI3 * _u = NULL;


	// somente para apagar leds nos presets, nada mais
	int lastPresetChannel;
	int lastPresetPitch;

	ofFbo * fboMC = NULL;
	
	int holdPresetNumber = 0;

	elementListMidiController elementLearn;

	int apcMiniLeds[64] = {
		56, 57, 58, 59, 60, 61, 62, 63,
		48, 49, 50, 51, 52, 53, 54, 55,
		40, 41, 42, 43, 44, 45, 46, 47,
		32, 33, 34, 35, 36, 37, 38, 39,
		24, 25, 26, 27, 28, 29, 30, 31,
		16, 17, 18, 19, 20, 21, 22, 23,
		8,  9,  10, 11, 12, 13, 14, 15,
		0,  1,  2,  3,  4,  5,  6,  7
	};

	int testeIndex[4] = { 0,1,3,5 };

	string folder = "";
	

	// 144 1 56 0

	//--------------------------------------------------------------
	void testeApcMini() {
		

		
		
		if (fboMC != NULL) {
			testeFbo.begin();
			//ofClear(0,255);
			ofClear(0,255);
			ofSetColor(255);
			float x = testeFbo.getWidth()/2 - fboMC->getWidth()/2;
			float y = testeFbo.getHeight()/2 - fboMC->getHeight()/2;
			fboMC->draw(x, y);
			testeFbo.end();
			ofSetColor(255);
			//testeFbo.draw(0,0, testeFbo.getWidth()*10, testeFbo.getHeight()*10);
			testeFbo.readToPixels(fboTrailsPixels);
			
			int index = 0;
			for (auto & c : apcMiniLeds) {
				ofColor cor = fboTrailsPixels.getColor(index*3);
				index++;
				int vel = testeIndex[int(ofMap(cor.r, 0, 255, 0, 3))];
				//cout << vel << "";
				midiControllerOut.sendNoteOn(1, c, vel);
			}
			
			//cout << "---" << endl;
			
		}
	}

	//--------------------------------------------------------------
	void setup(string midiDevice) {
		ofxMidi::setConnectionListener(this);

	//	testeFbo.allocate(8,8,GL_RGBA);
	//	fboTrailsPixels.allocate( testeFbo.getWidth(), testeFbo.getHeight(), OF_IMAGE_COLOR_ALPHA);
		testeFbo.allocate(8,8,GL_RGB);
		fboTrailsPixels.allocate( testeFbo.getWidth(), testeFbo.getHeight(), OF_IMAGE_COLOR);

		testeFbo.begin();
		ofClear(0,255);
		testeFbo.end();

		cout << "ofxDmtrUIMidiController setup ::: " + midiDevice << endl;
		midiControllerIn.openPort(midiDevice);
		midiControllerOut.openPort(midiDevice); // by number
		midiControllerIn.ignoreTypes(false, false, false);
		midiControllerIn.addListener(this);

		string filename = folder + midiDevice + ".txt";

		// isopen - adicionei em 12 de junho de 2016
		if (ofFile::doesFileExist(filename) && midiControllerIn.isOpen()) {

			// 16 de julho de 2016
			// forma que encontrei de associar o primeiro UI da lista e pegar o textToVector dele.
			for (auto & m : _u->textToVector(filename)) {
				if (m != "" && m.substr(0,1) != "#") {
					elementListMidiController te;
					vector <string> cols = ofSplitString(m, "\t");
					te.ui = 		cols[1];
					te.tipo = 	cols[2];
					te.nome = 	cols[3];
					if (cols.size() > 4) {
						te.valor = cols[4];
					}

					// XAXA TODO sera q precisa? deu problema aqui
	//				if (te.tipo == "radio") {
	//					vector <string> valores = ofSplitString(te.nome, " ");
	//					te.nome = valores[0];
	//					te.valor = valores[1];
	//				}
					//			cout << "ui : "   + te.ui << endl;
					//			cout << "tipo : " + te.tipo << endl;
					//			cout << "nome : " + te.nome << endl;
					vector <string> vals = ofSplitString(cols[0], " ");
					int status = 	ofToInt(vals[0]);
					int channel = 	ofToInt(vals[1]);
					int pitch = 		ofToInt(vals[2]);
					int control = 	ofToInt(vals[3]);
					// mais pra frente fazer namespace aqui.
					te.channel = channel;
					te.pitch = pitch;
					//mapaMidiController[status][channel][pitch][control] = te;

					// new stuff ofxDmtrUI3 2017
					string index = ofToString(status) + " " + ofToString(channel) + " " +  ofToString(pitch) + " " +  ofToString(control);
					midiControllerMap[index] = te;

					elements.push_back(&midiControllerMap[index]);

					//elements.push_back(&mapaMidiController[status][channel][pitch][control]);
				}
			}
		}

		ofAddListener(ofEvents().exit, this, &ofxDmtrUIMidiController::onExit);
	//	ofAddListener(ofEvents().keyPressed, 	this, &onKeyPressed);
	//	ofAddListener(ofEvents().keyReleased,	this, &onKeyReleased);


		ofAddListener(_u->settings.uiEvent,this, &ofxDmtrUIMidiController::uiEvent);

	}
	// END SETUP



	// provavelmente o teste foi feito com a APC20
	//--------------------------------------------------------------
	void teste() {
		testeFbo.begin();
		//ofClear(0,255);
		ofSetColor(0,_u->pInt["trails"]);
		ofDrawRectangle(0,0,100,100);

		float x1 = ofNoise(ofGetFrameNum()/50.0,1) * testeFbo.getWidth();
		float x2 = ofNoise(ofGetFrameNum()/100.0,2) * testeFbo.getWidth();
		float y1 = ofNoise(ofGetFrameNum()/120.0,3) * testeFbo.getHeight();
		float y2 = ofNoise(ofGetFrameNum()/200.0,4) * testeFbo.getHeight();
		ofSetColor(255);
		//ofDrawLine(x1, y1, x2, y2);
		float raio = y1 * 1.0;
		ofDrawEllipse(x1, y1, raio, raio);

		testeFbo.end();

		testeFbo.draw(0,0, testeFbo.getWidth()*10, testeFbo.getHeight()*10);
		testeFbo.readToPixels(fboTrailsPixels);

		int fator = 1;
		int vel = _u->pInt["vel"];

		// APC20
		// verde 1-2
		// laranja 3-4
		// amarelo 5-6 (not blink / blink)

		for (int x=0; x<8; x++) {
			for (int y=0; y<5; y++) {
				int ch = x + 1;
				int pitch = y + 53;
				ofColor cor = fboTrailsPixels.getColor(x * fator, y * fator);
				int vel = testeIndex[int(ofMap(cor.r, 0, 255, 0, 3))];
	//			if ((cor.r + cor.g + cor.b) < 127*3) {
	//				midiControllerOut.sendNoteOff(ch, pitch);
	//			} else
				{
					midiControllerOut.sendNoteOn(ch, pitch, vel);
				}
			}
		}
	}

	//--------------------------------------------------------------
	void uiEventMidi(vector<string> & strings) {
		elementLearn.nome = strings[0];
		elementLearn.ui = strings[1];
		elementLearn.tipo = "float";
	}


	void onExit(ofEventArgs &data) {  exit(); }

	//--------------------------------------------------------------
	void newMidiMessage(ofxMidiMessage& msg) {
	//	cout << "status:"  +ofToString(msg.status) << endl;
	//	cout << "pitch:"   +ofToString(msg.pitch) << endl;
	//	cout << "channel:" +ofToString(msg.channel) << endl;
	//	cout << "control:" +ofToString(msg.control) << endl;
	//	cout << "-------" << endl;

		string index = ofToString(msg.status) + " " + ofToString(msg.channel) + " " + ofToString(msg.pitch) + " " + ofToString(msg.control);

		if ( midiControllerMap.find(index) != midiControllerMap.end() ) {
			// action
			elementListMidiController *te = &midiControllerMap[index];
			ofxDmtrUI3 * _ui;
			if (te->ui == "master") {
				_ui = _u;
			} else {
				_ui = &_u->uis[te->ui];
			}

			if (te->tipo == "radio") {
				if (te->valor == "") {
					int nElements = ((radio*)_ui->getElement(te->nome))->elements.size();
					float valor = ofMap(msg.value, 0, 127, 0, nElements);
					_ui->futureCommands.push_back(future(te->ui, te->nome, "radioSetFromIndex", valor));
				}
				else {
					_ui->futureCommands.push_back(future(te->ui, te->nome, "radioSet", te->valor));
				}
			}
			
			else if (te->tipo == "float" || te->tipo == "int") {
				slider * s = (slider*)_ui->getElement(te->nome);
				if (s != NULL) {
					float valor = ofMap(msg.value, 0, 127, s->min, s->max);
					s->set(valor);
				}
			}

			else if (te->tipo == "bang") {
				//cout << "YES BANG" << endl;
				bang * e = (bang*)_ui->getElement(te->nome);
				if (e != NULL) {
					e->plau();
				}
				//_ui->getElement(te->nome)->set(!_ui->pBool[te->nome]);
	//			((bang*)_ui->getElement(te->nome))->set(!_ui->pBool[te->nome]);
	//			((bang*)_ui->getElement(te->nome))->set(bool(true));
				//((bang*)_ui->getElement(te->nome))->bang();
				//((booleano*)_ui->getElement(te->nome))->set(bool(true), true);
				
				// XAXA
				//_ui->getElement(te->nome)->flip();
			}
			else if (te->tipo == "boolon") {
				//_ui->pBool[te->nome] = true;
				_ui->getElement(te->nome)->set(true);
				midiControllerOut.sendNoteOn(msg.channel, msg.pitch);
			}
			else if (te->tipo == "booloff") {
				//_ui->pBool[te->nome] = false;
				_ui->getElement(te->nome)->set(false);
				midiControllerOut.sendNoteOff(msg.channel, msg.pitch);
			}
			else if (te->tipo == "bool") {
				_ui->getElement(te->nome)->set(!_ui->pBool[te->nome]);
				//_ui->pBool[te->nome] = !_ui->pBool[te->nome];
				// midi out pra controladora e colorir os botoes
				if (_ui->pBool[te->nome]) {
					midiControllerOut.sendNoteOn(msg.channel, msg.pitch);
				} else {
					midiControllerOut.sendNoteOff(msg.channel, msg.pitch);
				}
			}

			else if (te->tipo == "preset") {
				//cout << "preset!" << endl;
				// evita o problema no different thread
				if (_u != NULL) {
					// TODO XAXA
					
					midiControllerOut.sendNoteOn(lastPresetChannel, lastPresetPitch, 0); // 1 green 3 red 5 yellow
					_u->futureCommands.push_back(future("master", "presets", "loadAllPresets", ofToInt(te->nome)));
					_u->nextPreset.push_back(ofToInt(te->nome));
					
	//				cout << msg.channel << endl;
	//				cout << msg.pitch << endl;
					midiControllerOut.sendNoteOn(msg.channel, msg.pitch, 3); // 1 green 3 red 5 yellow
					
					lastPresetChannel = msg.channel;
					lastPresetPitch = msg.pitch;
					//http://community.akaipro.com/akai_professional/topics/midi-information-for-apc-mini
					//127 = verde
				}
			}
			
			else if (te->tipo == "savePresetNumber") {
				if (_u != NULL) {
					cout << te->tipo << endl;
					holdPresetNumber = _u->getPresetNumber();
				}
			}
			
			else if (te->tipo == "restorePresetNumber") {
				if (_u != NULL) {
					cout << te->tipo << endl;
					_u->futureCommands.push_back(future("master", "presets", "loadAllPresets", holdPresetNumber));
				}
			}

			// REMOVER?
			else if (te->tipo == "presetHold") {
				// evita o problema no different thread
				if (_u != NULL) {
					// TODO XAXA
					midiControllerOut.sendNoteOn(lastPresetChannel, lastPresetPitch, 0); // 1 green 3 red 5 yellow
					_u->futureCommands.push_back(future("master", "presets", "loadPresetHold", ofToInt(te->nome)));
					_u->nextPreset.push_back(ofToInt(te->nome));
					midiControllerOut.sendNoteOn(msg.channel, msg.pitch, 3); // 1 green 3 red 5 yellow
					lastPresetChannel = msg.channel;
					lastPresetPitch = msg.pitch;
					//http://community.akaipro.com/akai_professional/topics/midi-information-for-apc-mini
					//127 = verde
				}
			}
			
			// REMOVER?
			else if (te->tipo == "presetRelease") {
				//cout << "presetRelease" << endl;
				// evita o problema no different thread
				if (_u != NULL) {
					// TODO XAXA
					midiControllerOut.sendNoteOn(lastPresetChannel, lastPresetPitch, 0); // 1 green 3 red 5 yellow
					_u->futureCommands.push_back(future("master", "presets", "loadPresetRelease", ofToInt(te->nome)));
					//_u->nextPreset.push_back(ofToInt(te->nome));
					midiControllerOut.sendNoteOn(msg.channel, msg.pitch, 3); // 1 green 3 red 5 yellow
					lastPresetChannel = msg.channel;
					lastPresetPitch = msg.pitch;
					
					//http://community.akaipro.com/akai_professional/topics/midi-information-for-apc-mini
					//127 = verde
					
				}
			}



		} else {
			// discard note off ?
			if (msg.status != 128) {
				cout << index << endl;
			}
		}

		//midiControllerMap
		//te = &mapaMidiController[msg.status][msg.channel][msg.pitch][msg.control];


	//	else {
	//		string message =
	//		ofToString(msg.status) + " " +
	//		ofToString(msg.channel) + " " +
	//		ofToString(msg.pitch) + " " +
	//		ofToString(msg.control);
	//
	//		mapaMidiController[msg.status][msg.channel][msg.pitch][msg.control] = elementLearn;
	//
	//		//cout << "Empty" << endl;
	//		if (message != lastString) {
	//			lastString = message;
	//			cout << lastString << endl;
	//		}
	//	}


	//	if (te->ui != "") {
	//		_ui->redraw();
	//	}

		midiKeys[msg.pitch] = msg.status == 144;

		midiMessage = msg;
	}


	//--------------------------------------------------------------
	void midiInputAdded(string name, bool isNetwork) {
		cout << "input added" << endl;
		cout << name << endl;
	//	stringstream msg;
	//	msg << "ofxMidi: input added: " << name << " network: " << isNetwork;
	}

	//--------------------------------------------------------------
	void midiInputRemoved(string name, bool isNetwork) {
		cout << "input removed" << endl;
		cout << name << endl;
	//	stringstream msg;
	//	msg << "ofxMidi: input removed: " << name << " network: " << isNetwork << endl;
	}

	//--------------------------------------------------------------
	void setUI(ofxDmtrUI3 & u) {
		_u = &u;
	}

	//--------------------------------------------------------------
	void exit() {
		midiControllerOut.closePort();
		midiControllerIn.closePort();
	}

	//--------------------------------------------------------------
	void uiEvent(uiEv & event) {
		string nome = event.name;
		//cout << nome << endl;
	//	if (nome == "loadPresetAll") {
	//	if (nome == "allPresets") {
	//		for (auto & e : elements) {
	//			// corrigir
	//
	//			//if (e->varType == BOOLEAN) {
	//			//cout << "tipo :: " << e->tipo << endl;
	//			if (e->tipo == "bool" || e->tipo == "boolon" || e->tipo == "booloff" ) {
	//				if (_u->uis[e->ui].pBool[e->nome]) {
	//					midiControllerOut.sendNoteOn(e->channel, e->pitch, 127);
	//				} else {
	//					midiControllerOut.sendNoteOff(e->channel, e->pitch, 127);
	//				}
	//			}
	//		}
	//	}
	}

};
