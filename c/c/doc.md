# c/c - (c)ommon library, C-related [↩](../doc.md)

* [[lib]](lib/doc.md) - fundamentals
* [[test]](test/doc.md) - unit&integration test support

file    | purpose
--------|--------
c.h     | The most important definitions. Include everywhere (indirectly).
c_def.h | Data defining macros.

### Terminology:
* there are data *struct*ures
* structures have *attribute*s (= data members)
* structures may have *method*s (= member functions)

### Shortcuts in text:
* `[C]` denotes C context
* `[C++]`denotes C++ context

## Data structures
Defined in [C] for export, extended in [C++] for implementation.

### In a typical data structure:
1. Attributes are public and immutable.
2. [C++] Methods are public.
3. [C++] Inheritance is not to be used to add attributes, only methods.

Exceptions, if made, should have a clear purpose.

The collaterals and consequences of the above are:
1.
    * Use `struct`, not `class`.
    * No need to state `public:`.
    * There is no need for getters or setters.
    * Structures can be safely-ish shared among many readers.
    * It must be explicitly stated when an attribute value is modified;
      that should be typically done before the structure is shared.
2.
    * No need to state `public:`.
    * Most methods are to be `const`.
    * Even non-const methods must explicitly state when an attribute is modified.
3.
    * Prefer data composition.
    * If the access to an attribute or a method needs to be restricted,
      prefer `private:`.

## Declaration of data structures:

The standard declaration of a data structure follows this pattern:

1. Declare [C] data layout with POD attributes. (POD: primitive types and pointers.)
2. Subclass [C++] and add methods.

Always use a namespace. In [C], the namespace is prefixed to the structure name.
In [C++], the namespace is used directly.

### Header files

In modules, where [C] and [C++] are mixed:
* `.h` files can be included in [C] and [C++].
* `.hpp` files are meant to be included only in [C++].

In [C++]-only modules, `.h` may be used for [C++]-only, as usual.

### Interfaces

In the mixed [C/C++] context, [C] interface `.i` files are generated from the
corresponding `.h` files. Therefore care is to be taken to isolate their [C++]
segments. The standard declaration pattern uses macros that do that.

## The standard structure declaration
(See [mem.h](lib/str.h) for details.)

1. Include `c_def.h`; typedef [C] POD as needed.

```C
#include <c/c/c_def.h>

typedef char*       pstr;
typedef char const* pcstr;
```

2. Specify the namespace and the data structure name (the trailing underscores
make the names stand out):
```C
#define NS___ c
#define DS___ str
```

3. Declare the [C] layout, using (only) these macros:
  * `_c_struct` to start
  * `_c_struct_end` to end
  * `_atr(typ, name)` to declare a POD attribute
  * `_ptr(typ, name)` to declare a pointer
  * `_c_con(args)` to declare a [C++] constructor
```C
_c_struct
  _atr(sz_t, sz)
  _ptr(void, p)

  _c_con(sz_t, pcvoid)
_c_struct_end
```
Note:
  * We use two-space indentation.
  * Data declaration precedes that of methods.
  * `_c_con` macro (rather than `_con`) declares a constructor. The constructor
    code will be emitted only if the header files is processed in [C++].
  * Since this is a [C] structure, there is no destructor;

4. Subclass ("substructure") the [C] structure, and add constructors,
   a destructor, methods, operators, ... using these macros:
   * `_cpp_struct` to start
   * `_cpp_struct_end` to end
   * `_con(args)` for a constructor
   * `_des()` for a destructor
   * `_mth(type, name, args)` for a const method (`args` must contain parentheses)
   * `_mth_mut(type, name, args)` for a non-const (mutating) method
   * `_op(name)` for a const operator
   * `_op_mut(name)` for a mutating operator
   * `_op_inline(name, expr)` for a (const) inline operator (a kind of a getter)
   * `_fry(type, name, args)` for a static method (aka *factory*)
   * `_cst(type, name)` for a static attribute (constant)

The opening `_cpp_struct` conveniently declare these typedefs:
  * `typ` for the structure itself
  * `rc` as a const reference
  * `ref` as a non-const reference
  * `rval` as a r-value (or universal) reference
  * and an alias `c_base` for the base [C] structure.

Optionally put after the opening `_cpp_struct` these convenience macros:
  * `NO_MOVE` to disallow moving
  * `NO_COPY` to disallow copying
  * `NO_GEN` to disallow both moving and copying
  * `COMPARABLE` to declare `int compare(rc) const`
  * `EQ_NE` to declare `==` and `!=` operators
  * `LGTE` to declare `<`, `<=`, `>`, and `>=` operators
  * `COMP_OPS` as a shortcut for `EQ_NE LGTE`

Since any [C++] code needs to be guarded - excluded from [C], use macros:
  * `_cpp_private` to enter `private:` into the declaration
  * `_cpp_code(...)` to guard any other [C++]-only code
  * or `#if _is_cpp_` ... `#endif`

```C
_cpp_struct COMPARABLE COMP_OPS
  _con()
  _con(pcstr)
  _con(sz_t maxSz, pcstr)
  _con(rc)
  _con(rval)
  _des()

  _mth(bool, empty, ())
  _mth(bool, eq,    (pcstr))

  _mth_mut(rc, set, (pcstr))
  _mth_mut(rc, set, (rval))

  _op_inline (pcstr(), p)

  _mth(str, trim, ())
  _fry(str, cat,  (pcstr, pcstr))

  _cst(str, nul)
_cpp_struct_end

_cpp_code(typedef c::str::rc strc;)

_cpp_code(
namespace c { namespace unsafe {
str str_frm(pcstr, ...);
str str_cat(pcstr, ...);
}})
```

5. Finally, undef the namespace and structiúre names:
```C
#undef DS___
#undef NS___
```
♦
