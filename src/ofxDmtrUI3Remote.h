/*
ofxDmtrUI3Remote 
 Created by Dimitre Lima on 18/05/16.
 Copyright 2016 Dmtr.org. All rights reserved.
*/

#pragma once

#if defined( TARGET_OF_IPHONE ) || defined( TARGET_OF_IOS ) || defined( TARGET_ANDROID )
#define DMTRUI_TARGET_TOUCH
#endif

#include "ofEvents.h"
#include "ofxDmtrUI3.h"
#include "ofxOsc.h"

#ifdef DMTRUI_TARGET_TOUCH
#include "ofxAccelerometer.h"
#endif

class ofxDmtrUI3Remote : public ofBaseApp {
public:
//	void update();
//	void draw();
//	void onDraw(ofEventArgs &data);
//	void onUpdate(ofEventArgs &data);
//	void sendInterface(bool clear = true);
//	void sendVars();
//	void setupRemote();
//	void setListeners();
//	void sendAccel(float x, float y, float z);
//	
//	void setupServer(ofxDmtrUI3 * ui);
//	void enableAccelerometer();
//	void uiEvent(uiEv & e);

	
	ofxOscSender 	send;
	ofxOscReceiver 	receive;


	string lastAdd = "";
	string linha = "";



	bool useAccelerometer = false;

	ofEvent<ofVec3f> accelEvent;

	string 	serverHostname = "";
	//int 	serverPort = 8000;
	string 	remoteHostname = "";
	int 	remotePort = 9000;


	string remoteStyle = "";

	map <elementType, string> stringType;
	map <dmtrUIVarType, string> stringVarType;

	//void sendElement(element &e);
	//bool debug = true;
	bool debug = true;

	// check if the remote side is interfaced
	bool remoteIsInterfaced = false;
	// check if my server is setup. works for both sides
	bool serverIsSetup = false;
	// check if my side have interface. always on server, sometimes on remote.
	bool haveInterface = false;

	// remove in near future
	bool considerUIEvents = true;

	bool fireEvent = true;

//	static int actualPort;
	//static int actualPort = 7999;
	int actualPort = 8000;

	int serverPort;



	// AVERIGUAR ISSO AQUI
	ofxDmtrUI3 ui;

	//	ofxDmtrUI3 *_ui = &ui;
	// pointer pra interface classico. aponta o que precisa ser visto
	ofxDmtrUI3 *_ui = NULL;
	// teste
	ofxDmtrUI3 * _u = NULL;


	
	// experimental
	vector <ofxDmtrUI3 *> _uis;
	bool hasMasterInterface = false;

	void addUI(ofxDmtrUI3 * ui) {
		_uis.push_back(ui);
		ofAddListener(_uis.back()->settings.uiEvent, this, &ofxDmtrUI3Remote::uiEvent);
	}

	void setupMainUI(ofxDmtrUI3 & u) {
		_u = &u;
		hasMasterInterface = true;
	}

	ofxDmtrUI3Remote() {
		actualPort ++;
		serverPort = actualPort;
		//cout << "ofxDmtrUI3Remote YES " << serverPort << endl;
		ofAddListener(ofEvents().draw, this, &ofxDmtrUI3Remote::onDraw);
		ofAddListener(ofEvents().update, this, &ofxDmtrUI3Remote::onUpdate);
	}


	ofxOscMessage getOscMessageFromElement(string address, element & e) {
		ofxOscMessage m;
		m.setAddress(address);
		string name = e.name;
		
		if (e.varType == DMTRUI_FLOAT) {
			m.addFloatArg(_ui->pFloat[name]);
		}
		else if (e.varType == DMTRUI_INT) {
			m.addIntArg(_ui->pInt[name]);
		}
		else if (e.varType == DMTRUI_STRING) {
			m.addStringArg(_ui->pString[name]);
		}
		else if (e.varType == DMTRUI_BOOLEAN) {
			m.addBoolArg(_ui->pBool[name]);
		}
		else if (e.varType == DMTRUI_POINT) {
			m.addFloatArg(_ui->pPoint[name].x);
			m.addFloatArg(_ui->pPoint[name].y);
		}
		
		if (m.getNumArgs() > 0) {
			if (debug) {
				cout << "SENDING :: " + m.getAddress() << endl;
				cout << remoteHostname << ":" << remotePort << endl;
				if (e.varType == DMTRUI_FLOAT) {
					cout << m.getArgAsFloat(0) << endl;
				}
			}
			return m;
//			send.sendMessage(m, false);
		}
	}
	
	
	struct ofxDmtrUIRemoteEvent {
	public:
		string name;
		ofxDmtrUIRemoteEvent(string n) : name(n) {}
	};

	ofEvent<ofxDmtrUIRemoteEvent> remoteEvent;

	string interfaceSpecialCommand = "/uiSpecialCommand/";

	//--------------------------------------------------------------
	void setListeners() {
		if (_ui != NULL) {
			ofAddListener(_ui->settings.uiEvent, this, &ofxDmtrUI3Remote::uiEvent);
		}
	}

	//--------------------------------------------------------------
	void setupServer(ofxDmtrUI3 * u) {
		haveInterface = true;
		_ui = u;

		receive.setup(serverPort);

		try {
			serverIsSetup = send.setup(remoteHostname, remotePort);
		} catch (exception){
			cout << "ofxDmtrUI3Remote :: &&& no internet &&&" << endl;
		}

		if (serverIsSetup) {

			string message = "ofxDmtrUI3Remote server \r";
			message += "ui = " + _ui->UINAME + "\r";
			message += "server = " + serverHostname + ":" + ofToString(serverPort) + "\r";
			message += "remote = " + remoteHostname + ":" + ofToString(remotePort) ;

			_ui->messageBox(message);

			setListeners();
		}
	}

	//--------------------------------------------------------------
	void setupRemote() {
		remoteIsInterfaced = true;
		//setupAll();
		ui.UINAME = "REMOTE";
		_ui = &ui;

		try {
			receive.setup(remotePort);

			string message = "ofxDmtrUI3Remote REMOTE \r";
			//message += "Addr = " + remoteHostname + "\r";
			message += "Port = " + ofToString(remotePort);
			ui.messageBox(message);

			setListeners();
		} catch (exception){
			cout << "ofxDmtrUI3Remote :: &&& no internet &&&" << endl;
		}
		
		ui.settings.software->opacity = 255;
	}



	//--------------------------------------------------------------
	void update() {

	#ifdef DMTRUI_TARGET_TOUCH
		if (useAccelerometer) {
			ofxOscMessage m;
			m.setAddress(interfaceSpecialCommand + "accelerometer");
	//        m.addFloatArg(ofxAccelerometer.getOrientation().x);
	//        m.addFloatArg(ofxAccelerometer.getOrientation().y);
	//        m.addFloatArg(ofxAccelerometer.getOrientation().z);
			send.sendMessage(m, false);
		}
	#endif


		// RECEIVE OSC
		while(receive.hasWaitingMessages()){
			ofxOscMessage m;
			receive.getNextMessage(m);
			lastAdd = m.getAddress();
			
			if (debug) {
				cout << "receiving message :: " + m.getAddress() << endl;
			}
			
			vector <string> addr = ofSplitString(m.getAddress(), "/");
			
			ofxDmtrUIRemoteEvent e = ofxDmtrUIRemoteEvent(m.getAddress());
			ofNotifyEvent(remoteEvent, e);

			if (addr[0] == "master") {
				int n = m.getArgAsInt(0);
				_u->loadPresetAll(n);
			}
			
			
			if (("/"+addr[1]+"/") == interfaceSpecialCommand) {
				considerUIEvents = false;
				string action = addr[2];

	 #ifdef DMTRUI_TARGET_TOUCH
				if (addr[2] == "useAccelerometer") {
					if (m.getArgAsBool(0)) {
						//ofxAccelerometer.setup();
						useAccelerometer = true;
					}
				}
	#endif
				
				if (action == "clear") {
					_ui->clear();
				}

				else if (action == "uiname") {
					_ui->UINAME = m.getArgAsString(0);
					_ui->settings.uiname = m.getArgAsString(0);
					if (debug) {
						cout << "remote received UINAME :: " << m.getArgAsString(0) << endl;
					}
				}

				else if (action == "freeze") {
					considerUIEvents = false;
					if (debug) {
						cout << "freeze" << endl;
					}
				}
				else if (action == "unfreeze") {
					considerUIEvents = true;
					if (debug) {
						cout << "unfreeze" << endl;
					}
				}

				else if (action == "sendmevars") {
					remoteIsInterfaced = true;
					sendVars();
				}

				else if (action == "location") {
					remoteHostname = m.getArgAsString(0);
					if (m.getNumArgs() > 1) {
						remotePort = m.getArgAsInt(1);
					}
					if (debug) {
						cout << "location :: " + remoteHostname << endl;
					}

					send.setup(remoteHostname, remotePort);
					serverIsSetup = true;
				}


				if (m.getAddress() == interfaceSpecialCommand + "interface") {
					ofBuffer blob = m.getArgAsBlob(0);

					// keepvars? vai funcionar? xaxa todo
					_ui->clear(true);
					vector <string> lines;

					for(auto & line: blob.getLines()) {
						lines.push_back(line);
					}
					
					_ui->createFromLines(lines);
					_ui->autoFit();

					haveInterface = true;
					//interfaced = true;
					

					_ui->updateLookup();

					// get me variables
					ofxOscMessage m2;
					m2.setAddress(interfaceSpecialCommand + "sendmevars");
					send.sendMessage(m2, false);
				}

				else if (m.getAddress() == interfaceSpecialCommand + "createFromLine") {
					_ui->createFromLine(m.getArgAsString(0));
					_ui->updateLookup();
				}

	//			else if (m.getAddress() == interfaceSpecialCommand + "remote") {
	//				ofxOscMessage m;
	//				m.setAddress(interfaceSpecialCommand + "interface");
	//				//por mais uma linha com "clear" antes.
	//				ofBuffer buff2 = ofBufferFromFile("remoto.txt"); //remoto.txt
	//				m.addBlobArg(buff2);
	//				send.sendMessage(m, false);
	//			}

				else if (m.getAddress() == interfaceSpecialCommand + "accelerometer") {
					_ui->pFloat["accelX"] = m.getArgAsFloat(0);
					_ui->pFloat["accelY"] = m.getArgAsFloat(1);
					_ui->pFloat["accelZ"] = m.getArgAsFloat(2);

					ofVec3f acc = ofVec3f(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
					ofNotifyEvent(accelEvent, acc);
					// fazer um evento aqui
				}

				considerUIEvents = true;

			} else {

				
				if (haveInterface && considerUIEvents && _u != NULL && addr.size() > 2) {
					if (debug) {
					}

					string uiname = addr[1];
					
					cout << addr.size() << endl;
					string name = addr[2];
					
					if (_u != NULL) {
						_ui = &_u->uis[uiname];
					}

					fireEvent = false;
					bool propagateEvent = false;
					
					
					// prova de conceito mas eventualmente nao vai funcionar ainda por causa do propagateevent. refazer isso logo em breve
					ofxOscArgType k = m.getArgType(0);
					if (k == OFXOSC_TYPE_FLOAT) {
						_ui->set(name, (float) m.getArgAsFloat(0), false);
					}
					else if (k == OFXOSC_TYPE_INT32 || k == OFXOSC_TYPE_INT64) {
						_ui->set(name, (int) m.getArgAsInt(0), false);
					}
					else if (k == OFXOSC_TYPE_FALSE) {
						_ui->set(name, (bool) false, false);
					}
					else if (k == OFXOSC_TYPE_TRUE) {
						_ui->set(name, (bool) true, false);
					}
					else if (k == OFXOSC_TYPE_STRING) {
						_ui->set(name, (string) m.getArgAsString(0), false);
					}
					fireEvent = true;
				}
			}
		}
	}

	//--------------------------------------------------------------
	void draw() {
		if (!serverIsSetup && !haveInterface) {
			string strAnimation[4] = { "\\", "|", "/", "-" };
			ofDrawBitmapString("Dmtr.org ofxDmtrUI \nWaiting OSC Data " + strAnimation[ofGetFrameNum()%4], 20, 40);
		}

		if (debug) {
			ofSetColor(255);
			ofDrawBitmapString(lastAdd, 20, ofGetWindowHeight()-30);
			ofDrawBitmapString(linha, 20, ofGetWindowHeight()-50);
		}
	}

	//--------------------------------------------------------------
	void onDraw(ofEventArgs &data) { draw(); }

	//--------------------------------------------------------------
	void onUpdate(ofEventArgs &data) { update(); }

	//--------------------------------------------------------------
	void enableAccelerometer() {
		//if (sendSetup)
		{
			ofxOscMessage m;
			m.setAddress(interfaceSpecialCommand + "useAccelerometer");
			m.addBoolArg(true);
			send.sendMessage(m, false);
		}
	}

	//--------------------------------------------------------------
	void sendInterface(bool clear) {
		if (debug) {
			cout << "ofxDmtrUI3Remote :: sendInterface to " + remoteHostname << endl;
		}
		if (serverIsSetup) {
			if (debug) {
				cout << "sendsetup!" << endl;
			}

			ofxOscBundle b;

			{
				ofxOscMessage m;
				m.setAddress(interfaceSpecialCommand + "uiname");
				m.addStringArg(_ui->UINAME);
				b.addMessage(m);
			}

			{
				ofxOscMessage m;
				m.setAddress(interfaceSpecialCommand + "freeze");
				b.addMessage(m);
			}
			{
				ofxOscMessage m;
				m.setAddress(interfaceSpecialCommand + "location");
				m.addStringArg(serverHostname);
				m.addIntArg(serverPort);
				b.addMessage(m);
			}
			{
				ofxOscMessage m;
				m.setAddress(interfaceSpecialCommand + "interface");
				ofBuffer blob;
				if (clear) {
					blob.append("uiClear\n");
				}

				if (remoteStyle != "") {
					blob.append(remoteStyle);
				}
				
				if (ofFile::doesFileExist("uiRemote.txt")) {
					blob.append(ofBufferFromFile("uiRemote.txt"));
				}
				blob.append(_ui->createdFromTextFile);
				m.addBlobArg(blob);
				b.addMessage(m);
			}
			send.sendBundle(b);
		} else {
			cout << "ofxDmtrUI3Remote :: server is not setup ..." << endl;
		}
	}

	//--------------------------------------------------------------
	void sendVars() {

		if (debug) {
			cout << "ofxDmtrUI3Remote :: sendVars :: " << _ui->UINAME  << endl;
		}
		{
			ofxOscBundle b;
			for (auto & e : _ui->elements) {
				ofxOscMessage m = getOscMessageFromElement("/" + _ui->UINAME + "/" + e->name, *e);
				if (m.getNumArgs() > 0) {
					b.addMessage(m);
				}

				if (debug) {
					cout << "sending message :: " + m.getAddress() << endl;
				}
			}

			send.sendBundle(b);
		}
	}

	//--------------------------------------------------------------
	void sendAccel(float x, float y, float z) {
		//if (sendSetup)
		{
			ofxOscMessage m;
			m.setAddress(interfaceSpecialCommand + "accelerometer");
			m.addFloatArg(x);
			m.addFloatArg(y);
			m.addFloatArg(z);
			send.sendMessage(m, false);
		}
	}

	//--------------------------------------------------------------
	void uiEvent(uiEv &e) {
		if (serverIsSetup) {
			if (e.uiGlobal) {
				cout << "UI GLOBAL " << endl;
				cout << e.name << endl;
				if (e.name == "createFromText") {
					considerUIEvents = false;
					sendInterface(true);
				}
				else if (e.name == "load" || e.name == "loadPresetAll") {
					// e se nao tiver interface ainda?
					considerUIEvents = true;
					//sendInterface(true);
					sendVars();
				}
			} else {
				

				//temporario pro touchosc
				//if (!e.isDir && remoteIsInterfaced && fireEvent) {
				if (!e.isDir) {

					
					if (_u != NULL) {
						_ui = &_u->uis[e.uiname];
					}

					
					string address = "/" + e.uiname + "/" + e.name;
					
					//ofxOscMessage m = getOscMessageFromElement(address, *_ui->getElement(e.name));

					ofxOscMessage m;
					m.setAddress(address);

					// AQUI TA REPETIDO
					if (e.varType == DMTRUI_FLOAT) {
						m.addFloatArg(_ui->pFloat[e.name]);
					}
					else if (e.varType == DMTRUI_INT) {
						m.addIntArg(_ui->pInt[e.name]);
					}
					else if (e.varType == DMTRUI_STRING) {
						m.addStringArg(_ui->pString[e.name]);
					}
					else if (e.varType == DMTRUI_BOOLEAN) {
						m.addBoolArg(_ui->pBool[e.name]);
					}
					else if (e.varType == DMTRUI_POINT) {
						m.addFloatArg(_ui->pPoint[e.name].x);
						m.addFloatArg(_ui->pPoint[e.name].y);
					}

					if (m.getNumArgs() > 0) {
						if (debug) {
							cout << "SENDING :: " + m.getAddress() << endl;
							cout << remoteHostname << ":" << remotePort << endl;
							if (e.varType == DMTRUI_FLOAT) {
								cout << m.getArgAsFloat(0) << endl;
							}
						}
						send.sendMessage(m, false);
					}
				}
			}
		}
	}

	
};

