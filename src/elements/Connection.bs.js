// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Printf = require("bs-platform/lib/js/printf.js");
var $$String = require("bs-platform/lib/js/string.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Caml_primitive = require("bs-platform/lib/js/caml_primitive.js");

var component = ReasonReact.statelessComponent("Connection");

function curveConnect(sourcePosition, sinkPosition, nudge) {
  var delta_000 = /* x */sinkPosition[/* x */0] - sourcePosition[/* x */0];
  var delta_001 = /* y */sinkPosition[/* y */1] - sourcePosition[/* y */1];
  var match = delta_000 < 0.0;
  var match$1 = delta_001 < 0.0;
  var direction_000 = /* x */match ? -1.0 : 1.0;
  var direction_001 = /* y */match$1 ? -1.0 : 1.0;
  var goesUpward = direction_000 < 0.0;
  var match$2 = direction_001 < 0.0;
  var leftIsDownward = match$2 ? goesUpward : !goesUpward;
  var curveSize_000 = /* x */Caml_primitive.caml_float_min(Math.abs(delta_000) / 2.0, 15.0);
  var curveSize_001 = /* y */Caml_primitive.caml_float_min(Math.abs(delta_001) / 2.0, 15.0);
  return Curry.app(Printf.sprintf(/* Format */[
                  /* String_literal */Block.__(11, [
                      "h ",
                      /* Float */Block.__(8, [
                          /* Float_f */0,
                          /* No_padding */0,
                          /* No_precision */0,
                          /* String_literal */Block.__(11, [
                              " a ",
                              /* Float */Block.__(8, [
                                  /* Float_f */0,
                                  /* No_padding */0,
                                  /* No_precision */0,
                                  /* Char_literal */Block.__(12, [
                                      /* "," */44,
                                      /* Float */Block.__(8, [
                                          /* Float_f */0,
                                          /* No_padding */0,
                                          /* No_precision */0,
                                          /* String_literal */Block.__(11, [
                                              " 0 0,",
                                              /* Int */Block.__(4, [
                                                  /* Int_i */3,
                                                  /* No_padding */0,
                                                  /* No_precision */0,
                                                  /* Char_literal */Block.__(12, [
                                                      /* " " */32,
                                                      /* Float */Block.__(8, [
                                                          /* Float_f */0,
                                                          /* No_padding */0,
                                                          /* No_precision */0,
                                                          /* Char_literal */Block.__(12, [
                                                              /* "," */44,
                                                              /* Float */Block.__(8, [
                                                                  /* Float_f */0,
                                                                  /* No_padding */0,
                                                                  /* No_precision */0,
                                                                  /* String_literal */Block.__(11, [
                                                                      " v ",
                                                                      /* Float */Block.__(8, [
                                                                          /* Float_f */0,
                                                                          /* No_padding */0,
                                                                          /* No_precision */0,
                                                                          /* String_literal */Block.__(11, [
                                                                              " a ",
                                                                              /* Float */Block.__(8, [
                                                                                  /* Float_f */0,
                                                                                  /* No_padding */0,
                                                                                  /* No_precision */0,
                                                                                  /* Char_literal */Block.__(12, [
                                                                                      /* "," */44,
                                                                                      /* Float */Block.__(8, [
                                                                                          /* Float_f */0,
                                                                                          /* No_padding */0,
                                                                                          /* No_precision */0,
                                                                                          /* String_literal */Block.__(11, [
                                                                                              " 0 0,",
                                                                                              /* Int */Block.__(4, [
                                                                                                  /* Int_i */3,
                                                                                                  /* No_padding */0,
                                                                                                  /* No_precision */0,
                                                                                                  /* Char_literal */Block.__(12, [
                                                                                                      /* " " */32,
                                                                                                      /* Float */Block.__(8, [
                                                                                                          /* Float_f */0,
                                                                                                          /* No_padding */0,
                                                                                                          /* No_precision */0,
                                                                                                          /* Char_literal */Block.__(12, [
                                                                                                              /* "," */44,
                                                                                                              /* Float */Block.__(8, [
                                                                                                                  /* Float_f */0,
                                                                                                                  /* No_padding */0,
                                                                                                                  /* No_precision */0,
                                                                                                                  /* String_literal */Block.__(11, [
                                                                                                                      " h ",
                                                                                                                      /* Float */Block.__(8, [
                                                                                                                          /* Float_f */0,
                                                                                                                          /* No_padding */0,
                                                                                                                          /* No_precision */0,
                                                                                                                          /* End_of_format */0
                                                                                                                        ])
                                                                                                                    ])
                                                                                                                ])
                                                                                                            ])
                                                                                                        ])
                                                                                                    ])
                                                                                                ])
                                                                                            ])
                                                                                        ])
                                                                                    ])
                                                                                ])
                                                                            ])
                                                                        ])
                                                                    ])
                                                                ])
                                                            ])
                                                        ])
                                                    ])
                                                ])
                                            ])
                                        ])
                                    ])
                                ])
                            ])
                        ])
                    ]),
                  "h %f a %f,%f 0 0,%i %f,%f v %f a %f,%f 0 0,%i %f,%f h %f"
                ]), [
              delta_000 / 2.0 - curveSize_000 * direction_000 + nudge,
              curveSize_000,
              curveSize_001,
              leftIsDownward ? 1 : 0,
              curveSize_000 * direction_000,
              curveSize_001 * direction_001,
              delta_001 - curveSize_001 * 2.0 * direction_001,
              curveSize_000,
              curveSize_001,
              leftIsDownward ? 0 : 1,
              curveSize_000 * direction_000,
              curveSize_001 * direction_001,
              delta_000 / 2.0 - curveSize_000 * direction_000 - nudge
            ]);
}

function make(sourcePosition, sinkPosition, $staropt$star, color, $staropt$star$1, nodeWidth, xPadding, onClick, sourceIndex, _children) {
  var isSelected = $staropt$star !== undefined ? $staropt$star : false;
  var segments = $staropt$star$1 !== undefined ? $staropt$star$1 : /* [] */0;
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
              var nudge = 5.0 * (
                sourceIndex % 2 === 0 ? sourceIndex / 2 | 0 : (-(sourceIndex - 1 | 0) | 0) / 2 | 0
              );
              var tmp;
              if (Belt_List.length(segments) === 0) {
                tmp = curveConnect(sourcePosition, sinkPosition, nudge);
              } else {
                var match = Belt_List.reduce(segments, /* tuple */[
                      /* [] */0,
                      sourcePosition
                    ], (function (param, segmentY) {
                        var lastPosition = param[1];
                        return /* tuple */[
                                /* :: */[
                                  curveConnect(lastPosition, /* record */[
                                        /* x */lastPosition[/* x */0] - xPadding,
                                        /* y */segmentY + nudge
                                      ], nudge) + Curry._1(Printf.sprintf(/* Format */[
                                            /* String_literal */Block.__(11, [
                                                " h ",
                                                /* Float */Block.__(8, [
                                                    /* Float_f */0,
                                                    /* No_padding */0,
                                                    /* No_precision */0,
                                                    /* Char_literal */Block.__(12, [
                                                        /* " " */32,
                                                        /* End_of_format */0
                                                      ])
                                                  ])
                                              ]),
                                            " h %f "
                                          ]), -nodeWidth),
                                  param[0]
                                ],
                                /* record */[
                                  /* x */lastPosition[/* x */0] - xPadding - nodeWidth,
                                  /* y */segmentY + nudge
                                ]
                              ];
                      }));
                tmp = $$String.concat("               ", Belt_List.reverse(/* :: */[
                          curveConnect(match[1], sinkPosition, nudge),
                          match[0]
                        ]));
              }
              var path = Curry._2(Printf.sprintf(/* Format */[
                        /* Char_literal */Block.__(12, [
                            /* "M" */77,
                            /* Float */Block.__(8, [
                                /* Float_f */0,
                                /* No_padding */0,
                                /* No_precision */0,
                                /* Char_literal */Block.__(12, [
                                    /* "," */44,
                                    /* Float */Block.__(8, [
                                        /* Float_f */0,
                                        /* No_padding */0,
                                        /* No_precision */0,
                                        /* Char_literal */Block.__(12, [
                                            /* " " */32,
                                            /* End_of_format */0
                                          ])
                                      ])
                                  ])
                              ])
                          ]),
                        "M%f,%f "
                      ]), sourcePosition[/* x */0], sourcePosition[/* y */1]) + tmp;
              var tmp$1 = {
                d: path,
                fill: "transparent",
                pointerEvents: "visibleStroke",
                stroke: isSelected ? "red" : color,
                strokeWidth: "5"
              };
              if (onClick !== undefined) {
                tmp$1.onClick = Caml_option.valFromOption(onClick);
              }
              return React.createElement("path", tmp$1);
            }),
          /* initialState */component[/* initialState */10],
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */component[/* reducer */12],
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.curveConnect = curveConnect;
exports.make = make;
/* component Not a pure module */
