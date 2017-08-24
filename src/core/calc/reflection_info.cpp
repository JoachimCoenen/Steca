/*******************************************************************************
 * STeCa2 - StressTextureCalculator ver. 2
 *
 * Copyright (C) 2016-7 Forschungszentrum Jülich GmbH
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the COPYING and AUTHORS files for more details.
 ******************************************************************************/

#include "reflection_info.hpp"

namespace core { namespace calc {
//------------------------------------------------------------------------------

ReflectionInfo::ReflectionInfo(l::deg alpha, l::deg beta)
: ReflectionInfo(alpha, beta, Range(),
                 inten_t(l::flt_nan), inten_t(l::flt_nan),
                 tth_t(l::flt_nan), tth_t(l::flt_nan),
                 fwhm_t(l::flt_nan), fwhm_t(l::flt_nan))
{}

ReflectionInfo::ReflectionInfo(l::deg alpha, l::deg beta, gma_rge rgeGma,
    inten_t inten, inten_t intenError, tth_t tth, tth_t tthError, fwhm_t fwhm, fwhm_t fwhmError)
  : ReflectionInfo(data::Meta::sh(), alpha, beta, rgeGma,
                   inten, intenError, tth, tthError, fwhm, fwhmError)
{
}

ReflectionInfo::ReflectionInfo(data::Meta::sh md_,
    l::deg alpha_, l::deg beta_, gma_rge rgeGma_,
    inten_t inten_, inten_t intenError_,
    tth_t   tth_,   tth_t   tthError_,
    fwhm_t  fwhm_,  fwhm_t  fwhmError_)
: md(md_)
, alpha(alpha_), beta(beta_), rgeGma(rgeGma_)
, inten(inten_), intenError(intenError_)
, tth(tth_),     tthError(tthError_)
, fwhm(fwhm_),  fwhmError(fwhmError_)
{
}

//using namespace typ;
//using namespace data;

///* NOTE Invalid output parameters are set to NaNs. However, some analysis
// * programs
// * require -1 as unknown value; thus, NaN parameter values should be output
// * as -1 when output is written for these programs (polefigure!).
// */

//str_vec ReflectionInfo::dataTags(bool out) {
//  str_vec tags;

//  for_i_(uint(eReflAttr::NUM_REFL_ATTR))
//    tags.append(reflStringTag(i, out));

//  tags.append(Metadata::attributeTags(out));

//  return tags;
//}

//str const ReflectionInfo::reflStringTag(uint attr, bool out) {
//  switch (eReflAttr(attr)) {
//  case eReflAttr::ALPHA:
//    return out ? "alpha" : "α";
//  case eReflAttr::BETA:
//    return out ? "beta" : "β";
//  case eReflAttr::GAMMA1:
//    return out ? "gamma1" : "γ1";
//  case eReflAttr::GAMMA2:
//    return out ? "gamma2" : "γ2";
//  case eReflAttr::INTEN:
//    return "inten";
//  case eReflAttr::SIGMA_INTEN:
//    return out ? "sinten" : "σinten";
//  case eReflAttr::TTH:
//    return out ? "2theta" : "2θ";
//  case eReflAttr::SIGMA_TTH:
//    return out ? "s2theta" : "σ2θ";
//  case eReflAttr::FWHM:
//    return "fwhm";
//  case eReflAttr::SIGMA_FWHM:
//    return out ? "sfwhm" : "σfwhm";
//  default:
//    NEVER;
//    return nullptr;
//  }
//}

//cmp_vec ReflectionInfo::dataCmps() {
//  static cmp_vec cmps;
//  if (cmps.isEmpty()) {
//    cmps = cmp_vec { cmp_real, cmp_real, cmp_real, cmp_real,
//                     cmp_real, cmp_real, cmp_real,
//                     cmp_real, cmp_real, cmp_real };
//    cmps.append(Metadata::attributeCmps());
//  }

//  return cmps;
//}

//ReflectionInfo::ReflectionInfo()
//: ReflectionInfo(shp_Metadata(),
//                 l::flt_nan, l::flt_nan, Range(),
//                 inten_t(l::flt_nan), inten_t(l::flt_nan),
//                 tth_t(l::flt_nan),   tth_t(l::flt_nan),
//                 l::flt_nan, l::flt_nan)
//{
//}


//ReflectionInfo::ReflectionInfo(shp_Metadata md, deg alpha, deg beta,
//                               gma_rge rgeGma)
//: ReflectionInfo(md, alpha, beta, rgeGma,
//                 inten_t(l::flt_nan), inten_t(l::flt_nan),
//                 tth_t(l::flt_nan),   tth_t(l::flt_nan),
//                 fwhm_t(l::flt_nan),  fwhm_t(l::flt_nan))
//{
//}



//row_t ReflectionInfo::data() const {
//  row_t row{QVariant(alpha()), QVariant(beta()),
//            QVariant(rgeGma().min), QVariant(rgeGma().max),
//            QVariant(inten()), QVariant(intenError()),
//            QVariant(tth()),   QVariant(tthError()),
//            QVariant(fwhm()),  QVariant(fwhmError())};

//  row.append(md_ ? md_->attributeValues() : Metadata::attributeNaNs());
//  return row;
//}

////------------------------------------------------------------------------------

//ReflectionInfos::ReflectionInfos() {
//  invalidate();
//}

//void ReflectionInfos::append(ReflectionInfo::rc info) {
//  super::append(info);
//  invalidate();
//}

//inten_t ReflectionInfos::averageInten() const {
//  if (qIsNaN(avgInten_)) {
//    avgInten_ = 0;
//    uint cnt  = 0;

//    for (auto& info : *this) {
//      qreal inten = info.inten();
//      if (qIsFinite(inten)) {
//        avgInten_ += inten;
//        ++cnt;
//      }
//    }

//    if (cnt) avgInten_ /= cnt;
//  }

//  return avgInten_;
//}

//inten_rge::rc ReflectionInfos::rgeInten() const {
//  if (!rgeInten_.isValid()) {
//    for_i_(count())
//      rgeInten_.extendBy(at(i).inten());
//  }

//  return rgeInten_;
//}

//void ReflectionInfos::invalidate() {
//  avgInten_ = inten_t(l::flt_nan);
//  rgeInten_.invalidate();
//}

//------------------------------------------------------------------------------
}}

// eof