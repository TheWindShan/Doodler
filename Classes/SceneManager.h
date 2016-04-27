//
//  SceneManager.h
//  Doodler
//
//  Created by AimSpike on 4/27/16.
//
//

#ifndef SceneManager_h
#define SceneManager_h

#include "DrawingCanvas.h"
#include "NetworkingWrapper.h"

class SceneManager : public NetworkingDelegate
{

private:

    SceneManager();
    ~SceneManager();

    DrawingCanvas* drawingCanvas;
    NetworkingWrapper* networkingWrapper;
    
    void receivedData(const void* data, unsigned long length) override;
    void stateChanged(ConnectionState state) override;

    void loadDrawingScene(bool networked);

public:
    
    static SceneManager* getInstance();
    
    void enterSoloGame();
    void returnToLobby();
    
    void sendData(const void* data, unsigned long length);
    void connectAndEnterNetworkedGame();

};


#endif /* SceneManager_hpp */
