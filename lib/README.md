# development library

This is a generic C++11 application development library. It strives to be
light-weight and minimal. The fundamental philosophy is that of frugality that is the prerequisite for code
brevity and low complexity. Therefore:

* The library contains only what has been so far needed.
* If anything else is needed in the future, it shall be added at that point.
* `std::` resources, when used, are "reimplemented": that is, hidden behind a
  thin, simple veneer of private inheritance; only methods that are needed are
  reexported (`using ...`) or implemented.

folder/file | purpose
---         | ---
[dev/](dev/) | general purpose development library; depends only on `std::`
[qt/](qt/)   | Qt-specific library; depends on `dev/` and Qt

