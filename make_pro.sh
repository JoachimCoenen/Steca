PRO=STeCa2.pro
echo -e '# generated project\n' > $PRO

cat >> $PRO <<EOT
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = c STeCa
EOT

(cd c     && ./make_pro.sh)
(cd STeCa && ./make_pro.sh)
