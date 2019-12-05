'use strict';

var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");
var DefinedNodeToJson$ReasonReactExamples = require("./DefinedNodeToJson.bs.js");

function f(nodeKind) {
  if (typeof nodeKind === "number") {
    return Json_encode.object_(/* :: */[
                /* tuple */[
                  "type",
                  "reference"
                ],
                /* [] */0
              ]);
  } else if (nodeKind.tag) {
    return DefinedNodeToJson$ReasonReactExamples.f(nodeKind[0]);
  } else {
    return Json_encode.object_(/* :: */[
                /* tuple */[
                  "type",
                  "list"
                ],
                /* :: */[
                  /* tuple */[
                    "length",
                    nodeKind[0]
                  ],
                  /* [] */0
                ]
              ]);
  }
}

exports.f = f;
/* No side effect */
