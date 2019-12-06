let v =
  GraphMake.f(
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
