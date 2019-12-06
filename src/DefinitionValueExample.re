let v =
  GraphMake.f(
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
