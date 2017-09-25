#!/usr/bin/php
<?php
chdir(dirname(__FILE__));

function extractCm ($f) {
  // between /*:> or //:> and <:*/ or //<:
  $s = file_get_contents($f);
  preg_match_all("~(/\*:>|//:>)(.*?)(<:\*/|//<:)~s", $s, $res);
  // print_r($res);
  return join("\n", $res[2]);
}

$pg = '../docs/pg/';
@mkdir($pg);

$pgSrc = [
  ['about',     '',               'About Steca', [
    'manifest.cpp']],
  ['docs',      'docs.html',      'Documentation', []],
  ['get',       'get.html',       'Get Steca', []],
  ['gui',       'gui.html',       'GUI overview', []],
  ['session',   'session.html',   'Sessions', []],
  ['workflow',  'workflow.html',  'Workflow', []],
  ['output',    'output.html',    'Output', []],
];

$toc = '';

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

file_put_contents($pg.'toc.js', $toc);

// eof
