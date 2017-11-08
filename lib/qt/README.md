# development library (Qt-specific)

folder/file           | purpose
---                   | ---
[inc/](inc/)          | frequently used code
[typ/](typ/)          | types for specific purposes
[io/](io/)            | input/output-related types
[test/](test/)        | support for (in code embedded) unit tests
[defs.hpp](defs.hpp)  | virtually *every* C++ header in a project should include this file

# app_lib - application development library [↩](../nav.md)

Definitions, declarations, types, etc. for building a C++/Qt application.
Nothing is Steca-specific. The library can be reused for other projects.

* [[inc]](inc/doc.md) - common include files: definitions, number and pointer types
* [[typ]](typ/doc.md) - useful types
* [[qt]] (qt/doc.md)  - Qt-related
