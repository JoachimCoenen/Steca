//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      core/loaders/fastyamlloader.cpp
//! @brief     Implements loader functions.
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#include "fastyamlloader.h"
#include <QStringBuilder> // for ".." % ..
#include <QMap>
#include <sstream>

namespace loadYAML {

YamlNode&& parseYamlFast(YamlParserType parser, YamlNode&& node) {
    /*
    //Container<yaml_event_t, void, &yaml_event_delete> event = Container<yaml_event_t, void, &yaml_event_delete>(yaml_event_t());
    yaml_event_t event;

    if (!yaml_parser_parse(parser.get(), &event)) {
       THROW(QString::fromStdString("Parser error " + std::to_string((*parser).error)));
    }

    switch((event).type)
    {
    case YAML_NO_EVENT:
        yaml_event_delete(&event);
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_NO_EVENT");
        break;
    case YAML_STREAM_START_EVENT:
        yaml_event_delete(&event);
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_STREAM_START_EVENT");
        return parseYamlFast(parser, std::move(node));
        break;
    case YAML_STREAM_END_EVENT:
        yaml_event_delete(&event);
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_STREAM_END_EVENT");
        node.isEnd = true;
        return std::move(node);
        break;
    case YAML_DOCUMENT_START_EVENT:
        yaml_event_delete(&event);
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_DOCUMENT_START_EVENT");
        return parseYamlFast(parser, std::move(node));
        break;
    case YAML_DOCUMENT_END_EVENT:
        yaml_event_delete(&event);
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_DOCUMENT_END_EVENT");
        node.isEnd = true;
        return std::move(node);
        break;
    case YAML_SEQUENCE_START_EVENT: {
        yaml_event_delete(&event);
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_SEQUENCE_START_EVENT");
        node.nodeType_ = YamlNode::eNodeType::SEQUENCE;
        YamlNode::SequenceType& sequence = node.getSequence();

        do {
            auto&& innnerNode = parseYamlFast(parser, YamlNode());

            if (innnerNode.isEnd)
                break;
            sequence.push_back(std::move(innnerNode));
        } while (true);

        //for (auto&& innnerNode = parseYamlFast(parser, YamlNode()); !innnerNode.isEnd; innnerNode = parseYamlFast(parser, YamlNode())) {
        //    sequence.push_back(std::move(innnerNode));
        //}
        return std::move(node);
    }
    break;
    case YAML_SEQUENCE_END_EVENT:
        yaml_event_delete(&event);
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_SEQUENCE_END_EVENT");
        node.isEnd = true;
        return std::move(node);
        break;
    case YAML_MAPPING_START_EVENT: {
        yaml_event_delete(&event);
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_MAPPING_START_EVENT");
        node.nodeType_ = YamlNode::eNodeType::MAP;
        YamlNode::MapType& map = node.getMap();

        for (auto key = parseYamlFast(parser, YamlNode()); !key.isEnd; key = parseYamlFast(parser, YamlNode())) {
            DEBUG_OUT_TEMP("DEBUG[parseYamlFast] key.value() == " << key.value());
            //map.insert({key.value(), value});
            map.insert(key.value(), parseYamlFast(parser, YamlNode()));
        }
        return std::move(node);
    }
    break;
    case YAML_MAPPING_END_EVENT:
        yaml_event_delete(&event);
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_MAPPING_END_EVENT");
        node.isEnd = true;
        return std::move(node);
        break;
    case YAML_ALIAS_EVENT:
        yaml_event_delete(&event);
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_ALIAS_EVENT");
        THROW(QString("Got alias (anchor %s)"));
        break;
    case YAML_SCALAR_EVENT:
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast] YAML_SCALAR_EVENT = " << QString::fromLatin1((char*)event.data.scalar.value));
        node.nodeType_ = YamlNode::eNodeType::SCALAR;
        node.scalar_ = QString::fromLatin1((char*)event.data.scalar.value);
        yaml_event_delete(&event);
        return std::move(node);
        break;
    }
    DEBUG_OUT_TEMP("DEBUG[load_yaml] after switch");
*/
}


yaml_event_type_t parser_parse(YamlParserType parser, yaml_event_t& event) {
    if (!yaml_parser_parse(parser.get(), &event)) {
       THROW(QString::fromStdString("Parser error " + std::to_string((*parser).error)));
    }
    return event.type;
}

YamlNode parseYamlFast2(YamlParserType parser, const yaml_event_t& prevEvent) {
    //Container<yaml_event_t, void, &yaml_event_delete> event = Container<yaml_event_t, void, &yaml_event_delete>(yaml_event_t());
    //yaml_event_t event;



    switch(prevEvent.type)
    {
    case YAML_NO_EVENT:
        THROW("DEBUG[parseYamlFast2] YAML_NO_EVENT");
        break;
    case YAML_STREAM_START_EVENT: {
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast2] YAML_STREAM_START_EVENT");
        yaml_event_t event;
        parser_parse(parser, event);
        return parseYamlFast2(parser, event);
    }
    break;
    case YAML_STREAM_END_EVENT:
        THROW("DEBUG[parseYamlFast2] YAML_STREAM_END_EVENT");
        break;
    case YAML_DOCUMENT_START_EVENT: {
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast2] YAML_DOCUMENT_START_EVENT");
        yaml_event_t event;
        parser_parse(parser, event);
        return parseYamlFast2(parser, event);
    }
    break;
    case YAML_DOCUMENT_END_EVENT:
        THROW("DEBUG[parseYamlFast2] YAML_DOCUMENT_END_EVENT");
        break;
    case YAML_SEQUENCE_START_EVENT: {
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast2] YAML_SEQUENCE_START_EVENT");
        //node.nodeType_ = YamlNode::eNodeType::SEQUENCE;
        YamlNode node = YamlNode(new YamlNode::SequenceType());
        YamlNode::SequenceType& sequence = node.getSequence();

        yaml_event_t event;
        while(YAML_SEQUENCE_END_EVENT != parser_parse(parser, event)) {
            sequence.push_back(parseYamlFast2(parser, event));
            yaml_event_delete(&event);
        };
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast2] YAML_SEQUENCE_END_EVENT");

        //for (auto&& innnerNode = parseYamlFast(parser, YamlNode()); !innnerNode.isEnd; innnerNode = parseYamlFast(parser, YamlNode())) {
        //    sequence.push_back(std::move(innnerNode));
        //}
        yaml_event_delete(&event);
        return node;//YamlNode(sequence);
    }
    break;
    case YAML_SEQUENCE_END_EVENT:
        THROW("DEBUG[parseYamlFast2] YAML_SEQUENCE_END_EVENT -- BAD");
        break;
    case YAML_MAPPING_START_EVENT: {
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast2] YAML_MAPPING_START_EVENT");
        //node.nodeType_ = YamlNode::eNodeType::MAP;
        YamlNode node = YamlNode(new YamlNode::MapType());
        YamlNode::MapType& map = node.getMap();

        yaml_event_t event;
        while(YAML_MAPPING_END_EVENT != parser_parse(parser, event)) {
            QString key = QString::fromLatin1((char*)event.data.scalar.value);
            DEBUG_OUT_TEMP("DEBUG[parseYamlFast2] key == " << key);
            yaml_event_delete(&event);
            parser_parse(parser, event);
            map.insert(key, parseYamlFast2(parser, event));
            yaml_event_delete(&event);
        }

        //for (auto key = parseYamlFast(parser, YamlNode()); !key.isEnd; key = parseYamlFast(parser, YamlNode())) {
        //    DEBUG_OUT_TEMP("DEBUG[parseYamlFast] key.value() == " << key.value());
        //    //map.insert({key.value(), value});
        //    map.insert(key.value(), parseYamlFast(parser, YamlNode()));
        //}
        yaml_event_delete(&event);
        return node;
    }
    break;
    case YAML_MAPPING_END_EVENT:
        THROW("DEBUG[parseYamlFast2] YAML_MAPPING_END_EVENT");
        break;
    case YAML_ALIAS_EVENT:
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast2] YAML_ALIAS_EVENT");
        THROW(QString("Got alias (anchor %s)"));
        break;
    case YAML_SCALAR_EVENT:
        DEBUG_OUT_TEMP("DEBUG[parseYamlFast2] YAML_SCALAR_EVENT = " << QString::fromLatin1((char*)prevEvent.data.scalar.value));
        //node.nodeType_ = YamlNode::eNodeType::SCALAR;
        if ((char*)prevEvent.data.scalar.tag
                && std::string((char*)prevEvent.data.scalar.tag) == "!array2d") {
          std::shared_ptr<std::vector<float>> vec(new std::vector<float>);

          std::stringstream arrayStr((char*)prevEvent.data.scalar.value);
          int width; arrayStr >> width;
          int height; arrayStr >> height;
          for (int i = 0; i < width*height; i++) {
              int v;
              arrayStr >> v;
              vec->push_back(v);
              //vect.push_back(atoi((char*)event_.data.scalar.value));
          }
          return YamlNode(vec);
        }
        return YamlNode(QString::fromLatin1((char*)prevEvent.data.scalar.value));
        break;
    }
    //DEBUG_OUT_TEMP("DEBUG[parseYamlFast2] after switch");

}

const YamlNode loadYamlFast(const std::string& filePath) {
    FILEContainer file(fopen(filePath.c_str(), "r"));
    YamlParserType parser( new yaml_parser_t());

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
    parser_parse(parser, event);
    return parseYamlFast2(parser, event);
    yaml_event_delete(&event);
    // qDebug() << "DEBUG[load_yaml] after parseYamlFast";
    yaml_parser_delete(parser.get());
}

} // namespace load
