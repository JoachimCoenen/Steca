PRO=STeCa2.pro
echo -e '# generated project\n' > $PRO

cat >> $PRO <<EOT
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = lib/dev lib/qt src
EOT

(cd lib/dev && ./.make_pro.sh)
(cd lib/qt  && ./.make_pro.sh)
(cd src     && ./.make_pro.sh)

# eof
