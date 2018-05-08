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
#include "deferredyamlloader.h"
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

namespace  {


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

Rawfile loadJsonStrArr(const QString& filePath)
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
            const QJsonArray& imageArray = scan.loadArr("image");
            const JsonObj& dimensionsNode = scan.loadObj("dimensions");

            const size2d size(dimensionsNode.loadInt("width"), dimensionsNode.loadInt("height"));

            std::vector<float> image;
            // fill image row after row...:
            qDebug() << "DEBUG[load_yaml] before read scan";
            for (const QJsonValue& imageLine: imageArray) {
                const QString& imageString = imageLine.toString();
                std::stringstream imageStr(imageString.toStdString());
                while (!imageStr.eof()) {
                    int v;
                    imageStr >> v;
                    image.push_back(v);
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

Rawfile loadYamlFastArr(const QString& filePath)
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
            auto dimensionsNode = scan["dimensions"];

            const size2d size(
                    dimensionsNode["width"].value().toInt(),
                    dimensionsNode["height"].value().toInt()
            );

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

Rawfile loadYamlFastStrArr(const QString& filePath)
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
            auto dimensionsNode = scan["dimensions"];

            const size2d size(
                    dimensionsNode["width"].value().toInt(),
                    dimensionsNode["height"].value().toInt()
            );

            qDebug() << QString("DEBUG[load_yaml] before read scan #%1").arg(scanCounter);
            std::vector<float> image;
            // fill image row after row...:
            for (auto& rowNode: imageNode) {

                std::stringstream imageStr(rowNode.value().toStdString());
                while (!imageStr.eof()) {
                    int v;
                    imageStr >> v;
                    image.push_back(v);
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

Rawfile loadYamlFastStr(const QString& filePath)
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
            auto dimensionsNode = scan["dimensions"];

            const size2d size(
                    dimensionsNode["width"].value().toInt(),
                    dimensionsNode["height"].value().toInt()
            );

            qDebug() << QString("DEBUG[load_yaml] before read scan #%1").arg(scanCounter);
            auto image = (imageNode.getVector());
            // fill image row after row...:
         /*   std::stringstream imageStr(imageNode.value().toStdString());
            while (!imageStr.eof()) {
                int v;
                imageStr >> v;
                image.push_back(v);
            }
        */
            qDebug() << QString("DEBUG[load_yaml] after read scan #%1").arg(scanCounter);

            rawfile.addDataset(std::move(metadata), size, std::move(*image));
        }

        return rawfile;
    qDebug() << "DEBUG[load_yaml] done";
    } catch (YAML::Exception e) {
        THROW("Invalid data in file "+filePath+":\n" + e.what());
    }
    // just to avoid compiler warnings:
    return Rawfile("");
}

typedef loadYAML::YamlIterator::IteratorType IteratorType;

void parseSingleScanDeferred(loadYAML::YamlIterator& scanIterator, Rawfile& rawfile)
{
    static unsigned int scanCounter = 0;

    scanCounter++;
    Metadata metadata;
    size2d size;
    std::vector<float> image;

    qDebug() << QString("DEBUG[loadYamlDeferred] before read scan #%1").arg(scanCounter);
    while(!(++scanIterator).isEnd()) {
        if (scanIterator.getKey() == "time") {
            metadata.time = QString::fromStdString(scanIterator.getValue()).toDouble();
        } else if (scanIterator.getKey() == "monitor") {
            metadata.monitorCount = QString::fromStdString(scanIterator.getValue()).toDouble();
        } else if (scanIterator.getKey() == "dimensions") {
            YamlNode& dimNode = *scanIterator.getCurrentNode();
            size = size2d(dimNode["width"].value().toInt(), dimNode["height"].value().toInt());
        } else if (scanIterator.getKey() == "image") {
            IteratorType imageIterator = scanIterator.getInnerIterator();
            while(!(++*imageIterator).isEnd()) {
                (imageIterator->getInnerIterator())->fillVectorWithInts(image);
                // loadYAML::YamlIterator rowIterator =
                // while(!(++rowIterator).isEnd()) {
                //     image.push_back(std::stoi( rowIterator.getValue() ));
                // }
            }
        } else {
            scanIterator.getCurrentNode();
        }
    }
    qDebug() << QString("DEBUG[loadYamlDeferred] after read scan #%1").arg(scanCounter);
    rawfile.addDataset(std::move(metadata), size, std::move(image));

}

    void parseScansDeferred(loadYAML::YamlIterator& scansIterator, Rawfile& rawfile)
{
    while(!(++scansIterator).isEnd()) {
        parseSingleScanDeferred(*scansIterator.getInnerIterator(), rawfile);
    }

}

void parseMeasurementDeferred(loadYAML::YamlIterator& measurementIterator, Rawfile& rawfile)
{
    static unsigned int scanCounter = 0;

    while(!(++measurementIterator).isEnd()) {
        if (measurementIterator.getKey() == "scan") {
            parseScansDeferred(*measurementIterator.getInnerIterator(), rawfile);
        } else {
            measurementIterator.getCurrentNode();
        }
    }
}

Rawfile loadYamlDeferred(const QString& filePath)
{
    static unsigned int scanCounter = 0;
    try {
        loadYAML::FILEContainer file(fopen(filePath.toLatin1(), "r"));
        loadYAML::YamlParserType parser( new yaml_parser_t());

        /* Initialize parser */
        if(!yaml_parser_initialize(&*parser))
            THROW("Failed to initialize parser!");
        if(*file == nullptr)
            THROW("Failed to open file!");

        /* Set input file */
        yaml_parser_set_input_file(&*parser, *file);
        // qDebug() << "DEBUG[load_yaml] before parseYamlFast";
       // return parseYamlFast(parser, YamlNode());

        yaml_event_t event;
        while (!YAML_MAPPING_START_EVENT == loadYAML::parser_parse(parser, event)) {
            yaml_event_delete(&event);
        }
        yaml_event_delete(&event);
        loadYAML::parser_parse(parser, event);
        yaml_event_delete(&event);
        loadYAML::parser_parse(parser, event);
        yaml_event_delete(&event);

        loadYAML::YamlIterator fileIterator(true, parser);

        Rawfile rawfile(filePath);

        while(!(++fileIterator).isEnd()) {
            if (fileIterator.getKey() == "measurement") {
                parseMeasurementDeferred(*fileIterator.getInnerIterator(), rawfile);
            } else {
                fileIterator.getCurrentNode();
            }
        }

        yaml_parser_delete(parser.get());
        return rawfile;
    } catch (YAML::Exception e) {
        THROW("Invalid data in file "+filePath+":\n" + e.what());
    }
    // just to avoid compiler warnings:
    return Rawfile("");
}

Rawfile loadYamlDeferredStr(const QString& filePath)
{
    static unsigned int scanCounter = 0;
    try {
        loadYAML::FILEContainer file(fopen(filePath.toLatin1(), "r"));
        loadYAML::YamlParserType parser( new yaml_parser_t());

        /* Initialize parser */
        if(!yaml_parser_initialize(&*parser))
            THROW("Failed to initialize parser!");
        if(*file == nullptr)
            THROW("Failed to open file!");

        /* Set input file */
        yaml_parser_set_input_file(&*parser, *file);
        // qDebug() << "DEBUG[load_yaml] before parseYamlFast";
       // return parseYamlFast(parser, YamlNode());

        yaml_event_t event;
        while (!YAML_MAPPING_START_EVENT == loadYAML::parser_parse(parser, event)) {
            yaml_event_delete(&event);
        }
        yaml_event_delete(&event);
        loadYAML::parser_parse(parser, event);
        yaml_event_delete(&event);
        loadYAML::parser_parse(parser, event);
        yaml_event_delete(&event);

        loadYAML::YamlIterator fileIterator(true, parser);

        Rawfile rawfile(filePath);

        while(!(++fileIterator).isEnd()) {
            if (fileIterator.getKey() == "measurement") {
                loadYAML::YamlIterator measurementIterator = *fileIterator.getInnerIterator();
                while(!(++measurementIterator).isEnd()) {
                    if (measurementIterator.getKey() == "scan") {
                        loadYAML::YamlIterator scansIterator = *measurementIterator.getInnerIterator();
                        while(!(++scansIterator).isEnd()) {
                            scanCounter++;
                            Metadata metadata;
                            size2d size;
                            std::vector<float> image;

                            qDebug() << QString("DEBUG[loadYamlDeferred] before read scan #%1").arg(scanCounter);
                            IteratorType scanIterator = scansIterator.getInnerIterator();
                            while(!(++*scanIterator).isEnd()) {
                                if (scanIterator->getKey() == "time") {
                                    metadata.time = QString::fromStdString(scanIterator->getValue()).toDouble();
                                } else if (scanIterator->getKey() == "monitor") {
                                    metadata.monitorCount = QString::fromStdString(scanIterator->getValue()).toDouble();
                                } else if (scanIterator->getKey() == "dimensions") {
                                    YamlNode& dimNode = *scanIterator->getCurrentNode();
                                    size = size2d(dimNode["width"].value().toInt(), dimNode["height"].value().toInt());
                                    if (image.size() == 0) {
                                        image.reserve(size.count());
                                    }
                                } else if (scanIterator->getKey() == "image") {
                                    //IteratorType imageIterator = scanIterator->getInnerIterator();
                                    //++*imageIterator;
                                    scanIterator->fillVectorWithIntsFromScalar(image);
                                } else {
                                    scanIterator->getCurrentNode();
                                }
                            }

                            rawfile.addDataset(std::move(metadata), size, std::move(image));
                        }
                        qDebug() << QString("DEBUG[loadYamlDeferred] after read scan #%1").arg(scanCounter);
                    } else {
                        measurementIterator.getCurrentNode();
                    }
                }
            } else {
                fileIterator.getCurrentNode();
            }
        }

        yaml_parser_delete(parser.get());
        return rawfile;
    } catch (YAML::Exception e) {
        THROW("Invalid data in file "+filePath+":\n" + e.what());
    }
    // just to avoid compiler warnings:
    return Rawfile("");
}

typedef std::function<Rawfile (const QString& filePath)> LoaderFunction;

const std::map<QString, LoaderFunction> loaderFunctions = {
    {"arr.json",    &loadJsonArr},
    {"strarr.json", &loadJsonStrArr},
    {"str.json",    &loadJsonStr},
    {"arr.yaml",    &loadYamlFastArr},
    {"strarr.yaml", &loadYamlFastStrArr},
    {"str.yaml",    &loadYamlFastStr},
    //{"arr2d.yaml",  &loadYamlFastArr2d},
};

}

namespace load {

Rawfile loadYaml(const QString& filePath)
{
    qDebug() << filePath.right(filePath.length()-filePath.lastIndexOf("_")-1);
    auto a = loaderFunctions.at(filePath.right(filePath.length()-filePath.lastIndexOf("_")-1));
    return a(filePath);
    return loadYamlDeferred(filePath);
}
} // namespace load
