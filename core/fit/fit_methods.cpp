//  ***********************************************************************************************
//
//  Steca: stress and texture calculator
//
//! @file      core/fit/fit_methods.cpp
//! @brief     Implements class FitWrapper
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
//  ***********************************************************************************************

#include "core/fit/fit_methods.h"
//#include "LevMar/LM/levmar.h"
#include "minimizer.h"
#include "core/typ/curve.h"
#include "qcr/base/debug.h"
#include <qmath.h>

template <typename T>
T* remove_const(T const* t)
{
    return const_cast<T*>(t);
}
#ifdef MINIMIZER_H
Fitted FitWrapper::execFit(const FitFunction* f,const Curve& curve, std::vector<double> parValue, bool onlyPositiveParams)
{
    int nPar = f->nPar();
    ASSERT(parValue.size()==nPar);

    if (curve.size()<nPar)
        return {}; // signals failure

    std::vector<double> parError(nPar);

    Matrix covarMat;

    if (onlyPositiveParams) {
        std::vector<double> minParams (nPar, 0.0);
        parValue = levenbergMarquardtLowerBound(curve.ys(), parValue, minParams,
            [&](const double *params, double *ys) {
                f->setY(params, curve.xs().size(), curve.xs().data(), ys);
            },
            [&](const double *params, double *jacobian) {
                f->setDY(params, curve.xs().size(), curve.xs().data(), jacobian);
            }, covarMat, { 1e-12, 1e-12, 1e-18 }, nPar > 1);
    } else {
        parValue = levenbergMarquardt(curve.ys(), parValue,
            [&](const double *params, double *ys) {
                f->setY(params, curve.xs().size(), curve.xs().data(), ys);
            },
            [&](const double *params, double *jacobian) {
                f->setDY(params, curve.xs().size(), curve.xs().data(), jacobian);
            }, covarMat, { 1e-12, 1e-12, 1e-18 }, nPar > 1);
    }
    // pass fit results
    for (int ip=0; ip<nPar; ++ip)
        parError[ip] = sqrt(covarMat.get(ip, ip)); // the diagonal
    return Fitted(f, parValue, parError);

}

#else

Fitted FitWrapper::execFit(const FitFunction* f,const Curve& curve, std::vector<double> parValue, bool onlyPositiveParams)
{
    int nPar = f->nPar();
    ASSERT(parValue.size()==nPar);

    if (curve.size()<nPar)
        return {}; // signals failure

    std::vector<double> parError(nPar);
    std::vector<double> covar(nPar * nPar); // output covariance matrix
    //std::vector<double> parMin(nPar), parMax(nPar);

    // minimizer options mu, epsilon1, epsilon2, epsilon3
    double opts[] = { LM_INIT_MU, 1e-12, 1e-12, 1e-18 };
    int const maxIterations = 1000;
    double info[LM_INFO_SZ];

    f_ = f;
    X_ = &curve.xs();

    DelegateCalculationDbl fitFct(this, &FitWrapper::callbackY);
    DelegateCalculationDbl Jacobian(this, &FitWrapper::callbackJacobianLM);

    // workspace for dlevmar_bc_der():
    size_t workSpaceSize = LM_DER_WORKSZ(nPar, curve.size());
    std::vector<double> workSpace(workSpaceSize);

    if (onlyPositiveParams) {
        std::vector<double> minParams (nPar, 0.0);
        dlevmar_bc_der(
            &fitFct, &Jacobian, parValue.data(), remove_const(curve.ys().data()), nPar,
            curve.size(), minParams.data(), nullptr, // remove_const(parMax.data()),
            nullptr,
            maxIterations, opts, info, workSpace.data(), covar.data(), nullptr);
    } else {
        dlevmar_der(
            &fitFct, &Jacobian, parValue.data(), remove_const(curve.ys().data()), nPar,
            curve.size(),
            maxIterations, opts, info, workSpace.data(), covar.data(), nullptr);
    }

    // pass fit results
    for (int ip=0; ip<nPar; ++ip)
        parError[ip] = sqrt(covar[ip * nPar + ip]); // the diagonal
    return Fitted(f, parValue, parError);
}
#endif

void FitWrapper::callbackY(double* P, double* Y, int, int, void*)
{
    f_->setY(P, X_->size(), X_->data(), Y);
}

void FitWrapper::callbackJacobianLM(double* P, double* Jacobian, int, int, void*)
{
    f_->setDY(P, X_->size(), X_->data(), Jacobian);
}
