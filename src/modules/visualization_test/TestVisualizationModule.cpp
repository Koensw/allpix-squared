/**
 * @author Koen Wolters <koen.wolters@cern.ch>
 */

#include "TestVisualizationModule.hpp"

#include "G4RunManager.hh"
#include "G4UIsession.hh"
#include "G4UIterminal.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

#include "../../core/AllPix.hpp"
#include "../../core/utils/log.h"

using namespace allpix;

const std::string TestVisualizationModule::name = "visualization_test";

TestVisualizationModule::TestVisualizationModule(AllPix *apx, ModuleIdentifier id, Configuration config): Module(apx, id) {
    config_ = config;
}
TestVisualizationModule::~TestVisualizationModule() {}

void TestVisualizationModule::init() {
    LOG(INFO) << "INITIALIZING VISUALIZATION";
    
    //suppress all geant4 output
    SUPPRESS_STREAM(G4cout);
    
    //initialize the session and the visualization manager
    vis_manager_g4_ = std::make_shared<G4VisExecutive>("quiet");
    vis_manager_g4_->Initialize();
    
    //suppress all geant4 output
    
    // execute standard commands
    //FIXME: should execute this directly and not through the UI
    G4UImanager *UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/vis/scene/create");
    UI->ApplyCommand("/vis/sceneHandler/create "+config_.get<std::string>("driver"));
    UI->ApplyCommand("/vis/sceneHandler/attach");
    
    UI->ApplyCommand("/vis/viewer/create");
    
    RELEASE_STREAM(G4cout);
    
    // execute initialization macro if provided
    if(config_.has("macro_init")){
        UI->ApplyCommand("/control/execute "+config_.get<std::string>("macro_init"));
    }
}

// run the deposition
void TestVisualizationModule::run() {
    LOG(INFO) << "VISUALIZING RESULT";

    if(config_.has("macro_run")){    
        G4UImanager *UI = G4UImanager::GetUIpointer();
        UI->ApplyCommand("/control/execute "+config_.get<std::string>("macro_run"));
    }
    
    if(config_.get("interactive", 0)){
        std::unique_ptr<G4UIsession> session = std::make_unique<G4UIterminal>();
        session->SessionStart();
    }else vis_manager_g4_->GetCurrentViewer()->ShowView();
    
    LOG(INFO) << "END VISUALIZATION";
}

