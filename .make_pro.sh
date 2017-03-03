PRO=STeCa2.pro
echo -e '# generated project\n' > $PRO

cat >> $PRO <<EOT
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = c2 STeCa
EOT

(cd c2    && ./.make_pro.sh)
(cd STeCa && ./.make_pro.sh)
