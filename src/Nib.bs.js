// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var $$String = require("bs-platform/lib/js/string.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Helpers$ReactTemplate = require("./Helpers.bs.js");

var component = ReasonReact.reducerComponent("Nib");

function make(isSource, connectionSide, emit, _children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */(function (self) {
              var match = self[/* state */1][0];
              if (match !== undefined) {
                Caml_option.valFromOption(match).addEventListener("finish-drawing", (function ($$event) {
                        return Curry._1(emit, /* FinishDrawing */Block.__(2, [/* record */[
                                        /* pointerID : Touch */[$$event.detail.identifier],
                                        /* connectionSide */connectionSide,
                                        /* isSource */isSource
                                      ]]));
                      }));
                return /* () */0;
              } else {
                return /* () */0;
              }
            }),
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (self) {
              return React.createElement("div", {
                          ref: (function (ref) {
                              self[/* state */1][0] = (ref == null) ? undefined : Caml_option.some(ref);
                              return /* () */0;
                            }),
                          className: $$String.concat(" ", /* :: */[
                                isSource ? "source" : "sink",
                                /* :: */[
                                  "nib",
                                  /* [] */0
                                ]
                              ]),
                          onMouseDown: (function ($$event) {
                              return Curry._1(emit, /* StartDrawing */Block.__(0, [/* record */[
                                              /* pointerID : Mouse */0,
                                              /* drawingConnection : record */[
                                                /* connectionSide */connectionSide,
                                                /* startIsSource */isSource,
                                                /* point */Helpers$ReactTemplate.pointFromMouse($$event)
                                              ]
                                            ]]));
                            }),
                          onMouseUp: (function (param) {
                              return Curry._1(emit, /* FinishDrawing */Block.__(2, [/* record */[
                                              /* pointerID : Mouse */0,
                                              /* connectionSide */connectionSide,
                                              /* isSource */isSource
                                            ]]));
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
                                            return Curry._1(emit, /* StartDrawing */Block.__(0, [/* record */[
                                                            /* pointerID : Touch */[touch.identifier],
                                                            /* drawingConnection : record */[
                                                              /* connectionSide */connectionSide,
                                                              /* startIsSource */isSource,
                                                              /* point : record */[
                                                                /* x */touch.clientX,
                                                                /* y */touch.clientY
                                                              ]
                                                            ]
                                                          ]]));
                                          }));
                            })
                        });
            }),
          /* initialState */(function (param) {
              return /* record */[/* contents */undefined];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (param, param$1) {
              return /* NoUpdate */0;
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.make = make;
/* component Not a pure module */