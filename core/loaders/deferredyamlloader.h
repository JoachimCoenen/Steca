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
#include <fstream>
#include <sstream>
#include <yaml.h>
#include <map>
#include <QVariant>
#include <QMap>
#include <assert.h>

//! Functions loadRawfile and loadComment, and their dependences.

namespace loadYAML {

class YamlIterator {
public:
    typedef std::string ScalarType;
    typedef ScalarType KeyType;
    typedef std::map<KeyType, YamlNode> MapType;
    typedef std::vector<YamlNode> SequenceType;
    typedef std::shared_ptr<YamlNode> NodeType;
    typedef std::shared_ptr<YamlIterator> IteratorType;

    YamlIterator(bool isMappingIterator, YamlParserType parser)
        : isMappingIterator_(isMappingIterator)
        , parser_(parser)
    { }

    void finishThisNode() {
        if (nodeState == eNodeState::UNTOUCHED) {
            //getInnerIterator();
            yaml_event_t event;

            int counter = 0;
            int maxStackDepth = 0;
            int stackDepth = 0;
            bool noEvent = false;
            do {
                noEvent = false;
                switch(parser_parse(parser_, event))
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
                    stackDepth--; break;
                // Data
                case YAML_ALIAS_EVENT:
                    THROW("Got alias (anchor s)\n"); break;
                case YAML_SCALAR_EVENT:
                    break;
                }
                counter++;
                maxStackDepth = stackDepth > maxStackDepth ? stackDepth : maxStackDepth;
                yaml_event_delete(&event);
            } while (0 >= stackDepth && !noEvent);
            DEBUG_OUT_TEMP("DEBUG[yamlIterator::finishThisNode] counted " << counter << " tokens up.");
            nodeState = eNodeState::FINISHED;
        }
        if (nodeState == eNodeState::ITERATING) {
            assert(currentIterator_ != nullptr);
            int counter = 0;
            while (!currentIterator_->isEnd()) {
                counter++;
                ++(*currentIterator_);
            }
            DEBUG_OUT_TEMP("DEBUG[yamlIterator::finishThisNode] counted " << counter << " iterators up.");
            nodeState = eNodeState::FINISHED;
        }
    }

    YamlIterator& operator++ (void) {
        assert(!isEndIterator_);
        finishThisNode();
        nodeState = eNodeState::UNTOUCHED;
        currentNode_ = nullptr;
        currentIterator_ = nullptr;

        yaml_event_delete(&event_);
        parser_parse(parser_, event_);
        DEBUG_OUT_TEMP("DEBUG[yamlIterator::++] event_.type == " << event_.type);

        isEndIterator_ = false
                || YAML_MAPPING_END_EVENT == event_.type
                || YAML_SEQUENCE_END_EVENT == event_.type
                || YAML_NO_EVENT == event_.type
                ;
        if (isMappingIterator_ && !isEndIterator_) {
            key_ = std::string((char*)event_.data.scalar.value);
            DEBUG_OUT_TEMP("DEBUG[yamlIterator::++] key == " << QString::fromStdString(key_));
            yaml_event_delete(&event_);
            parser_parse(parser_, event_);
        } else if (isEndIterator_) {
            nodeState = eNodeState::FINISHED;
        }
        return *this;
    }

    NodeType getCurrentNode() {
        if (nullptr == currentNode_) {
            currentNode_ = NodeType(new YamlNode(constructCurrentNode()));
        } // else
        return currentNode_;
    }

    IteratorType getInnerIterator()
    {
        nodeState = eNodeState::ITERATING;
        if (nullptr == currentIterator_) {
            currentIterator_ = IteratorType(new YamlIterator(YAML_MAPPING_START_EVENT == event_.type, parser_));
            if (YAML_SCALAR_EVENT == event_.type) {
                currentIterator_->nodeState = eNodeState::FINISHED;
                DEBUG_OUT_TEMP("DEBUG[yamlIterator::getInnerIterator] YAML_SCALAR_EVENT");
            }
        }
        return currentIterator_;
    }

    ScalarType getValue()
    {
        nodeState = eNodeState::FINISHED;
        return std::string((char*)event_.data.scalar.value);
    }

    template<typename T>
    inline void fillVectorWithInts(std::vector<T>& vect)
    {
        yaml_event_delete(&event_);
        parser_parse(parser_, event_);

        while (YAML_SEQUENCE_END_EVENT != event_.type) {
            vect.push_back(atoi((char*)event_.data.scalar.value));
            yaml_event_delete(&event_);
            parser_parse(parser_, event_);
        }
        DEBUG_OUT_TEMP("DEBUG[yamlIterator::fillVectorWithInts] vect.size() == " << vect.size());
        nodeState = eNodeState::FINISHED;
        isEndIterator_ = true;
    }

    template<typename T>
    inline void fillVectorWithIntsFromScalar(std::vector<T>& vect)
    {
        //yaml_event_delete(&event_);
        //parser_parse(parser_, event_);

        auto str = std::string((char*)event_.data.scalar.value);
        std::stringstream imageStr(str);
        while (!imageStr.eof()) {
            int v;
            imageStr >> v;
            vect.push_back(v);
            //vect.push_back(atoi((char*)event_.data.scalar.value));
        }
        DEBUG_OUT_TEMP("DEBUG[yamlIterator::fillVectorWithInts] vect.size() == " << vect.size());
        nodeState = eNodeState::FINISHED;
        //isEndIterator_ = true;
    }

    KeyType getKey() { return key_; }

    bool isEnd() { return isEndIterator_; }

private:

    const YamlNode constructCurrentNode() {
    assert(eNodeState::UNTOUCHED == nodeState);
        const auto& node = parseYamlFast2(parser_, event_);
        nodeState = eNodeState::FINISHED;
        return node;
    }

    enum class eNodeState {
        UNTOUCHED = 0,
        ITERATING = 1,
        FINISHED = 2,
    };

    const bool isMappingIterator_;
    bool isEndIterator_ = false;
    eNodeState nodeState = eNodeState::FINISHED;
    NodeType currentNode_ = nullptr;
    IteratorType currentIterator_ = nullptr;
    KeyType key_ = "";

    yaml_event_t event_;
    const YamlParserType parser_;

};


/*

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
*/
} // namespace loadYAML

#endif // DEFERREDYAMLLOADER_H
