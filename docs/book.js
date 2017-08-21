var book = {
  timestamp: '2017.08.21',
  isDebug: 'true',
  conf: {
    title:  'Steca2',
    banner: 'Steca2 - docs',
  },

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
  }
};

book.toc = {
  simple: true,
  lst: [
    ['about', 'about.html', 'About Steca'],
    ['get',   'get.html',   'Get Steca'],
  ],
  ids: {
    'about': 0, 'get': 1,
  },
  sec: {
    0: 0, 1: 0,
  },
  pnt: {
    0: null, 1: 0,
  },
  fil: {
    'about.html': 0,
    'get.html'  : 1,
  }
};

// eof
