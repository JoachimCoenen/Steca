// (dev_lib)

#include "path.hpp"
#include "../typ/buf.hpp"

#ifdef _WIN32
  #include <stdlib.h>
  #include <direct.h>
#else
  #include <libgen.h>
  #include <unistd.h>
#endif


namespace l_io {
//------------------------------------------------------------------------------

path::path(strc s) : base(s) {}

str path::filename() const {
#ifdef _WIN32
  char fname[_MAX_FNAME], ext[_MAX_EXT];
  _splitpath(c_str(), nullptr, nullptr, fname, ext);
  char path[_MAX_PATH];
  _makepath(path, nullptr, nullptr, fname, ext);
  return path;
#else
  l::buf copy(c_str());
  return str(basename(copy.data()));
#endif
}

path path::dir() const {
#ifdef _WIN32
  char drive[_MAX_DRIVE], dir[_MAX_DIR];
  _splitpath(c_str(), drive, dir, nullptr, nullptr);
  char path[_MAX_PATH];
  _makepath(path, drive, dir, nullptr, nullptr);
  return path;
#else
  l::buf copy(c_str());
  return path(dirname(copy.data()));
#endif
}

path path::absolute() const {
#ifdef _WIN32
  char path[_MAX_PATH];
  return _fullpath(path, c_str(), _MAX_PATH);
#else
  l::buf dir(FILENAME_MAX);
  return path(realpath(c_str(), dir.data()));
#endif
}

str path::cwd() {
  l::buf dir(FILENAME_MAX);
#ifdef _WIN32
  _getcwd(dir.data(), dir.size());
#else
  getcwd(dir.data(), dir.size());
#endif
  return str(dir.data());
}

void path::cd() const {
#ifdef _WIN32
  _chdir(c_str());
#else
  chdir(c_str());
#endif
}

//------------------------------------------------------------------------------
}
// eof
