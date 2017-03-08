export QTDIR=/opt/qt54
export PATH=$QTDIR/bin:$PATH

git submodule update --init --recursive
mkdir build; cd build

if [ $CXX == "g++" ]; then
  cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE -D WITH_COVERAGE=1 ..
else
  cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE ..
fi

make -j9
