'use strict';

var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");
var ConnectionNibToJson$ReasonReactExamples = require("./ConnectionNibToJson.bs.js");
var ConnectionNodeToJson$ReasonReactExamples = require("./ConnectionNodeToJson.bs.js");

function f(connectionSide) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "node",
                ConnectionNodeToJson$ReasonReactExamples.f(connectionSide[/* node */0])
              ],
              /* :: */[
                /* tuple */[
                  "nib",
                  ConnectionNibToJson$ReasonReactExamples.f(connectionSide[/* nib */1])
                ],
                /* [] */0
              ]
            ]);
}

exports.f = f;
/* No side effect */
