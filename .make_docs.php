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

chdir(dirname(__FILE__).'/');   // ensure cwd
@mkdir($pg = './docs/pg/');     // ensure (make) output directory for pages

$FILE = ''; $LINE = 0;
function error ($s) {           // report error and stop
  global $FILE, $LINE;
  echo "** error in $FILE, line $LINE: $s **\n"; die;
}

$tocJs = '';

function docsLevel ($cwd) {     // generate one level of docs
  global $FILE, $LINE;
  if (false === ($f = @file($dt = $cwd.'.docs_toc')))
    error('bad file ' . $dt);

  global $tocJs;

  $lineNo = 0; $firstLine = true;
  foreach ($f as $line) {
    $FILE = $dt; $LINE = ++$lineNo;
    // trimmed line content, of the part before '#', or of the whole line
    if (! ($line = trim(strstr($line.'#', '#', true))))
      continue;

    // separate and trim
    $ps = array_map('trim', explode("|", $line));

    if ($firstLine && 3 == count($ps)) {
      $firstLine = false;
      @list ($id, $title, $srcFiles) = $ps;
      $tocJs .= "['$id', '${cwd}_index.html', '$title'], ";
      generate($cwd, $id, $title, '_index.html', $srcFiles);
      continue;
    }

    if ($firstLine)
      error('bad first line');

    if (4 == count($ps)) {
      @list ($id, $title, $outFile, $srcFiles) = $ps;
      $tocJs .= "['$id', '${cwd}${outFile}', '$title'], ";
      generate($cwd, $id, $title, $outFile, $srcFiles);
      continue;
    }

    if (1 == count($ps)) {  // subdir
      list ($dir) = $ps;
      if (strlen($dir) < 2 || '/' != substr($dir, -1))
        error('bad dir ' . $dir);
      $tocJs .= '[';
      docsLevel($cwd . $dir);
      $tocJs .= '], ';
      continue;
    }

    error('bad line');
  }
}

$ids = [];  // a set of all $id's
function generate($cwd, $id, $title, $outFile, $srcFiles) {
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
    saveHtmlFile($pg.$cwd.$outFile, $src);
  }
}

// extract text between /*(begMark) or //(begMark) and (endMark)* / or //(endMark)
function extractCm ($s) {
  $begMark = '::>'; $endMark = '<::';
  preg_match_all("~(/\*$begMark|//$begMark)(.*?)($endMark\* /|//$endMark)~s", $s, $res);
  return join("\n", $res[2]);
}

function saveHtmlFile($fs, $s) {
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
docsLevel('./');
error_log("[$tocJs]");

// eof
