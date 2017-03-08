cd build
xvfb-run STeCa_tests/STeCa_tests

if [ $GXX =0 'g++' ]; then
  lcov -c -d . -o coverage.info
  lcov -r coverage.info 'tests/*' '/opt/*' '/usr/*' -o coverage.info
  lcov -l coverage.info
  coveralls-lcov --repo-token ${COVERALLS_TOKEN} coverage.info
fi
