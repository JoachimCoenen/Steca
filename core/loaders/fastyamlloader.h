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
#include<memory>
#include <QVariant>
#include <QMap>
#include <QStringBuilder>
#include <QDebug>

//! Functions loadRawfile and loadComment, and their dependences.

namespace loadYAML {

struct Hello {

    Hello() :
        val1(false), val2("") { }

    //Hello(Hello& other) :
    //    val1(other.val1), val2(other.val2) { }

    Hello(bool _val1) :
        val1(_val1), val2("") { }

    Hello(const std::string _val2) :
        val1(false), val2(_val2) { }
    ~Hello() { }

    bool val1;
    std::string val2;
};




template<typename T, typename r, r(*clenupEvent)(T*)>
struct Container {
    Container() : value_(new T()) { }
    Container(T* value) : value_(value) { }
    Container(T&& value) : value_(&value) { }
    ~Container() { clenupEvent(&*value_);}


    T*const getReference() {
        return value_.get();
    }

    T& operator *() {
        if (value_ == nullptr) {
            THROW("value is nullptr, but should be initialized")
        }
        return *value_;
    }
private:
    std::shared_ptr<T> value_;
};

//typedef Container<yaml_parser_t, void, &yaml_parser_delete> YamlParserType;
typedef std::shared_ptr<yaml_parser_t> YamlParserType;

class YamlNode {
public:
    typedef QString KeyType;
    typedef QMap<KeyType, YamlNode> MapType;
    typedef std::vector<YamlNode> SequenceType;
    typedef QString ScalarType;

    enum class eNodeType {
        MAP,
        SEQUENCE,
        SCALAR,
    };
/*
    YamlNode(MapType&& map) : map_(&map)
    {
    }

    YamlNode(SequenceType&& sequence) : sequence_(&sequence)
    {

    }
*/
    YamlNode(ScalarType&& scalar) : scalar_(scalar), nodeType_(eNodeType::SCALAR)
    {

    }

    YamlNode(eNodeType nodeType) : nodeType_(nodeType)
    {

    }

    YamlNode(bool _isEnd) : isEnd(_isEnd)
    {

    }

    YamlNode()
    {

    }

    YamlNode(const YamlNode& other)
        : nodeType_(other.nodeType_),
          map_(other.map_),
          sequence_(other.sequence_),
          scalar_(other.scalar_),
          isEnd(other.isEnd)
    {

    }

    ~YamlNode()
    {
    }

    bool isEnd = false;

    inline bool IsDefined() { return true; } // for now...

    inline ScalarType& value() {
        return getScalar();
    }

    YamlNode& operator= ( const YamlNode& other )
    {
        nodeType_ = other.nodeType_;
        map_ = other.map_;
        sequence_ = other.sequence_;
        scalar_ = other.scalar_;
        isEnd = other.isEnd;
        return *this;
    }

    inline YamlNode& operator[](const KeyType& key)
    {
        //return getMap().find(key)->second;
        return getMap().find(key).value();
    }

    inline YamlNode& operator[](const int& index)
    {
        return getSequence().at(index);
    }

    const int size() const {
        return sequence_.size();
    }
    inline SequenceType::iterator begin()
    {
        switch (nodeType_) {
        case eNodeType::MAP:
            THROW("node(map) doesn't have an iterator")
            break;
        case eNodeType::SEQUENCE:
            return sequence_.begin();
            break;
        case eNodeType::SCALAR:
            THROW("node(scalar) doesn't have an iterator")
        }
    }

    inline SequenceType::iterator end()
    {
        switch (nodeType_) {
        case eNodeType::MAP:
            THROW("node(map) doesn't have an iterator")
            break;
        case eNodeType::SEQUENCE:
            return sequence_.end();
            break;
        case eNodeType::SCALAR:
            THROW("node(scalar) doesn't have an iterator")
        }
    }

    inline SequenceType::const_iterator cbegin() const
    {
        switch (nodeType_) {
        case eNodeType::MAP:
            THROW("node(map) doesn't have an iterator")
            break;
        case eNodeType::SEQUENCE:
            return sequence_.cbegin();
            break;
        case eNodeType::SCALAR:
            THROW("node(scalar) doesn't have an iterator")
        }
    }

    inline SequenceType::const_iterator cend() const
    {
        switch (nodeType_) {
        case eNodeType::MAP:
            THROW("node(map) doesn't have an iterator")
            break;
        case eNodeType::SEQUENCE:
            return sequence_.cend();
            break;
        case eNodeType::SCALAR:
            THROW("node(scalar) doesn't have an iterator")
        }
    }

    inline const eNodeType nodeType() { return nodeType_; }

    friend YamlNode parseYamlFast(const YamlParserType parser);
private:

    eNodeType nodeType_;
    //const std::shared_ptr<MapType> map_ = nullptr;
    //const std::shared_ptr<SequenceType> sequence_ = nullptr;
    MapType map_;// = nullptr;
    SequenceType sequence_;// = nullptr;
    ScalarType scalar_ = "42";

    inline MapType& getMap() { return map_; }
    inline SequenceType& getSequence() { return sequence_; }
    inline ScalarType& getScalar() { return scalar_; }
};


YamlNode parseYamlFast(YamlParserType parser) {
    //Container<yaml_event_t, void, &yaml_event_delete> event = Container<yaml_event_t, void, &yaml_event_delete>(yaml_event_t());
    yaml_event_t event;

    if (!yaml_parser_parse(parser.get(), &event)) {
       THROW(QString::fromStdString("Parser error " + std::to_string((*parser).error)));
    }

    switch((event).type)
    {
    case YAML_NO_EVENT:
        yaml_event_delete(&event);
        qDebug() << "DEBUG[parseYamlFast] YAML_NO_EVENT";
        break;
    /* Stream start/end */
    case YAML_STREAM_START_EVENT:
        yaml_event_delete(&event);
        qDebug() << "DEBUG[parseYamlFast] YAML_STREAM_START_EVENT";
        return parseYamlFast(parser);
    case YAML_STREAM_END_EVENT:
        yaml_event_delete(&event);
        qDebug() << "DEBUG[parseYamlFast] YAML_STREAM_END_EVENT";
        return YamlNode(true);
    /* Block delimeters */
    case YAML_DOCUMENT_START_EVENT:
        yaml_event_delete(&event);
        qDebug() << "DEBUG[parseYamlFast] YAML_DOCUMENT_START_EVENT";
        return parseYamlFast(parser);
    case YAML_DOCUMENT_END_EVENT:
        yaml_event_delete(&event);
        qDebug() << "DEBUG[parseYamlFast] YAML_DOCUMENT_END_EVENT";
        return YamlNode(true);
    case YAML_SEQUENCE_START_EVENT: {
        yaml_event_delete(&event);
        qDebug() << "DEBUG[parseYamlFast] YAML_SEQUENCE_START_EVENT";
        YamlNode node(YamlNode::eNodeType::SEQUENCE);
        YamlNode::SequenceType& sequence = node.getSequence();

        for (auto parseResult = parseYamlFast(parser); !parseResult.isEnd; parseResult = parseYamlFast(parser)) {
            sequence.push_back(parseResult);
        }
        return node;
    }
    break;
    case YAML_SEQUENCE_END_EVENT:
        yaml_event_delete(&event);
        qDebug() << "DEBUG[parseYamlFast] YAML_SEQUENCE_END_EVENT";
        return YamlNode(true);
    case YAML_MAPPING_START_EVENT: {
        yaml_event_delete(&event);
        qDebug() << "DEBUG[parseYamlFast] YAML_MAPPING_START_EVENT";
        YamlNode node(YamlNode::eNodeType::MAP);
        YamlNode::MapType& map = node.getMap();
        for (auto key = parseYamlFast(parser); !key.isEnd; key = parseYamlFast(parser)) {
            qDebug() << "DEBUG[parseYamlFast] key.value() == " << key.value();
            auto value = parseYamlFast(parser);
            //map.insert({key.value(), value});
            map.insert(key.value(), value);
        }
        return node;
    }
    break;
    case YAML_MAPPING_END_EVENT:
        yaml_event_delete(&event);
        qDebug() << "DEBUG[parseYamlFast] YAML_MAPPING_END_EVENT";
        return new YamlNode(true);
    /* Data */
    case YAML_ALIAS_EVENT:
        yaml_event_delete(&event);
        qDebug() << "DEBUG[parseYamlFast] YAML_ALIAS_EVENT";
        THROW(QString("Got alias (anchor %s)") /*% (*event).data.alias.anchor)*/);
        break;
    case YAML_SCALAR_EVENT:
        qDebug() << "DEBUG[parseYamlFast] YAML_SCALAR_EVENT = " << QString::fromLatin1((char*)event.data.scalar.value);
        //auto node = YamlNode(event.data.scalar.value);
        //yaml_event_delete(&event);
        return YamlNode(QString::fromLatin1((char*)event.data.scalar.value));
        break;
    }
    qDebug() << "DEBUG[load_yaml] after switch";

}

struct FILEContainer {
    //Container() : value_(new T()) { }
    FILEContainer(FILE* file) : value_(file) { }
    ~FILEContainer() { fclose(value_); }
    FILE* operator *() {
        if (value_ == nullptr) {
            THROW("value is nullptr, but should be initialized")
        }
        return value_;
    }
private:
    FILE* value_;
};


const YamlNode loadYamlFast(const std::string& filePath) {
    FILEContainer file(fopen(filePath.c_str(), "r"));
    YamlParserType parser( new yaml_parser_t());

    qDebug() << "DEBUG[load_yaml] after openFile";
    /* Initialize parser */
    if(!yaml_parser_initialize(&*parser))
        THROW("Failed to initialize parser!");
    if(*file == nullptr)
        THROW("Failed to open file!");

    /* Set input file */
    yaml_parser_set_input_file(&*parser, *file);
    qDebug() << "DEBUG[load_yaml] before parseYamlFast";
    return parseYamlFast(parser);
    qDebug() << "DEBUG[load_yaml] after parseYamlFast";
    yaml_parser_delete(parser.get());
}


} // namespace load

#endif // FASTYAMLLOADER_H
