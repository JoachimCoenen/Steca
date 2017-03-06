PRO=STeCa.pro
echo -e '# generated project\n' > $PRO

DATE=`date +%y%m%d_%H%M`

cat >> $PRO <<EOT
TARGET   = STeCa-$DATE
TEMPLATE = app
CONFIG  += c++11 silent
QT      += widgets svg multimedia

INCLUDEPATH += \$\$PWD/..
EOT

function files {
  where=$1; ext=$2
  find $where -type f -name \*.$ext -exec echo ' ' {} \\ \;
}

MODULES='core core_qt gui'
echo -e '\nHEADERS += \\' >> $PRO
for m in $MODULES ; do files $m h >> $PRO ; done
for m in $MODULES ; do files $m hpp >> $PRO ; done

echo -e '\nSOURCES += \\' >> $PRO
for m in $MODULES ; do files $m cpp >> $PRO ; done
echo ' ' main.cpp >> $PRO

cat >> $PRO <<EOT

# for Windoze & Linux
win32:CONFIG(release, debug|release): LIBS += -L\$\$OUT_PWD/../c2/release/ -lc2
else:win32:CONFIG(debug, debug|release): LIBS += -L\$\$OUT_PWD/../c2/debug/ -lc2
else:unix:!macx: LIBS += -L\$\$OUT_PWD/../c2/ -lc2
EOT
