#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QtCore/qlist.h>
#include <QtCore/qfile.h>
#include <qcontainerfwd.h>
#include "nlohmann/json.hpp"

struct fanProfile {
    QString name;
    int inUse[2];//c,g
    int MTconfig[2][4];
    int TStempList[2][10];
    int TSspeedList[2][10];
};

struct commandEntry {
    QString name;
    QString content;
};

class ConfigManager {
public:
    int profileCount;
    int commandCount;
    QList<fanProfile> fanProfiles;
    QList<commandEntry> commands;
    int profileInUse;
    bool useStaticSpeed;
    int staticSpeed[2];
    bool useSpeedLimit;
    int speedLimit[2];
    int timeIntervals[2];
    bool useClevoAuto;
    bool maxSpeed;
    bool monitorGpu;
    bool gpuAutoDetectEnabled;
    QString gpuSysDir;
    QString gpuDevDir;

    ConfigManager();
    void readFromJson();
    void saveToJson();
    void createConfigJson();

private:
    QFile configFile;
    nlohmann::json configJson;
    const QString configFileName="config.json";
};

#endif
