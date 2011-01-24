var Iconv = require('iconv').Iconv
  , detect = require('./build/default/charset-detector').detect;

exports.reencode = function(buffer) {
  var match = detect(buffer)
    , iconv = new Iconv(match.encoding, 'UTF-8');
  match.content = iconv.convert(buffer).toString('UTF-8');
  return match;
};
