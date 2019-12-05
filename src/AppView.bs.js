'use strict';

var Json = require("@glennsl/bs-json/src/Json.bs.js");
var React = require("react");
var EncodeConnectionNode$ReasonReactExamples = require("./EncodeConnectionNode.bs.js");

function AppView(Props) {
  return React.createElement("div", undefined, "hello", Json.stringify(EncodeConnectionNode$ReasonReactExamples.f(/* GraphConnection */0)));
}

var make = AppView;

exports.make = make;
/* react Not a pure module */
