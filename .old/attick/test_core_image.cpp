//#include "test_core_image.h"
//REGISTER_TEST_SUITE(TestCoreImage)

//#include "types/typ_image.h"

//void TestCoreImage::testImage(uint w, uint h) {
//  // Testing if data is corectly added and can be accessed again
//  uint const specialI = qMin(2u,w-1), specialJ = qMin(4u,h-1); // that's 42!

//  ENSURE(specialI<w && specialJ<h)

//  float inten = 1.24f, specialInten = 3*inten;

//  core::size2d size(w,h);
//  typ::Image im;
//  im.fill(inten,size);

//  uint pos = specialI + w*specialJ;

//  auto data = im.intensData();
//  QCOMPARE(im.count(),w*h);

//  for_i (im.count())
//    QCOMPARE(*data++,inten);

//  im.setInten(pos,specialInten);
//  QCOMPARE(*(im.intensData()+pos),specialInten);

//  // checking if intensities are correct
//  for_ij(w, h) {
//    if (i == specialI && j == specialJ)
//      QCOMPARE(im.at(i, j), specialInten);
//    else
//      QCOMPARE(im.at(i, j), inten);
//  }
//}

//void TestCoreImage::testImage() {
//  testImage(10,100);
//  testImage(256,256);
//  testImage(256,25);
//}

//// eof
