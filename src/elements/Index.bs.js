// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var ReactDOMRe = require("reason-react/src/ReactDOMRe.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var App$ReactTemplate = require("./App.bs.js");
var Definition$ReactTemplate = require("../Definition.bs.js");

var example = Definition$ReactTemplate.makeGraph("Example Definition", "An example function", /* array */[
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
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionCallNode */0,
                /* definitionID */"example"
              ]])
        ]
      ],
      /* tuple */[
        "node2",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionCallNode */0,
                /* definitionID */"example"
              ]])
        ]
      ],
      /* tuple */[
        "node3",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionCallNode */0,
                /* definitionID */"example"
              ]])
        ]
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

var simple = Definition$ReactTemplate.makeGraph("Simple Definition", "A simple function", /* array */[
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
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : ValueNode */1,
                /* definitionID */"one"
              ]])
        ]
      ],
      /* tuple */[
        "node2",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : ValueNode */1,
                /* definitionID */"one"
              ]])
        ]
      ],
      /* tuple */[
        "node3",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionCallNode */0,
                /* definitionID */"plus"
              ]])
        ]
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

var one = Definition$ReactTemplate.makeDefinition(undefined, undefined, undefined, undefined, /* ConstantImplementation */Block.__(0, [/* NumberValue */Block.__(1, [1.0])]), /* () */0);

var point = Definition$ReactTemplate.makeDefinition("2D Point", "X and Y coordinates", /* array */[
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

var plus = Definition$ReactTemplate.makeDefinition("+", "Add two numbers, resulting in a number.", /* array */[
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

var pointExample = Definition$ReactTemplate.makeGraph("Point Example", "", /* array */[], /* array */[
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
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : ConstructorNode */4,
                /* definitionID */"point"
              ]])
        ]
      ],
      /* tuple */[
        "destructure",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : AccessorNode */5,
                /* definitionID */"point"
              ]])
        ]
      ],
      /* tuple */[
        "x",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : ValueNode */1,
                /* definitionID */"one"
              ]])
        ]
      ],
      /* tuple */[
        "y",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : ValueNode */1,
                /* definitionID */"one"
              ]])
        ]
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

var referenceExample = Definition$ReactTemplate.makeGraph("Reference example", "", /* array */[], /* array */[/* tuple */[
        "reference",
        "Reference"
      ]], /* array */[/* tuple */[
        "reference",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : ReferenceNode */0
        ]
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

var exampleInterface = Definition$ReactTemplate.makeDefinition("Example Interface", undefined, /* array */[
      /* tuple */[
        "in-left",
        "Left"
      ],
      /* tuple */[
        "in-right",
        "Right"
      ]
    ], /* array */[
      /* tuple */[
        "out-left",
        "Left"
      ],
      /* tuple */[
        "out-right",
        "Right"
      ]
    ], /* InterfaceImplementation */Block.__(1, [/* record */[
          /* inputTypes */Belt_MapString.fromArray(/* array */[
                /* tuple */[
                  "in-left",
                  /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
                ],
                /* tuple */[
                  "in-right",
                  /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
                ]
              ]),
          /* outputTypes */Belt_MapString.fromArray(/* array */[
                /* tuple */[
                  "out-left",
                  /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
                ],
                /* tuple */[
                  "out-right",
                  /* PrimitiveValueType */Block.__(0, [/* NumberType */1])
                ]
              ])
        ]]), /* () */0);

var interfaceExample = Definition$ReactTemplate.makeGraph("Interface Example", undefined, undefined, /* array */[/* tuple */[
        "result",
        "Result"
      ]], /* array */[
      /* tuple */[
        "definition",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionDefinitionNode */3,
                /* definitionID */"example-interface"
              ]])
        ]
      ],
      /* tuple */[
        "plus1",
        /* record */[
          /* scope : NodeScope */["definition"],
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionCallNode */0,
                /* definitionID */"plus"
              ]])
        ]
      ],
      /* tuple */[
        "plus2",
        /* record */[
          /* scope : NodeScope */["definition"],
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionCallNode */0,
                /* definitionID */"plus"
              ]])
        ]
      ],
      /* tuple */[
        "plus3",
        /* record */[
          /* scope : NodeScope */["definition"],
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionCallNode */0,
                /* definitionID */"plus"
              ]])
        ]
      ],
      /* tuple */[
        "plus4",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionCallNode */0,
                /* definitionID */"plus"
              ]])
        ]
      ]
    ], /* array */[
      /* tuple */[
        /* record */[
          /* node : GraphConnection */0,
          /* nib : NibConnection */Block.__(0, ["result"])
        ],
        /* record */[
          /* node : NodeConnection */["definition"],
          /* nib : ValueConnection */0
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["definition"],
          /* nib : NibConnection */Block.__(0, ["out-left"])
        ],
        /* record */[
          /* node : NodeConnection */["plus1"],
          /* nib : NibConnection */Block.__(0, ["result"])
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["plus1"],
          /* nib : NibConnection */Block.__(0, ["left"])
        ],
        /* record */[
          /* node : NodeConnection */["plus3"],
          /* nib : NibConnection */Block.__(0, ["result"])
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["plus2"],
          /* nib : NibConnection */Block.__(0, ["left"])
        ],
        /* record */[
          /* node : NodeConnection */["plus3"],
          /* nib : NibConnection */Block.__(0, ["result"])
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["plus1"],
          /* nib : NibConnection */Block.__(0, ["right"])
        ],
        /* record */[
          /* node : NodeConnection */["plus2"],
          /* nib : NibConnection */Block.__(0, ["result"])
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["definition"],
          /* nib : NibConnection */Block.__(0, ["out-right"])
        ],
        /* record */[
          /* node : NodeConnection */["plus4"],
          /* nib : NibConnection */Block.__(0, ["result"])
        ]
      ],
      /* tuple */[
        /* record */[
          /* node : NodeConnection */["plus3"],
          /* nib : NibConnection */Block.__(0, ["right"])
        ],
        /* record */[
          /* node : NodeConnection */["definition"],
          /* nib : NibConnection */Block.__(0, ["in-right"])
        ]
      ]
    ], /* () */0);

var nestedInlineExample = Definition$ReactTemplate.makeGraph("Nested Inline Functions Example", undefined, undefined, /* array */[/* tuple */[
        "result",
        "Result"
      ]], /* array */[
      /* tuple */[
        "definition",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionDefinitionNode */3,
                /* definitionID */"example-interface"
              ]])
        ]
      ],
      /* tuple */[
        "definition2",
        /* record */[
          /* scope : NodeScope */["definition"],
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionDefinitionNode */3,
                /* definitionID */"example-interface"
              ]])
        ]
      ],
      /* tuple */[
        "definition3",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionDefinitionNode */3,
                /* definitionID */"example-interface"
              ]])
        ]
      ],
      /* tuple */[
        "plus",
        /* record */[
          /* scope : NodeScope */["definition3"],
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionCallNode */0,
                /* definitionID */"plus"
              ]])
        ]
      ],
      /* tuple */[
        "plus2",
        /* record */[
          /* scope : GraphScope */0,
          /* kind : DefinedNode */Block.__(1, [/* record */[
                /* kind : FunctionCallNode */0,
                /* definitionID */"plus"
              ]])
        ]
      ]
    ], /* array */[/* tuple */[
        /* record */[
          /* node : NodeConnection */["plus"],
          /* nib : NibConnection */Block.__(0, ["right"])
        ],
        /* record */[
          /* node : NodeConnection */["plus2"],
          /* nib : NibConnection */Block.__(0, ["result"])
        ]
      ]], /* () */0);

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
      ],
      /* tuple */[
        "interface-example",
        interfaceExample
      ],
      /* tuple */[
        "nested-inline-example",
        nestedInlineExample
      ]
    ]);

ReactDOMRe.renderToElementWithId(ReasonReact.element(undefined, undefined, App$ReactTemplate.make(definitions, /* array */[])), "graph");

exports.example = example;
exports.simple = simple;
exports.one = one;
exports.point = point;
exports.plus = plus;
exports.pointExample = pointExample;
exports.referenceExample = referenceExample;
exports.exampleInterface = exampleInterface;
exports.interfaceExample = interfaceExample;
exports.nestedInlineExample = nestedInlineExample;
exports.definitions = definitions;
/* example Not a pure module */
