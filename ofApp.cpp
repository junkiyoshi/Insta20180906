#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
	this->font.loadFont("fonts/Kazesawa-bold.ttf", 180, true, true, true);

	for (int i = 0; i < ofGetHeight(); i++) 
	{
		this->y_noise.push_back(0.f); 
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}
//--------------------------------------------------------------
void ofApp::update() {

	this->fbo.begin();
	ofClear(0);
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofRotate(45);
	
	string word = "SPLIT";
	this->font.drawString(word, this->font.stringWidth(word) * -0.5, this->font.stringHeight(word) * 0.5);

	this->fbo.end();

	float noise_value;
	for (int y = 0; y < this->y_noise.size(); y++) {

		if (y % 10 == 0) { 
			
			noise_value = ofMap(ofNoise(y * 0.05, ofGetFrameNum() * 0.01), 0, 1, -1, 1);

			if (noise_value > 0.65) {

				noise_value -= 0.65;
			}
			else if (noise_value < -0.65) {

				noise_value += 0.65;
			}
			else {

				noise_value = 0.0;
			}
			
		}

		this->y_noise[y] = noise_value;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniformTexture("tex", this->fbo.getTexture(), 1);
	this->shader.setUniform1fv("y_noise", &this->y_noise[0], ofGetHeight());

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}