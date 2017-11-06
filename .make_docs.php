#!/usr/bin/php
<?php
/* Traverses the source tree $srcDir and creates the doc tree $docDir:
- reads _index.cm
- copies *.cm files over
- makes *.cm files from specified sources
*/

$srcDir = dirname(__FILE__).'/';
$docDir = $srcDir . 'docs/pg/';

// make destination directory empty
@exec('rm -rf ' . $docDir);
mkdir($docDir);

// helpers
function msg ($msg) {
  error_log($msg);
}

function error ($msg) {
  msg($msg); exit(-1);
}

function getTocLine ($f, $n) { // split by ';' into $n parts
  $l = '';
  while (!feof($f)) {
    $l = fgets($f);
    if ('@toc' != substr($l, 0, 4))         // read only toc lines
      continue;
    $l = substr($l, 4);
    if (false !== ($pos = strpos($l, '#'))) // cut off comments
      $l = substr($l, 0, $pos);
    if (($l = trim($l)))                    // pad if needed
      return array_pad(array_map('trim', explode(';', $l)), $n, '');
  }

  return false;
}

// extract text between /*(begMark) and (endMark)*/
// or BETWEEN //(begMark) and //(endMark)
function findMark($s, $marks) {
  $pos = false;
  foreach ($marks as $i => $m) {
    if (false !== ($p = strpos($s, $m)))
      if (false === $pos || $p < $pos) {
        $pos = $p; $mi = $i;  // find the first match in $s
      }
  }
  return false === $pos ? false : [$pos, strlen($marks[$mi])];
}

function extractCm (&$s) {
  $begMarks = ['/*::>*/', '/*::>', '//::>'];  // from longer to shorter
  $endMarks = ['/*<::*/', '<::*/', '//<::'];

  $res = '';
  while (false !== ($found = findMark($s, $begMarks))) {
    list ($pos, $len) = $found;
    $s = substr($s, $pos + $len);
    if (false === ($found = findMark($s, $endMarks)))
      error('no end mark');
    list ($pos, $len) = $found;
    $res .= substr($s, 0, $pos); $s = substr($s, $pos + $len);
  }

  return $res;
}

function genFile ($relPath, $file, $srcFiles) {
  global $srcDir, $docDir;
  $relFile = $relPath.$file;
  msg('> '.$relFile);
  $s = @file_get_contents($srcDir.$relFile); // can non-exist

  if ($srcFiles)
    foreach (array_map('trim', explode(",", $srcFiles)) as $sf)
      if (false === ($src = @file_get_contents($srcDir.$relPath.$sf)))
        error('bad source file ' . $relPath.$sf);
      else
        $s .= '.cm' == substr($sf, -3) ? $src : extractCm($src);

  if (!$s)  // no read nor generated content
    error('cannot make ' . $relFile);
  file_put_contents($docDir.$relFile, $s);
}

// traverse source tree
function traverse ($relPath) {
  global $srcDir, $docDir; $index = '_index.cm';
  msg(': ' . $relPath);
  @mkdir($docDir.$relPath);

  if (!($f = @fopen($srcDir.$relPath.$index, 'r')))
    error('bad index ' . $relPath);

  // first @toc entry -> index
  list ($id, $title, $srcFiles) = getTocLine($f, 3);
  if (!$id || !$title)
    error('bad toc line in: ' . $index);

  genFile($relPath, $index, $srcFiles);

  // other @toc entries
  for (;;) {
    if (false === ($l = getTocLine($f, 4)))
      break;

    list ($idOrSub, $file, $title, $srcFiles) = $l;
    if ($idOrSub && !$file) {
      if ('/' === substr($idOrSub, -1)) {
        traverse($relPath.$idOrSub);
        continue;
      }
    } else if ($idOrSub && $file) {
      genFile($relPath, $file, $srcFiles);
      continue;
    }

    error('bad toc line in: ' . $index);
  }

  fclose($f);
}

// do it
traverse('');

// eof
