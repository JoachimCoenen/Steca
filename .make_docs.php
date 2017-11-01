#!/usr/bin/php
<?php
/*
generates js toc (table-of-content) by scanning the file tree, looking for
'.docs_toc' files; those are made of lines that are:
- id | title | source-files ...         (the first line, generates _index.html)
- id | title | out-file.html | source-files ...   (on subsequent lines)
- subdir/                               (descend, and search for .docs_toc)
- # comment
*/

chdir(dirname(__FILE__).'/');       // ensure cwd
$pg = 'docs/pg/';                   // output directory for pages
@exec("rm -rf $pg"); mkdir($pg);    // make it empty

$FILE = ''; $LINE = 0;
function error ($s) {               // report error and stop
  global $FILE, $LINE;
  echo "** error in $FILE, line $LINE: $s **\n"; die;
}

function docsLevel ($cwd, $dir) {   // generate one level of docs
  $cwd .= $dir;
  global $FILE, $LINE;
  if (false === ($f = @file($dt = $cwd.'.docs_toc')))
    error('bad file ' . $dt);

  $res = '';

  $lineNo = 0; $first = true;
  foreach ($f as $line) {
    $FILE = $dt; $LINE = ++$lineNo;
    // trimmed content before '#' (# comment ...)
    if (! ($line = trim(strstr($line.'#', '#', true))))
      continue;

    // split by '|' and trim
    $ps = array_map('trim', explode("|", $line));

    if (3 == count($ps)) {
      @list ($id, $title, $srcFiles) = $ps;
      if ($first) {
        $res .= "['$id', '$dir', '$title'], ";
        genHtml($cwd, $id, $title, '_index.html', $srcFiles);
        $first = false;
      } else {
        $outFile = $id.'.html';
        $res .= "['$id', '$outFile', '$title'], ";
        genHtml($cwd, $id, $title, $outFile, $srcFiles);
        $first = false;
      }
      continue;
    }

    if ($first)
      error('bad first line');

    if (1 == count($ps)) {  // subdir
      list ($dir) = $ps;
      if (strlen($dir) < 2 || '/' != substr($dir, -1))
        error('bad dir ' . $dir);
      $res .= docsLevel($cwd, $dir);
      continue;
    }

    error('bad line');
  }

  return "[$res], ";
}

$ids = [];  // a set of all $id's
function genHtml($cwd, $id, $title, $outFile, $srcFiles) {
  global $ids, $pg;
  if (@$ids[$id])
    error('duplicate id');
  $ids[$id] = true;

  if (! ($sfs = array_map('trim', explode(",", $srcFiles))))
    error('bad source file list');

  foreach ($sfs as $sf) {
    if (false === ($src = @file_get_contents($cwd.$sf)))
      error('bad source file ' . $sf);
    if ('.cm' != substr($sf, -3))
      $src = extractCm($src);
    @mkdir($pg.$cwd);
    saveHtml($pg.$cwd.$outFile, $src);
  }
}

// extract text between /*(begMark) or //(begMark) and (endMark)* / or //(endMark)
function extractCm ($s) {
  $begMark = '::>'; $endMark = '<::';
  preg_match_all("~(/\*$begMark|//$begMark)(.*?)($endMark\* /|//$endMark)~s", $s, $res);
  return join("\n", $res[2]);
}

function saveHtml($fs, $s) {
  $s = htmlentities($s);
  file_put_contents($fs,
<<<HEREDOC
<!DOCTYPE html><html lang="en">
<head>

<meta charset="utf-8">
<style>body>pre{display:none;}</style>
<script>tocjs = '../pg/toc.js'</script>
<script src ="../CM/load.js"></script>

</head>

<body><pre>
$s
<pre></body>
</html>
HEREDOC
  );
}

// top level - go!
$tocJs = docsLevel('', '');
error_log("$tocJs");

file_put_contents($pg.'toc.js',
  <<<HEREDOC
book.toc = {
  simple: false,
  src: $tocJs
};
HEREDOC
);

// eof
