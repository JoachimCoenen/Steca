PRO=dev_lib.pro
echo -e '# generated project\n' > $PRO

cat >> $PRO <<EOT
TARGET   = dev_lib
TEMPLATE = lib
CONFIG  += static c++11 silent
EOT

function files {
  where=$1; ext=$2
  find $where -type f -name \*.$ext -exec echo ' ' {} \\ \;
}

MODULES='inc test typ'
echo -e '\nHEADERS += \\' >> $PRO
for m in $MODULES ; do files $m h >> $PRO ; done
for m in $MODULES ; do files $m hpp >> $PRO ; done

echo -e '\nSOURCES += \\' >> $PRO
for m in $MODULES ; do files $m cpp >> $PRO ; done

# eof
