PRO=src.pro
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

MODULES='core core_legacy gui'
echo -e '\nHEADERS += \\' >> $PRO
for m in $MODULES ; do files $m h >> $PRO ; done
for m in $MODULES ; do files $m hpp >> $PRO ; done

echo -e '\nSOURCES += \\' >> $PRO
for m in $MODULES ; do files $m cpp >> $PRO ; done
echo ' ' main.cpp \\ >> $PRO
echo ' ' manifest.cpp >> $PRO

cat >> $PRO <<EOT

RESOURCES += \
  gui/rc.qrc

# for Windoze & Linux
win32:CONFIG(release, debug|release): LIBS += -L\$\$OUT_PWD/../lib/qt/release/ -lqt_lib -L\$\$OUT_PWD/../lib/dev/release/ -ldev_lib
else:win32:CONFIG(debug, debug|release): LIBS += -L\$\$OUT_PWD/../lib/qt/debug/ -lqt_lib -L\$\$OUT_PWD/../lib/dev/debug/ -ldev_lib
else:unix:!macx: LIBS += -L\$\$OUT_PWD/../lib/qt/ -lqt_lib -L\$\$OUT_PWD/../lib/dev/ -ldev_lib
EOT
