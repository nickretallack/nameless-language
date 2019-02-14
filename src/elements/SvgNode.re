[%%debugger.chrome];
open Definition;
open Helpers;

let component = ReasonReact.statelessComponent("Node");

let make =
    (
      ~node: node,
      ~definitions: definitions,
      ~position: point,
      ~size: point,
      ~nodeWidth: float,
      ~textHeight: float,
      ~selected: bool=false,
      ~onClick=?,
      ~onMouseDown=?,
      ~onTouchStart=?,
      ~onMouseUp=?,
      ~onTouchEnd=?,
      _children,
    ) => {
  ...component,
  render: _self => {
    let makeNode = (name: string, onDoubleClick) =>
      <NibsBox
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
      let name = getDisplayName(definition, "en");
      let onDoubleClick = _event =>
        ReasonReact.Router.push("#" ++ definitionID);

      if (isFunctionDefinitionNode(node)) {
        <SvgDefinitionBox
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
  },
};
