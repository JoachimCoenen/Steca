try {
  eval('let arrow = () => 1');
} catch (err) {
  document.querySelector('article').innerHTML =
    '<br/><h2>These pages require a newer version of JavaScript. ' +
    'Please use a modern web browser.</h2>';
}

var book = {
  conf: {
    title:  'Steca2',
    banner: 'Steca2 - docs',
  },

  pagePath: '', pageFile: '',

  goto: function (ln, anchor, target) {
    const ev = window.event;
    if (undefined !== ev && (ev.ctrlKey || ev.metaKey))
      return true; // follow href in another tab
    top.postMessage(['goto', ln, anchor, target], '*');
    return false;
  },

  scrollTo: function (anchor) {
    if (!anchor)
      return;
    const els = document.querySelector(`a[name="${anchor.substr(1)}"]`);
    if (els)
      els.scrollIntoView(true);
  },

  iframeSrc: function (id) {
    const file = book.toc.lst[book.toc.ids[id]][1];
    return book.pageRoot + file;
  },

  // TODO consolidate
  link (ln) {
    let idx = book.toc.ids[ln];
    if (undefined !== idx)
      return book.toc.lst[idx][1];

    if (ln.startsWith('/'))
      return ln.substr(1);

    if (0 <= ln.indexOf('://'))
      return ln;

    let pos = ln.indexOf(':');
    if (pos < 0)
      return ln;

    const head = ln.substr(0, pos), tail = ln.substr(++pos);
    idx = book.toc.ids[head];
    if (undefined === idx)
      return ln;

    const fil = book.toc.lst[idx][1];
    const dir = fil.substr(0, fil.lastIndexOf('/'));
    return book.pageRoot + dir + '/' + tail; // TODO PAGES
  },

  compileToc () {
    const toc = this.toc;
    if (!toc.src)
      return;

    toc.ids = {}; toc.sec = {}, toc.pnt = {}; toc.fil = {}; toc.lst = [];
    let i = 0; const index = toc.static ? '_index.html' : '_index.cm';
    const level = function (pnt, path, src) {
      let first = true, pth, sec;
      for (const s of src) {
        if (first) {
          const [id, p, t] = s; pth = path + p; sec = i;
          toc.ids[id] = i;
          toc.sec[i]  = sec;
          toc.pnt[i]  = pnt;
          toc.fil[pth + index] = i;
          toc.lst.push([id, pth + index, t]);
          ++i; first = false;
          continue;
        }
        if (Array.isArray(s[0])) {
          level(i-1, pth, s);
          continue;
        }
        const [id, fil, t] = s;
        toc.ids[id] = i;
        toc.sec[i]  = sec;
        toc.pnt[i]  = sec;
        toc.fil[pth + fil] = i;
        toc.lst.push([id, pth + fil, t]);
        ++i;
      }
    };

    level(null, '', toc.src);
  }
};

if (cm_file.endsWith('.html')) // static
  book.pageFile = cm_file;

// eof
