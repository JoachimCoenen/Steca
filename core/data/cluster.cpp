//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      core/data/cluster.cpp
//! @brief     Implements classes Sequence, Cluster
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#include "core/session.h"
#include <qmath.h>

//  ***********************************************************************************************
//! @class Sequence

Sequence::Sequence(const QVector<const Measurement*>& measurements)
    : members_(measurements)
{}

//! Returns metadata, averaged over Sequence members. Result is cached.
const Metadata* Sequence::avgeMetadata() const
{
    if (!md_.get())
        compute_metadata();
    return md_.get();
}

#define AVG_ONES(what_function)                 \
    double avg = 0;                             \
    for (const Measurement* one : members_)     \
        avg += one->what_function();            \
    avg /= count();                             \
    return avg;

deg Sequence::omg() const { AVG_ONES(omg) }

deg Sequence::phi() const { AVG_ONES(phi) }

deg Sequence::chi() const { AVG_ONES(chi) }

// combined range of combined cluster
#define RGE_COMBINE(combineOp, what)            \
    Range rge;                                  \
    for (const Measurement* one : members_)     \
        rge.combineOp(one->what);               \
    return rge;

Range Sequence::rgeGma() const { RGE_COMBINE(extendBy, rgeGma()) }

Range Sequence::rgeGmaFull() const { RGE_COMBINE(extendBy, rgeGmaFull()) }

Range Sequence::rgeTth() const { RGE_COMBINE(extendBy, rgeTth()) }

Range Sequence::rgeInten() const { RGE_COMBINE(intersect, rgeInten()) }

double Sequence::avgMonitorCount() const { AVG_ONES(monitorCount) }

double Sequence::avgDeltaMonitorCount() const { AVG_ONES(deltaMonitorCount) }

double Sequence::avgDeltaTime() const { AVG_ONES(deltaTime) }

size2d Sequence::imageSize() const
{
    // all images have the same size; simply take the first one
    return first()->imageSize();
}

Curve Sequence::toCurve() const
{
    return toCurve(rgeGma());
};

Curve Sequence::toCurve(const Range& _rgeGma) const
{
    double _normFactor = normFactor();
    return toCurve(_normFactor, _rgeGma);
};

Curve Sequence::toCurve(double _normFactor) const
{
    return toCurve(_normFactor, rgeGma());
};

Curve Sequence::toCurve(double _normFactor, const Range& _rgeGma) const
{
    QVector<float> intens = collectIntens(_rgeGma);
    int count = intens.count();
    if (!count)
        return {};
    Curve res;
    Range _rgeTth = rgeTth();
    deg minTth = _rgeTth.min;
    deg deltaTth = _rgeTth.width() / count;
    for_i (count)
        res.append(minTth + deltaTth * i, double(intens.at(i) * _normFactor));
    return res;
};

double Sequence::normFactor() const
{
    double num = 1, den = 1;

    switch (gSession->normMode()) {
    case eNorm::MONITOR:
        num = gSession->activeClusters().avgMonitorCount();
        den = avgMonitorCount();
        break;
    case eNorm::DELTA_MONITOR:
        num = gSession->activeClusters().avgDeltaMonitorCount();
        den = avgDeltaMonitorCount();
        break;
    case eNorm::DELTA_TIME:
        num = gSession->activeClusters().avgDeltaTime();
        den = avgDeltaTime();
        break;
    case eNorm::BACKGROUND:
        num = gSession->calcAvgBackground();
        den = gSession->calcAvgBackground(*this);
        break;
    case eNorm::NONE:
        break;
    }

    double ret = float((num > 0 && den > 0) ? num / den : Q_QNAN);
    if (qIsNaN(ret))
        qWarning() << "Bad normalisation value";
    return ret;
}

//! Called only by toCurve(..).
QVector<float> Sequence::collectIntens(const Range& rgeGma) const
{
    const Range tthRge = rgeTth();
    const deg tthWdt = tthRge.width();

    const ImageCut& cut = gSession->imageCut();
    const int pixWidth = gSession->imageSize().w - cut.left() - cut.right();

    int numBins;
    if (1 < count()) { // combined cluster
        const Measurement* one = first();
        deg delta = one->rgeTth().width() / pixWidth;
        numBins = qCeil(tthWdt / delta);
    } else {
        numBins = pixWidth; // simply match the pixels
    }

    QVector<float> intens(numBins, 0);
    QVector<int> counts(numBins, 0);

    deg minTth = tthRge.min, deltaTth = tthWdt / numBins;

    for (const Measurement* one : members_)
        one->collectIntens(intens, counts, rgeGma, minTth, deltaTth);

    // sum or average
    if (gSession->intenScaledAvg()) {
        double scale = gSession->intenScale();
        for_i (numBins) {
            int cnt = counts.at(i);
            if (cnt > 0)
                intens[i] *= scale / cnt;
        }
    }

    return intens;
}

//! Computes metadata cache md_.
void Sequence::compute_metadata() const
{
    auto* m = new Metadata;
    md_.reset(m);

    const Metadata& firstMd = *(first()->metadata());
    m->date = firstMd.date;
    m->comment = firstMd.comment;

    // sums: delta mon. count and time,
    // takes the last ones (presumed the maximum) of mon. count and time,
    // averages the rest
    for (const Measurement* one : members_) {
        const Metadata* d = one->metadata();

        m->motorXT += d->motorXT;
        m->motorYT += d->motorYT;
        m->motorZT += d->motorZT;

        m->motorOmg += d->motorOmg;
        m->motorTth += d->motorTth;
        m->motorPhi += d->motorPhi;
        m->motorChi += d->motorChi;

        m->motorPST += d->motorPST;
        m->motorSST += d->motorSST;
        m->motorOMGM += d->motorOMGM;

        m->nmT += d->nmT;
        m->nmTeload += d->nmTeload;
        m->nmTepos += d->nmTepos;
        m->nmTeext += d->nmTeext;
        m->nmXe += d->nmXe;
        m->nmYe += d->nmYe;
        m->nmZe += d->nmZe;

        m->deltaMonitorCount += d->deltaMonitorCount;
        m->deltaTime += d->deltaTime;

        if (m->monitorCount > d->monitorCount)
            qWarning() << "decreasing monitor count in combined cluster";
        if (m->time > d->time)
            qWarning() << "decreasing time in combined cluster";
        m->monitorCount = d->monitorCount;
        m->time = d->time;
    }

    double fac = 1.0 / count();

    m->motorXT *= fac;
    m->motorYT *= fac;
    m->motorZT *= fac;

    m->motorOmg *= fac;
    m->motorTth *= fac;
    m->motorPhi *= fac;
    m->motorChi *= fac;

    m->motorPST *= fac;
    m->motorSST *= fac;
    m->motorOMGM *= fac;

    m->nmT *= fac;
    m->nmTeload *= fac;
    m->nmTepos *= fac;
    m->nmTeext *= fac;
    m->nmXe *= fac;
    m->nmYe *= fac;
    m->nmZe *= fac;
}

//  ***********************************************************************************************
//! @class Cluster

Cluster::Cluster(
    const QVector<const Measurement*>& measurements,
    const class Datafile& file, const int index, const int offset)
    : Sequence(measurements)
    , file_(file)
    , index_(index)
    , offset_(offset)
    , activated_(true)
{
}

//! note: the caller must emit sigActivated
void Cluster::setActivated(bool on)
{
    if (on==activated_)
        return;
    activated_ = on;
}

int Cluster::totalOffset() const
{
    return file_.offset_ + offset();
}

bool Cluster::isIncomplete() const
{
    return count()<gSession->dataset().binning();
}
