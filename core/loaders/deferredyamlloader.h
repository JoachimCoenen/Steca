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

#ifndef DEFERREDYAMLLOADER_H
#define DEFERREDYAMLLOADER_H

#include "fastyamlloader.h"
#include "core/raw/rawfile.h"
#include "core/typ/exception.h"
#include<yaml.h>
#include<map>
#include <QVariant>
#include <QMap>

//! Functions loadRawfile and loadComment, and their dependences.
/*
namespace loadYAML {


template <typename T>
class yamlIterator {
public:
    //typedef T value_type;
    //typedef T& reference_type;
    //typedef const T& const_reference_type;
    //typedef T* pointer_type;
    //typedef const T* const_pointer_type;

    yamlIterator(bool isMappingIterator)  : isMappingIterator_(isMappingIterator) {

    }

    void finishThisNode() {
        Container<yaml_event_t, &yaml_event_delete>  event;

        int stackDepth = 0;
        bool noEvent = false;
        do {
            noEvent = false;
            if (!yaml_parser_parse(parser, *event)) {
                THROW("Parser error %d" % parser.error);
            }
            switch(*event.type)
            {
            case YAML_NO_EVENT: noEvent = true; break;
            // Stream start/end
            case YAML_STREAM_START_EVENT:
                stackDepth++; break;
            case YAML_STREAM_END_EVENT:
                stackDepth--; break;
            // Block delimeters
            case YAML_DOCUMENT_START_EVENT:
                stackDepth++; break;
            case YAML_DOCUMENT_END_EVENT:
                stackDepth--; break;
            case YAML_SEQUENCE_START_EVENT:
                stackDepth++; break;
            case YAML_SEQUENCE_END_EVENT:
                stackDepth--; break;
            case YAML_MAPPING_START_EVENT:
                stackDepth++; break;
            case YAML_MAPPING_END_EVENT:
                return ParsingResult(true);
            // Data
            case YAML_ALIAS_EVENT:
                THROW("Got alias (anchor %s)\n" % *event.data.alias.anchor); break;
            case YAML_SCALAR_EVENT:
                break;
            }
            yaml_event_delete(*event);
        } while (0 >= stackDepth && !noEvent);

        thisNodeHasFinished_ = true;
    }

    yamlIterator& operator++ (void) {
        if (!thisNodeHasFinished_) {
            finishThisNode();
        }
        thisNodeHasFinished_ = false;
        currentNode_ = nullptr;

        if(isMappingIterator_) {
            auto parseResult = parseYamlFast(parser_);
            key_ = parseResult.node;
        } // else key_ = nullptr;


        Container<yaml_event_t, &yaml_event_delete>  event;

        if (!yaml_parser_parse(*parser, *event)) {
            THROW("Parser error %d" % *parser.error);
        }

        return this;
    }


    NodeType getCurrentNode() {
        if (nullptr == currentNode_) {
            currentNode_ = constructCurrentNode();
        } // else
        return currentNode_;
    }




private:
    const YamlNode&& constructCurrentNode() {
    Assert(false == thisNodeHasFinished_)
        const auto& parsingResult = parseYamlFast(parser_);
        if (parsingResult.isEnd) {
            THROW("Invalid or non-existent Node")
        }
        thisNodeHasFinished_ = true;
        return parsingResult.node;
    }

    const bool isMappingIterator_;
    bool isEndIterator_ = false;
    bool thisNodeHasFinished_ = false;
    NodeType currentNode_ = nullptr;
    KeyType key_ = nullptr;


    const std::shared_ptr<yaml_parser_t> parser_;

    const_reference_type operator *() {
        return ;
    }
public:
    typedef std::map<KeyType, YamlNode> MapType;
    typedef std::vector<YamlNode> SequenceType;
    typedef std::string ScalarType;
    typedef std::shared_ptr<YamlNode> NodeType;
    typedef NodeType KeyType;
};



class DeferredYamlLoader {
public:

	static DeferredYamlLoader&& loadFromFile(const std::string& filePath) {
		return DeferredYamlLoader(filePath);
	}

	~DeferredYamlLoader() 
	{
		if (file_ != nullptr) {
			fclose(file_);
			file_ = nullptr;
		}
	}

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

	DeferredYamlLoader(const std::string& filePath) 
	{
		filePath_ = filePath;
		file_ = fopen(filePath, "r");
		if(file_ == NULL) {
			THROW("Failed to open file!");
		}
        // Initialize parser
		if(!yaml_parser_initialize(&parser_)) {
			fclose(file_);
			file_ = nullptr;
			THROW("Failed to initialize parser!");
		}

        // Set input file
		yaml_parser_set_input_file(&parser_, file_);
	  
	}

    const KeyType& key_;
    yaml_parser_t parser_;
    yaml_event_t  event_;

	std::stack<>
	
	FILE* file_ = nullptr;
	const std::string& filePath_;



};

} // namespace loadYAML
*/
#endif // DEFERREDYAMLLOADER_H
