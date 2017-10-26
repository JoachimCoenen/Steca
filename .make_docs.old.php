#!/usr/bin/php
<?php
chdir(dirname(__FILE__).'/src');

// output directory
@mkdir($pg = '../docs/pg/');

// source data definition
// [prefix, output file, toc entry, [source files...]]
$pgSrc = [
  ['about', '', 'About Steca',
    ['manifest.cpp']],
  ['docs', 'docs.html', 'Documentation',
    []],
];

// extract text between /*(begMark) or //(begMark) and (endMark)*/ or //(endMark)
function extractCm ($f) {
  $begMark = '::>'; $endMark = '<::';

  $s = file_get_contents($f);
  preg_match_all("~(/\*$begMark|//$begMark)(.*?)($endMark\*/|//$endMark)~s", $s, $res);

  return join("\n", $res[2]);
}

// toc js code
$toc = '';

// create html pages
foreach ($pgSrc as $src) {
  list($id, $file, $tx, $fs) = $src;

  $toc .= "    ['$id', '$file', '$tx'],\n";
  if (!$file)
    $file = '_index.html';

  $cm = '';
  foreach ($fs as $f)
    $cm .= htmlentities(extractCm($f));

  $html = <<<HEREDOC
<!DOCTYPE html><html lang="en">
<head>

<meta charset="utf-8">
<style>body>pre{display:none;}</style>
<script>tocjs = '../pg/toc.js'</script>
<script src ="../CM/load.js"></script>

</head>

<body><pre>
$cm
<pre></body>
</html>
HEREDOC;

  file_put_contents($pg.$file, $html);
}

$toc = <<<HEREDOC
book.toc = {
  simple: false,
  src: [
$toc  ],
};
HEREDOC;

// output toc
file_put_contents($pg.'toc.js', $toc);

// eof
