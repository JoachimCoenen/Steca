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

MODULES='core gui'
echo -e '\nHEADERS += \\' >> $PRO
for m in $MODULES ; do files $m h >> $PRO ; done

echo -e '\nSOURCES += \\' >> $PRO
for m in $MODULES ; do files $m cpp >> $PRO ; done

