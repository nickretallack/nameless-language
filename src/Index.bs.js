// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var $$Array = require("bs-platform/lib/js/array.js");
var Block = require("bs-platform/lib/js/block.js");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var ReactDOMRe = require("reason-react/src/ReactDOMRe.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var App$ReactTemplate = require("./App.bs.js");
var Definition$ReactTemplate = require("./Definition.bs.js");
var WindowSize$ReactTemplate = require("./WindowSize.bs.js");

function makeNibDocs(nibs) {
  return Belt_MapString.fromArray(Belt_Array.map(nibs, (function (param) {
                    return /* tuple */[
                            param[0],
                            Definition$ReactTemplate.makeTranslatable(param[1], "en")
                          ];
                  })));
}

function makeDefinition($staropt$star, $staropt$star$1, $staropt$star$2, $staropt$star$3, implementation, _unit) {
  var name = $staropt$star !== undefined ? $staropt$star : "";
  var description = $staropt$star$1 !== undefined ? $staropt$star$1 : "";
  var inputs = $staropt$star$2 !== undefined ? $staropt$star$2 : /* array */[];
  var outputs = $staropt$star$3 !== undefined ? $staropt$star$3 : /* array */[];
  return /* record */[
          /* implementation */implementation,
          /* documentation : record */[
            /* name */Definition$ReactTemplate.makeTranslatable(name, "en"),
            /* description */Definition$ReactTemplate.makeTranslatable(description, "en"),
            /* inputs */makeNibDocs(inputs),
            /* outputs */makeNibDocs(outputs)
          ],
          /* display : record */[
            /* inputOrdering */$$Array.to_list($$Array.map((function (param) {
                        return param[0];
                      }), inputs)),
            /* outputOrdering */$$Array.to_list($$Array.map((function (param) {
                        return param[0];
                      }), outputs))
          ]
        ];
}

function makeGraph(name, description, inputs, outputs, nodes, connections, _unit) {
  return makeDefinition(name, description, inputs, outputs, /* GraphImplementation */Block.__(3, [/* record */[
                  /* connections */Belt_Map.fromArray(connections, Definition$ReactTemplate.ConnectionComparator),
                  /* nodes */Belt_MapString.fromArray(nodes)
                ]]), /* () */0);
}

var example = makeGraph("Example Definition", "An example function", /* array */[
      /* tuple */[
        "in1",
        "In 1"
      ],
      /* tuple */[
        "in2",
        "In 2"
      ]
    ], /* array */[
      /* tuple */[
        "out1",
        "Out 1"
      ],
      /* tuple */[
        "out2",
        "Out 2"
      ]
    ], /* array */[
      /* tuple */[
        "node1",
        /* DefinedNode */Block.__(1, [/* record */[
              /* kind : FunctionCallNode */0,
              /* definitionID */"example"
            ]])
      ],
      /* tuple */[
        "node2",
        /* DefinedNode */Block.__(1, [/* record */[
              /* kind : FunctionCallNode */0,
              /* definitionID */"example"
            ]])
      ],
      /* tuple */[
        "node3",
        /* DefinedNode */Block.__(1, [/* record */[
              /* kind : FunctionCallNode */0,
              /* definitionID */"example"
            ]])
      ]
    ], /* array */[
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["node1"],
          /* nib : NibConnection */Block.__(0, ["in1"])
        ],
        /* record */[
          /* node : NodeConnection */["node2"],
          /* nib : NibConnection */Block.__(0, ["out1"])
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["node1"],
          /* nib : NibConnection */Block.__(0, ["in2"])
        ],
        /* record */[
          /* node : NodeConnection */["node2"],
          /* nib : NibConnection */Block.__(0, ["out2"])
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : GraphConnection */0,
          /* nib : NibConnection */Block.__(0, ["out2"])
        ],
        /* record */[
          /* node : NodeConnection */["node1"],
          /* nib : NibConnection */Block.__(0, ["out2"])
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["node2"],
          /* nib : NibConnection */Block.__(0, ["in1"])
        ],
        /* record */[
          /* node : GraphConnection */0,
          /* nib : NibConnection */Block.__(0, ["in1"])
        ]
      ]
    ], /* () */0);

var simple = makeGraph("Simple Definition", "A simple function", /* array */[
      /* tuple */[
        "in1",
        "In 1"
      ],
      /* tuple */[
        "in2",
        "In 2"
      ]
    ], /* array */[
      /* tuple */[
        "out1",
        "Out 1"
      ],
      /* tuple */[
        "out2",
        "Out 2"
      ]
    ], /* array */[
      /* tuple */[
        "node1",
        /* DefinedNode */Block.__(1, [/* record */[
              /* kind : ValueNode */1,
              /* definitionID */"one"
            ]])
      ],
      /* tuple */[
        "node2",
        /* DefinedNode */Block.__(1, [/* record */[
              /* kind : ValueNode */1,
              /* definitionID */"one"
            ]])
      ],
      /* tuple */[
        "node3",
        /* DefinedNode */Block.__(1, [/* record */[
              /* kind : FunctionCallNode */0,
              /* definitionID */"plus"
            ]])
      ]
    ], /* array */[
      /* tuple */[
        /* record */[
          /* node : GraphConnection */0,
          /* nib : NibConnection */Block.__(0, ["out1"])
        ],
        /* record */[
          /* node : NodeConnection */["node3"],
          /* nib : NibConnection */Block.__(0, ["result"])
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["node3"],
          /* nib : NibConnection */Block.__(0, ["left"])
        ],
        /* record */[
          /* node : NodeConnection */["node1"],
          /* nib : ValueConnection */0
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["node3"],
          /* nib : NibConnection */Block.__(0, ["right"])
        ],
        /* record */[
          /* node : NodeConnection */["node2"],
          /* nib : ValueConnection */0
        ]
      ]
    ], /* () */0);

var one = makeDefinition("1", undefined, undefined, undefined, /* ConstantImplementation */Block.__(0, [/* NumberValue */Block.__(1, [1.0])]), /* () */0);

var point = makeDefinition("2D Point", "X and Y coordinates", /* array */[
      /* tuple */[
        "x",
        "X"
      ],
      /* tuple */[
        "y",
        "Y"
      ]
    ], undefined, /* RecordTypeImplementation */Block.__(4, [Belt_MapString.fromArray(/* array */[
              /* tuple */[
                "x",
                /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
              ],
              /* tuple */[
                "y",
                /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
              ]
            ])]), /* () */0);

var plus = makeDefinition("+", "Add two numbers, resulting in a number.", /* array */[
      /* tuple */[
        "left",
        "Left"
      ],
      /* tuple */[
        "right",
        "Right"
      ]
    ], /* array */[/* tuple */[
        "result",
        "Result"
      ]], /* ExternalImplementation */Block.__(2, [/* record */[
          /* name */"+",
          /* interface : record */[
            /* inputTypes */Belt_MapString.fromArray(/* array */[
                  /* tuple */[
                    "left",
                    /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
                  ],
                  /* tuple */[
                    "right",
                    /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
                  ]
                ]),
            /* outputTypes */Belt_MapString.fromArray(/* array */[/* tuple */[
                    "result",
                    /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
                  ]])
          ]
        ]]), /* () */0);

var pointExample = makeGraph("Point Example", "", /* array */[], /* array */[
      /* tuple */[
        "x",
        "X"
      ],
      /* tuple */[
        "y",
        "Y"
      ]
    ], /* array */[
      /* tuple */[
        "constructor",
        /* DefinedNode */Block.__(1, [/* record */[
              /* kind : ConstructorNode */4,
              /* definitionID */"point"
            ]])
      ],
      /* tuple */[
        "destructure",
        /* DefinedNode */Block.__(1, [/* record */[
              /* kind : AccessorNode */5,
              /* definitionID */"point"
            ]])
      ],
      /* tuple */[
        "x",
        /* DefinedNode */Block.__(1, [/* record */[
              /* kind : ValueNode */1,
              /* definitionID */"one"
            ]])
      ],
      /* tuple */[
        "y",
        /* DefinedNode */Block.__(1, [/* record */[
              /* kind : ValueNode */1,
              /* definitionID */"one"
            ]])
      ]
    ], /* array */[
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["constructor"],
          /* nib : NibConnection */Block.__(0, ["x"])
        ],
        /* record */[
          /* node : NodeConnection */["x"],
          /* nib : ValueConnection */0
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["constructor"],
          /* nib : NibConnection */Block.__(0, ["y"])
        ],
        /* record */[
          /* node : NodeConnection */["y"],
          /* nib : ValueConnection */0
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["destructure"],
          /* nib : ValueConnection */0
        ],
        /* record */[
          /* node : NodeConnection */["constructor"],
          /* nib : ValueConnection */0
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : GraphConnection */0,
          /* nib : NibConnection */Block.__(0, ["x"])
        ],
        /* record */[
          /* node : NodeConnection */["destructure"],
          /* nib : NibConnection */Block.__(0, ["x"])
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : GraphConnection */0,
          /* nib : NibConnection */Block.__(0, ["y"])
        ],
        /* record */[
          /* node : NodeConnection */["destructure"],
          /* nib : NibConnection */Block.__(0, ["y"])
        ]
      ]
    ], /* () */0);

var referenceExample = makeGraph("Reference example", "", /* array */[], /* array */[/* tuple */[
        "reference",
        "Reference"
      ]], /* array */[/* tuple */[
        "reference",
        /* ReferenceNode */0
      ]], /* array */[/* tuple */[
        /* record */[
          /* node : GraphConnection */0,
          /* nib : NibConnection */Block.__(0, ["reference"])
        ],
        /* record */[
          /* node : NodeConnection */["reference"],
          /* nib : ValueConnection */0
        ]
      ]], /* () */0);

var exampleInterface = makeDefinition("Example Interface", undefined, /* array */[
      /* tuple */[
        "left",
        "Left"
      ],
      /* tuple */[
        "right",
        "Right"
      ]
    ], /* array */[/* tuple */[
        "result",
        "Result"
      ]], /* InterfaceImplementation */Block.__(1, [/* record */[
          /* inputTypes */Belt_MapString.fromArray(/* array */[
                /* tuple */[
                  "left",
                  /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
                ],
                /* tuple */[
                  "right",
                  /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
                ]
              ]),
          /* outputTypes */Belt_MapString.fromArray(/* array */[/* tuple */[
                  "result",
                  /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
                ]])
        ]]), /* () */0);

var definitions = Belt_MapString.fromArray(/* array */[
      /* tuple */[
        "example",
        example
      ],
      /* tuple */[
        "simple",
        simple
      ],
      /* tuple */[
        "one",
        one
      ],
      /* tuple */[
        "plus",
        plus
      ],
      /* tuple */[
        "point",
        point
      ],
      /* tuple */[
        "point-example",
        pointExample
      ],
      /* tuple */[
        "reference-example",
        referenceExample
      ],
      /* tuple */[
        "example-interface",
        exampleInterface
      ]
    ]);

ReactDOMRe.renderToElementWithId(ReasonReact.element(undefined, undefined, WindowSize$ReactTemplate.make((function (size) {
                return ReasonReact.element(undefined, undefined, App$ReactTemplate.make(size, definitions, /* array */[]));
              }), /* array */[])), "graph");

console.log(example);

exports.makeNibDocs = makeNibDocs;
exports.makeDefinition = makeDefinition;
exports.makeGraph = makeGraph;
exports.example = example;
exports.simple = simple;
exports.one = one;
exports.point = point;
exports.plus = plus;
exports.pointExample = pointExample;
exports.referenceExample = referenceExample;
exports.exampleInterface = exampleInterface;
exports.definitions = definitions;
/* example Not a pure module */
