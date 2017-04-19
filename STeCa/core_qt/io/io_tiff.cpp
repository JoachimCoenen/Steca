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

#include "io.hpp"
#include <c2/c/ptr.h>
#include <c2/qt/qstr.hpp>
#include <c2/inc/c_cpp>

#include <QFile> // TODO do w/o Qt?
#include <QDataStream>
#include <QFileInfo>
#include <QDir>

// The dat file looks like so:
/*
; comments

; file name, phi, monitor, Exposuretime

Aus-Weimin-00001.tif -90
Aus-Weimin-00002.tif -85
Aus-Weimin-00003.tif -80
Aus-Weimin-00004.tif -75
Aus-Weimin-00005.tif -70
Aus-Weimin-00006.tif -65
Aus-Weimin-00007.tif -60
Aus-Weimin-00008.tif -55
Aus-Weimin-00009.tif -50
*/

namespace core { namespace io {
//------------------------------------------------------------------------------

using data::Files;
using data::File;
using data::Set;
using data::Meta;

using data::flt_vec;

//------------------------------------------------------------------------------

#define IS_ASCII \
  check_or_err_(2==dataType, BAD_FORMAT)

#define IS_NUMBER \
  check_or_err_((1==dataType || 3==dataType || 4==dataType) && 1==dataCount, BAD_FORMAT)

#define CHECK_NUMBER(val) \
  IS_NUMBER; check_or_err_(val==dataOffset, BAD_FORMAT)


static void loadTiff(File& file, c::path::rc path,
                     phi_t phi, flt32 mon, flt32 tim) may_err {

  auto md = c::scope(new Meta(file.files.dict));

  mut(md->phi) = phi;
  mut(md->mon) = mon;
  mut(md->tim) = tim;

  QFile f(path.p);
  check_or_err_(f.open(QFile::ReadOnly), "cannot open file");

  // see http://www.fileformat.info/format/tiff/egff.htm
  QDataStream is(&f);
  is.setFloatingPointPrecision(QDataStream::SinglePrecision);

  auto check = [&is]() {
    check_or_err_(QDataStream::Ok == is.status(), "could not read data");
  };

  // magic
  quint16 magic; is >> magic;

  if (0x4949 == magic)      // II - intel
    is.setByteOrder(QDataStream::LittleEndian);
  else if (0x4d4d == magic) // MM - motorola
    is.setByteOrder(QDataStream::BigEndian);
  else
    c::err("bad magic");

  quint16 version; is >> version;
  check_or_err_(42 == version, "bad version");

  quint32 imageWidth    = 0, imageHeight  = 0,
          bitsPerSample = 0, sampleFormat = 0,
          rowsPerStrip  = 0xffffffff,
          stripOffsets  = 0, stripByteCounts = 0;

  quint16 tagId, dataType;
  quint32 dataCount, dataOffset;

  auto seek = [&f](qint64 offset) {
    check_or_err_(f.seek(offset), "bad offset");
  };

  auto asUint = [&]() -> uint {
    check_or_err_(1==dataCount, "bad data count");
    switch (dataType) {
    case 1: // byte
      return dataOffset & 0x000000ff; // some tif files did have trash there
    case 3: // short
      return dataOffset & 0x0000ffff;
    case 4:
      return dataOffset;
    }

    c::err("not a simple number");
  };

  auto asStr = [&]() {
    check_or_err_(2==dataType, "bad data type");
    auto lastPos = f.pos();

    seek(dataOffset);
    QByteArray data = f.readLine(dataCount);
    seek(lastPos);

    return c::str(data);
  };

  quint32 dirOffset; is >> dirOffset; check();
  seek(dirOffset);

  quint16 numDirEntries; is >> numDirEntries;

  for_i (numDirEntries) {
    is >> tagId >> dataType >> dataCount >> dataOffset; check();

    switch (tagId) {
    // numbers
    case 256:
      imageWidth = asUint();
      break;
    case 257:
      imageHeight = asUint();
      break;
    case 258:
      bitsPerSample = asUint();
      break;
    case 259: // Compression
      check_or_err_(1==asUint(), "compressed data");
      break;
    case 273:
      stripOffsets = asUint();
      break;
    case 277: // SamplesPerPixel
      check_or_err_(1==asUint(), "more than one sample per pixel");
      break;
    case 278:
      rowsPerStrip = asUint();
      break;
    case 279:
      stripByteCounts = asUint();
      break;
    case 284: // PlanarConfiguration
      check_or_err_(1==asUint(), "not planar");
      break;
    case 339:
      sampleFormat = asUint(); // 1 unsigned, 2 signed, 3 IEEE
      break;

    // text
    case 269: // DocumentName
      mut(md->comment).set(asStr());
      break;
//    case 306: // DateTime
//      date = asStr();
//      break;
    default:
//      TR("* NEW TAG *" << tagId << dataType << dataCount << dataOffset)
      break;
    }
  }

  check_or_err_(
      imageWidth>0 && imageHeight>0 && stripOffsets>0 && stripByteCounts>0 &&
      imageHeight <= rowsPerStrip, "bad format");

  check_or_err_(
      (1==sampleFormat || 2==sampleFormat || 3==sampleFormat) &&
      32==bitsPerSample, "unhandled format");

  c::sz2 size(imageWidth, imageHeight);

  uint count = imageWidth * imageHeight;
  count_arr2 intens(size);

  check_or_err_((bitsPerSample/8) * count == stripByteCounts, "bad format");

  seek(stripOffsets);

  for_i (intens.size().size())
    switch (sampleFormat) {
    case 1: {
      quint32 sample; is >> sample;
      intens.setAt(i, sample);
      break;
    }
    case 2: {
      qint32 sample; is >> sample;
      intens.setAt(i, sample);
      break;
    }
    case 3: {
      float sample; is >> sample;
      intens.setAt(i, sample);
      break;
    }
    }

  check();

  file.addSet(
    c::share(new Set(
      c::share(md),
      c::share(new Image(intens)))));
}

//------------------------------------------------------------------------------

File::sh loadTiffDat(Files& files, c::path::rc path) may_err {
  File::sh file(new File(files, path.basename()));

  QFile f(path.p);
  check_or_err_(f.open(QFile::ReadOnly), "cannot open file");

  QFileInfo info(path.p);
  QDir dir = info.dir();

  QByteArray line;
  while (!(line = f.readLine()).isEmpty()) {
    QString s(line);

    // cut off comment
    int commentPos = s.indexOf(';');
    if (commentPos >= 0)
      s = s.left(commentPos);

    // split to parts
    if ((s = s.simplified()).isEmpty())
      continue;

    auto lst = s.split(' ');
    auto cnt = lst.count();
    check_or_err_(2 <= cnt && cnt <= 4, "bad metadata format");

    // file, phi, monitor, expTime
    bool ok;
    QString tiffFileName = lst.at(0);
    auto phi = phi_t(lst.at(1).toFloat(&ok));
    check_or_err_(ok, "bad phi value");

    qreal monitor = 0;
    if (cnt > 2) {
      monitor = lst.at(2).toDouble(&ok);
      check_or_err_(ok, "bad monitor value");
    }

    qreal expTime = 0;
    if (cnt > 3) {
      expTime = lst.at(3).toDouble(&ok);
      check_or_err_(ok, "bad expTime value");
    }

    try {
      // load one dataset
      loadTiff(mut(*file), toStr(dir.filePath(tiffFileName)), phi, monitor, expTime);
    } catch (std::exception &e) {
      c::err(toStr(tiffFileName), ": ", e.what());
      throw;
    }
  }

  return file;
}

//------------------------------------------------------------------------------
}}
// eof
