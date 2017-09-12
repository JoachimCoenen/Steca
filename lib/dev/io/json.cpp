// (qt_lib)

#include "json.hpp"
#include "../inc/defs.inc"
#include "../typ/ij.hpp"
#include "../typ/xy.hpp"

namespace l_io {
//------------------------------------------------------------------------------

namespace key {
str const
  I, J, X, Y;
}

//------------------------------------------------------------------------------

Json::indent_t::indent_t() : depth(0), newLine(false) {}

Json::indent_t Json::indent_t::next(bool newLine) const {
  indent_t nextLevel(*this);
  ++mut(nextLevel.depth);
  if (newLine)
    mut(nextLevel.newLine) = true;
  return nextLevel;
}

Json::Val::Val(Typ typ_) : typ(typ_) {}
Json::Val::~Val() {}

static void space(std::ostream& os, Json::indent_t& indent, bool force = false) {
  if (indent.newLine || force) {
    os << "\n";
    for_i_(indent.depth * 2)  // indent by 2 spaces
      os << ' ';
    mut(indent.newLine) = false;
  }
}

static str INF_P("+inf"), INF_M("-inf");

void Json::ValNum::saveTo(std::ostream& os, indent_t indent) const {
  EXPECT_(!l::isnan(val))
  space(os, indent);

  if (l::isinf(val))
    os << (0 <= val ? INF_P : INF_M);
  else
    os << val;
}

void Json::ValStr::saveTo(std::ostream& os, indent_t indent) const {
  space(os, indent);

  /* The original https://github.com/dropbox/json11
   * also properly escapes \b, \f, \r, \t and unicode;
   * we don't
   */
  os << '"';
  for_i_(val.size()) {
    auto&& c = val.at(i);
    switch (c) {
    case '\\':
      os << "\\\\";
      break;
    case '"':
      os << "\\\"";
      break;
    case '\n':
      os << "\\n";
      break;
    default:
      os << c;
    }
  }
  os << '"';
}

void Json::ValVec::saveTo(std::ostream& os, indent_t indent) const {
  space(os, indent);
  os << "[";

  uint i = 0;
  for (auto&& v : val) {
    if (0 < i++)
      os << ",";
    v.val->saveTo(os, indent.next(true));
  }

  space(os, indent, true);
  os << "]";
}

void Json::ValObj::saveTo(std::ostream& os, indent_t indent) const {
  space(os, indent);
  os << "{";

  uint i = 0;
  for (auto&& v : val) {
    if (0 < i++)
      os << ",";

    auto indent1 = indent.next(true);
    space(os, indent1);
    os << '"' << v.first << "\": ";
    v.second.saveTo(os, indent1);
  }

  space(os, indent, true);
  os << "}";
}

//------------------------------------------------------------------------------

Json::~Json() {}

Json::Json(Typ typ) : val(nullptr) {
  Val *v;
  switch (typ) {
  case NUM:
    v = new ValNum(0);
    break;
  case STR:
    v = new ValStr(str::null);
    break;
  case VEC:
    v = new ValVec(Vec());
    break;
  case OBJ:
    v = new ValObj(Obj());
    break;
  }

  mut(val).reset(v);
}

Json::Json(flt32 v)   : val(new ValNum(v)) {}
Json::Json(strc v)    : val(new ValStr(v)) {}
Json::Json(Vec::rc v) : val(new ValVec(v)) {}
Json::Json(Obj::rc v) : val(new ValObj(v)) {}

Json::Json(strc key, Json::rc val_) : val() {
  Obj obj; obj.add(key, val_);
  mut(val).reset(new ValObj(obj));
}

Json::ref Json::add(strc key, rc that) {
  check_or_err_(OBJ == val->typ, "json: bad type");
  auto&& thisObj = (*static_cast<ValObj const*>(val.ptr())).val;
  mut(thisObj).add(key, that);

  RTHIS
}

Json::ref Json::add(rc that) may_err {
  check_or_err_(VEC == val->typ, "json: bad type");
  auto&& vec = (*static_cast<ValVec const*>(val.ptr())).val;
  mut(vec).add(that);
  RTHIS
}

Json Json::operator+(rc that) const may_err {
  Json plus(*this);

  check_or_err_(OBJ == plus.val->typ, "json: bad type");

  check_or_err_(OBJ == that.val->typ, "json: bad type");
  auto&& thatObj = (*static_cast<ValObj const*>(that.val.ptr())).val;

  for (auto it = thatObj.begin(); it != thatObj.end(); ++it)
    plus.add(it->first, it->second);

  return plus;
}

sz_t Json::size() const may_err {
  check_or_err_(VEC == val->typ, "json: bad type");
  auto&& vec = (*static_cast<ValVec const*>(val.ptr())).val;
  return vec.size();
}

Json::rc Json::at(uint i) const may_err {
  check_or_err_(VEC == val->typ, "json: bad type");
  auto&& vec = (*static_cast<ValVec const*>(val.ptr())).val;
  return vec.at(i);
}

Json::rc Json::at(strc key) const may_err {
  check_or_err_(OBJ == val->typ, "json: bad type");
  auto&& obj = (*static_cast<ValObj const*>(val.ptr())).val;
  return obj.at(key);
}

int Json::asInt() const may_err {
  check_or_err_(NUM == val->typ, "json: bad type");
  return l::to_int(l::round((*static_cast<ValNum const*>(val.ptr())).val));
}

uint Json::asUint() const may_err {
  int i = asInt();
  check_or_err_(0 <= i, "json: bad number");
  return l::to_uint(i);
}

flt32 Json::asFlt() const may_err {
  check_or_err_(NUM == val->typ, "json: bad type");
  return (*static_cast<ValNum const*>(val.ptr())).val;
}

strc Json::asStr() const may_err {
  check_or_err_(STR == val->typ, "json: bad type");
  return (*static_cast<ValStr const*>(val.ptr())).val;
}

Json::Vec::rc Json::asVec() const may_err {
  check_or_err_(VEC == val->typ, "json: bad type");
  return (*static_cast<ValVec const*>(val.ptr())).val;
}

Json::Obj::rc Json::asObj() const may_err {
  check_or_err_(OBJ == val->typ, "json: bad type");
  return (*static_cast<ValObj const*>(val.ptr())).val;
}

void Json::saveTo(std::ostream& os) const {
  val->saveTo(os, indent_t());
  os << "\n";
}

void Json::saveTo(std::ostream& os, indent_t indent) const {
  val->saveTo(os, indent);
}

static void check(std::istream& is) may_err {
  check_or_err_(!is.bad(), "bad json");
}

static void skipSpace(std::istream& is) {
  while (std::isspace(is.peek()))
    is.ignore();
  check(is);
}

static char getc(std::istream& is) may_err {
  char c; is >> c;
  check(is);
  return c;
}

static bool isMatch(std::istream& is, char c) may_err {
  skipSpace(is);
  if (c != is.peek())
    return false;

  is.ignore();
  return true;
}

static void match(std::istream& is, char c) may_err {
  if (isMatch(is, c))
    return;

  auto d = is.peek();
  l::err(CAT("json: expecting '", c, "', have '", d, "'"));
}

Json Json::loadFrom(std::istream& is) may_err {
  skipSpace(is);

  auto c = is.peek();
  if (std::isdigit(c))
    return loadNum(is);

  switch (is.peek()) {
  case '+': case '-':
    return loadNum(is);
  case '"':
    return loadStr(is);
  case '[':
    return loadVec(is);
  case '{':
    return loadObj(is);
  }

  l::err("bad json");
}

Json Json::loadNum(std::istream& is) may_err {
  bool neg = false;
  if (isMatch(is, '+'))
    ; // eat +
  else if (isMatch(is, '-'))
    neg = true;

  if (std::isdigit(is.peek())) {
    flt32 v; is >> v;
    check(is);
    return Json(neg ? -v : v);
  }

  if (isMatch(is, 'i') && isMatch(is, 'n') && isMatch(is, 'f'))
    return Json(flt32(neg ? -l::flt_inf : +l::flt_inf));

  l::err("json: bad number");
}

Json Json::loadStr(std::istream& is) may_err {
  match(is, '"');

  str v;
  for (;;) {
    char c = getc(is);
    if ('"' == c)
      break;
    if ('\\' == c)
      v += ('n' == (c = getc(is))) ? '\n' : c;
    else
      v += c;
  }

  return Json(v);
}

Json Json::loadVec(std::istream& is) may_err {
  match(is, '[');

  Vec vec;
  if (!isMatch(is, ']')) {
    do
      vec.add(loadFrom(is));
    while (isMatch(is, ','));
    match(is, ']');
  }

  return Json(vec);
}

Json Json::loadObj(std::istream& is) may_err {
  match(is, '{');

  Obj obj;
  if (!isMatch(is, '}')) {
    do {
      str key = loadStr(is).asStr();
      match(is, ':');
      obj.add(key, loadFrom(is));
    } while (isMatch(is, ','));
    match(is, '}');
  }

  return Json(obj);
}

//------------------------------------------------------------------------------

Json::Json(l::ij::rc v) : Self(OBJ) {
  add(key::I, Json(flt32(v.i)));
  add(key::J, Json(flt32(v.j)));
}

l::ij Json::asIJ() const {
  return l::ij(at(key::I).asInt(), at(key::I).asInt());
}

Json::Json(l::xy::rc v) : Self(OBJ) {
  add(key::X, Json(flt32(v.x)));
  add(key::Y, Json(flt32(v.y)));
}

l::xy Json::asXY() const {
  return l::xy(at(key::X).asFlt(), at(key::Y).asFlt());
}

//------------------------------------------------------------------------------

TEST_("json",
  Json::Obj obj; obj.add("a", Json(2));
  Json::Vec vec({Json(8), Json(l::flt_inf), Json("f"), Json(8)});

  Json::Obj obj2;
  obj2.add("obj", Json(obj)); obj2.add("vec", Json(vec));

  Json json(obj2);

  std::stringstream ss;
  json.saveTo(ss);

  Json json2 = Json::loadFrom(ss);
  std::stringstream ss2;
  json2.saveTo(ss2);

  CHECK_EQ(ss.str(), ss2.str());
)

//------------------------------------------------------------------------------
}
// eof
