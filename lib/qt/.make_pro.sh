PRO=qt.pro
echo -e '# generated project\n' > $PRO

cat >> $PRO <<EOT
TARGET   = qt_lib
TEMPLATE = lib
CONFIG  += static c++11 silent
QT      += widgets

INCLUDEPATH += \$\$PWD/../..
EOT

function files {
  where=$1; ext=$2
  find $where -type f -name \*.$ext -exec echo ' ' {} \\ \;
}

echo -e '\nHEADERS += \\' >> $PRO
files . h >> $PRO

echo -e '\nSOURCES += \\' >> $PRO
files . cpp >> $PRO

# eof
