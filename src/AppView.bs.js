'use strict';

var Json = require("@glennsl/bs-json/src/Json.bs.js");
var React = require("react");
var ConnectionSideToJson$ReasonReactExamples = require("./ConnectionSideToJson.bs.js");
var ConnectionSideToString$ReasonReactExamples = require("./ConnectionSideToString.bs.js");
var ExplicitConnectionSideKey$ReasonReactExamples = require("./ExplicitConnectionSideKey.bs.js");

function AppView(Props) {
  var connectionSide = /* record */[
    /* node : GraphConnection */0,
    /* nib : ValueConnection */0
  ];
  var explicitConnectionSide = /* record */[
    /* connectionSide */connectionSide,
    /* isSource */false
  ];
  return React.createElement("div", undefined, ConnectionSideToString$ReasonReactExamples.f(connectionSide), Json.stringify(ConnectionSideToJson$ReasonReactExamples.f(connectionSide)), ExplicitConnectionSideKey$ReasonReactExamples.f(explicitConnectionSide));
}

var make = AppView;

exports.make = make;
/* react Not a pure module */
