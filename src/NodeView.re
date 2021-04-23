[@react.component]
let make =
    (
      ~node: Node.t,
      ~nodeID: NodeID.t,
      ~definitions: DefinitionMap.t,
      ~languageName: LanguageName.t,
      ~position: Point.t,
      ~size: Point.t,
      ~nodeWidth: float,
      ~textHeight: float,
      ~selected: bool=false,
      ~emit,
      ~onClick=?,
      ~onPointerDown=?,
      ~onPointerUp=?,
    ) => {
  let makeNode = (name: string, onDoubleClick) =>
    <NibsBoxView
      name
      position
      height={size.y}
      nodeWidth
      textHeight
      selected
      ?onDoubleClick
      ?onClick
      ?onPointerDown
      ?onPointerUp
    />;

  switch (node.kind) {
  | ReferenceNode => makeNode("Reference", None)
  | ListNode(_) => makeNode("List", None)
  | DefinedNode({definitionID}) =>
    let definition = Belt.Map.String.getExn(definitions, definitionID);
    let name = DefinitionGetDisplayName.f(definition, languageName);
    let onDoubleClick = _event =>
      RescriptReactRouter.push("#" ++ definitionID);

    if (NodeIsFunctionDefinition.f(node)) {
      <DefinitionBoxView
        name
        position
        size
        nodeWidth
        textHeight
        selected
        onDoubleClick
        nodeScope={NodeScope(nodeID)}
        emit
        ?onClick
        ?onPointerDown
        ?onPointerUp
      />;
    } else {
      makeNode(name, Some(onDoubleClick));
    };
  };
};
