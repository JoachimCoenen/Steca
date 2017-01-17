PRO=c.pro
echo -e '# generated project\n' > $PRO

cat >> $PRO <<EOT
TARGET   = c
TEMPLATE = lib
CONFIG  += static c++11 silent
QT      += widgets
EOT

function files {
  where=$1; ext=$2
  find $where -type f -name \*.$ext -exec echo ' ' {} \\ \;
}

MODULES='c cpp qt'
echo -e '\nHEADERS += \\' >> $PRO
for m in $MODULES ; do files $m h >> $PRO ; done

echo -e '\nSOURCES += \\' >> $PRO
for m in $MODULES ; do files $m cpp >> $PRO ; done

