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
#include <dev_lib/defs.inc>
#include <dev_lib/io/fin.hpp>

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


static void loadTiff(File& file, l_io::path::rc path,
                     phi_t phi, flt32 mon, flt32 tim) may_err {
  auto md = l::scope(new Meta(file.files.dict));

  mut(md->phi) = phi;
  mut(md->mon) = mon;
  mut(md->tim) = tim;

  l_io::fbin fin(path);

  // see http://www.fileformat.info/format/tiff/egff.htm


//  QDataStream is(&f);
//  is.setFloatingPointPrecision(QDataStream::SinglePrecision);

//  auto check = [&is]() {
//    check_or_err_(QDataStream::Ok == is.status(), "could not read data");
//  };

  // magic
  auto magic = fin.get16();

  if (0x4949 == magic)      // II - intel
    mut(fin.endianness) = l_io::endian::le;
  else if (0x4d4d == magic) // MM - motorola
    mut(fin.endianness) = l_io::endian::be;
  else
    l::err("bad magic");

  auto version = fin.get16();
  check_or_err_(42 == version, "bad version");

  uint32 imageWidth    = 0, imageHeight  = 0,
         bitsPerSample = 0, sampleFormat = 0,
         rowsPerStrip  = 0xffffffff,
         stripOffsets  = 0, stripByteCounts = 0;

  uint16 tagId, dataType;
  uint32 dataCount, dataOffset;

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

    l::err("not a simple number");
  };

  auto asStr = [&]() {
    check_or_err_(2==dataType, "bad data type");
    auto lastPos = fin.tell();
    fin.seek(dataOffset);
    auto data = fin.read(dataCount);
    fin.seek(lastPos);
    return data.asStr();
  };

  auto dirOffset = fin.get32();
  fin.seek(dirOffset);

  auto numDirEntries = fin.get16();

  for_i_(numDirEntries) {
    tagId      = fin.get16();
    dataType   = fin.get16();
    dataCount  = fin.get32();
    dataOffset = fin.get32();

    switch (tagId) {
    // numbers
    case 256:
      imageWidth    = asUint();
      break;
    case 257:
      imageHeight   = asUint();
      break;
    case 258:
      bitsPerSample = asUint();
      break;
    case 259: // Compression
      check_or_err_(1==asUint(), "compressed data not supported");
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
      mut(md->comment) = asStr();
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

  l::sz2 size(imageWidth, imageHeight);

  uint count = imageWidth * imageHeight;
  count_arr2 intens(size);

  check_or_err_((bitsPerSample/8) * count == stripByteCounts, "bad format");

  fin.seek(stripOffsets);

  for_i_(intens.size().size())
    switch (sampleFormat) {
    case 1: {
      uint32 sample = fin.get32();
      intens.setAt(i, sample);
      break;
    }
    case 2: {
      int32 sample = (int32)fin.get32();
      intens.setAt(i, sample);
      break;
    }
    case 3: {
      flt32 sample = fin.getflt32();
      intens.setAt(i, sample);
      break;
    }
    }

  file.addSet(
    l::share(new Set(
      l::share(md.take().ptr()),
      l::share(new Image(intens)))));
}

////------------------------------------------------------------------------------

File::sh loadTiffDat(Files& files, l_io::path::rc path) may_err {
//  File::sh file(new File(files, path.basename()));

//  QFile f(path.c_str());
//  check_or_err_(f.open(QFile::ReadOnly), "cannot open file");

//  QFileInfo info(path.c_str());
//  QDir dir = info.dir();

//  QByteArray line;
//  while (!(line = f.readLine()).isEmpty()) {
//    QString s(line);

//    // cut off comment
//    int commentPos = s.indexOf(';');
//    if (commentPos >= 0)
//      s = s.left(commentPos);

//    // split to parts
//    if ((s = s.simplified()).isEmpty())
//      continue;

//    auto lst = s.split(' ');
//    auto cnt = lst.count();
//    check_or_err_(2 <= cnt && cnt <= 4, "bad metadata format");

//    // file, phi, monitor, expTime
//    bool ok;
//    QString tiffFileName = lst.at(0);
//    auto phi = phi_t(lst.at(1).toFloat(&ok));
//    check_or_err_(ok, "bad phi value");

//    qreal monitor = 0;
//    if (cnt > 2) {
//      monitor = lst.at(2).toDouble(&ok);
//      check_or_err_(ok, "bad monitor value");
//    }

//    qreal expTime = 0;
//    if (cnt > 3) {
//      expTime = lst.at(3).toDouble(&ok);
//      check_or_err_(ok, "bad expTime value");
//    }

//    try {
//      // load one dataset
//      loadTiff(mut(*file), l_qt::fromQt(dir.filePath(tiffFileName)), phi, monitor, expTime);
//    } catch (std::exception &e) {
//      l::err(CAT(l_qt::fromQt(tiffFileName), ": ", e.what()));
//      throw;
//    }
//  }

//  return file;
}

TEST_("loadTiff",
  Files files;
  File::sh file(new File(files, ""));
  loadTiff(mut(*file), l_io::path("testdata.tif"), phi_t(0), 0, 0);
)

//------------------------------------------------------------------------------
}}
// eof
