//  ***********************************************************************************************
//
//  libqcr: capture and replay Qt widget actions
//
//! @file      qcr/engine/cell.h
//! @brief     Defines class Cell
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2018-
//! @author    Joachim Wuttke
//
//  ***********************************************************************************************

#ifndef CELL_H
#define CELL_H

#include "qcr/engine/debug.h"
#include <QObject>
#include <functional>
#include <set>
#include <vector>

extern class Cell* gRoot;

//! Manages update dependences.
class Cell {
public:
    Cell() = delete;
    Cell(const QString& name) : name_(name) {}
    typedef long int stamp_t;
    stamp_t update();
    void addSource(Cell*);
    void rmSource(Cell*);
    void clearSources();
    void connectAction(std::function<void()>&&);
    const QString& name() const { return name_; }
protected:
    virtual void recompute() {};
    void actOnChange();
private:
    const QString name_;
    stamp_t timestamp_ { 0 };
    std::set<Cell*> sources_;
    std::vector<std::function<void()>> actionsOnChange_;
};

class ValueCell : public Cell {
public:
    ValueCell(const QString& name) : Cell(name) {}
protected:
    static stamp_t latestTimestamp__;
    static stamp_t mintTimestamp() { return ++latestTimestamp__; }
};

//! Holds a single data value, and functions to be run upon change
template<class T>
class SingleValueCell : public ValueCell {
public:
    SingleValueCell() = delete;
    SingleValueCell(const QString& name, T value) : ValueCell(name), value_(value) {}
    T val() const { return value_; }
    void setCoerce(std::function<T(T)> coerce) { coerce_ = coerce; }
    void setPostHook(std::function<void(T)> postHook) { postHook_ = postHook; }
    void setVal(T, bool userCall=false);
    void reCoerce() { setVal(value_); }
private:
    T value_;
    std::function<void(T)> postHook_ = [](T) {};
    std::function<T(T)> coerce_ = [](T val) { return val; };
};

//  ***********************************************************************************************
//  class SingleValueCell implementation

template<class T>
void SingleValueCell<T>::setVal(T val, bool userCall)
{
    T newval = coerce_(val);
    if (newval==value_)
        return;
    value_ = newval;
    actOnChange();
    if (userCall) {
        mintTimestamp();
        postHook_(newval);
        ASSERT(gRoot);
        gRoot->update();
    } else {
        qDebug() << "non-user call";
    }
}

#endif // CELL_H
