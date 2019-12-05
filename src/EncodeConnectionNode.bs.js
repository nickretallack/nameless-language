'use strict';

var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");

function f(connectionNode) {
  return Json_encode.object_(connectionNode ? /* :: */[
                /* tuple */[
                  "type",
                  "node"
                ],
                /* :: */[
                  /* tuple */[
                    "nodeID",
                    connectionNode[0]
                  ],
                  /* [] */0
                ]
              ] : /* :: */[
                /* tuple */[
                  "type",
                  "graph"
                ],
                /* [] */0
              ]);
}

exports.f = f;
/* No side effect */
