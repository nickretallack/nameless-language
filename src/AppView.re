[@react.component]
let make = () => {
  let connectionNode = ConnectionNode.GraphConnection;
  <div>
    {React.string(ConnectionNodeToString.f(connectionNode))}
    {React.string(Json.stringify(ConnectionNodeToJSON.f(connectionNode)))}
  </div>;
};
