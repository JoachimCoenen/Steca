//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      core/loaders/load_yaml.cpp
//! @brief     Implements function loadYaml
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#include "fastyamlloader.h"
#include "core/raw/rawfile.h"
#include "qcr/engine/debug.h"
#include "yaml-cpp/include/yaml-cpp/yaml.h"
#include <fstream>
#include<memory>
#include "core/typ/json.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringBuilder> // for ".." % ..

namespace  {

void readInstrument(const YAML::Node& node, Rawfile& rawfile)
{
    if (!node.IsDefined())
        return;

    // const auto name       = node["name"].as<std::string>("");
    // const auto operators  = node["operators"].as<std::vector<std::string>>();
    // const auto facility   = node["facility"].as<std::string>("");
    // const auto website    = node["website"].as<std::string>("");
    // const auto references = node["references"].as<std::vector<std::string>>();
}

void readFormat(const YAML::Node& node, Rawfile& rawfile)
{
    if (!node.IsDefined())
        return;

    // const auto identifier = node["identifier"].as<std::string>("");
      // units = name: unit. eg: time: second, or clearance: millimeter:
    // const auto units  = node["units"].as<std::map<std::string, std::string>>();
}

void readExperiment(const YAML::Node& node, Rawfile& rawfile)
{
    if (!node.IsDefined())
        return;

    // const auto number = node["number"].as<std::string>("");
    // const auto proposal = node["proposal"].as<std::string>("");
    // const auto title = node["title"].as<std::string>("");
    // const auto remark = node["remark"].as<std::string>("");
}

void readSample(const YAML::Node& node, Metadata& metadata)
{
    metadata.motorXT = node["position"]["xt"]["value"].as<double>(Q_QNAN);
    metadata.motorYT = node["position"]["yt"]["value"].as<double>(Q_QNAN);
    metadata.motorZT = node["position"]["zt"]["value"].as<double>(Q_QNAN);
    metadata.motorOmg = node["orientation"]["omgs"]["value"].as<double>(Q_QNAN);
    metadata.motorTth = node["orientation"]["tths"]["value"].as<double>(Q_QNAN);
    metadata.motorPhi = node["orientation"]["phis"]["value"].as<double>(Q_QNAN);
    metadata.motorChi = node["orientation"]["chis"]["value"].as<double>(Q_QNAN);
}

void readSetup(const YAML::Node& node, Metadata& metadata)
{
    metadata.motorPST = Q_QNAN; // node["orientation"]["xt"]["value"].as<double>(Q_QNAN);
    metadata.motorSST = Q_QNAN; // node["orientation"]["xt"]["value"].as<double>(Q_QNAN);
    metadata.motorOMGM = node["monochromator"]["omgm"]["value"].as<double>(Q_QNAN);
    metadata.nmT = Q_QNAN;
    metadata.nmTeload = Q_QNAN;
    metadata.nmTepos = Q_QNAN;
    metadata.nmTeext = Q_QNAN;
    metadata.nmXe = Q_QNAN;
    metadata.nmYe = Q_QNAN;
    metadata.nmZe = Q_QNAN;
}

void readSingleScan(const YAML::Node& node, Metadata& metadata, Rawfile& rawfile)
{
    if (!node.IsDefined())
        return;

    metadata.time = node["time"].as<double>(Q_QNAN);
    metadata.monitorCount = node["monitor"].as<double>(Q_QNAN);
    const auto sum = node["sum"].as<double>(Q_QNAN);
    const auto imageNode = node["image"];
    const auto dimensionsNode = node["dimensions"];

    const size2d size(dimensionsNode["width"].as<int>(), dimensionsNode["height"].as<int>());

    std::vector<float> image;
    // fill image row after row...:
    qDebug() << "DEBUG[load_yaml] before read scan";
    std::stringstream imageStr(imageNode.as<std::string>());
    //for(const auto& lineNode: imageNode)
    {
    //    std::stringstream imageStr(lineNode.as<std::string>());
        while (!imageStr.eof()) {
            int v;
            imageStr >> v;
            image.push_back(v);
        }
    }
    qDebug() << "DEBUG[load_yaml] after read scan";

    rawfile.addDataset(std::move(metadata), size, std::move(image));
}

void readSingleScanOLD(const YAML::Node& node, Metadata& metadata, Rawfile& rawfile)
{
    if (!node.IsDefined())
        return;

    metadata.time = node["time"].as<double>(Q_QNAN);
    metadata.monitorCount = node["monitor"].as<double>(Q_QNAN);
    const auto sum = node["sum"].as<double>(Q_QNAN);
    const auto imageNode = node["image"];

    const size2d size(imageNode[0].size(), imageNode.size());

    qDebug() << "DEBUG[load_yaml] before read scan";
    std::vector<float> image;
    // fill image row after row...:
    for (const auto& rowNode: imageNode) {
        for (const auto& pixelNode: rowNode) {
            image.push_back(pixelNode.as<float>());
        }
    }

    qDebug() << "DEBUG[load_yaml] after read scan";

    rawfile.addDataset(std::move(metadata), size, std::move(image));
}

void readScans(const YAML::Node& node, Metadata& metadata, Rawfile& rawfile)
{
    if (!node.IsDefined())
        return;
    if (!node.IsSequence()) {
        THROW("invalid YAML format: 'scans' should be a list, but isn't.")
    }
    for (const YAML::Node& innerNode: node) {
        Metadata metadataCopy(std::move(metadata));
        // Copy the QStrings back, because std::move removes them from metadata:
        metadata.date    = metadataCopy.date;
        metadata.comment = metadataCopy.comment;
        readSingleScanOLD(innerNode, metadataCopy, rawfile);
    }
}

void readMeasurement(const YAML::Node& node, Rawfile& rawfile)
{
    if (!node.IsDefined())
        return;

    // const auto unique_identifier = node["unique_identifier"].as<std::string>("");
    // const auto number = node["number"].as<std::string>(""); // Integer maybe?

    auto metadata = Metadata();

    metadata.date    = QString::fromStdString(node["history"]["started"].as<std::string>(""));
    metadata.comment = QString::fromStdString(node["history"]["scan"].as<std::string>(""));

    readSample(node["sample"], metadata);
    readSetup(node["setup"], metadata);
    readScans(node["scan"], metadata, rawfile); // adds the scans to the rawfile
}

} // namespace


namespace load {


Rawfile loadYamlREAL(const QString& filePath)
{
    try {
        qDebug() << "DEBUG[load_yaml] before load file";
        YAML::Node yamlFile = YAML::LoadFile(filePath.toStdString()); // throws: ParserException, BadFile;
        qDebug() << "DEBUG[load_yaml] after load file";

        Rawfile rawfile(filePath);
        // readInstrument (yamlFile["instrument"] , rawfile);
        // readFormat     (yamlFile["format"]     , rawfile);
        // readExperiment (yamlFile["experiment"] , rawfile);
        readMeasurement(yamlFile["measurement"], rawfile);
        return rawfile;
    qDebug() << "DEBUG[load_yaml] done";
    } catch (YAML::Exception e) {
        THROW("Invalid data in file "+filePath+":\n" + e.what());
    }
    // just to avoid compiler warnings:
    return Rawfile("");
}

Rawfile loadJsonStr(const QString& filePath)
{
    try {
        qDebug() << "DEBUG[load_yaml] before load file";

        QFile file(filePath);
        if (!(file.open(QIODevice::ReadOnly | QIODevice::Text))) {
            qWarning() << ("Cannot open file for reading: " % filePath);
            return Rawfile("");
        }

        QJsonParseError parseError;
        QJsonDocument doc(QJsonDocument::fromJson(file.readAll(), &parseError));
        if (!(QJsonParseError::NoError == parseError.error))
            THROW("Error parsing session file");
        JsonObj top(doc.object());
        qDebug() << "DEBUG[load_yaml] after load file";

        Rawfile rawfile(filePath);
        // readInstrument (yamlFile["instrument"] , rawfile);
        // readFormat     (yamlFile["format"]     , rawfile);
        // readExperiment (yamlFile["experiment"] , rawfile);
        const JsonObj& measurement = top.loadObj("measurement");
        const QJsonArray& scans = measurement.loadArr("scan");

        for (const QJsonValue& scanVal: scans) {
            const JsonObj& scan(scanVal.toObject());
            const QString& imageString = scan.loadString("image");
            const JsonObj& dimensionsNode = scan.loadObj("dimensions");

            const size2d size(dimensionsNode.loadInt("width"), dimensionsNode.loadInt("height"));

            std::vector<float> image;
            // fill image row after row...:
            qDebug() << "DEBUG[load_yaml] before read scan";
            std::stringstream imageStr(imageString.toStdString());
            qDebug() << "DEBUG[load_yaml] after imageString.toStdString()";
            while (!imageStr.eof()) {
                int v;
                imageStr >> v;
                image.push_back(v);
            }
            qDebug() << "DEBUG[load_yaml] after read scan";

            rawfile.addDataset(Metadata(), size, std::move(image));
        }

        return rawfile;
    qDebug() << "DEBUG[load_yaml] done";
    } catch (YAML::Exception e) {
        THROW("Invalid data in file "+filePath+":\n" + e.what());
    }
    // just to avoid compiler warnings:
    return Rawfile("");
}

Rawfile loadJsonArr(const QString& filePath)
{
    try {
        qDebug() << "DEBUG[load_yaml] before load file";

        QFile file(filePath);
        if (!(file.open(QIODevice::ReadOnly | QIODevice::Text))) {
            qWarning() << ("Cannot open file for reading: " % filePath);
            return Rawfile("");
        }

        QJsonParseError parseError;
        QJsonDocument doc(QJsonDocument::fromJson(file.readAll(), &parseError));
        if (!(QJsonParseError::NoError == parseError.error))
            THROW("Error parsing session file");
        JsonObj top(doc.object());
        qDebug() << "DEBUG[load_yaml] after load file";

        Rawfile rawfile(filePath);
        // readInstrument (yamlFile["instrument"] , rawfile);
        // readFormat     (yamlFile["format"]     , rawfile);
        // readExperiment (yamlFile["experiment"] , rawfile);
        const JsonObj& measurement = top.loadObj("measurement");
        const QJsonArray& scans = measurement.loadArr("scan");

        for (const QJsonValue& scanVal: scans) {
            const JsonObj& scan(scanVal.toObject());
            const JsonObj& dimensionsNode = scan.loadObj("dimensions");

            const size2d size(dimensionsNode.loadInt("width"), dimensionsNode.loadInt("height"));

            qDebug() << "DEBUG[load_yaml] before read scan";
            const QJsonArray& imageArray = scan.loadArr("image");
            std::vector<float> image;
            // fill image row after row...:
            for (const auto& rowVal: imageArray) {
                for (const auto& pixelNode: rowVal.toArray()) {
                    image.push_back(pixelNode.toDouble(Q_QNAN));
                }
            }
            qDebug() << "DEBUG[load_yaml] after read scan";

            rawfile.addDataset(Metadata(), size, std::move(image));
        }

        return rawfile;
    qDebug() << "DEBUG[load_yaml] done";
    } catch (YAML::Exception e) {
        THROW("Invalid data in file "+filePath+":\n" + e.what());
    }
    // just to avoid compiler warnings:
    return Rawfile("");
}

Rawfile loadText(const QString& filePath)
{
    try {
        qDebug() << "DEBUG[load_yaml] before load file";
        Rawfile rawfile(filePath);

        std::ifstream file( filePath.toStdString() );
        if (file) {

            int w, h;
            file >> w >> h;

            const size2d size(w, h);

            std::vector<float> image;
            qDebug() << "DEBUG[load_yaml] before read scan";
            while(!file.eof()) {
                int v;
                file >> v;
                image.push_back(v);
            }
            qDebug() << "DEBUG[load_yaml] after read scan";
            file.close();

            rawfile.addDataset(Metadata(), size, std::move(image));

        }


        return rawfile;

        qDebug() << "DEBUG[load_yaml] done";
    } catch (YAML::Exception e) {
        THROW("Invalid data in file "+filePath+":\n" + e.what());
    }
    // just to avoid compiler warnings:
    return Rawfile("");
}

Rawfile loadYamlREAL2(const QString& filePath)
{
    try {
        qDebug() << "DEBUG[load_yaml] before load file";
        YAML::Node yamlFile = YAML::LoadFile(filePath.toStdString()); // throws: ParserException, BadFile;
        qDebug() << "DEBUG[load_yaml] after load file";

        const auto imageNode = yamlFile["image"];
        const size2d size(yamlFile["width"].as<int>(), yamlFile["height"].as<int>());

        std::vector<float> image;
        // fill image row after row...:
        qDebug() << "DEBUG[load_yaml] before read scan";
        std::stringstream imageStr(imageNode.as<std::string>());
        while (!imageStr.eof()) {
            int v;
            imageStr >> v;
            image.push_back(v);
        }
        qDebug() << "DEBUG[load_yaml] after read scan";

        Rawfile rawfile(filePath);
        rawfile.addDataset(Metadata(), size, std::move(image));

        return rawfile;
    qDebug() << "DEBUG[load_yaml] done";
    } catch (YAML::Exception e) {
        THROW("Invalid data in file "+filePath+":\n" + e.what());
    }
    // just to avoid compiler warnings:
    return Rawfile("");
}

typedef loadYAML::YamlNode YamlNode;


Rawfile loadYamlFast(const QString& filePath)
{
    try {
        qDebug() << "DEBUG[load_yaml] before load file";
        YamlNode yamlFile(loadYAML::loadYamlFast(filePath.toStdString())); // throws: ParserException, BadFile;
        qDebug() << "DEBUG[load_yaml] after load file";

        YamlNode& scans = yamlFile["measurement"]["scan"];

        \
        Rawfile rawfile(filePath);

        static unsigned int scanCounter = 0;
        for (auto& scan : scans) {
            scanCounter++;
            Metadata metadata;

            metadata.time = scan["time"].value().toDouble();
            metadata.monitorCount = scan["monitor"].value().toDouble();
            auto sum = scan["sum"].value().toDouble();
            auto imageNode = scan["image"];

            const size2d size(imageNode[0].size(), imageNode.size());

            qDebug() << QString("DEBUG[load_yaml] before read scan #%1").arg(scanCounter);
            std::vector<float> image;
            // fill image row after row...:
            for (auto& rowNode: imageNode) {
                for (auto& pixelNode: rowNode) {
                    image.push_back(pixelNode.value().toInt());
                }
            }

            qDebug() << QString("DEBUG[load_yaml] after read scan #%1").arg(scanCounter);

            rawfile.addDataset(std::move(metadata), size, std::move(image));
        }

        return rawfile;
    qDebug() << "DEBUG[load_yaml] done";
    } catch (YAML::Exception e) {
        THROW("Invalid data in file "+filePath+":\n" + e.what());
    }
    // just to avoid compiler warnings:
    return Rawfile("");
}

Rawfile loadYaml(const QString& filePath)
{
    return loadYamlFast(filePath);
}
} // namespace load
