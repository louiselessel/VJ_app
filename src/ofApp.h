#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"

#define HOST "127.0.0.1"              // HARDCODE SET OSC HOST AND PORT HERE
#define PORT 12345

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw(); 
    
    void stuffFromMatt ();
    
    void reset();
    void randomMovingRectangles();
    void lightRectangles();
    void aboriginalPattern();
    
    ofColor colorFromCenter(float h, float s, float b, int x, int y, float change);
    
    void basicGrid();
    void drawWithTriangles();
    void drawWithSine();
    void drawRandomLines();
    void drawCircles();
    void drawCircles2();
    void rotateRectangles();
    
    void randomWithoutFlicker();
    float lastX, lastY;
    float randomXPositions[10000];
    float randomYPositions[10000];
    
    int count =0;
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    // Syphon
    ofxSyphonServer mainOutputSyphonServer;           // send whole screen
    //ofxSyphonServer individualTextureSyphonServer;      // send just specific tex
    ofxSyphonClient mClient;
    //vector <ofTexture> syphonTex;
    
    // OSC
    ofxOscSender sender;
    ofxOscMessage m;
    string mAddress = "/pixXpixVJ";          // set OSC address
    
    // GUI
    ofxPanel panel;
    ofParameterGroup paramsOSC;
    ofParameter <string> host;
    ofParameter <string> port;
    ofParameter <float> val;
    
    
};

