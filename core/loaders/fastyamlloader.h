//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      core/loaders/fastyamlloader.h
//! @brief     Defines loader functions in namespace load.
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#ifndef FASTYAMLLOADER_H
#define FASTYAMLLOADER_H

#include "core/raw/rawfile.h"
#include "core/typ/exception.h"
#include<yaml.h>
#include<map>
#include <QVariant>
#include <QMap>

//! Functions loadRawfile and loadComment, and their dependences.

namespace load {

class YamlNode {
public:
    YamlNode(MapType&& map) : map_(&std::move(map))
    {

    }

    YamlNode(SequenceType&& sequence) : sequence_(&std::move(sequence))
    {

    }

    YamlNode(ScalarType&& scalar) : sequence_(std::move(scalar))
    {

    }

    ~YamlNode()
    {
        if (map_) delete(map_);
        if (sequence_) delete(sequence_);
    }

    inline const ScalarType& value() {
        return getScalar();
    }

    inline const YamlNode& operator[](const KeyType& key) const
    {
        return getMap().find(key)*;
    }

    inline const YamlNode& operator[](const int& index) const
    {
        return getSequence().at(index);
    }

    inline SequenceType::iterator begin()
    {
        switch (nodeType_) {
        case MAP:
            return getMap().begin();
            break;
        case SEQUENCE:
            return getSequence().begin();
            break;
        case SCALAR:
            THROW("node doesn't have an iterator")
        }
    }

    inline SequenceType::iterator end()
    {
        switch (nodeType_) {
        case MAP:
            return getMap().end();
            break;
        case SEQUENCE:
            return getSequence().end();
            break;
        case SCALAR:
            THROW("node doesn't have an iterator")
        }
    }

    inline SequenceType::const_iterator cbegin()
    {
        switch (nodeType_) {
        case MAP:
            return getMap().cbegin();
            break;
        case SEQUENCE:
            return getSequence().cbegin();
            break;
        case SCALAR:
            THROW("node doesn't have an iterator")
        }
    }

    inline SequenceType::const_iterator cend()
    {
        switch (nodeType_) {
        case MAP:
            return getMap().cend();
            break;
        case SEQUENCE:
            return getSequence().cend();
            break;
        case SCALAR:
            THROW("node doesn't have an iterator")
        }
    }

    inline const eNodeType nodeType() { return nodeType_; }

    friend const ParsingResult parseYamlFast(yaml_parser_t *parser, yaml_event_t *event);

    typedef std::map<KeyType, YamlNode> MapType;
    typedef std::vector<YamlNode> SequenceType;
    typedef std::string ScalarType;
    typedef std::string KeyType;

    enum class eNodeType {
        MAP,
        SEQUENCE,
        SCALAR,
    };
private:

    eNodeType nodeType_;
    MapType*const map_ = nullptr;
    std::vector<YamlNode>*const sequence_ = nullptr;
    ScalarType const scalar_ = "";

    inline const MapType& getMap() { return *map_; }
    inline const SequenceType& getSequence() { return *sequence_; }
    inline const ScalarType& getScalar() { return scalar_; }
};

const YamlNode& loadYamlFast(const Qstring& filePath) {
    FILE *fh = fopen("config/public.yaml", "r");
      yaml_parser_t parser;
      yaml_event_t  event;   /* New variable */

      /* Initialize parser */
      if(!yaml_parser_initialize(&parser))
        THROW("Failed to initialize parser!");
      if(fh == NULL)
        THROW("Failed to open file!");

      /* Set input file */
      yaml_parser_set_input_file(&parser, fh);

      /* START new code */
      do {
        if (!yaml_parser_parse(&parser, &event)) {
           THROW("Parser error %d" % parser.error);
           exit(EXIT_FAILURE);
        }

        switch(event.type)
        {
        case YAML_NO_EVENT: puts("No event!"); break;
        /* Stream start/end */
        case YAML_STREAM_START_EVENT: puts("STREAM START"); break;
        case YAML_STREAM_END_EVENT:   puts("STREAM END");   break;
        /* Block delimeters */
        case YAML_DOCUMENT_START_EVENT: puts("<b>Start Document</b>"); break;
        case YAML_DOCUMENT_END_EVENT:   puts("<b>End Document</b>");   break;
        case YAML_SEQUENCE_START_EVENT: puts("<b>Start Sequence</b>"); break;
        case YAML_SEQUENCE_END_EVENT:   puts("<b>End Sequence</b>");   break;
        case YAML_MAPPING_START_EVENT:  puts("<b>Start Mapping</b>");  break;
        case YAML_MAPPING_END_EVENT:    puts("<b>End Mapping</b>");    break;
        /* Data */
        case YAML_ALIAS_EVENT:  printf("Got alias (anchor %s)\n", event.data.alias.anchor); break;
        case YAML_SCALAR_EVENT: printf("Got scalar (value %s)\n", event.data.scalar.value); break;
        }
        if(event.type != YAML_STREAM_END_EVENT)
          yaml_event_delete(&event);
      } while(event.type != YAML_STREAM_END_EVENT);
      yaml_event_delete(&event);
      /* END new code */

      /* Cleanup */
      yaml_parser_delete(&parser);
      fclose(fh);
      return 0;


}

struct ParsingResult {
    ParsingResult(const YamlNode& _Node) :
        node(_Node), isEnd(false) { }
    ParsingResult(const bool _isEnd) :
        node(nullptr), isEnd(_isEnd) { }
    const YamlNode& node;
    const bool isEnd;
};

struct EventContainer {
    EventDeleter() : event_(){ }
    EventDeleter(yaml_event_t& event) : event_(event){ }
    ~EventDeleter() { yaml_event_delete(&event_); }
    yaml_event_t event_;
};

const ParsingResult parseYamlFast(yaml_parser_t *parser) {
    EventContainer event;

    if (!yaml_parser_parse(parser, &event.event)) {
       THROW("Parser error %d" % parser.error);
    }
    switch(event.type)
    {
    case YAML_NO_EVENT: puts("No event!"); break;
    /* Stream start/end */
    case YAML_STREAM_START_EVENT: puts("STREAM START"); break;
    case YAML_STREAM_END_EVENT:   puts("STREAM END");   break;
    /* Block delimeters */
    case YAML_DOCUMENT_START_EVENT: puts("<b>Start Document</b>"); break;
    case YAML_DOCUMENT_END_EVENT:   puts("<b>End Document</b>");   break;
    case YAML_SEQUENCE_START_EVENT: puts("<b>Start Sequence</b>"); {
        YamlNode node(new YamlNode::SequenceType());
        YamlNode::SequenceTyp& sequence = node.getSequence();
        for (auto parsResult = parseYamlFast(parser); !parsResult.isEnd; parsResult = parseYamlFast(parser)) {
            sequence.push_back(parsResult.node);
        }
        return ParsingResult(node);
    }
    break;
    case YAML_SEQUENCE_END_EVENT:   puts("<b>End Sequence</b>");
        return ParsingResult(true);
    case YAML_MAPPING_START_EVENT:  puts("<b>Start Mapping</b>"); {
        YamlNode node(new YamlNode::SequenceType());
        YamlNode::MapType& map = node.getMap();
        for (auto parsResult = parseYamlFast(parser); !parsResult.isEnd; parsResult = parseYamlFast(parser)) {
            map[parsResult.node.value()] = parseYamlFast(parser).node;
        }
        return ParsingResult(node);
    }
    break;
    case YAML_MAPPING_END_EVENT:    puts("<b>End Mapping</b>");
        return ParsingResult(true);
    /* Data */
    case YAML_ALIAS_EVENT:  THROW("Got alias (anchor %s)\n" % event.data.alias.anchor); break;
    case YAML_SCALAR_EVENT:
        ParsingResult(YamlNode(event.event_.data.scalar.value));
        break;
    }
    yaml_event_delete(&event);
  /* END new code */


}



//! load a file; file type will be sensed
Rawfile loadRawfile(const QString& filePath);

QString loadComment(const QFileInfo& info);

} // namespace load

#endif // FASTYAMLLOADER_H
