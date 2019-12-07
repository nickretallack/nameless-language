[@react.component]
let make =
    (
      ~node: Node.t,
      ~definitions: DefinitionMap.t,
      ~position: Point.t,
      ~size: Point.t,
      ~nodeWidth: float,
      ~textHeight: float,
      ~selected: bool=false,
      ~onClick=?,
      ~onMouseDown=?,
      ~onTouchStart=?,
      ~onMouseUp=?,
      ~onTouchEnd=?,
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
      ?onMouseDown
      ?onMouseUp
      ?onTouchStart
      ?onTouchEnd
    />;

  switch (node.kind) {
  | ReferenceNode => makeNode("Reference", None)
  | ListNode(_) => makeNode("List", None)
  | DefinedNode({definitionID}) =>
    let definition = Belt.Map.String.getExn(definitions, definitionID);
    let name = DefinitionGetDisplayName.f(definition, "en");
    let onDoubleClick = _event =>
      ReasonReact.Router.push("#" ++ definitionID);

    if (NodeIsFunctionDefinition.f(node)) {
      <DefinitionBoxView
        name
        position
        size
        nodeWidth
        textHeight
        selected
        onDoubleClick
        ?onClick
        ?onMouseDown
        ?onTouchStart
        ?onMouseUp
        ?onTouchEnd
      />;
    } else {
      makeNode(name, Some(onDoubleClick));
    };
  };
};
