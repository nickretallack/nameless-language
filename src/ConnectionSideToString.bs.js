'use strict';

var ConnectionNibToString$ReasonReactExamples = require("./ConnectionNibToString.bs.js");
var ConnectionNodeToString$ReasonReactExamples = require("./ConnectionNodeToString.bs.js");

function f(connectionSide) {
  return ConnectionNodeToString$ReasonReactExamples.f(connectionSide[/* node */0]) + ("-" + ConnectionNibToString$ReasonReactExamples.f(connectionSide[/* nib */1]));
}

exports.f = f;
/* No side effect */
