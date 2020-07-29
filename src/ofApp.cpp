#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Setup the screen.
    ofSetVerticalSync(true);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetFrameRate(60);
    ofBackground(0);
    
    // Setup the mesh.
    mesh.setMode(OF_PRIMITIVE_POINTS);
//    mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    glPointSize(2.0);
    
    // Setup the camera.
    cam.setDistance(1000);
}

ofVec4f ofApp::generate_random_velocity(){
    return ofVec4f(ofRandom(-1.0, 1.0),ofRandom(-1.0, 1.0),ofRandom(-1.0, 1.0),0.0);
}

ofVec3f snoiseVec3( ofVec3f x ){
    float s   = ofNoise(ofVec3f( x ));
    float s1  = ofNoise(
                       ofVec3f(
                            x.y + ofRandom(10.0),
                            x.z + ofRandom(10.0),
                            x.x + ofRandom(10.0)
                            )
                       );
    float s2  = ofNoise(
                       ofVec3f(
                            x.z + ofRandom(10.0),
                            x.x + ofRandom(10.0),
                            x.y + ofRandom(10.0)
                            )
                       );
    return ofVec3f( s , s1 , s2 );
}

ofVec3f curlNoise( ofVec3f p ){
    const float e = 0.0009765625;
    const float e2 = 2.0 * e;
    
    ofVec3f dx = ofVec3f( e   , 0.0 , 0.0 );
    ofVec3f dy = ofVec3f( 0.0 , e   , 0.0 );
    ofVec3f dz = ofVec3f( 0.0 , 0.0 , e   );
    
    ofVec3f p_x0 = snoiseVec3( p - dx );
    ofVec3f p_x1 = snoiseVec3( p + dx );
    ofVec3f p_y0 = snoiseVec3( p - dy );
    ofVec3f p_y1 = snoiseVec3( p + dy );
    ofVec3f p_z0 = snoiseVec3( p - dz );
    ofVec3f p_z1 = snoiseVec3( p + dz );
    
    float x = p_y1.z - p_y0.z - p_z1.y + p_z0.y;
    float y = p_z1.x - p_z0.x - p_x1.z + p_x0.z;
    float z = p_x1.y - p_x0.y - p_y1.x + p_y0.x;
    
    ofVec3f out = ofVec3f( x , y , z );
    out /= e2;
    
    return out.normalize();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Setup the fps and time.
    auto fps = ceil(ofGetFrameRate());
    time = ofGetElapsedTimef();
    ofSetWindowTitle("fps : "+ofToString(fps)+"  time : "+ofToString(ceil(time)));
    
    // Generate the particle then store the current time in the fourth dimension.
    for (int i = 0; i < 30; ++i)
        parPos.push_back(ofVec4f(0.0,0.0,0.0, time));
    
    // Generate the velocity.
    for (int i = 0; i < 30; ++i)
        parVel.push_back(generate_random_velocity());
    
    // Calculate speed.
//    for (auto i = 0; i < parPos.size(); i++){
//        parVel[i].x += ofMap(ofNoise(parPos[i].x), 0.0, 1.0, -1.0, 1.0);
//        parVel[i].y += ofMap(ofNoise(parPos[i].y), 0.0, 1.0, -1.0, 1.0);
//        parVel[i].z += ofMap(ofNoise(parPos[i].z), 0.0, 1.0, -1.0, 1.0);
//    }
    for (int i = 0; i < parPos.size(); i++){
        //parVel[i].x += curlNoise(parPos[i]).x / 5.0;
        parVel[i].y += ofMap(curlNoise(parPos[i]).y, -1.0, 1.0, 0.0, 1.0)/10.0;
        //parVel[i].z += ofMap(curlNoise(parPos[i]).z, -1.0, 1.0, 0.0, 1.0)/10.0;
    }
    
    // Add.
    for (int i = 0; i < parPos.size(); i++){
        parPos[i] += parVel[i];
    }
    
    // Delete if survial time is passed.
    for (int i = 0; i < parPos.size(); i++){
        if (time - parPos[i].w > 3.0){
            parPos.erase(parPos.begin() + i);
            parVel.erase(parVel.begin() + i);
        }
    }
}



//--------------------------------------------------------------
void ofApp::draw(){
    
    cam.begin();
    
    // Draw mesh.
    mesh.clear();
    for (auto& pos : parPos){
        mesh.addVertex(pos);
        ofColor hsb;
        hsb.setHsb(ofMap(time - pos.w, 0.0, 3.0, 0.0 ,255), 255, 255);
        mesh.addColor(hsb);
    }
    ofPushMatrix();
    ofTranslate(0, -400);
    mesh.draw();
    ofPopMatrix();
    
    cam.end();
    
    ofDrawSphere(mouseX, mouseY, 0.0, 30.0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
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
