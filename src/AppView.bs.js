'use strict';

var Json = require("@glennsl/bs-json/src/Json.bs.js");
var React = require("react");
var ConnectionNodeToJSON$ReasonReactExamples = require("./ConnectionNodeToJSON.bs.js");
var ConnectionNodeToString$ReasonReactExamples = require("./ConnectionNodeToString.bs.js");

function AppView(Props) {
  return React.createElement("div", undefined, ConnectionNodeToString$ReasonReactExamples.f(/* GraphConnection */0), Json.stringify(ConnectionNodeToJSON$ReasonReactExamples.f(/* GraphConnection */0)));
}

var make = AppView;

exports.make = make;
/* react Not a pure module */
