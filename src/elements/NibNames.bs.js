// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var React = require("react");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Pervasives = require("bs-platform/lib/js/pervasives.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");

var component = ReasonReact.statelessComponent("NibNames");

function make(position, height, $staropt$star, $staropt$star$1, nodeWidth, textHeight, _children) {
  var sources = $staropt$star !== undefined ? $staropt$star : /* [] */0;
  var sinks = $staropt$star$1 !== undefined ? $staropt$star$1 : /* [] */0;
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */component[/* didMount */4],
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (_self) {
              var renderTexts = function (nibs, isSource) {
                return Belt_List.toArray(Belt_List.mapWithIndex(nibs, (function (index, display) {
                                  return React.createElement("text", {
                                              alignmentBaseline: "central",
                                              textAnchor: isSource ? "start" : "end",
                                              x: Pervasives.string_of_float(position[/* x */0] + (
                                                    isSource ? 10.0 : nodeWidth - 10.0
                                                  )),
                                              y: Pervasives.string_of_float(textHeight * (index + 0.5))
                                            }, display[/* name */0]);
                                })));
              };
              return React.createElement(React.Fragment, undefined, React.createElement("rect", {
                              height: Pervasives.string_of_float(height * textHeight),
                              width: Pervasives.string_of_float(nodeWidth),
                              fill: "black",
                              fillOpacity: "0.1",
                              x: Pervasives.string_of_float(position[/* x */0]),
                              y: Pervasives.string_of_float(position[/* y */1])
                            }), renderTexts(sources, true), renderTexts(sinks, false));
            }),
          /* initialState */component[/* initialState */10],
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */component[/* reducer */12],
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.make = make;
/* component Not a pure module */
