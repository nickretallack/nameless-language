'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Printf = require("bs-platform/lib/js/printf.js");
var ConnectionNibKey$ReasonReactExamples = require("./ConnectionNibKey.bs.js");
var ConnectionNodeKey$ReasonReactExamples = require("./ConnectionNodeKey.bs.js");

function f(explicitConnectionSide) {
  var match = explicitConnectionSide[/* isSource */1];
  return Curry._3(Printf.sprintf(/* Format */[
                  /* String_literal */Block.__(11, [
                      "nib-",
                      /* String */Block.__(2, [
                          /* No_padding */0,
                          /* Char_literal */Block.__(12, [
                              /* "-" */45,
                              /* String */Block.__(2, [
                                  /* No_padding */0,
                                  /* Char_literal */Block.__(12, [
                                      /* "-" */45,
                                      /* String */Block.__(2, [
                                          /* No_padding */0,
                                          /* End_of_format */0
                                        ])
                                    ])
                                ])
                            ])
                        ])
                    ]),
                  "nib-%s-%s-%s"
                ]), ConnectionNodeKey$ReasonReactExamples.f(explicitConnectionSide[/* connectionSide */0][/* node */0]), ConnectionNibKey$ReasonReactExamples.f(explicitConnectionSide[/* connectionSide */0][/* nib */1]), match ? "source" : "sink");
}

exports.f = f;
/* No side effect */
