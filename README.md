Unicoder
========

Unicoder is a simple node.js library for normalizing the encoding of text.

Unicoder takes a Buffer containing a bytestring representing a document
with an unknown encoding, then guesses the encoding using a binding to
ICU, and finally reencodes the Buffer into a JavaScript String.

Installation:

    make install

Prerequisites:

  * node-iconv
  * libicu (including header files)
