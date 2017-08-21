cd build/tests/steca
xvfb-run ./tests_steca

if [ $CXX == 'g++' ]; then
  lcov -c -d . -o coverage.info
  lcov -r coverage.info 'tests/*' '/opt/*' '/usr/*' -o coverage.info
  lcov -l coverage.info
  # coveralls-lcov -t ${COVERALLS_TOKEN} coverage.info
  coveralls-lcov coverage.info
fi
