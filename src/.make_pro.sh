PRO=steca.pro
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

MODULES='core gui'
echo -e '\nHEADERS += \\' >> $PRO
for m in $MODULES ; do files $m h >> $PRO ; done
for m in $MODULES ; do files $m hpp >> $PRO ; done

echo -e '\nSOURCES += \\' >> $PRO
for m in $MODULES ; do files $m cpp >> $PRO ; done
echo ' ' main.cpp >> $PRO

cat >> $PRO <<EOT

RESOURCES += \
  gui/rc.qrc

# for Windoze & Linux
win32:CONFIG(release, debug|release): LIBS += -L\$\$OUT_PWD/../qt_lib/release/ -lqt_lib -L\$\$OUT_PWD/../dev_lib/release/ -ldev_lib
else:win32:CONFIG(debug, debug|release): LIBS += -L\$\$OUT_PWD/../qt_lib/debug/ -lqt_lib -L\$\$OUT_PWD/../dev_lib/debug/ -ldev_lib
else:unix:!macx: LIBS += -L\$\$OUT_PWD/../qt_lib/ -lqt_lib -L\$\$OUT_PWD/../dev_lib/ -ldev_lib
EOT
