//
//  ofxLINEEYE.h
//  ofxLINEEYE_example
//
//  Created by Motoi Ishibashi on 2018/02/14.
//
//

/* <usage>
 
 ofxLINEEYE lineeye;
 lineeye.setup("192.168.7.180");

 //0番portをON, portは0から4の5ポート
 lineeye.setState(0, true);

 //0番portをOFF
 lineeye.setState(0, false);
*/


#ifndef ofxLINEEYE_h
#define ofxLINEEYE_h

#include "ofMain.h"
#include "ofxNetwork.h"

class ofxLINEEYE : public ofThread
{
public:
    ofxLINEEYE()
    {
        
    }
    
    ~ofxLINEEYE()
    {
        stop();
        waitForThread();
        client.close();
    }
    
    void setup(string _host, int _port = 10003)
    {
        host = _host;
        port = _port;
        ofLogVerbose() << "ofxLINEEYE : Open LINEEYE device : " << host << " " << port;
        
        client.setup(host, port);
        char buf[2] = {(char)0xE0, (char)0x00};
        
        client.sendRawBytes(buf, 1);
       
        
        start();
    }
    
    unsigned char getIOStates()
    {
        return IOState;
    }
    
    void setState(int index, bool state){
        
        //cout << index << endl;
        
        if( 0 > index || index >=5){
            return;
        }
        
        else{
            
            lock();
            
            if(!client.isConnected()){
                ofLogVerbose() << "ofxLINEEYE : reconnect LINEEYE";
                client.setup(host, port);
            }
            
            char buf[] = {(char)0xF0, (char)0x00};
            if(state == true){
                buf[1] = (0x01 << index);
                buf[1] |= IOState;
            }
            else{
                if( ((IOState >> index) & 0x01) == 0x01 ){
                    buf[1] = (0x01 << index);
                    buf[1] ^= IOState;
                }
            }
            
            //printf("%02x %02x\n", buf[1], IOState);
            //IOState = buf[1];
            client.sendRawBytes(buf, 2);
            //client.sendRawBytes(buf, 2);
            
            unlock();
            
            //ofSleepMillis(10);
        }
        
    }
    
    bool getState(int index){
        if( 0 > index || index >= 5){
            return false;
        }
        else{
            return ( ((IOState >> index) & 0x01)  > 0) ? true : false;
        }
    }
    
private:
    
    void start()
    {
        startThread();
    }
    
    void stop()
    {
        stopThread();
    }
    
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            
            lock();
            
            if(!client.isConnected()){
                ofLogVerbose() << "rofxLINEEYE : econnect LINEEYE";
                client.setup(host, port);
            }
            
            
            
            char buf[2];
            //client.sendRawBytes(buf, 1);

            
            
            if(client.receiveRawBytes(buf, 2) > 0){
                IOState = (unsigned char)buf[1];
                //printf("%02x\n", IOState);
                
                //cout << str << endl;
            
                //unlock();
            }
            
            unlock();
            ofSleepMillis(2);
        }
    }
    
    ofxTCPClient client;
    unsigned char IOState;
    string host;
    int port;
                
    
    
    
};

#endif /* ofxLINEEYE_h */
