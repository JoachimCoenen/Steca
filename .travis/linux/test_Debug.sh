cd build
xvfb-run tests_steca/tests_steca

if [ $GXX == 'g++' ]; then
  lcov -c -d . -o coverage.info
  lcov -r coverage.info 'tests/*' '/opt/*' '/usr/*' -o coverage.info
  lcov -l coverage.info
  coveralls-lcov --repo-token ${COVERALLS_TOKEN} coverage.info
fi
