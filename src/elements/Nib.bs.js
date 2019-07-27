// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Belt_Debug = require("bs-platform/lib/js/belt_Debug.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");
var Definition$ReactTemplate = require("../Definition.bs.js");

Belt_Debug.setupChromeDebugger(/* () */0);

var color = "rgb(160,160,160)";

var component = ReasonReact.reducerComponent("Nib");

function make(isSource, connectionSide, position, text, emit, isHighlighted, value, _children) {
  return /* record */Block.record([
            "debugName",
            "reactClassInternal",
            "handedOffState",
            "willReceiveProps",
            "didMount",
            "didUpdate",
            "willUnmount",
            "willUpdate",
            "shouldUpdate",
            "render",
            "initialState",
            "retainedProps",
            "reducer",
            "jsElementWrapped"
          ], [
            component[/* debugName */0],
            component[/* reactClassInternal */1],
            component[/* handedOffState */2],
            component[/* willReceiveProps */3],
            (function (self) {
                var match = self[/* state */1][0];
                if (match !== undefined) {
                  Caml_option.valFromOption(match).addEventListener("finish-drawing", (function ($$event) {
                          return Curry._1(emit, /* PointerAction */Block.variant("PointerAction", 0, [/* record */Block.record([
                                            "pointerID",
                                            "action"
                                          ], [
                                            Block.simpleVariant("Touch", [$$event.detail.identifier]),
                                            Block.variant("FinishDrawing", 3, [/* record */Block.record([
                                                    "connectionSide",
                                                    "isSource"
                                                  ], [
                                                    connectionSide,
                                                    isSource
                                                  ])])
                                          ])]));
                        }));
                  return /* () */0;
                } else {
                  return /* () */0;
                }
              }),
            component[/* didUpdate */5],
            component[/* willUnmount */6],
            component[/* willUpdate */7],
            component[/* shouldUpdate */8],
            (function (self) {
                return React.createElement(React.Fragment, undefined, React.createElement("text", {
                                alignmentBaseline: "central",
                                textAnchor: isSource ? "start" : "end",
                                x: Helpers$ReactTemplate.pixels(position[/* x */0] + 10.0 * (
                                      isSource ? 1.0 : -1.0
                                    )),
                                y: Helpers$ReactTemplate.pixels(position[/* y */1])
                              }, text), value !== undefined ? React.createElement("text", {
                                  alignmentBaseline: "central",
                                  textAnchor: "end",
                                  x: Helpers$ReactTemplate.pixels(position[/* x */0] - 10.0),
                                  y: Helpers$ReactTemplate.pixels(position[/* y */1])
                                }, Definition$ReactTemplate.displayValue(value)) : null, React.createElement("circle", {
                                ref: (function (ref) {
                                    self[/* state */1][0] = (ref == null) ? undefined : Caml_option.some(ref);
                                    return /* () */0;
                                  }),
                                onMouseDown: (function ($$event) {
                                    return Curry._1(emit, /* PointerAction */Block.variant("PointerAction", 0, [/* record */Block.record([
                                                      "pointerID",
                                                      "action"
                                                    ], [
                                                      0,
                                                      Block.variant("StartDrawing", 0, [/* record */Block.record([
                                                              "explicitConnectionSide",
                                                              "point"
                                                            ], [
                                                              Block.record([
                                                                  "connectionSide",
                                                                  "isSource"
                                                                ], [
                                                                  connectionSide,
                                                                  isSource
                                                                ]),
                                                              Helpers$ReactTemplate.pointFromMouse($$event)
                                                            ])])
                                                    ])]));
                                  }),
                                onMouseUp: (function (param) {
                                    return Curry._1(emit, /* PointerAction */Block.variant("PointerAction", 0, [/* record */Block.record([
                                                      "pointerID",
                                                      "action"
                                                    ], [
                                                      0,
                                                      Block.variant("FinishDrawing", 3, [/* record */Block.record([
                                                              "connectionSide",
                                                              "isSource"
                                                            ], [
                                                              connectionSide,
                                                              isSource
                                                            ])])
                                                    ])]));
                                  }),
                                onTouchEnd: (function ($$event) {
                                    return Helpers$ReactTemplate.iterateTouches($$event, (function (touch) {
                                                  document.elementFromPoint(touch.clientX, touch.clientY).dispatchEvent(new CustomEvent("finish-drawing", {
                                                            detail: {
                                                              identifier: touch.identifier
                                                            }
                                                          }));
                                                  return /* () */0;
                                                }));
                                  }),
                                onTouchStart: (function ($$event) {
                                    return Helpers$ReactTemplate.iterateTouches($$event, (function (touch) {
                                                  return Curry._1(emit, /* PointerAction */Block.variant("PointerAction", 0, [/* record */Block.record([
                                                                    "pointerID",
                                                                    "action"
                                                                  ], [
                                                                    Block.simpleVariant("Touch", [touch.identifier]),
                                                                    Block.variant("StartDrawing", 0, [/* record */Block.record([
                                                                            "explicitConnectionSide",
                                                                            "point"
                                                                          ], [
                                                                            Block.record([
                                                                                "connectionSide",
                                                                                "isSource"
                                                                              ], [
                                                                                connectionSide,
                                                                                isSource
                                                                              ]),
                                                                            Block.record([
                                                                                "x",
                                                                                "y"
                                                                              ], [
                                                                                touch.clientX,
                                                                                touch.clientY
                                                                              ])
                                                                          ])])
                                                                  ])]));
                                                }));
                                  }),
                                cx: Helpers$ReactTemplate.pixels(position[/* x */0]),
                                cy: Helpers$ReactTemplate.pixels(position[/* y */1]),
                                fill: isSource ? color : "white",
                                r: "6",
                                stroke: isHighlighted ? "red" : color,
                                strokeWidth: "2"
                              }));
              }),
            (function (param) {
                return /* record */Block.record(["contents"], [undefined]);
              }),
            component[/* retainedProps */11],
            (function (param, param$1) {
                return /* NoUpdate */0;
              }),
            component[/* jsElementWrapped */13]
          ]);
}

var sidePadding = 10.0;

exports.sidePadding = sidePadding;
exports.color = color;
exports.component = component;
exports.make = make;
/*  Not a pure module */
