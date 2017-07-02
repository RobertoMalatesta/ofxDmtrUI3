#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowPosition(40, 40);

	// ALL Elements definitions are loaded from this file:
	u.createSoftwareFromText("u.txt");
	u.setFbo(u.mapFbos["fbo"]);

	for (auto & ui : u.allUIs) {
		ofAddListener(ui->settings.uiEvent,this, &ofApp::uiEvents);
	}

	ofSetVerticalSync(false);
	//ofSetFrameRate(60);
	ofSetCircleResolution(120);
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::desenha(){
	if (*cena == "test") {
		float raio = ui->pFloat["raio"];
		float x = ofNoise(ofGetElapsedTimef()*.2) * fbo->getWidth();
		float y = ofNoise(ofGetElapsedTimef()*.3)* fbo->getHeight();
		if (ui->pBool["drawCircle"]){
			ofSetColor(ofColor(ui->pFloat["r"],ui->pFloat["g"],ui->pFloat["b"]));
			ofDrawCircle(x, y, raio, raio);

			ofSetColor(255);
			ofDrawBitmapString(ui->pString["blend"], x, y);
		}

		if (ui->pBool["circle2"]){
			float x = ui->pPoint["slider2d"].x * fbo->getWidth();
			float y = ui->pPoint["slider2d"].y * fbo->getHeight();
			ofSetColor(ofColor(ui->pFloat["r2"],ui->pFloat["g2"],ui->pFloat["b2"]));
			ofDrawCircle(x, y, 100);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor(ui->pFloat["bg_r"],ui->pFloat["bg_g"],ui->pFloat["bg_b"]));

	//ofBackground(40);
	fbo->begin();
	ofClear(0,255);
	desenha();
	fbo->end();
	float w = fbo->getWidth() * u.pFloat["fboScale"];
	float h = fbo->getHeight() * u.pFloat["fboScale"];
	fbo->draw(u.pInt["fboX"], u.pInt["fboY"], w, h);

	// modificar pra events
	ui->settings.flowFree = ui->pBool["flowFree"];
	ui->settings.opacity = ui->pFloat["opacity"];
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::uiEvents(uiEv & e) {
	//cout << e.name << endl;
}
