#include "ofxDmtrUISyphonIn.h"

//--------------------------------------------------------------
void ofxDmtrUISyphonIn::setup() {
	syphonList.setup();
	syphonIn.setup();

	if (uiSyphon == NULL) {
		if (ofFile::doesFileExist("uiSyphon.txt")) {
			ui.createFromText("uiSyphon.txt");
		} else {
			int y = ofGetWindowHeight() - 200;
			ui.createFromLine("rect	0 "+ofToString(y)+" 20 20");
		}
		ui.setup();
		uiSyphon = &ui;
	}
	

	ofAddListener(syphonList.events.serverAnnounced, 	this, &ofxDmtrUISyphonIn::syphonUpdated);
	ofAddListener(syphonList.events.serverRetired, 		this, &ofxDmtrUISyphonIn::syphonUpdated);

	ofAddListener(ofEvents().keyPressed, 	this, &ofxDmtrUISyphonIn::onKeyPressed);
	ofAddListener(ofEvents().keyReleased,	this, &ofxDmtrUISyphonIn::onKeyReleased);


	ofAddListener(uiSyphon->settings.uiEvent, this, &ofxDmtrUISyphonIn::uiEvents);

	
	updateList();
	cout << "ofxDmtrUISyphonIn setup" << endl;
}

void ofxDmtrUISyphonIn::keyPressed(int key) { }
void ofxDmtrUISyphonIn::keyReleased(int key) { }
void ofxDmtrUISyphonIn::onKeyPressed(ofKeyEventArgs &data) { }
void ofxDmtrUISyphonIn::onKeyReleased(ofKeyEventArgs &data) { }


void ofxDmtrUISyphonIn::updateList() {
	vector <string> newLines;
	vector <string> options;
	
	// tempo
	string opcoes;
	for (auto & dir : syphonList.getServerList()) {
		cout << dir.appName << endl;
		cout << ignoreApp << endl;
		if (dir.appName != ignoreApp) {
			string nome = dir.serverName + "--" + dir.appName;
			options.push_back(nome);
			opcoes += nome+" ";
		}
	}
	
	
	uiSyphon->clear();

	if (options.size()) {
		opcoes = ofJoinString(options, "|");
		cout << opcoes << endl;
		uiSyphon->createFromLine("radioPipe	syphonIn	"+opcoes);
	}
	//uiSyphon->create("syphonIn", "radio", opcoes);
	// nao entendi porque
	//uiSyphon->createRadio("syphonIn", options);
	uiSyphon->createFromLine("autoFit");
	//uiSyphon->addAllListeners();
}

//--------------------------------------------------------------
void ofxDmtrUISyphonIn::syphonUpdated(ofxSyphonServerDirectoryEventArgs &arg) {
	updateList();
}


//--------------------------------------------------------------
void ofxDmtrUISyphonIn::uiEvents(uiEv & e) {
	if (e.name == "syphonIn") {
		vector <string> sp = ofSplitString(e.s, "--");
		if (sp.size()>1) {
			string server = sp[0];
			string app = sp[1];
			
			syphonIn.set(server,app);
			//syphonIn.set("", "");
			cout << "setting : " << server << " -- " << app << endl;
		}
	}
}
