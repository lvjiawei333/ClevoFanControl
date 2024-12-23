#include "ConfigManager.h"

#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>
#include <qlogging.h>

void ConfigManager::readFromJson() {
    qDebug()<<"readFromJson";

    //create if not exist
    if(!configFile.exists())
        createConfigJson();
    
    //read
    configFile.open(QIODevice::ReadOnly);
    std::string jsonStr=configFile.readAll().toStdString();
    configFile.close();
    configJson.clear();
    configJson=nlohmann::json::parse(jsonStr);

    //profiles
    this->profileCount=configJson["profiles"].size();
    for(int i=0;i<this->profileCount;i++) {
        fanProfile curProfile;
        curProfile.name=QString::fromStdString(configJson["profiles"][i][0]);
        for(int j=0;j<2;j++) {
            curProfile.inUse[j]=configJson["profiles"][i][j+1][0];
            for(int k=0;k<4;k++)
                curProfile.MTconfig[j][k]=configJson["profiles"][i][j+1][1][k];
            for(int k=0;k<10;k++) {
                curProfile.TStempList[j][k]=configJson["profiles"][i][j+1][2][0][k];
                curProfile.TSspeedList[j][k]=configJson["profiles"][i][j+1][2][1][k];
            }
        }
        fanProfiles.append(curProfile);
    }

    //commands
    this->commandCount=configJson["commands"].size();
    for(auto &i :configJson["commands"].items()) {
        commandEntry curCommand;
        curCommand.name=i.key().c_str();
        curCommand.content=((std::string)i.value()).c_str();
        commands.append(curCommand);
    }

    //others
    this->profileInUse=configJson["profileInUse"];
    this->useStaticSpeed=configJson["staticSpeed"][0];
    this->staticSpeed[0]=configJson["staticSpeed"][1];
    this->staticSpeed[1]=configJson["staticSpeed"][2];
    this->useSpeedLimit=configJson["speedLimit"][0];
    this->speedLimit[0]=configJson["speedLimit"][1];
    this->speedLimit[1]=configJson["speedLimit"][2];
    this->timeIntervals[0]=configJson["timeIntervals"][0];
    this->timeIntervals[1]=configJson["timeIntervals"][1];
    this->useClevoAuto=configJson["useClevoAuto"];
    this->maxSpeed=configJson["maxSpeed"];
    this->monitorGpu=configJson["monitorGpu"];
    this->gpuAutoDetectEnabled=configJson["gpuAutoDetect"];
    this->gpuSysDir=((std::string)configJson["gpuSysDir"]).c_str();
    this->gpuDevDir=((std::string)configJson["gpuDevDir"]).c_str();

    qDebug()<<"readFromJson finish";
}

void ConfigManager::saveToJson() {
    qDebug()<<"saveConfigJson";

    //profiles
    nlohmann::json profileArray=nlohmann::json::array();
    for(int i=0;i<profileCount;i++)
    {
        nlohmann::json curProfile={
            fanProfiles[i].name.toStdString(),
            {
                fanProfiles[i].inUse[0],
                fanProfiles[i].MTconfig[0],
                {
                    fanProfiles[i].TStempList[0],
                    fanProfiles[i].TSspeedList[0]
                }
            },
            {
                fanProfiles[i].inUse[1],
                fanProfiles[i].MTconfig[1],
                {
                    fanProfiles[i].TStempList[1],
                    fanProfiles[i].TSspeedList[1]
                }
            }
        };
        profileArray+=curProfile;
    }

    //save commands
    nlohmann::json commandsSaved=configJson["commands"];

    //create json
    configJson.clear();
    configJson={
        {"profiles",profileArray},
        {"commands",commandsSaved},
        {"profileInUse",this->profileInUse},
        {"staticSpeed",{this->useStaticSpeed,this->staticSpeed[0],this->staticSpeed[1]}},
        {"speedLimit",{this->useSpeedLimit,speedLimit[0],speedLimit[1]}},
        {"timeIntervals",timeIntervals},
        {"useClevoAuto",this->useClevoAuto},
        {"maxSpeed",this->maxSpeed},
        {"monitorGpu",this->monitorGpu},
        {"gpuAutoDetect",this->gpuAutoDetectEnabled}, 
        {"gpuSysDir",gpuSysDir.toStdString()},
        {"gpuDevDir",gpuDevDir.toStdString()}
    };
    
    //write file
    configFile.open(QIODevice::WriteOnly);
    configFile.write(configJson.dump().c_str());
    configFile.close();

    qDebug()<<"saveConfigJson finish";
}

ConfigManager::ConfigManager() {
    configFile.setFileName((QDir::currentPath() + QDir::separator() + configFileName));
}

void ConfigManager::createConfigJson() {
    //make default profile
    nlohmann::json defaultProfile={
        "default1",//name
        {//cpu fan
            1,//in use
            {80,75,20,5},//mt
            {//ts
                {58, 60, 63, 65, 68, 70, 73, 75, 78, 80},//temp list
                {20, 25, 30, 40, 50, 60, 70, 80, 90, 100}//speed list
            }
         },
         {//gpu fan
            1,//in use
            {80,75,20,5},//mt
            {//ts
                {58, 60, 63, 65, 68, 70, 73, 75, 78, 80},//temp list
                {20, 25, 30, 40, 50, 60, 70, 80, 90, 100}//speed list
            }
         }
    };
    configJson.clear();
    configJson={
    {"profiles",{defaultProfile,defaultProfile}},
        {"commands",{{"CPU-ondemond","cpupower frequency-set -g ondemand"},{"CPU-conservative","cpupower frequency-set -g conservative"}}},
        {"profileInUse",0},
        {"staticSpeed",{false,80,80}},
        {"speedLimit",{false,80,80}},
        {"timeIntervals",{2000,2000}},
        {"useClevoAuto",false},
        {"maxSpeed",false},
        {"monitorGpu",false},
        {"gpuAutoDetect",false}, 
        {"gpuSysDir",""},
        {"gpuDevDir",""}
    };
    configJson["profiles"][1][0]="default2";

    configFile.open(QIODevice::WriteOnly);
    configFile.write(configJson.dump().c_str());
    configFile.close();
}
