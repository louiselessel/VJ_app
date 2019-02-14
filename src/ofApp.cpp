#include "ofApp.h"

// TO DO
//    - Finish add GUI
//    - Orange rise
//    - white light opening from center
//    - pattern

//--------------------------------------------------------------
// Global vars

int cellSize = 20;
float xPos = 0.0f;
float yPos = 0.0f;

// hue, saturation, brightness
float h = 25.0f;
float s = 255.0f;
float b = 255.0f;
ofColor c = ofColor(0);

bool starttrig = true;
int i = 1;
float effect = 0.0f;
float easing = 0.0f;


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);
    ofBackground(0);

    c.setHsb(h, s, b);
    i = 1;
    
    //    for (int i = 0; i < 10000; i++) {
    //        randomXPositions[i] = ofRandom(ofGetWidth());
    //        randomYPositions[i] = ofRandom(ofGetHeight());
    //    }
    
    
    ///////// Syphon
    mainOutputSyphonServer.setName("ofx Output");                        // send whole screen
    // individualTextureSyphonServer.setName("ofx Texture Output");      // send just specific tex
    mClient.setup();
    mClient.set("","Simple Server"); //using Syphon app Simple Server, found at http://syphon.v002.info/
    
    // GUI
    panel.setup("", "settings.xml", 10, 100);   // location of control panel
    // Display OSC host and port info
    paramsOSC.add(host.set("Host", HOST));                // hardcoded version
    string port1 = to_string(PORT);
    paramsOSC.add(port.set("Port", port1));
    paramsOSC.add(val.set("val", 0, 0, 1080));
    panel.add(paramsOSC);
    
    // Display panel with all info
    panel.loadFromFile("settings.xml");
    
    // OSC
    // sender.setup( HOST, PORT );
}

//--------------------------------------------------------------
void ofApp::update(){
//    xPos += 2.0f;
//    if(ofGetWidth() < xPos){
//        xPos = 0.0f;
//    }
//    yPos += 0.5f;
//    if(ofGetHeight() < yPos){
//        yPos = 0.0f;
//    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255, 255, 255);
    
    //    sketches
    switch (i) {
        case 1:
            ofSetColor(c);
            ofDrawRectangle(0, ofGetHeight(), ofGetWidth(), -yPos);
            yPos += 1.0f;
            if (yPos > ofGetHeight()) {
                i++;
                reset();
            }
            break;
        case 2:
            lightRectangles();
            yPos += 2.0f;

            if (yPos > ofGetHeight()) {
                i++;
                reset();
            }
            break;
        case 3:
            //ofSetColor(c);
            randomMovingRectangles();
            break;
            
        default:
            break;
    }
//
//    if (starttrig == true) {
//        ofSetColor(255, 136, 0);
//        ofDrawRectangle(0, ofGetHeight(), ofGetWidth(), -yPos);
//        if (yPos > ofGetHeight()) {
//            starttrig = false;
//            yPos = 0.0f;        // reset y
//        }
//    }
//    else {
//
//    }
    //aboriginalPattern();
    
    // debug rect at top
//    ofSetColor(255,0,0);
//    ofDrawRectangle(0, 0, ofGetMouseX(), ofGetSeconds());
    

    
    ///////// Syphon
    mClient.draw(50, 50);
    mainOutputSyphonServer.publishScreen();                      // send whole screen
    // Only send the specific output (after adding gui this might be relevant)
    //individualTextureSyphonServer.publishTexture(&texDepth[0]);     // send depth tex
    //individualTextureSyphonServer.publishTexture(imgCV);     // send depth tex
    
    // PLACE GUI AFTER SYPHON, so it doesn't get in to the output
    // Print stuff to GUI
    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "Report" << endl
    << "val: " << val<< endl
    << "x: " << xPos<< endl
    << "y: " << yPos<< endl
    << endl
    << "OSC address: " << mAddress << endl
    << endl
    << "fps: " << ofGetFrameRate();
    ofDrawBitmapString(reportStr.str(), 20, ofGetHeight()-100);
    

    // Draw gui
//    panel.draw();
    
    
    //---------------------------------
    // OSC
    // Send all in one message
    /*
     m.setAddress(mAddress);
     m.addFloatArg( xyConvCM.x );
     m.addStringArg( "_" );
     m.addFloatArg( xyConvBB.y );
     
     sender.sendMessage( m );                                        // send stuff over OSC
     m.clear();
     */
}

//--------------------------------------------------------------
// Cheatsheet
// ofDrawRectangle(x, y, ofGetMouseX() / (ofGetWidth()/cellSize), ofGetMouseY() / (ofGetHeight()/cellSize));         // map width of each to be within cell
// newx = ofMap(x, 0, 10, 21, 22);                  // map function: from range, to range

void ofApp::reset() {
    xPos = 0.0f;
    yPos = 0.0f;
    effect = 0.0f;
    easing = 0.0f;
}

ofColor ofApp::colorFromCenter(float _h, float _s, float _b, int _x, int _y, float change) {
    float _centerX = ofGetWidth()/2;    // find center
    float _distX = abs(ofDist(_x, _y, _centerX, _y)); // distance from x to center eg. #: 0-300
    float _newX = 255-ofMap(_distX, 0, _centerX, 0, 255); // map to color range, invert effect: 255-
    _newX *= change; // ease change in
    
    ofColor _c;
    _c.setHsb(_h, _s-_newX, _b); // desaturate
    return _c;
}

void ofApp::lightRectangles() {
    easing += 0.01f;
    val = easing;
    // make small delay after easing in of white
    if (easing >= 1.0f) {
        easing = 1.0f;
        effect += 0.01f;
    }
    
    for (int x = 0; x < ofGetWidth(); x+=cellSize) {
        for (int y = 0; y < ofGetHeight(); y+=cellSize) {
            //float _c = ofGetSeconds();
            // color
            c = colorFromCenter(h, s, b, x, y, easing);
            ofSetColor(c);
            
            ofDrawRectangle(x, y, cellSize-effect, cellSize-effect);
            
//            float distanceToMouse =  ofDist(x,y,ofGetMouseX(),ofGetMouseY());
//            distanceToMouse+= count;
//            distanceToMouse/= 30;
//            float newGray = ofMap(sin(distanceToMouse),-1,1,0,255);
//            ofSetColor(newGray);
//            ofDrawRectangle(x,y,cellSize,cellSize);
        }
    }
}

void ofApp::randomMovingRectangles() {
    val = ofGetMouseX();
    for (int x = 0; x < ofGetWidth(); x+=cellSize) {
        for (int y = 0; y < ofGetHeight(); y+=cellSize) {
            // color
            float _centerX = ofGetWidth()/2;
            float _distX = abs(ofDist(x, y, _centerX, y)); // distance from x to center eg. #: 0-300
            float _x = 255-ofMap(_distX, 0, _centerX, 0, 255); // map to color range, invert effect: 255-
            c.setHsb(h, s-_x, b);
            ofSetColor(c);
            //move
            float _xPos = x + ofRandom(-5,5);
            float _yPos = y + ofRandom(-5,5);
            ofDrawRectangle(_xPos, _yPos, cellSize-1, cellSize-1);

        }
    }
}


//void ofApp::movingRectangles() {
//    int cellSize = 20;
//    ofBackground(0, 0, 0);
//    val = ofGetMouseX();
//    for (int x = 0; x < ofGetWidth(); x+=cellSize) {
//        for (int y = 0; y < ofGetHeight(); y+=cellSize) {
//            // distance to edge as a function between -1 and 1 in x
//            //1.0f - pow(abs(x), 2.0f);
//            //int newcolor = (1.0f - pow(abs(x), 2.0f)) * 100;
//            // 255-
//            //ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
//            int newcolor = ofMap(x, 0, ofGetWidth(), 0, 255);
//            //ofMap(sin(x),-1,1,0,255);
//            //ofMap(x, ofGetWidth()/2, ofGetWidth(), 0, 255); //ofDist(x,y, ofGetWidth()/2, ofGetHeight()/2);//, 0, 255);
//            val = newcolor;
//            ofSetColor(newcolor, newcolor, newcolor);
//            //ofSetColor(255, 136, 0);
//            ofDrawRectangle(x, y, 5, 5);
//
//        }
//    }
//}

void ofApp::aboriginalPattern() {
    ofBackground(0, 0, 0);
    val = ofGetMouseX();
    for (int x = 0; x < ofGetWidth(); x+=cellSize) {
        for (int y = 0; y < ofGetHeight(); y+=cellSize) {
            c.setHsb(h, s, b);
            ofSetColor(c);
            //
        }
    }
}

void ofApp::basicGrid() {
    int cellSize = 20;
    ofBackground(255);
    for(int x =0; x < ofGetWidth(); x+=cellSize) {
        for(int y =0; y < ofGetHeight(); y+=cellSize) {
            ofSetColor(x,ofGetSeconds()*2,ofGetMouseX());
            ofDrawRectangle(x,y,cellSize,cellSize);
        }
        
    }
}

void ofApp::drawWithTriangles() {
    int cellSize = 20;
    ofBackground(0);
    for(int x =0; x < ofGetWidth(); x+=cellSize) {
        for(int y =0; y < ofGetHeight(); y+=cellSize) {
            ofPushMatrix();
            float distanceToMouse =  ofDist(x,y,ofGetMouseX(),ofGetMouseY());
            
            ofTranslate(x,y);
            ofRotate(distanceToMouse/5);
            ofDrawTriangle(-cellSize/2, -cellSize/2, -cellSize/2,cellSize/2, cellSize/2, cellSize/2);
            ofPopMatrix();
            
        }
        
    }
}


void  ofApp::randomWithoutFlicker() {
    int cellSize = 20;
    //    ofBackground(0);
    for(int x =0; x < 10000; x+=5) {
        float xPosition = ofRandom(10000);
        float yPosition = randomYPositions[x];
        ofSetColor(xPosition/4,yPosition/4,ofGetMouseX()/4);
        ofSetLineWidth(1);
        
        ofDrawRectangle(xPosition,yPosition,cellSize,cellSize);
        
        
        
    }
    
}

int count = 0;
void  ofApp::drawWithSine() {
    int cellSize = 10;
    ofBackground(0);
    for(int x =0; x < ofGetWidth(); x+=cellSize) {
        for(int y =0; y < ofGetHeight(); y+=cellSize) {
            float distanceToMouse =  ofDist(x,y,ofGetMouseX(),ofGetMouseY());
            distanceToMouse+= count;
            distanceToMouse/= 30;
            float newGray = ofMap(sin(distanceToMouse),-1,1,0,255);
            ofSetColor(newGray);
            ofDrawRectangle(x,y,cellSize,cellSize);
        }
        
    }
    
}

void  ofApp::drawRandomLines() {
    int cellSize = 10;
    ofBackground(0,0,255,5);
    
    ofNoFill();
    //    ofSetColor(255, 0, 0);//stroke color
    for(int x =0; x < 1000; x+=10) {
        float thisX = ofGetMouseX() + ofRandom(-x, x);
        float thisY = ofGetMouseY() + ofRandom(-x, x);
        ofDrawLine(thisX, thisY, lastX, lastY);
        lastX = thisX;
        lastY = thisY;
        
        
    }
    ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
}

void ofApp::drawCircles() {
    ofBackground(255);
    int cellSize =40;
    for(int x =0; x < 1000; x+=cellSize) {
        for(int y =0; y < ofGetHeight(); y+=cellSize) {
            float distanceToMouse =  ofDist(x,y,ofGetMouseX(),ofGetMouseY());
            
            ofPushMatrix();
            ofTranslate(0,0,distanceToMouse);
            ofSetColor(distanceToMouse); //fill color
            ofDrawEllipse(x,y,cellSize*2,cellSize*2);
            ofPopMatrix();
            
            
            
        }}
    
    
}


void ofApp::rotateRectangles() {
    
    //    ofLightsData();
    ofBackground(0);
    int cellSize =50;
    for(int x =0; x < ofGetWidth(); x+=cellSize) {
        for(int y =0; y < ofGetHeight(); y+=cellSize) {
            float distanceToMouse =  ofDist(x,y,ofGetMouseX(),ofGetMouseY()) ;
            
            ofPushMatrix();
            ofTranslate(x,y);
            ofSetColor(distanceToMouse);
            ofRotateX(distanceToMouse/100.00);
            
            ofDrawRectangle(0,0,cellSize,cellSize);
            
            
            
            ofPopMatrix();
            
            
            
        }}
    
    
}

void ofApp::drawCircles2() {
    if (count > 10) {
        count = 0;
    }
    count++;
    //    count =5;
    
    ofBackground(255);
    int cellSize =30;
    for(int x =0; x < ofGetWidth(); x+=cellSize) {
        for(int y =0; y < ofGetHeight(); y+=cellSize) {
            float distanceToMouse =  ofDist(x,y,ofGetMouseX(),ofGetMouseY()) ;
            
            ofPushMatrix();
            ofTranslate(0,0,distanceToMouse);
            ofSetColor(distanceToMouse);
            //            ofSetColor(count*10,distanceToMouse,distanceToMouse); //fill color
            //            ofDrawRectangle(x,y,cellSize*2,cellSize*2);
            ofDrawRectangle(x,y,cellSize*count,cellSize*count);
            
            ofRotate(count);
            //            ofDrawRectangle(x,y,count*2,count*2);
            ofPopMatrix();
            
            
            
        }}
    
    
}

void ofApp::stuffFromMatt() {
    //    stuff from matt
    //    basicGrid();
    //    drawWithTriangles();
    //    drawWithSine();
    //    drawRandomLines();
    //    drawCircles2();
    //    rotateRectangles();
    //    randomWithoutFlicker();
    
    
    //    stuff from matt go through at some point
    //    ofDrawRectangle(xPos, 200,200,200);
    //    ofSetColor(ofColor::red);  // draw everything in red
    //    ofDrawCircle(xPos, 100, 10);
    //    int randomSeedColor = ofRandom(120,255);
    //    ofSetColor(ofRandom(0,255));
    //    ofSetRectMode(OF_RECTMODE_CENTER);
    //    for(int x =0; x < ofGetWidth(); x=  x +40) {
    //    for(int y =0; y < ofGetHeight(); y = y +20) {
    //         ofSetColor(ofGetMouseX()/2,randomSeedColor,100);  // draw everything in red
    ////         ofSetColor(ofColor::red);  // draw everything in red
    //         ofDrawRectangle(x, ofRandom(0,ofGetHeight()), 20, 10);
    //    }
    //    }
    //    ofDrawRectangle(ofGetMouseX(), ofGetMouseY(), 50, 50);  // Draw a 50 x 50 rect
}

void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    //    ofDrawRectangle(x, y,200,200);
    //    ofSetColor(ofColor::red);  // draw everything in red
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}


