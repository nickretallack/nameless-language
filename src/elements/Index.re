[%%debugger.chrome];
open Definition;

let example =
  makeGraph(
    ~name="Example Definition",
    ~description="An example function",
    ~inputs=[|("in1", "In 1"), ("in2", "In 2")|],
    ~outputs=[|("out1", "Out 1"), ("out2", "Out 2")|],
    ~nodes=[|
      (
        "node1",
        {
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "example"}),
          scope: GraphScope,
        },
      ),
      (
        "node2",
        {
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "example"}),
          scope: GraphScope,
        },
      ),
      (
        "node3",
        {
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "example"}),
          scope: GraphScope,
        },
      ),
    |],
    ~connections=[|
      (
        {node: NodeConnection("node1"), nib: NibConnection("in1")},
        {node: NodeConnection("node2"), nib: NibConnection("out1")},
      ),
      (
        {node: NodeConnection("node1"), nib: NibConnection("in2")},
        {node: NodeConnection("node2"), nib: NibConnection("out2")},
      ),
      (
        {node: GraphConnection, nib: NibConnection("out2")},
        {node: NodeConnection("node1"), nib: NibConnection("out2")},
      ),
      (
        {node: NodeConnection("node2"), nib: NibConnection("in1")},
        {node: GraphConnection, nib: NibConnection("in1")},
      ),
    |],
    (),
  );

let simple =
  makeGraph(
    ~name="Simple Definition",
    ~description="A simple function",
    ~inputs=[|("in1", "In 1"), ("in2", "In 2")|],
    ~outputs=[|("out1", "Out 1"), ("out2", "Out 2")|],
    ~nodes=[|
      (
        "node1",
        {
          kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
          scope: GraphScope,
        },
      ),
      (
        "node2",
        {
          kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
          scope: GraphScope,
        },
      ),
      (
        "node3",
        {
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
          scope: GraphScope,
        },
      ),
    |],
    ~connections=[|
      (
        {node: GraphConnection, nib: NibConnection("out1")},
        {node: NodeConnection("node3"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("node3"), nib: NibConnection("left")},
        {node: NodeConnection("node1"), nib: ValueConnection},
      ),
      (
        {node: NodeConnection("node3"), nib: NibConnection("right")},
        {node: NodeConnection("node2"), nib: ValueConnection},
      ),
    |],
    (),
  );

let one =
  makeDefinition(
    ~implementation=ConstantImplementation(NumberValue(1.0)),
    (),
  );

let point =
  makeDefinition(
    ~name="2D Point",
    ~description="X and Y coordinates",
    ~inputs=[|("x", "X"), ("y", "Y")|],
    ~implementation=
      RecordTypeImplementation(
        Belt.Map.String.fromArray([|
          ("x", PrimitiveValueType(NumberType)),
          ("y", PrimitiveValueType(NumberType)),
        |]),
      ),
    (),
  );

let plus =
  makeDefinition(
    ~name="+",
    ~description="Add two numbers, resulting in a number.",
    ~inputs=[|("left", "Left"), ("right", "Right")|],
    ~outputs=[|("result", "Result")|],
    ~implementation=
      ExternalImplementation({
        name: "+",
        interface: {
          inputTypes:
            Belt.Map.String.fromArray([|
              ("left", PrimitiveValueType(NumberType)),
              ("right", PrimitiveValueType(NumberType)),
            |]),
          outputTypes:
            Belt.Map.String.fromArray([|
              ("result", PrimitiveValueType(NumberType)),
            |]),
        },
      }),
    (),
  );

let minus =
  makeDefinition(
    ~name="-",
    ~description="Subtract two numbers, resulting in a number.",
    ~inputs=[|("left", "Left"), ("right", "Right")|],
    ~outputs=[|("result", "Result")|],
    ~implementation=
      ExternalImplementation({
        name: "-",
        interface: {
          inputTypes:
            Belt.Map.String.fromArray([|
              ("left", PrimitiveValueType(NumberType)),
              ("right", PrimitiveValueType(NumberType)),
            |]),
          outputTypes:
            Belt.Map.String.fromArray([|
              ("result", PrimitiveValueType(NumberType)),
            |]),
        },
      }),
    (),
  );

let times =
  makeDefinition(
    ~name="*",
    ~description="Multiply two numbers, resulting in a number.",
    ~inputs=[|("left", "Left"), ("right", "Right")|],
    ~outputs=[|("result", "Result")|],
    ~implementation=
      ExternalImplementation({
        name: "*",
        interface: {
          inputTypes:
            Belt.Map.String.fromArray([|
              ("left", PrimitiveValueType(NumberType)),
              ("right", PrimitiveValueType(NumberType)),
            |]),
          outputTypes:
            Belt.Map.String.fromArray([|
              ("result", PrimitiveValueType(NumberType)),
            |]),
        },
      }),
    (),
  );

let divide =
  makeDefinition(
    ~name="/",
    ~description="Subtract two numbers, resulting in a number.",
    ~inputs=[|("left", "Left"), ("right", "Right")|],
    ~outputs=[|("result", "Result")|],
    ~implementation=
      ExternalImplementation({
        name: "/",
        interface: {
          inputTypes:
            Belt.Map.String.fromArray([|
              ("left", PrimitiveValueType(NumberType)),
              ("right", PrimitiveValueType(NumberType)),
            |]),
          outputTypes:
            Belt.Map.String.fromArray([|
              ("result", PrimitiveValueType(NumberType)),
            |]),
        },
      }),
    (),
  );

let pointExample =
  makeGraph(
    ~name="Point Example",
    ~description="",
    ~inputs=[||],
    ~outputs=[|("x", "X"), ("y", "Y")|],
    ~nodes=[|
      (
        "constructor",
        {
          kind: DefinedNode({kind: ConstructorNode, definitionID: "point"}),
          scope: GraphScope,
        },
      ),
      (
        "destructure",
        {
          kind: DefinedNode({kind: AccessorNode, definitionID: "point"}),
          scope: GraphScope,
        },
      ),
      (
        "x",
        {
          kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
          scope: GraphScope,
        },
      ),
      (
        "y",
        {
          kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
          scope: GraphScope,
        },
      ),
    |],
    ~connections=[|
      (
        {node: NodeConnection("constructor"), nib: NibConnection("x")},
        {node: NodeConnection("x"), nib: ValueConnection},
      ),
      (
        {node: NodeConnection("constructor"), nib: NibConnection("y")},
        {node: NodeConnection("y"), nib: ValueConnection},
      ),
      (
        {node: NodeConnection("destructure"), nib: ValueConnection},
        {node: NodeConnection("constructor"), nib: ValueConnection},
      ),
      (
        {node: GraphConnection, nib: NibConnection("x")},
        {node: NodeConnection("destructure"), nib: NibConnection("x")},
      ),
      (
        {node: GraphConnection, nib: NibConnection("y")},
        {node: NodeConnection("destructure"), nib: NibConnection("y")},
      ),
    |],
    (),
  );

let referenceExample =
  makeGraph(
    ~name="Reference example",
    ~description="",
    ~inputs=[||],
    ~outputs=[|("reference", "Reference")|],
    ~nodes=[|("reference", {kind: ReferenceNode, scope: GraphScope})|],
    ~connections=[|
      (
        {node: GraphConnection, nib: NibConnection("reference")},
        {node: NodeConnection("reference"), nib: ValueConnection},
      ),
    |],
    (),
  );

let exampleInterface =
  makeDefinition(
    ~name="Example Interface",
    ~inputs=[|("in-left", "Left"), ("in-right", "Right")|],
    ~outputs=[|("out-left", "Left"), ("out-right", "Right")|],
    ~implementation=
      InterfaceImplementation({
        inputTypes:
          Belt.Map.String.fromArray([|
            ("in-left", PrimitiveValueType(NumberType)),
            ("in-right", PrimitiveValueType(NumberType)),
          |]),
        outputTypes:
          Belt.Map.String.fromArray([|
            ("out-left", PrimitiveValueType(NumberType)),
            ("out-right", PrimitiveValueType(NumberType)),
          |]),
      }),
    (),
  );

let interfaceExample =
  makeGraph(
    ~name="Interface Example",
    ~outputs=[|("result", "Result")|],
    ~nodes=[|
      (
        "definition",
        {
          scope: GraphScope,
          kind:
            DefinedNode({
              kind: FunctionDefinitionNode,
              definitionID: "example-interface",
            }),
        },
      ),
      (
        "plus1",
        {
          scope: GraphScope,
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
        },
      ) /*
         (
           "plus2",
           {
             scope: NodeScope("definition"),
             kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
           },
         ),
         (
           "plus3",
           {
             scope: NodeScope("definition"),
             kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
           },
         ),
         (
           "plus4",
           {
             scope: GraphScope,
             kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
           },
         ), */
    |],
    ~connections=[|
      /* (
           {node: GraphConnection, nib: NibConnection("result")},
           {node: NodeConnection("definition"), nib: ValueConnection},
         ),
         (
           {node: NodeConnection("definition"), nib: NibConnection("out-left")},
           {node: NodeConnection("plus1"), nib: NibConnection("result")},
         ),
         (
           {node: NodeConnection("plus1"), nib: NibConnection("left")},
           {node: NodeConnection("plus3"), nib: NibConnection("result")},
         ),
         (
           {node: NodeConnection("plus2"), nib: NibConnection("left")},
           {node: NodeConnection("plus3"), nib: NibConnection("result")},
         ),
         (
           {node: NodeConnection("plus1"), nib: NibConnection("right")},
           {node: NodeConnection("plus2"), nib: NibConnection("result")},
         ),
         (
           {
             node: NodeConnection("definition"),
             nib: NibConnection("out-right"),
           },
           {node: NodeConnection("plus4"), nib: NibConnection("result")},
         ),
         (
           {node: NodeConnection("plus3"), nib: NibConnection("right")},
           {
             node: NodeConnection("definition"),
             nib: NibConnection("in-right"),
           },
         ), */
    |],
    (),
  );

let nestedInlineExample =
  makeGraph(
    ~name="Nested Inline Functions Example",
    ~outputs=[|("result", "Result")|],
    ~nodes=[|
      (
        "definition",
        {
          scope: GraphScope,
          kind:
            DefinedNode({
              kind: FunctionDefinitionNode,
              definitionID: "example-interface",
            }),
        },
      ),
      (
        "definition2",
        {
          scope: NodeScope("definition"),
          kind:
            DefinedNode({
              kind: FunctionDefinitionNode,
              definitionID: "example-interface",
            }),
        },
      ),
      (
        "definition3",
        {
          scope: GraphScope,
          kind:
            DefinedNode({
              kind: FunctionDefinitionNode,
              definitionID: "example-interface",
            }),
        },
      ),
      (
        "plus",
        {
          scope: NodeScope("definition3"),
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
        },
      ),
      (
        "plus2",
        {
          scope: GraphScope,
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
        },
      ),
    |],
    ~connections=[|
      (
        {node: NodeConnection("plus"), nib: NibConnection("right")},
        {node: NodeConnection("plus2"), nib: NibConnection("result")},
      ),
    |],
    (),
  );

let yesLabel =
  makeDefinition(
    ~name="Yes",
    ~implementation=LabeledTypeImplementation(None),
    (),
  );

let noLabel =
  makeDefinition(
    ~name="No",
    ~implementation=LabeledTypeImplementation(None),
    (),
  );

let booleanUnion =
  makeDefinition(
    ~name="Boolean",
    ~implementation=
      UnionTypeImplementation(
        Belt.Set.fromArray(
          [|DefinedValueType("yes"), DefinedValueType("no")|],
          ~id=(module ValueTypeComparator),
        ),
      ),
    (),
  );

let branch =
  makeDefinition(
    ~name="Branch",
    ~description="Conditionally evaluate.",
    ~inputs=[|("if", "If"), ("then", "Then"), ("else", "Otherwise")|],
    ~outputs=[|("result", "Result")|],
    ~implementation=
      ExternalImplementation({
        name: "branch",
        interface: {
          inputTypes:
            Belt.Map.String.fromArray([|
              ("if", DefinedValueType("boolean")),
              ("then", AnyType),
              ("else", AnyType),
            |]),
          outputTypes: Belt.Map.String.fromArray([|("result", AnyType)|]),
        },
      }),
    (),
  );

let equals =
  makeDefinition(
    ~name="=",
    ~inputs=[|("left", "Left"), ("right", "Right")|],
    ~outputs=[|("result", "Result")|],
    ~implementation=
      ExternalImplementation({
        name: "=",
        interface: {
          inputTypes:
            Belt.Map.String.fromArray([|
              ("left", PrimitiveValueType(NumberType)),
              ("right", PrimitiveValueType(NumberType)),
            |]),
          outputTypes:
            Belt.Map.String.fromArray([|
              ("result", DefinedValueType("boolean")),
            |]),
        },
      }),
    (),
  );

let lessThan =
  makeDefinition(
    ~name="<",
    ~inputs=[|("left", "Left"), ("right", "Right")|],
    ~outputs=[|("result", "Result")|],
    ~implementation=
      ExternalImplementation({
        name: "<",
        interface: {
          inputTypes:
            Belt.Map.String.fromArray([|
              ("left", PrimitiveValueType(NumberType)),
              ("right", PrimitiveValueType(NumberType)),
            |]),
          outputTypes:
            Belt.Map.String.fromArray([|
              ("result", DefinedValueType("boolean")),
            |]),
        },
      }),
    (),
  );

let greaterThan =
  makeDefinition(
    ~name=">",
    ~inputs=[|("left", "Left"), ("right", "Right")|],
    ~outputs=[|("result", "Result")|],
    ~implementation=
      ExternalImplementation({
        name: ">",
        interface: {
          inputTypes:
            Belt.Map.String.fromArray([|
              ("left", PrimitiveValueType(NumberType)),
              ("right", PrimitiveValueType(NumberType)),
            |]),
          outputTypes:
            Belt.Map.String.fromArray([|
              ("result", DefinedValueType("boolean")),
            |]),
        },
      }),
    (),
  );

let factorial =
  makeGraph(
    ~name="Factorial",
    ~outputs=[|("result", "Result")|],
    ~inputs=[|("input", "Input")|],
    ~nodes=[|
      (
        "one",
        {
          kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
          scope: GraphScope,
        },
      ),
      (
        "branch",
        {
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "branch"}),
          scope: GraphScope,
        },
      ),
      (
        "less-than",
        {
          kind:
            DefinedNode({kind: FunctionCallNode, definitionID: "less-than"}),
          scope: GraphScope,
        },
      ),
      (
        "times",
        {
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "times"}),
          scope: GraphScope,
        },
      ),
      (
        "minus",
        {
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "minus"}),
          scope: GraphScope,
        },
      ),
      (
        "factorial",
        {
          kind:
            DefinedNode({kind: FunctionCallNode, definitionID: "factorial"}),
          scope: GraphScope,
        },
      ),
    |],
    ~connections=[|
      (
        {node: GraphConnection, nib: NibConnection("result")},
        {node: NodeConnection("branch"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("branch"), nib: NibConnection("if")},
        {node: NodeConnection("less-than"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("branch"), nib: NibConnection("then")},
        {node: NodeConnection("one"), nib: ValueConnection},
      ),
      (
        {node: NodeConnection("less-than"), nib: NibConnection("left")},
        {node: GraphConnection, nib: NibConnection("input")},
      ),
      (
        {node: NodeConnection("less-than"), nib: NibConnection("right")},
        {node: NodeConnection("one"), nib: ValueConnection},
      ),
      (
        {node: NodeConnection("branch"), nib: NibConnection("else")},
        {node: NodeConnection("times"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("times"), nib: NibConnection("left")},
        {node: GraphConnection, nib: NibConnection("input")},
      ),
      (
        {node: NodeConnection("times"), nib: NibConnection("right")},
        {node: NodeConnection("factorial"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("factorial"), nib: NibConnection("input")},
        {node: NodeConnection("minus"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("minus"), nib: NibConnection("left")},
        {node: GraphConnection, nib: NibConnection("input")},
      ),
      (
        {node: NodeConnection("minus"), nib: NibConnection("right")},
        {node: NodeConnection("one"), nib: ValueConnection},
      ),
    |],
    (),
  );

let factorial2 =
  makeGraph(
    ~name="Factorial",
    ~outputs=[|("result", "Result")|],
    ~inputs=[|("input", "Input")|],
    ~nodes=[|
      (
        "one1",
        {
          kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
          scope: GraphScope,
        },
      ),
      (
        "one2",
        {
          kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
          scope: GraphScope,
        },
      ),
      (
        "one3",
        {
          kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
          scope: GraphScope,
        },
      ),
      (
        "branch",
        {
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "branch"}),
          scope: GraphScope,
        },
      ),
      (
        "less-than",
        {
          kind:
            DefinedNode({kind: FunctionCallNode, definitionID: "less-than"}),
          scope: GraphScope,
        },
      ),
      (
        "times",
        {
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "times"}),
          scope: GraphScope,
        },
      ),
      (
        "minus",
        {
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "minus"}),
          scope: GraphScope,
        },
      ),
      (
        "factorial",
        {
          kind:
            DefinedNode({kind: FunctionCallNode, definitionID: "factorial"}),
          scope: GraphScope,
        },
      ),
    |],
    ~connections=[|
      (
        {node: GraphConnection, nib: NibConnection("result")},
        {node: NodeConnection("branch"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("branch"), nib: NibConnection("if")},
        {node: NodeConnection("less-than"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("branch"), nib: NibConnection("then")},
        {node: NodeConnection("one1"), nib: ValueConnection},
      ),
      (
        {node: NodeConnection("less-than"), nib: NibConnection("left")},
        {node: GraphConnection, nib: NibConnection("input")},
      ),
      (
        {node: NodeConnection("less-than"), nib: NibConnection("right")},
        {node: NodeConnection("one2"), nib: ValueConnection},
      ),
      (
        {node: NodeConnection("branch"), nib: NibConnection("else")},
        {node: NodeConnection("times"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("times"), nib: NibConnection("left")},
        {node: GraphConnection, nib: NibConnection("input")},
      ),
      (
        {node: NodeConnection("times"), nib: NibConnection("right")},
        {node: NodeConnection("factorial"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("factorial"), nib: NibConnection("input")},
        {node: NodeConnection("minus"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("minus"), nib: NibConnection("left")},
        {node: GraphConnection, nib: NibConnection("input")},
      ),
      (
        {node: NodeConnection("minus"), nib: NibConnection("right")},
        {node: NodeConnection("one3"), nib: ValueConnection},
      ),
    |],
    (),
  );

let definitions =
  Belt.Map.String.fromArray([|
    // Core types
    ("yes", yesLabel),
    ("no", noLabel),
    ("boolean", booleanUnion),
    // Externals
    ("plus", plus),
    ("minus", minus),
    ("times", times),
    ("divide", divide),
    ("less-than", lessThan),
    ("greater-than", greaterThan),
    ("branch", branch),
    // Stuff used in examples
    ("one", one),
    ("point", point),
    ("example-interface", exampleInterface),
    // Examples
    ("example", example),
    ("simple", simple),
    ("point-example", pointExample),
    ("reference-example", referenceExample),
    ("interface-example", interfaceExample),
    ("nested-inline-example", nestedInlineExample),
    ("factorial", factorial),
    ("factorial2", factorial2),
  |]);

ReactDOMRe.renderToElementWithId(<App definitions />, "graph");
