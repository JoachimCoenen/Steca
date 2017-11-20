wc $(find . \( -name \*.hpp -or -name \*.cpp \) ! \( -path ./src/core_legacy/\* -or -path ./src/gui/QCP/\* -or -path ./src/core/fit/LM/\* -or -path ./tests/\* \) | sort)
