'use strict';

var Json = require("@glennsl/bs-json/src/Json.bs.js");
var React = require("react");
var DefinedNodeToJson$ReasonReactExamples = require("./DefinedNodeToJson.bs.js");
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
  return React.createElement("div", undefined, ConnectionSideToString$ReasonReactExamples.f(connectionSide), Json.stringify(ConnectionSideToJson$ReasonReactExamples.f(connectionSide)), ExplicitConnectionSideKey$ReasonReactExamples.f(explicitConnectionSide), Json.stringify(DefinedNodeToJson$ReasonReactExamples.f(/* record */[
                      /* kind : FunctionCallNode */0,
                      /* definitionID */"123"
                    ])));
}

var make = AppView;

exports.make = make;
/* react Not a pure module */
