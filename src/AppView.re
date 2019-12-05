[@react.component]
let make = () => {
  let connectionSide =
    ConnectionSide.{node: GraphConnection, nib: ValueConnection};
  <div>
    {React.string(ConnectionSideToString.f(connectionSide))}
    {React.string(Json.stringify(ConnectionSideToJson.f(connectionSide)))}
  </div>;
};
