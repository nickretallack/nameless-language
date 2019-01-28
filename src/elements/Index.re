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
          scope: NodeScope("definition"),
          kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
        },
      ),
    |],
    ~connections=[|
      (
        {node: GraphConnection, nib: NibConnection("result")},
        {node: NodeConnection("definition"), nib: ValueConnection},
      ),
      (
        {node: NodeConnection("definition"), nib: NibConnection("out-left")},
        {node: NodeConnection("plus1"), nib: NibConnection("result")},
      ),
      (
        {node: NodeConnection("plus1"), nib: NibConnection("left")},
        {node: NodeConnection("definition"), nib: NibConnection("in-left")},
      ),
    |],
    (),
  );

let definitions =
  Belt.Map.String.fromArray([|
    ("example", example),
    ("simple", simple),
    ("one", one),
    ("plus", plus),
    ("point", point),
    ("point-example", pointExample),
    ("reference-example", referenceExample),
    ("example-interface", exampleInterface),
    ("interface-example", interfaceExample),
  |]);

ReactDOMRe.renderToElementWithId(
  <WindowSize render={size => <App definitions size />} />,
  "graph",
);
