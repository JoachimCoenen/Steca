#!/usr/bin/php
<?php

$srcDir = dirname(__FILE__).'/';
$dstDir = $srcDir . 'docs/pg/';

// make destination directory empty
@exec('rm -rf ' . $dstDir);
mkdir($dstDir);

const _INDEX_CM     = '_index.cm';    // source index files
const _INDEX_HTML   = '_index.html';  // output index files
const TOC_JS        = 'toc.js';       // comiled table-of-contents

// makes static pages, compiles TOC_JS
class DocMaker {
  private $srcDir, $dstDir;
  public function __construct ($srcDir, $dstDir) {
    $this->srcDir = $srcDir;
    $this->dstDir = $dstDir;
  }

  private static function error ($msg) {
    throw new Exception($msg);
  }

  private static function log ($msg) {
    error_log(' : ' . $msg);
  }

  public function make () {
    try {
      $i = -1; // counts toc entries
      $this->traverse(null, '', $i, 0);
      file_put_contents($this->dstDir.TOC_JS,
        "book.toc = {lst:[$this->lst], ids:{{$this->ids}}, sec:{{$this->sec}}, pnt:{{$this->pnt}}, fil:{{$this->fil}}};",
        LOCK_EX);
    } catch (Exception $e) {
      error_log(' ** ' . $e->getMessage() . ' **');
    }
  }

  // page ids must be unique
  private $haveIds = [];

  private function checkUniqueId ($id) {
    if (@$this->haveIds[$id])
      self::error('duplicate id: ' . $id);
    $this->haveIds[$id] = true;
  }

  private $lst = '', $ids = '', $sec = '', $pnt = '', $fil = '';

  private function getTocLine ($f, $n) { // split by ';' into $n parts
    $l = '';
    while (!feof($f)) {
      $l = fgets($f);
      if ('@toc' != substr($l, 0, 4))         // read only toc lines
        continue;
      $l = substr($l, 4);
      if (false !== ($pos = strpos($l, '#'))) // cut off comments
        $l = substr($l, 0, $pos);
      if (($l = trim($l)))
        return array_pad(array_map('trim', explode(';', $l)), $n, '');
    }

    return false;
  }

  private function traverse ($pntNo, $path, &$i, $level) {
    self::log("[$path]");
    $index = $path._INDEX_CM;
    if (!($f = @fopen($index, 'r')))
      self::error('bad index: ' . $index);

    @mkdir($this->dstDir.$path);

    // first @toc entry = index
    list($id, $title) = $this->getTocLine($f, 2);
    if (!$id || !$title)
      self::error('bad toc line in: ' . $index);
    self::checkUniqueId($id);

    $indNo = ++$i;

    $pf = $path._INDEX_HTML;
    $this->saveHtml($this->dstDir.$pf, file_get_contents($index), $level);

    $this->lst .= "['$id','$pf','".htmlentities($title)."'],";
    $this->ids .= "'$id':$i,";
    $this->sec .= "$i:$indNo,";
    $this->pnt .= null !== $pntNo ? "$i:$pntNo," : "$i:null,";
    $this->fil .= "'$pf':$i,";

    // other @toc entries
    for (;;) {
      if (false === ($l = $this->getTocLine($f, 4)))
        break;

      list($idOrSub, $file, $title, $srcFiles) = $l;
      if ($idOrSub && !$file && !$title && !$srcFiles) {
        if ('/' === substr($idOrSub, -1)) {
          $this->traverse($indNo, $path.$idOrSub, $i, $level+1);
          continue;
        }
      } else if ($idOrSub && $file && $title && $srcFiles) {
        $id = $idOrSub; ++$i; $file .= '.html';
        self::checkUniqueId($id);
        $pf = $path.$file;
        $this->genHtml($this->dstDir.$pf, $path, $srcFiles, $level);
        $this->lst .= "['$id','$pf','".htmlentities($title)."'],";
        $this->ids .= "'$id':$i,";
        $this->sec .= "$i:$indNo,";
        $this->pnt .= "$i:$indNo,";
        $this->fil .= "'$pf':$i,";
        continue;
      }

      self::error('bad toc line in: ' . $index);
    }

    fclose($f);
  }

  private function genHtml($fname, $path, $srcFiles, $level) {
    if (! ($srcFiles = array_map('trim', explode(",", $srcFiles))))
      self::error('bad source file list');

    foreach ($srcFiles as $sf) {
      if (false === ($src = @file_get_contents($path.$sf)))
        self::error('bad source file ' . $sf);
      if ('.cm' != substr($sf, -3))
        $src = $this->extractCm($src);
      $this->saveHtml($fname, $src, $level);
    }
  }

// extract text between /*(begMark) or //(begMark) and (endMark)* / or //(endMark)
  function extractCm ($s) {
    $begMark = '::>'; $endMark = '<::';
    preg_match_all("~(/\*$begMark|//$begMark)(.*?)($endMark\* /|//$endMark)~s", $s, $res);
    return join("\n", $res[2]);
  }

  private function saveHtml ($fname, $tx, $level) {
    self::log(' => ' . $fname);
    $tx = htmlentities($tx);
    $toRoot = str_repeat('../', $level);
    file_put_contents($fname,
<<<HEREDOC
<!DOCTYPE html><html lang="en">
<head>

<meta charset="utf-8">
<style>body>pre{display:none;}</style>
<script src ="${toRoot}../CM/load.js"></script>

</head>

<body><pre>
$tx
<pre></body>
</html>
HEREDOC
    );
  }

}

chdir(dirname(__FILE__).'/');               // ensure cwd
$pages = 'docs/pg/';                        // output directory for pages

(new DocMaker($srcDir, $dstDir))->make();

// eof
