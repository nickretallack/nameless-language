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
      _children,
    ) => {
  ...component,
  render: _self => {
    let makeNode = name =>
      <NibsBox name position height={size.y} nodeWidth textHeight />;

    switch (node.kind) {
    | ReferenceNode => makeNode("Reference")
    | ListNode(_) => makeNode("List")
    | DefinedNode({kind, definitionID}) =>
      let definition = Belt.Map.String.getExn(definitions, definitionID);
      let name = getDisplayName(definition, "en");
      if (isFunctionDefinitionNode(node)) {
        <SvgDefinitionBox name position size nodeWidth textHeight />;
      } else {
        makeNode(name);
      };
    };
  },
};
