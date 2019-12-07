[@react.component]
let make = () => {
  let connectionSide =
    ConnectionSide.{node: GraphConnection, nib: ValueConnection};
  let explicitConnectionSide =
    ExplicitConnectionSide.{connectionSide, isSource: false};
  let definedNode = DefinedNode.{kind: FunctionCallNode, definitionID: "123"};

  let definitions = DefinitionExamples.v;
  let example = DefinitionExamples.example;

  let node =
    switch (example.implementation) {
    | GraphImplementation(graphImplementation) =>
      Belt.Map.String.getExn(graphImplementation.nodes, "node1")
    | _ => raise(Not_found)
    };
  <div>
    <svg>
      <NibView
        isSource=true
        connectionSide
        position=Point.{x: 50.0, y: 50.0}
        text="Hello"
        isHighlighted=false
        value=None
      />
      <NodeView
        node
        definitions
        position=Point.{x: 10.0, y: 10.0}
        size=Point.{x: 100.0, y: 100.0}
        nodeWidth=300.0
        textHeight=20.0
      />
    </svg>
  </div>;
};
