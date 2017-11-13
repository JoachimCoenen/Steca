# Steca2
StressTextureCalculator, version 2. [Online documentation](https://scgmlz.github.io/Steca2/).

## Branches

### master

* contains only releases; squashed commit history \
  [![Travis-CI Build Status](https://travis-ci.org/scgmlz/Steca2.svg?branch=master)](https://travis-ci.org/scgmlz/Steca2)
  [![AppVeyor-CI Build Status](https://ci.appveyor.com/api/projects/status/github/scgmlz/steca2?branch=master&svg=true)](https://ci.appveyor.com/project/jburle/steca2)

### develop

* on the way to 2.1, with shared & immutable data \
  [![Travis-CI Build Status](https://travis-ci.org/scgmlz/Steca2.svg?branch=develop)](https://travis-ci.org/scgmlz/Steca2)
  [![AppVeyor-CI Build Status](https://ci.appveyor.com/api/projects/status/github/scgmlz/steca2?branch=develop&svg=true)](https://ci.appveyor.com/project/jburle/steca2)
  [![Coverage Status](https://coveralls.io/repos/github/scgmlz/Steca2/badge.svg?branch=develop)](https://coveralls.io/github/scgmlz/Steca2?branch=develop)

### develop-2.0.5

* further 2.0.x development (fixes etc.), until **develop** is ready for a release

## Source tree navigation

folder/file | purpose
---         | ---
[lib/](lib/)            | the development library
[src/](src/)            | the Steca application code
[tests/](tests/)        | subproject with tests
[.make_pro.sh](.make_pro.sh) | makes qmake (.pro) project files; we use those for Windows builds

