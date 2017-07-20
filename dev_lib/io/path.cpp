// (dev_lib)

#include "path.hpp"
#include "../typ/buf.hpp"

#ifdef _WIN32
  #include <stdlib.h>
#else
  #include <libgen.h>
  #include <unistd.h>
#endif


namespace l_io {
//------------------------------------------------------------------------------

path::path(strc s) : base(s) {}

str path::filename() const {
#ifdef _WIN32
  _splitpath
  // return str(std::tr2::sys::path(*this).stem().generic_string().c_str());
#else
  l::buf copy(c_str());
  return str(basename(copy.data()));
#endif
}

path path::dir() const {
#ifdef _WIN32
  _splitpath
  // return str(std::tr2::sys::path(*this).stem().generic_string().c_str());
#else
  l::buf copy(c_str());
  return path(dirname(copy.data()));
#endif
}

path path::absolute() const {
  l::buf dir(FILENAME_MAX);
#ifdef _WIN32
  return path(_realpath(c_str(), dir.data()));
#else
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
extern int chdir (const char *__path) __THROW __nonnull ((1)) __wur;
