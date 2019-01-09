// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var React = require("react");
var Caml_array = require("bs-platform/lib/js/caml_array.js");
var Caml_int32 = require("bs-platform/lib/js/caml_int32.js");
var Pervasives = require("bs-platform/lib/js/pervasives.js");
var ReactDOMRe = require("reason-react/src/ReactDOMRe.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Caml_primitive = require("bs-platform/lib/js/caml_primitive.js");
var Helpers$ReactTemplate = require("./Helpers.bs.js");

var component = ReasonReact.statelessComponent("Connection");

function make(sourcePosition, sinkPosition, $staropt$star, $staropt$star$1, _children) {
  var nudge = $staropt$star !== undefined ? $staropt$star : 0;
  var maxNudge = $staropt$star$1 !== undefined ? $staropt$star$1 : 1;
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
              var left = Caml_primitive.caml_int_min(sourcePosition[/* x */0], sinkPosition[/* x */0]);
              var top = Caml_primitive.caml_int_min(sourcePosition[/* y */1], sinkPosition[/* y */1]) - 5.0 / 2.0;
              var width = Pervasives.abs(sourcePosition[/* x */0] - sinkPosition[/* x */0] | 0);
              var height = Pervasives.abs(sourcePosition[/* y */1] - sinkPosition[/* y */1] | 0) + 5.0;
              var palette = /* array */[
                "fuchsia",
                "aqua",
                "lime",
                "blue"
              ];
              var containerStyle = {
                height: Helpers$ReactTemplate.floatPixels(height),
                left: Helpers$ReactTemplate.floatPixels(left),
                position: "absolute",
                top: Helpers$ReactTemplate.floatPixels(top),
                width: Helpers$ReactTemplate.floatPixels(width),
                pointerEvents: "none"
              };
              var sharedStyle = {
                borderColor: Caml_array.caml_array_get(palette, Caml_int32.mod_(nudge, palette.length)),
                borderStyle: "solid",
                borderWidth: "0",
                height: Helpers$ReactTemplate.floatPixels(height / 2.0),
                position: "absolute",
                width: Helpers$ReactTemplate.floatPixels(width / 2.0)
              };
              var centeredNudge = nudge - (maxNudge - 1 | 0) / 2.0;
              var match = maxNudge * 20.0 < width;
              var clampedNudgeMagnitude = match ? 20.0 : width / maxNudge;
              var actualNudge = centeredNudge * clampedNudgeMagnitude;
              var match$1 = 8.0 * 2.0 < height;
              var actualRoundness = match$1 ? 8.0 : height / 2.0;
              var sourceIsHigher = sourcePosition[/* y */1] > sinkPosition[/* y */1];
              var match$2 = sourcePosition[/* x */0] < sinkPosition[/* x */0];
              var match$3 = (
                match$2 ? sourceIsHigher : !sourceIsHigher
              ) ? /* tuple */[
                  ReactDOMRe.Style[/* combine */0](sharedStyle, {
                        borderTopWidth: "0",
                        borderBottomWidth: Helpers$ReactTemplate.floatPixels(5.0),
                        bottom: "0",
                        right: Helpers$ReactTemplate.floatPixels(width / 2.0 - actualNudge - 5.0 - clampedNudgeMagnitude / 4.0),
                        borderBottomRightRadius: Helpers$ReactTemplate.floatPixels(actualRoundness)
                      }),
                  ReactDOMRe.Style[/* combine */0](sharedStyle, {
                        borderTopWidth: Helpers$ReactTemplate.floatPixels(5.0),
                        borderBottomWidth: "0",
                        left: Helpers$ReactTemplate.floatPixels(width / 2.0 + actualNudge + clampedNudgeMagnitude / 4.0),
                        top: "0",
                        borderTopLeftRadius: Helpers$ReactTemplate.floatPixels(actualRoundness)
                      })
                ] : /* tuple */[
                  ReactDOMRe.Style[/* combine */0](sharedStyle, {
                        borderTopWidth: Helpers$ReactTemplate.floatPixels(5.0),
                        borderBottomWidth: "0",
                        right: Helpers$ReactTemplate.floatPixels(width / 2.0 + actualNudge),
                        top: "0",
                        borderTopRightRadius: Helpers$ReactTemplate.floatPixels(actualRoundness)
                      }),
                  ReactDOMRe.Style[/* combine */0](sharedStyle, {
                        borderTopWidth: "0",
                        borderBottomWidth: Helpers$ReactTemplate.floatPixels(5.0),
                        bottom: "0",
                        left: Helpers$ReactTemplate.floatPixels(width / 2.0 - actualNudge - 5.0),
                        borderBottomLeftRadius: Helpers$ReactTemplate.floatPixels(actualRoundness)
                      })
                ];
              var fullLeftStyle = ReactDOMRe.Style[/* combine */0](match$3[0], {
                    borderRightWidth: Helpers$ReactTemplate.floatPixels(5.0),
                    left: "0"
                  });
              var fullRightStyle = ReactDOMRe.Style[/* combine */0](match$3[1], {
                    borderLeftWidth: Helpers$ReactTemplate.floatPixels(5.0),
                    right: "0"
                  });
              return React.createElement("div", {
                          style: containerStyle
                        }, React.createElement("div", {
                              style: fullLeftStyle
                            }), React.createElement("div", {
                              style: fullRightStyle
                            }));
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
