[@react.component]
let make = () => {
  let connectionSide =
    ConnectionSide.{node: GraphConnection, nib: ValueConnection};
  let explicitConnectionSide =
    ExplicitConnectionSide.{connectionSide, isSource: false};
  let definedNode = DefinedNode.{kind: FunctionCallNode, definitionID: "123"};
  <div>
    {React.string(ConnectionSideToString.f(connectionSide))}
    {React.string(Json.stringify(ConnectionSideToJson.f(connectionSide)))}
    {React.string(ExplicitConnectionSideKey.f(explicitConnectionSide))}
    {React.string(Json.stringify(DefinedNodeToJson.f(definedNode)))}
  </div>;
};
