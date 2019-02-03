// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var $$Array = require("bs-platform/lib/js/array.js");
var Block = require("bs-platform/lib/js/block.js");
var Belt_Id = require("bs-platform/lib/js/belt_Id.js");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Caml_exceptions = require("bs-platform/lib/js/caml_exceptions.js");
var Helpers$ReactTemplate = require("./Helpers.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");

function connectionNodeToString(connectionNode) {
  if (connectionNode) {
    return "node-" + connectionNode[0];
  } else {
    return "graph";
  }
}

function connectionNibToString(connectionNib) {
  if (typeof connectionNib === "number") {
    return "value";
  } else if (connectionNib.tag) {
    return "index-" + String(connectionNib[0]);
  } else {
    return "nib-" + connectionNib[0];
  }
}

function connectionSideToString(connectionSide) {
  return connectionNodeToString(connectionSide[/* node */0]) + ("-" + connectionNibToString(connectionSide[/* nib */1]));
}

var cmp = Caml_obj.caml_compare;

var ConnectionComparator = Belt_Id.MakeComparable(/* module */[/* cmp */cmp]);

function definedNodeKindHasValueInput(kind) {
  if (kind !== 2) {
    return kind >= 5;
  } else {
    return true;
  }
}

function definedNodeKindHasValueOutput(kind) {
  switch (kind) {
    case 1 : 
    case 3 : 
    case 4 : 
        return true;
    case 0 : 
    case 2 : 
    case 5 : 
        return false;
    
  }
}

var cmp$1 = Caml_obj.caml_compare;

var ScopeComparator = Belt_Id.MakeComparable(/* module */[/* cmp */cmp$1]);

function isFunctionDefinitionNode(node) {
  var match = node[/* kind */1];
  if (typeof match === "number" || !match.tag) {
    return false;
  } else {
    return match[0][/* kind */0] === 3;
  }
}

function isKeywordNib(nib) {
  if (typeof nib === "number" || nib.tag) {
    return false;
  } else {
    return true;
  }
}

var primitiveValueTypes = /* array */[
  /* TextType */2,
  /* NumberType */1
];

function primitiveValueTypeToString(primitiveValueType) {
  switch (primitiveValueType) {
    case 0 : 
        return "integer";
    case 1 : 
        return "number";
    case 2 : 
        return "text";
    
  }
}

function stringToPrimitiveValueType(string) {
  switch (string) {
    case "integer" : 
        return /* IntegerType */0;
    case "number" : 
        return /* NumberType */1;
    case "text" : 
        return /* TextType */2;
    default:
      throw Caml_builtin_exceptions.not_found;
  }
}

var changeTypedFields = Belt_MapString.set;

function primitiveValueToType(primitiveValue) {
  switch (primitiveValue.tag | 0) {
    case 0 : 
        return /* IntegerType */0;
    case 1 : 
        return /* NumberType */1;
    case 2 : 
        return /* TextType */2;
    
  }
}

function primitiveValueToString(primitiveValue) {
  switch (primitiveValue.tag | 0) {
    case 0 : 
        return String(primitiveValue[0]);
    case 1 : 
        return Helpers$ReactTemplate.floatToString(primitiveValue[0]);
    case 2 : 
        return primitiveValue[0];
    
  }
}

function primitiveValueToTypeString(x) {
  return primitiveValueTypeToString(primitiveValueToType(x));
}

function implementationName(implementation) {
  switch (implementation.tag | 0) {
    case 0 : 
        return "constant";
    case 1 : 
        return "interface";
    case 2 : 
        return "external";
    case 3 : 
        return "function";
    case 4 : 
        return "record type";
    case 5 : 
        return "labeled type";
    case 6 : 
        return "union type";
    
  }
}

function getTranslated(translatable, language) {
  return Belt_MapString.getExn(translatable[/* translations */1], language)[/* text */0];
}

function setTranslated(translatable, language, text) {
  return /* record */[
          /* sourceLanguage */translatable[/* sourceLanguage */0],
          /* translations */Belt_MapString.update(translatable[/* translations */1], language, (function (maybeVettable) {
                  if (maybeVettable !== undefined) {
                    var vettable = maybeVettable;
                    return /* record */[
                            /* text */text,
                            /* vetted */vettable[/* vetted */1],
                            /* authorID */vettable[/* authorID */2]
                          ];
                  } else {
                    return /* record */[
                            /* text */text,
                            /* vetted */false,
                            /* authorID */undefined
                          ];
                  }
                }))
        ];
}

function makeTranslatable(text, language) {
  return /* record */[
          /* sourceLanguage */language,
          /* translations */Belt_MapString.fromArray(/* array */[/* tuple */[
                  language,
                  /* record */[
                    /* text */text,
                    /* vetted */true,
                    /* authorID */undefined
                  ]
                ]])
        ];
}

var emptyTranslatable = makeTranslatable("", "en");

function getDisplayName(definition, language) {
  var text = getTranslated(definition[/* documentation */1][/* name */0], language);
  if (text !== "") {
    return text;
  } else {
    var match = definition[/* implementation */0];
    if (match.tag) {
      return "(nameless " + (implementationName(definition[/* implementation */0]) + ")");
    } else {
      return primitiveValueToString(match[0]);
    }
  }
}

function displayKeywordNibs(definition, language, isInputs) {
  return Belt_List.map(isInputs ? definition[/* display */2][/* inputOrdering */0] : definition[/* display */2][/* outputOrdering */1], (function (nibID) {
                var documentation = definition[/* documentation */1];
                var nibs = isInputs ? documentation[/* inputs */2] : documentation[/* outputs */3];
                var translatable = Belt_MapString.getExn(nibs, nibID);
                var vettable = Belt_MapString.getExn(translatable[/* translations */1], language);
                return /* record */[
                        /* name */vettable[/* text */0],
                        /* nib : NibConnection */Block.__(0, [nibID])
                      ];
              }));
}

function displayKeywordInputs(definition, language) {
  return displayKeywordNibs(definition, language, true);
}

function displayKeywordOutputs(definition, language) {
  return displayKeywordNibs(definition, language, false);
}

function makeDisplayNibs($staropt$star, $staropt$star$1, $staropt$star$2, $staropt$star$3, _unit) {
  var inputs = $staropt$star !== undefined ? $staropt$star : /* [] */0;
  var outputs = $staropt$star$1 !== undefined ? $staropt$star$1 : /* [] */0;
  var internalInputs = $staropt$star$2 !== undefined ? $staropt$star$2 : /* [] */0;
  var internalOutputs = $staropt$star$3 !== undefined ? $staropt$star$3 : /* [] */0;
  return /* record */[
          /* inputs */inputs,
          /* outputs */outputs,
          /* internalInputs */internalInputs,
          /* internalOutputs */internalOutputs
        ];
}

function displayDefinedNode(definition, kind, language) {
  switch (kind) {
    case 0 : 
        return makeDisplayNibs(displayKeywordNibs(definition, language, true), displayKeywordNibs(definition, language, false), undefined, undefined, /* () */0);
    case 1 : 
        return makeDisplayNibs(undefined, /* :: */[
                    /* record */[
                      /* name */"",
                      /* nib : ValueConnection */0
                    ],
                    /* [] */0
                  ], undefined, undefined, /* () */0);
    case 2 : 
        return makeDisplayNibs(/* :: */[
                    /* record */[
                      /* name */"implementation",
                      /* nib : ValueConnection */0
                    ],
                    displayKeywordNibs(definition, language, true)
                  ], displayKeywordNibs(definition, language, false), undefined, undefined, /* () */0);
    case 3 : 
        return makeDisplayNibs(undefined, /* :: */[
                    /* record */[
                      /* name */"",
                      /* nib : ValueConnection */0
                    ],
                    /* [] */0
                  ], displayKeywordNibs(definition, language, true), displayKeywordNibs(definition, language, false), /* () */0);
    case 4 : 
        return makeDisplayNibs(displayKeywordNibs(definition, language, true), /* :: */[
                    /* record */[
                      /* name */"",
                      /* nib : ValueConnection */0
                    ],
                    /* [] */0
                  ], undefined, undefined, /* () */0);
    case 5 : 
        return makeDisplayNibs(/* :: */[
                    /* record */[
                      /* name */"",
                      /* nib : ValueConnection */0
                    ],
                    /* [] */0
                  ], displayKeywordNibs(definition, language, true), undefined, undefined, /* () */0);
    
  }
}

function displayNode(node, definitions, language) {
  var match = node[/* kind */1];
  if (typeof match === "number") {
    return makeDisplayNibs(undefined, /* :: */[
                /* record */[
                  /* name */"Reference",
                  /* nib : ValueConnection */0
                ],
                /* [] */0
              ], undefined, undefined, /* () */0);
  } else if (match.tag) {
    var match$1 = match[0];
    var definition = Belt_MapString.getExn(definitions, match$1[/* definitionID */1]);
    return displayDefinedNode(definition, match$1[/* kind */0], language);
  } else {
    return makeDisplayNibs(Belt_List.makeBy(match[0], (function (index) {
                      return /* record */[
                              /* name */String(index),
                              /* nib : PositionalConnection */Block.__(1, [index])
                            ];
                    })), /* :: */[
                /* record */[
                  /* name */"",
                  /* nib : ValueConnection */0
                ],
                /* [] */0
              ], undefined, undefined, /* () */0);
  }
}

function displayNibsToExplicitConnectionSides(displayNibs, node, isSource) {
  return Belt_List.map(displayNibs, (function (displayNib) {
                return /* record */[
                        /* name */displayNib[/* name */0],
                        /* explicitConnectionSide : record */[
                          /* connectionSide : record */[
                            /* node */node,
                            /* nib */displayNib[/* nib */1]
                          ],
                          /* isSource */isSource
                        ]
                      ];
              }));
}

function collectGraphNodeNibs(nodes, definitions) {
  return Belt_List.reduce(Belt_MapString.toList(nodes), /* [] */0, (function (acc, param) {
                var match = displayNode(param[1], definitions, "en");
                var connectionNode = /* NodeConnection */[param[0]];
                return Belt_List.concatMany(/* array */[
                            acc,
                            displayNibsToExplicitConnectionSides(match[/* inputs */0], connectionNode, false),
                            displayNibsToExplicitConnectionSides(match[/* outputs */1], connectionNode, true),
                            displayNibsToExplicitConnectionSides(match[/* internalInputs */2], connectionNode, true),
                            displayNibsToExplicitConnectionSides(match[/* internalOutputs */3], connectionNode, false)
                          ]);
              }));
}

function collectAllGraphNibs(definition, definitions) {
  var match = definition[/* implementation */0];
  if (match.tag === 3) {
    return Belt_List.concatMany(/* array */[
                displayNibsToExplicitConnectionSides(displayKeywordNibs(definition, "en", false), /* GraphConnection */0, false),
                displayNibsToExplicitConnectionSides(displayKeywordNibs(definition, "en", true), /* GraphConnection */0, true),
                collectGraphNodeNibs(match[0][/* nodes */1], definitions)
              ]);
  } else {
    throw Caml_builtin_exceptions.not_found;
  }
}

function functionDefinitionNibIndex(definition, connectionNib, isSink) {
  if (typeof connectionNib === "number") {
    return 0;
  } else if (connectionNib.tag) {
    throw Caml_builtin_exceptions.not_found;
  } else {
    var nibs = displayKeywordNibs(definition, "en", !isSink);
    return Helpers$ReactTemplate.findByIndexExn(nibs, (function (param) {
                  return Caml_obj.caml_equal(param[/* nib */1], connectionNib);
                }));
  }
}

function getNodeNibIndex(node, definitions, connectionNib, isSink) {
  var match = node[/* kind */1];
  var exit = 0;
  if (typeof match === "number" || !match.tag) {
    exit = 1;
  } else {
    var match$1 = match[0];
    if (match$1[/* kind */0] !== 3) {
      exit = 1;
    } else {
      return functionDefinitionNibIndex(Belt_MapString.getExn(definitions, match$1[/* definitionID */1]), connectionNib, isSink);
    }
  }
  if (exit === 1) {
    var match$2 = displayNode(node, definitions, "en");
    var inputs = match$2[/* inputs */0];
    var nibs = isSink ? inputs : match$2[/* outputs */1];
    var nibIndex = Helpers$ReactTemplate.findByIndexExn(nibs, (function (param) {
            return Caml_obj.caml_equal(param[/* nib */1], connectionNib);
          }));
    return (
            isSink ? nibIndex : nibIndex + Belt_List.length(inputs) | 0
          ) + 1 | 0;
  }
  
}

function getOutputIndex(node, definitions, connectionNib) {
  var match = node[/* kind */1];
  var exit = 0;
  if (typeof match === "number" || !match.tag) {
    exit = 1;
  } else {
    var match$1 = match[0];
    if (match$1[/* kind */0] !== 3) {
      exit = 1;
    } else {
      return functionDefinitionNibIndex(Belt_MapString.getExn(definitions, match$1[/* definitionID */1]), connectionNib, false);
    }
  }
  if (exit === 1) {
    var match$2 = displayNode(node, definitions, "en");
    return Helpers$ReactTemplate.findByIndexExn(match$2[/* outputs */1], (function (param) {
                  return Caml_obj.caml_equal(param[/* nib */1], connectionNib);
                }));
  }
  
}

function countNodeNibs(node, definitions) {
  var match = node[/* kind */1];
  if (typeof match === "number") {
    return 1;
  } else if (match.tag) {
    var match$1 = match[0];
    var nodeDefinition = Belt_MapString.getExn(definitions, match$1[/* definitionID */1]);
    var nodeDisplay = displayDefinedNode(nodeDefinition, match$1[/* kind */0], "en");
    return Belt_List.length(nodeDisplay[/* inputs */0]) + Belt_List.length(nodeDisplay[/* outputs */1]) | 0;
  } else {
    return match[0];
  }
}

function displayDefinedType(definition, language) {
  var match = definition[/* implementation */0];
  var tmp;
  switch (match.tag | 0) {
    case 1 : 
        tmp = "Function";
        break;
    case 4 : 
        tmp = "Record";
        break;
    default:
      throw Caml_builtin_exceptions.not_found;
  }
  return getTranslated(definition[/* documentation */1][/* name */0], language) + (" " + tmp);
}

function displayPrimitiveValueType(primitiveValueType) {
  switch (primitiveValueType) {
    case 0 : 
        return "Integer";
    case 1 : 
        return "Number";
    case 2 : 
        return "Text";
    
  }
}

function displayValueType(valueType, definitions, language) {
  if (valueType.tag) {
    return displayDefinedType(Belt_MapString.getExn(definitions, valueType[0]), language);
  } else {
    return displayPrimitiveValueType(valueType[0]);
  }
}

function makeNibDocs(nibs) {
  return Belt_MapString.fromArray(Belt_Array.map(nibs, (function (param) {
                    return /* tuple */[
                            param[0],
                            makeTranslatable(param[1], "en")
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
            /* name */makeTranslatable(name, "en"),
            /* description */makeTranslatable(description, "en"),
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

function makeGraph($staropt$star, $staropt$star$1, $staropt$star$2, $staropt$star$3, $staropt$star$4, $staropt$star$5, _unit) {
  var name = $staropt$star !== undefined ? $staropt$star : "";
  var description = $staropt$star$1 !== undefined ? $staropt$star$1 : "";
  var inputs = $staropt$star$2 !== undefined ? $staropt$star$2 : /* array */[];
  var outputs = $staropt$star$3 !== undefined ? $staropt$star$3 : /* array */[];
  var nodes = $staropt$star$4 !== undefined ? $staropt$star$4 : /* array */[];
  var connections = $staropt$star$5 !== undefined ? $staropt$star$5 : /* array */[];
  return makeDefinition(name, description, inputs, outputs, /* GraphImplementation */Block.__(3, [/* record */[
                  /* connections */Belt_Map.fromArray(connections, ConnectionComparator),
                  /* nodes */Belt_MapString.fromArray(nodes)
                ]]), /* () */0);
}

var InvalidConnection = Caml_exceptions.create("Definition-ReactTemplate.InvalidConnection");

var CycleDetected = Caml_exceptions.create("Definition-ReactTemplate.CycleDetected");

exports.connectionNodeToString = connectionNodeToString;
exports.connectionNibToString = connectionNibToString;
exports.connectionSideToString = connectionSideToString;
exports.ConnectionComparator = ConnectionComparator;
exports.definedNodeKindHasValueInput = definedNodeKindHasValueInput;
exports.definedNodeKindHasValueOutput = definedNodeKindHasValueOutput;
exports.ScopeComparator = ScopeComparator;
exports.isFunctionDefinitionNode = isFunctionDefinitionNode;
exports.isKeywordNib = isKeywordNib;
exports.primitiveValueTypes = primitiveValueTypes;
exports.primitiveValueTypeToString = primitiveValueTypeToString;
exports.stringToPrimitiveValueType = stringToPrimitiveValueType;
exports.changeTypedFields = changeTypedFields;
exports.primitiveValueToType = primitiveValueToType;
exports.primitiveValueToString = primitiveValueToString;
exports.primitiveValueToTypeString = primitiveValueToTypeString;
exports.implementationName = implementationName;
exports.getTranslated = getTranslated;
exports.setTranslated = setTranslated;
exports.makeTranslatable = makeTranslatable;
exports.emptyTranslatable = emptyTranslatable;
exports.getDisplayName = getDisplayName;
exports.displayKeywordNibs = displayKeywordNibs;
exports.displayKeywordInputs = displayKeywordInputs;
exports.displayKeywordOutputs = displayKeywordOutputs;
exports.makeDisplayNibs = makeDisplayNibs;
exports.displayDefinedNode = displayDefinedNode;
exports.displayNode = displayNode;
exports.displayNibsToExplicitConnectionSides = displayNibsToExplicitConnectionSides;
exports.collectGraphNodeNibs = collectGraphNodeNibs;
exports.collectAllGraphNibs = collectAllGraphNibs;
exports.functionDefinitionNibIndex = functionDefinitionNibIndex;
exports.getNodeNibIndex = getNodeNibIndex;
exports.getOutputIndex = getOutputIndex;
exports.countNodeNibs = countNodeNibs;
exports.displayDefinedType = displayDefinedType;
exports.displayPrimitiveValueType = displayPrimitiveValueType;
exports.displayValueType = displayValueType;
exports.makeNibDocs = makeNibDocs;
exports.makeDefinition = makeDefinition;
exports.makeGraph = makeGraph;
exports.InvalidConnection = InvalidConnection;
exports.CycleDetected = CycleDetected;
/* ConnectionComparator Not a pure module */
