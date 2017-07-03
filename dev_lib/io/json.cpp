// (qt_lib)

#include "json.hpp"
#include <dev_lib/defs.inc>

namespace l {
//------------------------------------------------------------------------------

Json::Val::Val(Typ typ_) : typ(typ_) {}
Json::Val::~Val() {}

static void space(std::ostream& os, uint indent) {
  for_i_(indent * 2)
    os << ' ';
}

static str INF_P("+inf"), INF_M("-inf");

void Json::ValNum::saveTo(std::ostream& os, uint indent) const {
  EXPECT_(!isnan(val))
  space(os, indent);

  if (isinf(val))
    os << (val ? INF_P : INF_M);
  else
    os << val;
}

void Json::ValStr::saveTo(std::ostream& os, uint indent) const {
  space(os, indent);

  /* The original https://github.com/dropbox/json11
   * also properly escapes \b, \f, \r, \t and unicode;
   * we don't
   */
  os << '"';
  for_i_(val.size()) {
    auto& c = val.at(i);
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

void Json::ValVec::saveTo(std::ostream& os, uint indent) const {
  space(os, indent);
  os << "[\n";

  uint i = 0;
  for (auto& v : val) {
    if (0 < i++)
      os << ",\n";
    v.val->saveTo(os, indent + 1);
  }

  os << "\n";
  space(os, indent);
  os << "]";
}

void Json::ValObj::saveTo(std::ostream& os, uint indent) const {
  space(os, indent);
  os << "{\n";

  uint i = 0;
  for (auto& v : val) {
    if (0 < i++)
      os << ",\n";
    space(os, indent + 1);
    os << '"' << v.first << "\":\n";
    v.second.saveTo(os, indent + 2);
  }

  os << "\n";
  space(os, indent);
  os << "}";
}

Json::Json(flt32 v)   : val(new ValNum(v)) {}
Json::Json(strc v)    : val(new ValStr(v)) {}
Json::Json(Vec::rc v) : val(new ValVec(v)) {}
Json::Json(Obj::rc v) : val(new ValObj(v)) {}

int Json::asInt() const {
  EXPECT_(NUM == val->typ)
  return l::round((*static_cast<ValNum const*>(val.ptr())).val);
}

flt32 Json::asFlt() const {
  EXPECT_(NUM == val->typ)
  return (*static_cast<ValNum const*>(val.ptr())).val;
}

strc Json::asStr() const {
  EXPECT_(STR == val->typ)
  return (*static_cast<ValStr const*>(val.ptr())).val;
}

Json::Vec::rc Json::asVec() const {
  EXPECT_(VEC == val->typ)
  return (*static_cast<ValVec const*>(val.ptr())).val;
}

Json::Obj::rc Json::asObj() const {
  EXPECT_(OBJ == val->typ)
  return (*static_cast<ValObj const*>(val.ptr())).val;
}

void Json::saveTo(std::ostream& os, uint indent) const {
  val->saveTo(os, indent);
  os << "\n";
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

  char d = is.peek();
  l::err(CAT("json: expecting '", c, "', have '", d, "'"));
}

Json Json::loadFrom(std::istream& is) may_err {
  skipSpace(is);

  char c = is.peek();
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

  err("bad json");
}

Json Json::loadNum(std::istream& is) may_err {
  int sig = 0;
  if (isMatch(is, '+'))
    sig = +1;
  else if (isMatch(is, '-'))
    sig = -1;

  if (std::isdigit(is.peek())) {
    flt32 v; is >> v;
    check(is);
    return Json((0==sig ? +1 : sig) * v);
  }

  if ((0 != sig) && isMatch(is, 'i') && isMatch(is, 'n') && isMatch(is, 'f'))
    return Json(sig * flt_inf);

  err("json: bad number");
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

dcl_(TestInt)
  atr_(int, val);
  TestInt(int val_) : val(val_) {}

  mth_(l::Json, toJson, ()) VAL_(l::Json(val))
dcl_end

TEST_("json",
  Json::Obj obj; obj.add("a", Json(2));
  Json::Vec vec({Json(8), Json(flt_inf), Json("f"), Json(8)});

  Json::Obj obj2;
  obj2.add("obj", Json(obj)); obj2.add("vec", Json(vec));

  Json json(obj2);

  std::stringstream ss;
  json.saveTo(ss);
  TR_(ss.str())

  Json json2 = Json::loadFrom(ss);
  std::stringstream ss2;
  json2.saveTo(ss2);
  TR_(ss2.str())

  CHECK_EQ(ss.str(), ss2.str());
)

//------------------------------------------------------------------------------
}
// eof

