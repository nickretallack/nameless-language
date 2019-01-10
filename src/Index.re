open Definition;

let makeNibDocs = (nibs: array((nibID, string))) =>
  Belt.Map.String.fromArray(
    Belt.Array.map(nibs, ((nibID, text)) =>
      (nibID, makeTranslatable(text, "en"))
    ),
  );

let makeDefinition =
    (
      ~name="",
      ~description="",
      ~inputs=[||],
      ~outputs=[||],
      ~implementation,
      _unit,
    ) => {
  documentation: {
    name: makeTranslatable(name, "en"),
    description: makeTranslatable(description, "en"),
    inputs: makeNibDocs(inputs),
    outputs: makeNibDocs(outputs),
  },
  display: {
    inputOrdering: Array.to_list(Array.map(((id, _name)) => id, inputs)),
    outputOrdering: Array.to_list(Array.map(((id, _name)) => id, outputs)),
  },
  implementation,
};

let makeGraph =
    (~name, ~description, ~inputs, ~outputs, ~nodes, ~connections, _unit) =>
  makeDefinition(
    ~name,
    ~description,
    ~inputs,
    ~outputs,
    ~implementation=
      GraphImplementation({
        nodes: Belt.Map.String.fromArray(nodes),
        connections:
          Belt.Map.fromArray(connections, ~id=(module ConnectionComparator)),
      }),
    (),
  );

let example =
  makeGraph(
    ~name="Example Definition",
    ~description="An example function",
    ~inputs=[|("in1", "In 1"), ("in2", "In 2")|],
    ~outputs=[|("out1", "Out 1"), ("out2", "Out 2")|],
    ~nodes=[|
      (
        "node1",
        DefinedNode({kind: FunctionCallNode, definitionID: "example"}),
      ),
      (
        "node2",
        DefinedNode({kind: FunctionCallNode, definitionID: "example"}),
      ),
      (
        "node3",
        DefinedNode({kind: FunctionCallNode, definitionID: "example"}),
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
      ("node1", DefinedNode({kind: ValueNode, definitionID: "one"})),
      ("node2", DefinedNode({kind: ValueNode, definitionID: "one"})),
      ("node3", DefinedNode({kind: FunctionCallNode, definitionID: "plus"})),
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
    ~name="1",
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
        DefinedNode({kind: ConstructorNode, definitionID: "point"}),
      ),
      (
        "destructure",
        DefinedNode({kind: AccessorNode, definitionID: "point"}),
      ),
      ("x", DefinedNode({kind: ValueNode, definitionID: "one"})),
      ("y", DefinedNode({kind: ValueNode, definitionID: "one"})),
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
    ~nodes=[|("reference", ReferenceNode)|],
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
    ~inputs=[|("left", "Left"), ("right", "Right")|],
    ~outputs=[|("result", "Result")|],
    ~implementation=
      InterfaceImplementation({
        inputTypes:
          Belt.Map.String.fromArray([|
            ("left", PrimitiveValueType(NumberType)),
            ("right", PrimitiveValueType(NumberType)),
          |]),
        outputTypes:
          Belt.Map.String.fromArray([|
            ("result", PrimitiveValueType(NumberType)),
          |]),
      }),
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
  |]);

ReactDOMRe.renderToElementWithId(
  <WindowSize render={size => <App definitions size />} />,
  "graph",
);

Js.log(example);
