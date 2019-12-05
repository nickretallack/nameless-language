'use strict';

var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");
var DefinedNodeKindToString$ReasonReactExamples = require("./DefinedNodeKindToString.bs.js");

function f(definedNode) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "type",
                "defined"
              ],
              /* :: */[
                /* tuple */[
                  "definitionID",
                  definedNode[/* definitionID */1]
                ],
                /* :: */[
                  /* tuple */[
                    "kind",
                    DefinedNodeKindToString$ReasonReactExamples.f(definedNode[/* kind */0])
                  ],
                  /* [] */0
                ]
              ]
            ]);
}

exports.f = f;
/* No side effect */
