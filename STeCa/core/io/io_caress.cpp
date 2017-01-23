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
#include <c/c/cpp>
#include <c/c/lib/num.h>
#include <c/c/lib/str.h>
#include <c/c/lib/mem.h>
#include "Caress/raw.i"
#include <string.h>

namespace core { namespace io {
//------------------------------------------------------------------------------

void loadCaress(data::Files& files, strc filePath) may_exc {
  using c::str;

  struct _close_data_file { ~_close_data_file() { close_data_file(); } } _auto_close;
  if (0 /*OK*/ != open_data_file(filePath,nullptr))
    err("Cannot open ", filePath);

  modname_t element, node;

  int32 e_number, // element number
        e_type,   // element type
        d_type,   // data type
        d_number; // data length (?)

  auto gds = [&]() -> str {
    if (d_number < 1)
      c::err("bad d_number: ", element);
    c::mem m(c::to_u(d_number) + 1);

    if (0 != get_data_unit(mut(m.p)))
      c::err("bad: ", "string data");

    return str(pcstr(m.p));
  };

  auto gdf = [&]() -> float {
    float f;
    if (0 != get_data_unit(&f))
      c::err("bad: ", "float data");
    return f;
  };

  bool newObject = false, workAfterStep = false, collectData = false;
  str s_masterCounter, s_date, s_comment;

  float tthAxis = 0, omgAxis = 0, chiAxis = 0, phiAxis = 0;
  bool  isRobot = false, isTable = false;

  auto checkRobot = [&]() {
    if (isTable)
      c::err("bad: ", "already have table");
    isRobot = true;
  };

  auto checkTable = [&]() {
    if (isRobot)
      c::err("bad: ", "already have robot");
    isTable = true;
  };

  for (;;) {
    auto resNextUnit = next_data_unit(&e_number, &e_type, element, node, &d_type, &d_number);
    if (2 /*END_OF_FILE_DETECTED*/ == resNextUnit)
      break;
    if (0 /*OK*/ != resNextUnit)
      err("Error in ", filePath);

//    WT(element << node << e_number << e_type << d_type << d_number)

    str el(str(8, element).trim()); WT(el)
    str en(str(8, node).trim());    WT(en)

    if (el.eq("MM1")) {
      s_masterCounter.set(node);
      continue;
    }

    if (el.eq("COM")) {
      s_comment.set(gds());
      continue;
    }

    if (el.eq("DATE")) {
      s_date.set(gds());
      continue;
    }

    if (el.eq("READ")) {
      if (en.empty())
        err("empty node for: ", el);

      float f = gdf();

      mut(*files.dict).add(en);

      if (en.eq("TTHS")) {
        checkTable(); tthAxis = f;
      } else
      if (en.eq("OMGS")) {
        checkTable(); omgAxis = f;
      } else
      if (en.eq("CHIS")) {
        checkTable(); chiAxis = f;
      } else
      if (en.eq("PHIS")) {
        checkTable(); phiAxis = f;
      } else
      if (en.eq("TTHR")) {
        checkRobot(); tthAxis = f;
      } else
      if (en.eq("OMGR")) {
        checkRobot(); omgAxis = f;
      } else
      if (en.eq("CHIR")) {
        checkRobot(); chiAxis = f;
      } else
      if (en.eq("PHIR")) {
        checkRobot(); phiAxis = f;
      };
      continue;
    }

    if (el.eq("SETVALUE")) {
      workAfterStep = en.eq("STEP");
      // new dataset
      if (workAfterStep) {
        if (!collectData) {
          collectData = true;
        } else {
          newObject = true;
        }
      } else {
        if (collectData) {
          collectData = false;
          newObject = true;
        }
      }
    }
  }

  //  data::shp_File file(new data::File(filePath));
  //  return file;
  //}
}

//------------------------------------------------------------------------------
}}
// eof


      //  bool newObject = false;
      //  bool workAfterStep = false;
      //  bool collectData = false;
      //  std::string s_masterCounter;
//  bool isRobot = false, isTable = false;

//  float tthAxis = 0, omgAxis = 0, phiAxis = 0, chiAxis = 0, tths = 0, omgs = 0, chis = 0,
//      phis = 0, tthr = 0, omgr = 0, chir = 0, phir = 0, xAxis = 0, yAxis = 0, zAxis = 0,
//      pstAxis = 0, sstAxis = 0, omgmAxis = 0;

//  int *intens = NULL;
//  uint imageSize = 0;

//  int mon = 0, tim1 = 0;

//  std::string s_date, s_comment;
//  char* c_comment;
//  double deltaTime = 0;
//  double prevTempTime = 0;
//  int deltaMon = 0;
//  int prevMon = 0;

//  bool end = false;
//  while (!end) {
//    int e_number, e_type, d_type, d_number;
//    char element[100];
//    element[99] = '\0';
//    char node[100];
//    node[99] = '\0';
//    // Return of next_data_unit 0=OK; 1=Error; 2=End of File
//    int returnNextDataUnit =
//        next_data_unit(&e_number, &e_type, element, node, &d_type, &d_number);
//    element[8] = '\0';
//    node[8] = '\0';

//    if (returnNextDataUnit == 1) // Error
//      continue;

//    if (returnNextDataUnit == 2) { // EOF
//      newObject = true;
//      end = true; // terminate while-loop after treating the current data unit
//    }

//    if (!strncmp(element, "SETVALUE", 8)) {
//      // SETVALUE STEP is mark for a new Dataset!
//      if (!strncmp(node, "STEP  ", 6))
//        workAfterStep = true;

//      if (workAfterStep) {
//        if (!strncmp(node, "STEP  ", 6)) {
//          if (!collectData) {
//            collectData = true;
//          } else {
//            newObject = true;
//          }
//        }
//      } else {
//        if (collectData) {
//          collectData = false;
//          newObject = true;
//        }
//      }
//    }

//    if (!workAfterStep) {
//      if (!strncmp(element, "MASTER1V", 8)) {
//        if (!strncmp(node, s_masterCounter.c_str(), 6)) {
//          if (!collectData)
//            collectData = true;
//          else
//            newObject = true;
//        }
//      }
//    }

//    if (newObject) {
//      newObject = false;

//      if (isRobot && isTable) {
//        if (tthr == 0 && tths != 0) // it's the robot
//          isRobot = false;
//        else if (tths == 0 && tthr != 0) // it's the table
//          isTable = false;
//        else
//          throw "inconsistent data set: not clear whether it's robot or table";
//      }

//      if (isRobot) {
//        // Mapping to euler angle and pos
//        tthAxis = tthr;
//        omgAxis = omgr;
//        chiAxis = 180 - chir;
//        phiAxis = phir;
//      }
//      if (isTable) {
//        // Mapping to euler angle and pos
//        tthAxis = tths;
//        omgAxis = omgs;
//        chiAxis = chis;
//        phiAxis = phis;
//      }

//      // Objekt inizialisieren
//      if (intens != NULL) {
//        int d, y;
//        char c_m[5];
//        std::string s_m;
//        std::istringstream istr(s_date);
//        istr >> d;
//        istr.ignore();
//        istr.get(c_m, 4);
//        istr.ignore();
//        istr >> y;
//        s_m = c_m;

//        double tempTime = 0;
//        if ((y < 2015) ||
//            ((y == 2015) && ((s_m.compare("Jan") == 0) || (s_m.compare("Feb") == 0)))) {
//          tempTime = double(tim1) / 100; // HACK REVIEW how deltaTime is used!!
//        } else {
//          tempTime = double(tim1);
//        }
//        deltaTime = tempTime - prevTempTime;
//        prevTempTime = tempTime;
//        deltaMon = mon - prevMon;
//        prevMon = mon;

//        uint detRel;

//        detRel = to_u(qRound(sqrt(imageSize)));
//        RUNTIME_CHECK(imageSize>0 && imageSize == detRel*detRel, "bad image size");

//        inten_vec convertedIntens(imageSize);
//        for_i (imageSize)
//          convertedIntens[i] = intens[i];

//        typ::size2d size(detRel,detRel);

//        // this is only for testing of a non-square image
//        // size.rheight() /= 2;

//        // Objekt inizialisieren
//        data::Metadata md;

//        md.date    = str::fromStdString(s_date);
//        md.comment = str::fromStdString(s_comment);

//        md.motorXT = xAxis;
//        md.motorYT = yAxis;
//        md.motorZT = zAxis;

//        md.motorOmg  = omgAxis;
//        md.motorTth  = tthAxis;
//        md.motorPhi  = phiAxis;
//        md.motorChi  = chiAxis;
//        md.motorPST  = pstAxis;
//        md.motorSST  = sstAxis;
//        md.motorOMGM = omgmAxis;

//        md.deltaMonitorCount = deltaMon;
//        md.monitorCount = mon;
//        md.deltaTime    = deltaTime;
//        md.time         = tempTime;

//        file->datasets().append(
//          data::shp_OneDataset(new data::OneDataset(md, size, convertedIntens))
//        );

//        delete[] intens; intens = NULL;
//        imageSize = 0;
//      }
//    }

        //    // Read Master Counter
        //    if (!strncmp(element, "MM1 ", 4)) {
        //      s_masterCounter = node; // Master Counter steht in Node
        //    }
        //    if (!strncmp(element, "COM ", 4)) {
        //      c_comment = new char[d_number + 1];
        //      if (get_data_unit(c_comment) != 0)
        //        s_comment = "no comment";
        //      else {
        //        c_comment[d_number] = '\0'; // terminiere Char-Array
        //        s_comment = c_comment;
        //      }
        //      delete[] c_comment;
        //    }
        //    if (!strncmp(element, "DATE ", 5)) {
        //      char* c_date = NULL;
        //      c_date = new char[d_number + 1];
        //      if (get_data_unit(c_date) != 0)
        //        s_date = "unknown";
        //      else {
        //        c_date[d_number] = '\0'; // terminiere Char-Array
        //        s_date = c_date;
        //      }
        //      delete[] c_date;
        //    }
//    if (!strncmp(element, "READ  ", 6)) {
//      if (!strncmp(node, "TTHS  ", 6)) {
//        if (get_data_unit(&tths) != 0)
//          tths = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "TTHR  ", 6)) {
//        if (get_data_unit(&tthr) != 0)
//          tthr = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "OMGS  ", 6)) {
//        if (get_data_unit(&omgs) != 0)
//          omgs = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "OMGR  ", 6)) {
//        if (get_data_unit(&omgr) != 0)
//          omgr = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "CHIS  ", 6)) {
//        if (get_data_unit(&chis) != 0)
//          chis = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "CHIR  ", 6)) {
//        if (get_data_unit(&chir) != 0)
//          chir = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "PHIS  ", 6)) {
//        if (get_data_unit(&phis) != 0)
//          phis = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "PHIR  ", 6)) {
//        if (get_data_unit(&phir) != 0)
//          phir = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "XT    ", 6))
//        if (get_data_unit(&xAxis) != 0)
//          xAxis = 0;
//      if (!strncmp(node, "XR    ", 6))
//        if (get_data_unit(&xAxis) != 0)
//          xAxis = 0;
//      if (!strncmp(node, "YT    ", 6))
//        if (get_data_unit(&yAxis) != 0)
//          yAxis = 0;
//      if (!strncmp(node, "YR    ", 6))
//        if (get_data_unit(&yAxis) != 0)
//          yAxis = 0;
//      if (!strncmp(node, "ZT    ", 6))
//        if (get_data_unit(&zAxis) != 0)
//          zAxis = 0;
//      if (!strncmp(node, "ZR    ", 6))
//        if (get_data_unit(&zAxis) != 0)
//          zAxis = 0;
//      if (!strncmp(node, "PST   ", 6))
//        if (get_data_unit(&pstAxis) != 0)
//          pstAxis = 0;
//      if (!strncmp(node, "SST   ", 6))
//        if (get_data_unit(&sstAxis) != 0)
//          sstAxis = 0;
//      if (!strncmp(node, "OMGM  ", 6))
//        if (get_data_unit(&omgmAxis) != 0)
//          omgmAxis = 0;
//    }
//    if (!strncmp(element, "SETVALUE", 8)) {
//      if (!strncmp(node, "TTHS  ", 6)) {
//        if (get_data_unit(&tths) != 0)
//          tths = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "TTHR  ", 6)) {
//        if (get_data_unit(&tthr) != 0)
//          tthr = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "OMGS  ", 6)) {
//        if (get_data_unit(&omgs) != 0)
//          omgs = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "OMGR  ", 6)) {
//        if (get_data_unit(&omgr) != 0)
//          omgr = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "CHIS  ", 6)) {
//        if (get_data_unit(&chis) != 0)
//          chis = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "CHIR  ", 6)) {
//        if (get_data_unit(&chir) != 0)
//          chir = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "PHIS  ", 6)) {
//        if (get_data_unit(&phis) != 0)
//          phis = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "PHIR  ", 6)) {
//        if (get_data_unit(&phir) != 0)
//          phir = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "XT    ", 6))
//        if (get_data_unit(&xAxis) != 0)
//          xAxis = 0;
//      if (!strncmp(node, "XR    ", 6))
//        if (get_data_unit(&xAxis) != 0)
//          xAxis = 0;
//      if (!strncmp(node, "YT    ", 6))
//        if (get_data_unit(&yAxis) != 0)
//          yAxis = 0;
//      if (!strncmp(node, "YR    ", 6))
//        if (get_data_unit(&yAxis) != 0)
//          yAxis = 0;
//      if (!strncmp(node, "ZT    ", 6))

//        if (get_data_unit(&zAxis) != 0)
//          zAxis = 0;
//      if (!strncmp(node, "ZR    ", 6))
//        if (get_data_unit(&zAxis) != 0)
//          zAxis = 0;
//      if (!strncmp(node, "PST   ", 6))
//        if (get_data_unit(&pstAxis) != 0)
//          pstAxis = 0;
//      if (!strncmp(node, "SST   ", 6))
//        if (get_data_unit(&sstAxis) != 0)
//          sstAxis = 0;
//      if (!strncmp(node, "OMGM  ", 6))
//        if (get_data_unit(&omgmAxis) != 0)
//          omgmAxis = 0;
//    }
//    if (!strncmp(element, "MASTER1V", 8)) {
//      if (!strncmp(node, "TTHS  ", 6)) {
//        if (get_data_unit(&tths) != 0)
//          tths = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "TTHR  ", 6)) {
//        if (get_data_unit(&tthr) != 0)
//          tthr = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "OMGS  ", 6)) {
//        if (get_data_unit(&omgs) != 0)
//          omgs = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "OMGR  ", 6)) {
//        if (get_data_unit(&omgr) != 0)
//          omgr = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "CHIS  ", 6)) {
//        if (get_data_unit(&chis) != 0)
//          chis = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "CHIR  ", 6)) {
//        if (get_data_unit(&chir) != 0)
//          chir = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "PHIS  ", 6)) {
//        if (get_data_unit(&phis) != 0)
//          phis = 0;
//        isTable = true;
//      }
//      if (!strncmp(node, "PHIR  ", 6)) {
//        if (get_data_unit(&phir) != 0)
//          phir = 0;
//        isRobot = true;
//      }
//      if (!strncmp(node, "XT    ", 6))
//        if (get_data_unit(&xAxis) != 0)
//          xAxis = 0;
//      if (!strncmp(node, "XR    ", 6))
//        if (get_data_unit(&xAxis) != 0)
//          xAxis = 0;
//      if (!strncmp(node, "YT    ", 6))
//        if (get_data_unit(&yAxis) != 0)
//          yAxis = 0;
//      if (!strncmp(node, "YR    ", 6))
//        if (get_data_unit(&yAxis) != 0)
//          yAxis = 0;
//      if (!strncmp(node, "ZT    ", 6))
//        if (get_data_unit(&zAxis) != 0)
//          zAxis = 0;
//      if (!strncmp(node, "ZR    ", 6))
//        if (get_data_unit(&zAxis) != 0)
//          zAxis = 0;
//      if (!strncmp(node, "PST   ", 6))
//        if (get_data_unit(&pstAxis) != 0)
//          pstAxis = 0;
//      if (!strncmp(node, "SST   ", 6))
//        if (get_data_unit(&sstAxis) != 0)
//          sstAxis = 0;
//      if (!strncmp(node, "OMGM  ", 6))
//        if (get_data_unit(&omgmAxis) != 0)
//          omgmAxis = 0;
//      if (!strncmp(node, "MON   ", 6))
//        if (get_data_unit(&mon) != 0)
//          mon = 0;
//      if (!strncmp(node, "TIM1  ", 6))
//        if (get_data_unit(&tim1) != 0)
//          tim1 = 0;
//      if (!strncmp(node, "ADET  ", 6)) {
//        if (d_type == 2) {
//          union {
//            char ch[MAXNUMBEROFCHANNELS];
//            int16 i2[MAXNUMBEROFCHANNELS];
//            int32 i4[MAXNUMBEROFCHANNELS];
//            int64 i8[MAXNUMBEROFCHANNELS];
//            float r4[MAXNUMBEROFCHANNELS];
//            double r8[MAXNUMBEROFCHANNELS];
//          } buffer;

//          intens = new int[d_number];
//          imageSize = to_u(d_number);
//          int section = 1;
//          int start_item = 1;
//          int tempNumItems, ret_val;
//          int remaining_items = d_number;
//          do /* read area detector array in MAXNUMBEROFCHANNELS blocks */
//          {
//            if (remaining_items >= MAXNUMBEROFCHANNELS)
//              tempNumItems = MAXNUMBEROFCHANNELS;
//            else
//              tempNumItems = remaining_items;
//            ret_val = get_data_partition(buffer.i4, &section, &start_item,
//                                         &tempNumItems, &d_type);
//            memmove(&intens[start_item - 1], buffer.i4, to_u(tempNumItems) * sizeof(int));
//            remaining_items -= MAXNUMBEROFCHANNELS;
//            start_item += MAXNUMBEROFCHANNELS;
//          } while (!ret_val && (remaining_items > 0));
//        }
//      }
//    }
//  }

//  return file;
//}

////------------------------------------------------------------------------------
//}
//// eof
