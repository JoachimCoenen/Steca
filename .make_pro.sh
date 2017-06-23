PRO=STeCa2.pro
echo -e '# generated project\n' > $PRO

cat >> $PRO <<EOT
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = dev_lib qt_lib steca
EOT

(cd dev_lib && ./.make_pro.sh)
(cd qt_lib  && ./.make_pro.sh)
(cd steca   && ./.make_pro.sh)

# eof
