book.toc = {
  simple: true, static: true, // TODO static to toc. ?
  src: [
    ['about',     '',               'About Steca'],
    ['docs',      'docs.html',      'Documentation'],
    ['get',       'get.html',       'Get Steca'],
    ['gui',       'gui.html',       'GUI overview'],
    ['session',   'session.html',   'Sessions'],
    ['workflow',  'workflow.html',  'Workflow'],
    ['output',    'output.html',    'Output'],
  ],

  // src: [
  //   ['mb_index', '', 'Mutebook'],
  //   [
  //     ['how', 'how/', 'How to write MB'],
  //     ['how_software', 'software.cm', 'Install software'],
  //     ['how_local', 'local.cm', 'Local MB'],
  //     ['how_pages', 'pages.cm', 'Folders and pages'],
  //     ['how_cm', 'cm.cm', 'Compact Markup'],
  //   ],
  //   [
  //     ['mb_draft', 'draft/', 'Draft'],
  //     ['mb_mupr', 'mupr_introduction.cm', 'Introduction to Music Production'],
  //     [
  //       ['mb_acou', 'Acoustics/', 'Acoustics'],
  //       ['ac_sound', 'sound_sources_and_waves.cm', 'Sound Sources and Sound Waves'],
  //       ['ac_waves', 'wave_attributes.cm', 'Wave Attributes'],
  //       ['ac_spl', 'soundpressure_spl.cm', 'Sound Pressure and Sound Pressure Level'],
  //       ['ac_room', 'room_acoustics.cm', 'Room Acoustics'],
  //     ],
  //     [
  //       [ 'mb_perc', 'Perception of Sound/_index.cm', 'Perception of Sound'],
  //     ],
  //     [
  //       ['mb_mics', 'Microphones/', 'Microphones'],
  //       ['mic_trans', 'transducer_principles.cm', 'Transducer Principles'],
  //       ['mic_oper', 'operating_principles.cm', 'Acoustic Operating Principles'],
  //       ['mic_spec', 'mic_specifications.cm', 'Microphone Specifications'],
  //     ],
  //   ],
  //   ['mb_glossary', 'glossary.cm', 'Glossary'],
  // ],
};

book.compileToc();