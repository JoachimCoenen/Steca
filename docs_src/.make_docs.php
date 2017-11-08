#!/usr/bin/php
<?php

chdir(dirname(__FILE__).'/../');
$srcDir  = 'docs_src/';
$docDir  = 'docs/pg/';

const INDEX  = '_index.cm';
const PROLOG = '_prolog';

function initDocDir () {
  $prolog = PROLOG; global $docDir;
  exec("rm -rf $docDir");
  exec("rsync -r --exclude=\*.cm --exclude=$prolog docs_src/* $docDir");
}

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

// extract cm text
function extractCmBlock ($s, &$res, $at, &$end) {
  if (false === ($beg = strpos($s, '/*[[[', $at)))
    return false;
  if (false === ($end = strpos($s, ']]]*/', $beg)))
    error('no end mark');

  $res = substr($s, $beg + 5, $end - $beg - 5);

  // remove leftover (if any) */
  if (false !== ($pos = strpos($res, '*/'))) {
    $p = strpos($res, '/*');
    if (false === $p || $pos < $p)
      $res = substr_replace($res, '', $pos, 2);
  }

  // remove leftover (if any) /*
  if (false !== ($pos = strrpos($res, '/*'))) {
    $p = strrpos($res, '*/');
    if (false === $p || $pos > $p)
      $res = substr_replace($res, '', $pos, 2);
  }

  return $beg;
}

function extractCmCode ($s, &$res, $at, &$end) {
  if (false === ($beg = strpos($s, '//[[[', $at)))
    return false;
  if (false === ($end = strpos($s, '//]]]', $beg)))
    error('no end mark');

  $res = substr($s, $beg + 5, $end - $beg - 5);
  $res = "\n~~~.cpp${res}~~~\n";

  return $beg;
}

function extractCm ($s) {
  $res = ''; $beg = 0; $endBlock = $endCode = 0;
  for (;;) {
    $begBlock = extractCmBlock($s, $resBlock, $beg, $endBlock);
    $begCode  = extractCmCode($s, $resCode, $beg, $endCode);

    if (false === $begBlock && false === $begCode)
      return $res;

    if (false !== $begBlock || (false === $begCode && $begBlock < $begCode)) {
      $res .= $resBlock; $beg = $endBlock;
    } else {
      $res .= $resCode; $beg = $endCode;
    }
  }
}

function genFile ($create, $relPath, $file, $rootFiles = '') {
  global $srcDir, $docDir;
  $relFile = $relPath.$file;
  if (!file_exists($relFile) && !file_exists($srcDir.$relFile) && !$create)
    return;

  @mkdir($docDir.$relPath);
  msg('> '.$relFile);

  // can not-exist
  $s = @file_get_contents($relFile);
  if (!$s)
    $s = @file_get_contents($srcDir.$relFile);

  if ($rootFiles)
    foreach (array_map('trim', explode(",", $rootFiles)) as $sf)
      if (false === ($src = @file_get_contents($relPath.$sf)))
        error('bad source (root) file ' . $relPath.$sf);
      else
        $s .= '.cm' == substr($sf, -3) ? $src : extractCm($src);

  if (!$s)  // no read nor generated content
    error('cannot make ' . $relFile);
  file_put_contents($docDir.$relFile, $s);
}

// traverse source tree
// files can be either under root or srcDir
function traverse ($relPath) {
  global $srcDir;
  msg(': ' . $relPath);

  if (!($f = @fopen($relPath.INDEX, 'r')))
    if (!($f = @fopen($srcDir.$relPath.INDEX, 'r')))
      error('bad index ' . $relPath);

  // first @toc entry -> index
  list ($id, $title, $srcFiles) = getTocLine($f, 3);
  if (!$id || !$title)
    error('bad toc line');

  genFile(false, $relPath, PROLOG, '');
  genFile(true,  $relPath, INDEX, $srcFiles);

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
      genFile(true, $relPath, $file, $srcFiles);
      continue;
    }

    error('bad toc line');
  }

  fclose($f);
}

// do it
initDocDir();
traverse('');

// eof
