#!/usr/bin/php
<?php

const _INDEX_CM     = '_index.cm';
const _INDEX_HTML   = '_index.html';
const PRE_TOC       = 'toc.js';

class TocCompiler {
  private $pages;

  public function __construct ($out) {
    $this->out = $out;
  }

  private static function error ($msg) {
    throw new Exception($msg);
  }

  private static function log ($msg) {
    error_log(' : ' . $msg);
  }

  public function compile () {
    try {
      $i = -1;
      $this->traverse(null, '', $i, 0);
      file_put_contents($this->out.PRE_TOC,
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

  private function getTocLine ($f, $n) { // split by ; into $n parts
    $l = '';
    while (!feof($f)) {
      $l = fgets($f);
      if ('@toc' != substr($l, 0, 4))
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
    self::log($path);
    $index = $path._INDEX_CM;
    if (!($f = @fopen($index, 'r')))
      self::error('bad index: ' . $index);

    @mkdir($this->out.$path);
    // first @toc entry - index
    list($id, $title) = $this->getTocLine($f, 2);
    if (!$id || !$title)
      self::error('bad toc line in: ' . $index);
    self::checkUniqueId($id);

    $indNo = ++$i;

    $pf = $path._INDEX_HTML;
    $this->saveHtml($this->out.$pf, file_get_contents($index), $level);

    $this->lst .= "['$id','$pf','".htmlentities($title)."'],";
    $this->ids .= "'$id':$i,";
    $this->sec .= "$i:$indNo,";
    $this->pnt .= null !== $pntNo ? "$i:$pntNo," : "$i:null,";
    $this->fil .= "'$pf':$i,";

    // other @toc entries
    for (;;) {
      if (false === ($l = $this->getTocLine($f, 3)))
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
        $this->genHtml($this->out.$pf, $path, $srcFiles, $level);
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
    self::log('=> ' . $fname . $level);
    $tx = htmlentities($tx);
    $toRoot = str_repeat('../', $level);
    file_put_contents($fname,
<<<HEREDOC
<!DOCTYPE html><html lang="en">
<head>

<meta charset="utf-8">
<style>body>pre{display:none;}</style>
<script>tocjs = '${toRoot}toc.js'</script>
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
@exec("rm -rf $pages"); mkdir($pages);     // make it empty

(new TocCompiler($pages))->compile();

// eof
/*
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
*/
