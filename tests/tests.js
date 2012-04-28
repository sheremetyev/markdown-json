var test = require('fs').readFileSync(__dirname + '/basic.test', 'utf8').replace(/\r\n/g,'\n');

var match = /^(.*)\n<<<\n([\s\S]*)>>>\n([\s\S]*)$/.exec(test);
var command = match[1];
var input   = match[2];
var output  = match[3];

require('child_process').exec(command,
  function (error, stdout, stderr) {
    if (error !== null) {
      console.log('ERROR: ' + error);
      console.log('STDERR:\n' + stderr);
    } else {
    	compare(output, stdout);
    }
  }
).stdin.end(input);

function compare(expected, actual) {
  expected = JSON.parse(expected);
  actual = JSON.parse(actual);
  require('assert').deepEqual(actual, expected);
}
