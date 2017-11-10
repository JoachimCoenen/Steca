#!/usr/bin/php
<?php

// helpers
function msg ($msg) {
  error_log($msg);
}

function error ($msg) {
  msg($msg); exit(-1);
}

// cwd to root
chdir(dirname(__FILE__).'/../');
$srcDir  = 'docs_src/';
$docDir  = 'docs/pg/';

const INDEX  = '_index.cm';
const PROLOG = '_prolog';
const CODE   = '_code';

// $docDir as a copy of auxiliary files (not .cm nor prolog) from $srcDir
function initDocDir () {
  $prolog = PROLOG; global $srcDir, $docDir;
  exec("rm -rf $docDir");
  exec("rsync -r --exclude=\*.cm --exclude=$prolog $srcDir* $docDir");
}

// get the next toc line from file $f, split by ';' into $n parts
function getTocLine ($f, $n) {
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

// extract cm text from string $s
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

// generate a file in $docDir, from a corresponding one in root or $srcDir
function genFile ($create, $relPath, $file, $extra, $srcFiles) {
  global $srcDir, $docDir;
  $relFile = $relPath.$file;
  if (!file_exists($relFile) && !file_exists($srcDir.$relFile) && !$create)
    return;

  // can not-exist
  $s = @file_get_contents($relFile);
  if (!$s)
    $s = @file_get_contents($srcDir.$relFile);

  $s .= $extra;

  if ($srcFiles)
    foreach (array_map('trim', explode(",", $srcFiles)) as $sf)
      if (false === ($src = @file_get_contents($relPath.$sf)))
        error('bad source file ' . $relPath.$sf);
      else
        $s .= '.cm' == substr($sf, -3) ? $src : extractCm($src);

  file_put_contents($docDir.$relFile, $s);
}

// traverse source tree in (root ∪ $srcDir)
function traverseDocs ($relPath = '') {
  global $srcDir, $docDir;
  msg('. ' . $relPath);

  if (!($f = @fopen($relPath.INDEX, 'r')))
    if (!($f = @fopen($srcDir.$relPath.INDEX, 'r')))
      error('bad index ' . $relPath);

  // first @toc entry -> index
  list ($id, $title, $indexSrcFiles) = getTocLine($f, 3);
  if (!$id || !$title)
    error('bad toc line');

  @mkdir($docDir.$relPath);

  genFile(false, $relPath, PROLOG, '', '');
  $tocCodeFiles = $codeFileList = '';

  // other @toc entries
  for (;;) {
    if (false === ($l = getTocLine($f, 4)))
      break;

    list ($id, $file, $title, $srcFiles) = $l;
    if ('@code' === $id) {            // automatic toc for code files
      foreach (scandir($relPath) as $_ => $file) {
        if (in_array(substr($file, -4), ['.hpp', '.cpp', '.inc'])) {
          $fileId = str_replace('/', '_', $relPath) . "_$file";
          $tocCodeFiles .= "\n@toc $fileId ; $file.cm ; $file ; $file";
          $ghUrl = "https://github.com/scgmlz/Steca2/blob/develop/$relPath$file";
          $codeFileList .= "* {:{img:/ico/github.png}|$ghUrl} {:$fileId}\n";
          genFile(true, $relPath, "$file.cm", '', $file);
        }
      }

      continue;
    } else if ($id && !$file) {
      if ('/' === substr($id, -1)) {  // subdir
        traverseDocs($relPath.$id);
        continue;
      }
    } else if ($id && $file) {        // a regular toc entry
      genFile(true, $relPath, $file, '', $srcFiles);
      continue;
    }

    error('bad toc line');
  }

  if ($codeFileList)
    $codeFileList = "\n==Files:\n$codeFileList\n";
  genFile(true,  $relPath, INDEX, $tocCodeFiles.$codeFileList, $indexSrcFiles);
  fclose($f);
}

// do it
msg('-- docs --');
initDocDir();
traverseDocs();

// eof
