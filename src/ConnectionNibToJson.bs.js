'use strict';

var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");

function f(connectionNib) {
  var tmp;
  tmp = typeof connectionNib === "number" ? /* :: */[
      /* tuple */[
        "type",
        "value"
      ],
      /* [] */0
    ] : (
      connectionNib.tag ? /* :: */[
          /* tuple */[
            "type",
            "positional"
          ],
          /* :: */[
            /* tuple */[
              "index",
              connectionNib[0]
            ],
            /* [] */0
          ]
        ] : /* :: */[
          /* tuple */[
            "type",
            "nib"
          ],
          /* :: */[
            /* tuple */[
              "nibID",
              connectionNib[0]
            ],
            /* [] */0
          ]
        ]
    );
  return Json_encode.object_(tmp);
}

exports.f = f;
/* No side effect */
