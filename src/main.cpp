#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(int argc, char* argv[]){
	ofSetupOpenGL(ofApp::WINDOW_WIDTH, ofApp::WINDOW_HEIGHT,OF_WINDOW);			// <-------- setup the GL context

	int nBalls = 10;

	if (argc > 1)
	{
		std::istringstream ss(argv[1]);
		int x;
		if ((ss >> x)) {
			nBalls = x;
		}
	}

	ofRunApp(new ofApp(nBalls));

}
