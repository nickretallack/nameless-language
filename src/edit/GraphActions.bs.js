// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Belt_Debug = require("bs-platform/lib/js/belt_Debug.js");

Belt_Debug.setupChromeDebugger(/* () */0);

function pointerIDToString(pointerID) {
  if (pointerID) {
    return "touch-" + String(pointerID[0]);
  } else {
    return "mouse";
  }
}

exports.pointerIDToString = pointerIDToString;
/*  Not a pure module */
